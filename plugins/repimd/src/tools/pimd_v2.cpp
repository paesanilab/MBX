#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include <cmath>
#include <cassert>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "tools/constants.h"

#include "nhc.h"
#include "pimd_v2.h"

#include "io-frame.h"

using h2o::hbar;
using h2o::kB;

////////////////////////////////////////////////////////////////////////////////

namespace kit {

//----------------------------------------------------------------------------//

pimd::pimd() : necklace() {
    m_fict_mass = 0;
    m_thermostats = 0;

    m_TI_lambda = 1.0;  // fully quantum
}

//----------------------------------------------------------------------------//

pimd::~pimd() {
    delete[] m_fict_mass;
    delete[] m_thermostats;
}

//----------------------------------------------------------------------------//

void pimd::init1(size_t natoms, size_t nbeads, double T, double* mass, double* cartpos, mt19937& prg) {
    assert(natoms > 0 && nbeads > 0);
    assert(nbeads % 2 == 0 || nbeads == 1);
    assert(T > 0.0 && mass != 0 && cartpos != 0);

    necklace::setup(natoms, nbeads);

    delete[] m_fict_mass;
    delete[] m_thermostats;

    m_time0 = 0.0;
    m_T = T;
    m_kT = kB * T;
    m_omega_M = std::sqrt(double(nbeads)) * m_kT / hbar;

    // fictitious masses

    m_fict_mass = new double[natoms * nbeads];

    for (size_t b = 0; b < nbeads; ++b) {
        const double factor = (b == 0 ? 1.0 : lambda(b));
        for (size_t i = 0; i < natoms; ++i) m_fict_mass[i + b * natoms] = mass[i] * factor;
    }

    // thermostats

    const size_t natom3 = 3 * natoms;
    const size_t th_size = nhc::size(nchain);
    m_thermostats = new double[natom3 * nbeads * th_size];

    const double tau = 2 * M_PI / m_omega_M;

    for (size_t b = 0; b < nbeads; ++b)
        for (size_t i = 0; i < natom3; ++i) {
            const size_t j = i + b * natom3;
            nhc::initialize(nchain, m_thermostats + th_size * j, tau, prg);
        }

    // initialize cartesian positions

    for (size_t b = 0; b < nbeads; ++b) std::copy(cartpos, cartpos + natom3, m_pos_cart + b * natom3);

    pos_c2n();  // cart -> nmode

    // initialize normal mode velocities

    m_Ekin_fict = 0.0;
    for (size_t b = 0; b < nbeads; ++b)
        for (size_t i = 0; i < natoms; ++i) {
            const size_t j = i + b * natoms;
            const size_t j3 = 3 * j;
            const double sigma = std::sqrt(m_kT / m_fict_mass[j]);
            for (size_t k = 0; k < 3; ++k) {
                const size_t l = j3 + k;
                m_vel_nmode[l] = sigma * prg.random_gaussian();
                m_Ekin_fict += m_fict_mass[j] * m_vel_nmode[l] * m_vel_nmode[l];
            }
        }

    m_Ekin_fict /= 2;

    // scale kinetic energy to the target temperature

    const double scale_factor = std::sqrt(0.5 * m_kT * nbeads * natom3 / m_Ekin_fict);
    for (size_t b = 0; b < nbeads; ++b)
        for (size_t i = 0; i < natom3; ++i) m_vel_nmode[i + b * natom3] *= scale_factor;

    m_Ekin_fict *= scale_factor * scale_factor;
}

//----------------------------------------------------------------------------//

void pimd::step(const double& dt, MPI_Comm comm) {
    int comm_rank, comm_size;
    MPI_Comm_rank(comm, &comm_rank);
    MPI_Comm_size(comm, &comm_size);

    const size_t th_size = nhc::size(nchain);

    const double tau = 2 * M_PI / m_omega_M;
    const double dt2 = dt / 2;

    const size_t natom3 = 3 * natom();
    const size_t ndof = nbead() * natom3;

    if (comm_rank != 0) goto forces;

    // 1. advance thermostats, velocities by dt/2, nmode position on dt

    for (size_t b = 0; b < nbead(); ++b)
        for (size_t i = 0; i < natom(); ++i) {
            const size_t j = b * natom() + i;
            const size_t j3 = 3 * j;
            const double mass = m_fict_mass[j];

            for (size_t k = 0; k < 3; ++k) {
                const size_t l = j3 + k;
                const double Ekin2 = mass * m_vel_nmode[l] * m_vel_nmode[l];
                const double aa = nhc::advance(nchain, m_thermostats + l * th_size, tau, Ekin2 / m_kT, dt2);

                m_vel_nmode[l] = aa * m_vel_nmode[l] + dt2 * m_frc_nmode[l] / mass;
                m_pos_nmode[l] += dt * m_vel_nmode[l];
            }
        }

    // 2. transform position to cartesian space, compute forces

    pos_n2c();  // preserves input

    // quantum <=> classical TI

    if (m_TI_lambda < 1.0) {
        for (size_t b = 0; b < nbead(); ++b) {
            for (size_t i = 0; i < natom3; ++i) {
                const size_t j = b * natom3 + i;
                m_pos_cart[j] = m_TI_lambda * m_pos_cart[j] + (1.0 - m_TI_lambda) * m_pos_nmode[i];
            }
        }
    }

forces:

    MPI_Bcast(m_pos_cart, ndof, MPI_DOUBLE, 0, comm);

    // zero out m_frc_cart
    std::fill(m_frc_cart, m_frc_cart + ndof + 2, 0.0);

    // compute the forces for each bead
    m_Epot_sum = 0.0;
    if (nbead() >= static_cast<size_t>(comm_size) && nbead() % comm_size == 0) {
        // use 1 thread per force()
        for (size_t b = comm_rank; b < nbead(); b += comm_size)
            m_Epot_sum += force(m_pos_cart + b * natom3, m_frc_cart + b * natom3, MPI_COMM_SELF);
    } else {
        // pass the comm down for force()
        for (size_t b = 0; b < nbead(); ++b)
            m_Epot_sum += force(m_pos_cart + b * natom3, m_frc_cart + b * natom3, comm);
    }
    m_frc_cart[ndof] = m_Epot_sum;

    // reduce the forces and m_Epot_sum
    MPI_Reduce(m_frc_cart, m_frc_nmode, ndof + 2, MPI_DOUBLE, MPI_SUM, 0, comm);

    std::swap(m_frc_cart, m_frc_nmode);

    if (comm_rank != 0) return;

    m_Epot_sum = m_frc_cart[ndof] / nbead();

    // transform forces back to normal modes
    frc_c2n();

    // classical <=> quantum TI
    if (m_TI_lambda < 1.0) {
        for (size_t b = 1; b < nbead(); ++b) {
            for (size_t i = 0; i < natom3; ++i) {
                const size_t j = i + b * natom3;
                m_frc_nmode[j] *= m_TI_lambda;
            }
        }
    }

    // add the harmonic part

    const double omega2 = m_omega_M * m_omega_M;

    m_Espring = 0.0;

    for (size_t b = 1; b < nbead(); ++b)
        for (size_t i = 0; i < natom(); ++i) {
            const size_t j = i + b * natom();
            const size_t j3 = 3 * j;
            const double tmp = m_fict_mass[j] * omega2;
            for (size_t k = 0; k < 3; ++k) {
                const size_t l = j3 + k;
                m_frc_nmode[l] -= tmp * m_pos_nmode[l];
                m_Espring += tmp * m_pos_nmode[l] * m_pos_nmode[l];
            }
        }

    m_Espring /= 2;

    // 3. advance velocities and thermostats by dt/2

    m_Ekin_fict = 0.0;

    for (size_t b = 0; b < nbead(); ++b)
        for (size_t i = 0; i < natom(); ++i) {
            const size_t j = b * natom() + i;
            const size_t j3 = 3 * j;
            const double mass = m_fict_mass[j];

            for (size_t k = 0; k < 3; ++k) {
                const size_t l = j3 + k;

                m_vel_nmode[l] += dt2 * m_frc_nmode[l] / mass;
                const double Ekin2 = mass * m_vel_nmode[l] * m_vel_nmode[l];
                const double aa = nhc::advance(nchain, m_thermostats + l * th_size, tau, Ekin2 / m_kT, dt2);

                m_vel_nmode[l] *= aa;
                m_Ekin_fict += Ekin2 * aa * aa;
            }
        }

    m_Ekin_fict /= 2;
}

//----------------------------------------------------------------------------//

double pimd::invariant() const {
    const size_t th_size = nhc::size(nchain);
    const double tau = 2 * M_PI / m_omega_M;

    double accu(0);
    for (size_t n = 0; n < 3 * nbead() * natom(); ++n) accu += nhc::invariant(nchain, m_thermostats + n * th_size, tau);

    return m_Ekin_fict + m_Epot_sum + m_Espring + m_kT * accu;
}

//----------------------------------------------------------------------------//

void pimd::change_temperature(const double& T) {
    assert(T > 0.0);
    assert(m_T > 0.0);

    const size_t th_size = nhc::size(nchain);

    const double th_scale_factor = T / m_T;
    const double vel_scale_factor = std::sqrt(th_scale_factor);

    // rescale velocities/thermostats
    for (size_t n = 0; n < 3 * natom() * nbead(); ++n) {
        m_vel_nmode[n] *= vel_scale_factor;
        nhc::scale(nchain, m_thermostats + n * th_size, th_scale_factor);
    }

    m_T = T;
    m_kT = kB * m_T;
    m_omega_M = std::sqrt(double(nbead())) * m_kT / hbar;
}

//----------------------------------------------------------------------------//

bool pimd::force1(const double* xyz, double* f) {
    return false;  // not implemented
}

//----------------------------------------------------------------------------//

// destroys m_frc_cart
void pimd::TI_integrand(double& full, double& monomers) {
    const size_t natom3 = 3 * natom();

    // cartesian positions/forces are computed/reduced in step()

    full = 0.0;
    for (size_t b = 0; b < nbead(); ++b) {
        for (size_t i = 0; i < natom3; ++i) {
            const size_t j = i + b * natom3;
            full += m_frc_cart[j] * (m_pos_nmode[i] - m_pos_cart[j]);
        }
    }

    full /= nbead();
    if (m_TI_lambda > 0.0) full /= m_TI_lambda;

    // put monomer forces into m_frc_nmode

    bool ok = true;

    for (size_t b = 0; b < nbead(); ++b) {
        for (size_t i = 0; i < natom3; i += 9) {
            const size_t j = i + b * natom3;
            ok = ok && force1(m_pos_cart + j, m_frc_cart + j);
        }
    }

    if (!ok) throw std::logic_error("pimd::force1() returned false in pimd::TI_integrand()");

    monomers = 0.0;
    for (size_t b = 0; b < nbead(); ++b) {
        for (size_t i = 0; i < natom3; ++i) {
            const size_t j = i + b * natom3;
            monomers += m_frc_cart[j] * (m_pos_nmode[i] - m_pos_cart[j]);
        }
    }

    monomers /= nbead();
    if (m_TI_lambda > 0.0) monomers /= m_TI_lambda;
}

//----------------------------------------------------------------------------//

void pimd::save(std::ostream& os, const double& time) const {
    assert(nbead() > 0 && natom() > 0);

    const size_t ndofs = 3 * natom() * nbead();
    const size_t th_size = nhc::size(nchain);

    os << io::sec("time", time) << io::sec("temperature", m_T) << io::sec("mass", natom(), m_fict_mass)
       << io::sec("positions", ndofs, m_pos_nmode) << io::sec("velocities", ndofs, m_vel_nmode)
       << io::sec("forces", ndofs, m_frc_nmode) << io::sec("thermostats", th_size * ndofs, m_thermostats)
       << kit::io::endframe();
}

//----------------------------------------------------------------------------//

void pimd::load(const char* filename, MPI_Comm comm) {
    assert(filename);

    setup(0, 0);

    delete[] m_fict_mass;
    m_fict_mass = 0;

    delete[] m_thermostats;
    m_thermostats = 0;

    int comm_rank;
    MPI_Comm_rank(comm, &comm_rank);

    if (comm_rank == 0) {  // begin of master only code

        std::ifstream ifs(filename);
        if (!ifs) {
            std::ostringstream oss;
            oss << "could not open '" << filename << "' for reading";
            throw std::runtime_error(oss.str());
        }

        io::frame f;

        f.require("time");
        f.require("temperature");
        f.require("mass");
        f.require("positions");
        f.require("velocities");
        f.require("thermostats");

        try {
            ifs >> f;
        } catch (const std::exception& e) {
            std::ostringstream oss;
            oss << "'" << filename << "': " << e.what();
            throw std::runtime_error(oss.str());
        }

        const std::vector<double>* ptr = f.get("time");
        assert(ptr);  // required
        if (ptr->size() > 1) {
            std::ostringstream oss;
            oss << "'" << filename << "': too many elements in the {time} section";
            throw std::runtime_error(oss.str());
        }
        m_time0 = ptr->operator[](0);

        ptr = f.get("temperature");
        assert(ptr);  // required
        if (ptr->size() > 1) {
            std::ostringstream oss;
            oss << "'" << filename << "': too many elements in the {temperature} section";
            throw std::runtime_error(oss.str());
        }
        m_T = ptr->operator[](0);
        m_kT = kB * m_T;

        const std::vector<double>* mptr = f.get("mass");
        assert(mptr);  // required
        const size_t natoms = mptr->size();

        if (natoms == 0) {
            std::ostringstream oss;
            oss << "'" << filename << "': too few masses";
            throw std::runtime_error(oss.str());
        }

        const std::vector<double>* posptr = f.get("positions");
        assert(posptr);  // required

        if (posptr->size() % (3 * natoms) != 0) {
            std::ostringstream oss;
            oss << "'" << filename
                << "': incommensurate number of elements"
                   " in mass/positions";
            throw std::runtime_error(oss.str());
        }

        const size_t nbeads = posptr->size() / natoms / 3;
        const size_t th_size = nhc::size(nchain);

        if ((nbeads % 2 != 0 && nbeads != 1) || nbeads == 0) {
            std::ostringstream oss;
            oss << "'" << filename << "': impossible number of beads";
            throw std::runtime_error(oss.str());
        }

        const std::vector<double>* velptr = f.get("velocities");
        assert(velptr);  // required

        const std::vector<double>* frcptr = f.get("forces");

        const std::vector<double>* thptr = f.get("thermostats");
        assert(thptr);  // required

        if (velptr->size() != posptr->size() || thptr->size() != th_size * posptr->size() ||
            (frcptr && frcptr->size() != posptr->size())) {
            std::ostringstream oss;
            oss << "'" << filename
                << "': incommensurate number of elements"
                   " in positions/velocities/forces/thermostats";
            throw std::runtime_error(oss.str());
        }

        // everyting is fine

        necklace::setup(natoms, nbeads);

        std::copy(posptr->begin(), posptr->end(), m_pos_nmode);
        std::copy(velptr->begin(), velptr->end(), m_vel_nmode);

        if (frcptr)
            std::copy(frcptr->begin(), frcptr->end(), m_frc_nmode);
        else
            std::fill(m_frc_nmode, m_frc_nmode + 3 * natom() * nbead(), 0.0);

        m_omega_M = std::sqrt(double(nbead())) * m_kT / hbar;

        // fictitious masses

        m_fict_mass = new double[natom() * nbead()];

        for (size_t b = 0; b < nbead(); ++b) {
            const double factor = (b == 0 ? 1.0 : lambda(b));
            for (size_t i = 0; i < natom(); ++i) m_fict_mass[i + b * natom()] = mptr->operator[](i) * factor;
        }

        // thermostats

        m_thermostats = new double[3 * natom() * nbead() * th_size];
        std::copy(thptr->begin(), thptr->end(), m_thermostats);

    }  // end of master only code

    int tmp[2] = {int(natom()), int(nbead())};
    MPI_Bcast(tmp, 2, MPI_INT, 0, comm);

    if (comm_rank != 0) {
        const size_t natoms = size_t(tmp[0]);
        const size_t nbeads = size_t(tmp[1]);

        setup(natoms, nbeads);

        m_fict_mass = new double[natom() * nbead()];  // for sphere in repimd.cpp
    }

    MPI_Bcast(m_fict_mass, natom() * nbead(), MPI_DOUBLE_PRECISION, 0, comm);
}

//----------------------------------------------------------------------------//

}  // namespace kit

////////////////////////////////////////////////////////////////////////////////

#ifndef PIMD_H
#define PIMD_H

#include <mpi.h>

#include <cassert>
#include <iostream>

#include "tools/mt19937.h"
#include "necklace.h"

//----------------------------------------------------------------------------//

namespace kit {

//----------------------------------------------------------------------------//

//
// simple PIMD integrator; master propagates everything,
// non-master nodes help compute classical forces/energies
//

struct pimd : public necklace {
    pimd();
    virtual ~pimd();

    // classical force; must return classical potential energy
    virtual double force(double*, double*, MPI_Comm) = 0;

    // classical monomer force; must return 'true' if supported
    virtual bool force1(const double*, double*);

    void step(const double&, MPI_Comm);  // does not increment m_time0
    double invariant() const;            // master only

    static const size_t nchain = 4;

    void load(const char*, MPI_Comm);
    void save(std::ostream&, const double&) const;

    inline const double& time0() const;        // master only
    inline const double& temperature() const;  // master only
    inline const double& mass(size_t) const;   // master only

    void change_temperature(const double& T_new);  // master only

    inline const double& TI_lambda() const;
    inline void set_TI_lambda(const double&);

    void TI_integrand(double&, double&);  // master only

    // mass is of length natoms, cartpos is 3*natoms
    void init1(size_t natoms, size_t nbeads, double T, double* mass, double* cartpos, mt19937&);

   protected:
    double m_time0;      // master only
    double m_T;          // master only
    double m_kT;         // master only
    double m_omega_M;    // master only
    double m_TI_lambda;  // master only

    double* m_fict_mass;
    double* m_thermostats;  // master only

    double m_Espring;    // reduced on master only
    double m_Epot_sum;   // reduced on master only
    double m_Ekin_fict;  // reduced on master only
};

//----------------------------------------------------------------------------//

inline const double& pimd::time0() const { return m_time0; }

//----------------------------------------------------------------------------//

inline const double& pimd::temperature() const { return m_T; }

//----------------------------------------------------------------------------//

inline const double& pimd::mass(size_t n) const {
    assert(n < natom() * nbead());
    return m_fict_mass[n];
}

//----------------------------------------------------------------------------//

inline const double& pimd::TI_lambda() const { return m_TI_lambda; }

//----------------------------------------------------------------------------//

inline void pimd::set_TI_lambda(const double& l) {
    assert(l >= 0.0 && l <= 1.0);
    m_TI_lambda = l;
}

//----------------------------------------------------------------------------//

}  // namespace kit

//----------------------------------------------------------------------------//

#endif  // PIMD_H

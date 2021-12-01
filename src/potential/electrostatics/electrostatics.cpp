/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "potential/electrostatics/electrostatics.h"

//#define DEBUG
//#define TIMING
//#define PRINT_TERMS

#include <iomanip>
#ifdef DEBUG
#include <iostream>
#endif
#ifdef TIMING
#include <chrono>
#include <iostream>
#endif

//#define _DEBUG_PERM
//#define _DEBUG_DIPOLE
//#define _DEBUG_ASPC
//#define _DEBUG_ITERATION 1
//#define _DEBUG_COMM
//#define _DEBUG_DIPFIELD
//#define _DEBUG_GRAD
//#define _DEBUG_PRINT_ENERGY
//#define _DEBUG_PRINT_GRAD

#if HAVE_MPI == 1
#define MBX_ELEC_P2P_COMM 1
#else
#define MBX_ELEC_P2P_COMM 0
#endif

// let expert users test this first; now using defaults
//#define MBX_ELEC_P2P_COMM 0

// When turning polarization off, don't set the 1/polarity value to max_dbl because it gets
// added to the potential and field values, generating inf values that result in NaN energies.
// Any large value will do; it doesn't enter the energy or forces below about 1e-12 anyway
const double BIGNUM = 1e50;

namespace elec {

const double PIQSRT = sqrt(M_PI);

std::vector<double> Electrostatics::GetSysXyz() { return sys_xyz_; }

std::vector<double> Electrostatics::GetSysChg() { return sys_chg_; }

std::vector<double> Electrostatics::GetSysPolfacs() { return polfac_; }

std::vector<double> Electrostatics::GetSysPols() { return pol_; }

std::vector<double> Electrostatics::GetSysChgGrad() { return sys_chg_grad_; }

std::vector<size_t> Electrostatics::GetSitesVector() { return sites_; }

std::vector<std::string> Electrostatics::GetMonIds() { return mon_id_; }

std::vector<size_t> Electrostatics::GetFirstIndex() { return first_ind_; }

std::vector<std::pair<std::string, size_t>> Electrostatics::GetMonTypeCount() { return mon_type_count_; }

std::vector<size_t> Electrostatics::GetSysIsLocal() { return islocal_; }

std::vector<int> Electrostatics::GetSysAtomTag() { return sys_atom_tag_; }

bool Electrostatics::GetDoGrads() { return do_grads_; }

double Electrostatics::GetDipoleTolerance() { return tolerance_; }

size_t Electrostatics::GetDipoleMaxIt() { return maxit_; }

std::string Electrostatics::GetDipoleConvergenceMethod() { return dip_method_; }

std::vector<double> Electrostatics::GetBox() { return box_; }

std::vector<double> Electrostatics::GetBoxAbc() { return box_ABCabc_; }

std::vector<double> Electrostatics::GetBoxInverse() { return box_inverse_; }

std::vector<double> Electrostatics::GetInternalXyz() { return xyz_; }

std::vector<double> Electrostatics::GetInternalChg() { return chg_; }

std::vector<double> Electrostatics::GetInternalPolSqrt() { return pol_sqrt_; }

std::vector<size_t> Electrostatics::GetInternalIsLocalAtomXyz() { return islocal_atom_xyz_; }

std::vector<size_t> Electrostatics::GetInternalIsLocalAtom() { return islocal_atom_; }

std::vector<int> Electrostatics::GetInternalAtomTag() { return atom_tag_; }

double Electrostatics::GetCutoff() { return cutoff_; }

nlohmann::json Electrostatics::GetJsonMonomers() { return mon_j_; }

void Electrostatics::SetCutoff(double cutoff) { cutoff_ = cutoff; }

void Electrostatics::SetEwaldAlpha(double alpha) { ewald_alpha_ = alpha; }

void Electrostatics::SetEwaldGridDensity(double density) { pme_grid_density_ = density; }

void Electrostatics::SetEwaldSplineOrder(int order) { pme_spline_order_ = order; }

void Electrostatics::SetDipoleTolerance(double tol) { tolerance_ = tol; }

void Electrostatics::SetDipoleMaxIt(size_t maxit) { maxit_ = maxit; }

void Electrostatics::SetPeriodicity(bool periodic) { simcell_periodic_ = periodic; }

void Electrostatics::SetExternalChargesAndPositions(std::vector<double> chg, std::vector<double> xyz) {
    external_charge_ = chg;
    external_charge_xyz_ = xyz;
}

void Electrostatics::SetExternalChargesAndPositions(std::vector<double> chg, std::vector<double> xyz,
                                                    std::vector<size_t> islocal, std::vector<int> tag) {
    external_charge_ = chg;
    external_charge_xyz_ = xyz;
    external_islocal_ = islocal;
    external_tag_ = tag;
}

std::vector<double> Electrostatics::GetExternalCharges() { return external_charge_; }

std::vector<double> Electrostatics::GetExternalChargesPositions() { return external_charge_xyz_; }

std::vector<double> Electrostatics::GetExternalChargesGradients() { return external_charge_grads_; }

void Electrostatics::SetJsonMonomers(nlohmann::json mon_j) { mon_j_ = mon_j; }

void Electrostatics::Initialize(const std::vector<double> &chg, const std::vector<double> &chg_grad,
                                const std::vector<double> &polfac, const std::vector<double> &pol,
                                const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                                const std::vector<size_t> &sites, const std::vector<size_t> &first_ind,
                                const std::vector<std::pair<std::string, size_t>> &mon_type_count,
                                const std::vector<size_t> &islocal, const std::vector<int> &sys_atom_tag,
                                const bool do_grads, const double tolerance, const size_t maxit,
                                const std::string dip_method, const std::vector<double> &box) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Charges:\n";
    for (size_t i = 0; i < chg.size(); i++) {
        std::cerr << chg[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Charge derivatives:\n";
    for (size_t i = 0; i < chg_grad.size(); i++) {
        std::cerr << chg_grad[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Polfacs:\n";
    for (size_t i = 0; i < polfac.size(); i++) {
        std::cerr << polfac[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Pols:\n";
    for (size_t i = 0; i < pol.size(); i++) {
        std::cerr << pol[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Sys Xyz:\n";
    for (size_t i = 0; i < sys_xyz.size(); i++) {
        std::cerr << sys_xyz[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "mon_id:\n";
    for (size_t i = 0; i < mon_id.size(); i++) {
        std::cerr << "\"" << mon_id[i] << "\" , ";
    }
    std::cerr << std::endl;

    std::cerr << "Sites:\n";
    for (size_t i = 0; i < sites.size(); i++) {
        std::cerr << sites[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "First Index:\n";
    for (size_t i = 0; i < first_ind.size(); i++) {
        std::cerr << first_ind[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "mon_type_count:\n";
    for (size_t i = 0; i < mon_type_count.size(); i++) {
        std::cerr << " { \"" << mon_type_count[i].first << "\" , " << mon_type_count[i].second << " } , ";
    }
    std::cerr << std::endl;

    std::cerr << "islocal:\n";
    for (size_t i = 0; i < islocal.size(); i++) {
        std::cerr << islocal[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "sys_atom_tag:\n";
    for (size_t i = 0; i < sys_atom_tag.size(); i++) {
        std::cerr << sys_atom_tag[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "do grads: " << do_grads << std::endl;
    std::cerr << "tolerance: " << tolerance << std::endl;
    std::cerr << "maxit: " << maxit << std::endl;
    std::cerr << "dipole method: " << dip_method << std::endl;

    std::cerr << "box:\n";
    for (size_t i = 0; i < box.size(); i++) {
        std::cerr << box[i] << " , ";
    }
    std::cerr << std::endl;

#endif
    pme_spline_order_ = 5;
    pme_grid_density_ = 1.2;
    ewald_alpha_ = 0;
    // Copy System data in electrostatics
    // sys_chg_ = std::vector<double>(chg.begin(),chg.end());
    sys_chg_ = chg;
    sys_chg_grad_ = chg_grad;
    polfac_ = polfac;
    pol_ = pol;
    sys_xyz_ = sys_xyz;
    islocal_ = islocal;
    sys_atom_tag_ = sys_atom_tag;
    mon_id_ = mon_id;
    sites_ = sites;
    first_ind_ = first_ind;
    mon_type_count_ = mon_type_count;
    do_grads_ = do_grads;
    tolerance_ = tolerance;
    maxit_ = maxit;
    dip_method_ = dip_method;
    box_ = box;
    box_ABCabc_ = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
    use_pbc_ = box.size();
    simcell_periodic_ = use_pbc_;
    cutoff_ = 1000.0;

    // Initialize other variables
    nsites_ = sys_chg_.size();
    size_t nsites3 = nsites_ * 3;
    rec_phi_and_field_ = std::vector<double>(nsites_ * 4, 0.0);
    sys_phi_ = std::vector<double>(nsites_, 0.0);
    phi_ = std::vector<double>(nsites_, 0.0);
    sys_Efq_ = std::vector<double>(nsites3, 0.0);
    sys_Efd_ = std::vector<double>(nsites3, 0.0);
    Efq_ = std::vector<double>(nsites3, 0.0);
    Efd_ = std::vector<double>(nsites3, 0.0);
    sys_mu_ = std::vector<double>(nsites3, 0.0);
    mu_ = std::vector<double>(nsites3, 0.0);
    sys_perm_mu_ = std::vector<double>(nsites3, 0.0);
    xyz_ = std::vector<double>(nsites3, 0.0);
    grad_ = std::vector<double>(nsites3, 0.0);
    sys_grad_ = std::vector<double>(nsites3, 0.0);
    chg_ = std::vector<double>(nsites_, 0.0);
    pol_sqrt_ = std::vector<double>(nsites3, 0.0);
    virial_ = std::vector<double>(9, 0.0);
    islocal_atom_ = std::vector<size_t>(nsites_, 0.0);
    islocal_atom_xyz_ = std::vector<size_t>(nsites3, 0.0);
    atom_tag_ = std::vector<int>(nsites_, 0);
    external_charge_.clear();
    external_charge_xyz_.clear();
    external_islocal_.clear();
    external_tag_.clear();
    aCC_ = 0.4;
    aCD_ = 0.4;
    aDD_ = 0.055;
#if NO_THOLE
    aCC_ = 99999;
    aCD_ = 99999;
    aDD_ = 99999;
#endif
    g34_ = std::exp(gammln(0.75));
    aCC1_4_ = std::pow(aCC_, 0.25);

    // ASPC parameters
    hist_num_aspc_ = 0;
    // TODO k is defaulted to 4 for now
    SetAspcParameters(6);
    mu_pred_ = std::vector<double>(nsites3, 0.0);

    if (use_pbc_) box_inverse_ = InvertUnitCell(box_);
    ReorderData();

    has_energy_ = false;

    nmon_total_ = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        nmon_total_ += mon_type_count_[mt].second;
    }

    sys_mol_perm_mu_ = std::vector<double>(3 * nmon_total_, 0.0);
    sys_mol_mu_ = std::vector<double>(3 * nmon_total_, 0.0);

    calc_virial_ = true;
    if (do_grads == false) {
        calc_virial_ = false;
    }

    if (!mpi_initialized_) {
        world_ = 0;
        mpi_rank_ = 0;
        proc_grid_x_ = 1;
        proc_grid_y_ = 1;
        proc_grid_z_ = 1;
        num_mpi_ranks_ = 1;
    }

    mbxt_ele_count_ = std::vector<size_t>(ELE_NUM_TIMERS, 0);
    mbxt_ele_time_ = std::vector<double>(ELE_NUM_TIMERS, 0.0);

    first = true;

    nn_first = true;
    nn_first_neigh = std::vector<size_t>(nsites_, -1);
    nn_num_neighs = std::vector<size_t>(nsites_, 0);

    nn_first_ext = true;
    nn_first_neigh_ext = std::vector<size_t>(external_charge_.size(), -1);
    nn_num_neighs_ext = std::vector<size_t>(external_charge_.size(), 0);

    user_fft_grid_ = std::vector<int>{};

    nsites_all_ = nsites_;
    if (nsites_ + external_charge_.size() > 0) {
        size_t nExtChg = external_charge_.size();

        // Define sizes.
        Efq_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        xyz_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        phi_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        chg_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        polfac_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        sites_all_ = std::vector<size_t>(nExtChg + mon_id_.size(), 1);
        mon_id_all_ = std::vector<std::string>(nExtChg + mon_id_.size(), "ext");
        islocal_all_ = std::vector<size_t>(nExtChg + nmon_total_, 1);
        islocal_atom_all_ = std::vector<size_t>(nExtChg + nsites_, 1);
        atom_tag_all_ = std::vector<int>(nExtChg + nsites_, 0);  // should only need nExtChg
        sys_xyz_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        sys_chg_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        rec_phi_and_field_all_ = std::vector<double>((nExtChg + nsites_) * 4, 0.0);
        external_charge_grads_ = std::vector<double>(3 * nExtChg, 0.0);

        nsites_all_ = nsites_ + nExtChg;

        // Fill the vectors. NOTE. All of them in internal order
        for (size_t i = 0; i < 3 * nsites_; i++) {
            xyz_all_[i] = xyz_[i];
            sys_xyz_all_[i] = sys_xyz_[i];
        }

        for (size_t i = 0; i < nsites_; i++) {
            chg_all_[i] = chg_[i];
            sys_chg_all_[i] = sys_chg_[i];
            polfac_all_[i] = polfac_[i];
            islocal_atom_all_[i] = islocal_atom_[i];
            atom_tag_all_[i] = atom_tag_[i];
        }

        for (size_t i = 0; i < nmon_total_; ++i) {
            islocal_all_[i] = islocal_[i];
        }

        for (size_t i = 0; i < mon_id_.size(); i++) {
            sites_all_[i] = sites_[i];
            mon_id_all_[i] = mon_id_[i];
        }

        for (size_t i = 0; i < nExtChg; i++) {
            xyz_all_[3 * nsites_ + i] = external_charge_xyz_[3 * i];
            xyz_all_[3 * nsites_ + nExtChg + i] = external_charge_xyz_[3 * i + 1];
            xyz_all_[3 * nsites_ + 2 * nExtChg + i] = external_charge_xyz_[3 * i + 2];
            sys_xyz_all_[3 * nsites_ + 3 * i] = external_charge_xyz_[3 * i];
            sys_xyz_all_[3 * nsites_ + 3 * i + 1] = external_charge_xyz_[3 * i + 1];
            sys_xyz_all_[3 * nsites_ + 3 * i + 2] = external_charge_xyz_[3 * i + 2];
        }

        for (size_t i = 0; i < nExtChg; i++) {
            sys_chg_all_[nsites_ + i] = external_charge_[i];
            chg_all_[nsites_ + i] = external_charge_[i];
            islocal_all_[nmon_total_ + i] = external_islocal_[i];
            islocal_atom_all_[nsites_ + i] = external_islocal_[i];
            atom_tag_all_[nsites_ + i] = external_tag_[i];
        }
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Internal xyz:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal charge:\n";
    for (size_t i = 0; i < chg_.size(); i++) {
        std::cerr << chg_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal pol_sqrt:\n";
    for (size_t i = 0; i < pol_sqrt_.size(); i++) {
        std::cerr << pol_sqrt_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal islocal_atom:\n";
    for (size_t i = 0; i < islocal_atom_.size(); i++) {
        std::cerr << islocal_atom_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal islocal_atom_xyz:\n";
    for (size_t i = 0; i < islocal_atom_xyz_.size(); i++) {
        std::cerr << islocal_atom_xyz_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal Atom tag:\n";
    for (size_t i = 0; i < atom_tag_.size(); i++) {
        std::cerr << atom_tag_[i] << " , ";
    }
    std::cerr << std::endl;
#endif
}

void Electrostatics::SetMPI(MPI_Comm world, size_t proc_grid_x, size_t proc_grid_y, size_t proc_grid_z) {
    mpi_initialized_ = true;
    world_ = world;
    proc_grid_x_ = proc_grid_x;
    proc_grid_y_ = proc_grid_y;
    proc_grid_z_ = proc_grid_z;
#if HAVE_MPI == 1
    MPI_Comm_rank(world_, &mpi_rank_);
#else
    mpi_rank_ = 0;
#endif
    num_mpi_ranks_ = proc_grid_x_ * proc_grid_y_ * proc_grid_z_;
}

void Electrostatics::SetNewParameters(const std::vector<double> &xyz, const std::vector<double> &chg,
                                      const std::vector<double> &chg_grad, const std::vector<double> &pol,
                                      const std::vector<double> &polfac, const std::string dip_method,
                                      const bool do_grads, const std::vector<double> &box, const double cutoff) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Charges:\n";
    for (size_t i = 0; i < chg.size(); i++) {
        std::cerr << chg[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Charge derivatives:\n";
    for (size_t i = 0; i < chg_grad.size(); i++) {
        std::cerr << chg_grad[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Polfacs:\n";
    for (size_t i = 0; i < polfac.size(); i++) {
        std::cerr << polfac[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Pols:\n";
    for (size_t i = 0; i < pol.size(); i++) {
        std::cerr << pol[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Sys Xyz:\n";
    for (size_t i = 0; i < xyz.size(); i++) {
        std::cerr << xyz[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "do grads: " << do_grads << std::endl;
    std::cerr << "cutoff: " << cutoff << std::endl;
    std::cerr << "dipole method: " << dip_method << std::endl;

    std::cerr << "box:\n";
    for (size_t i = 0; i < box.size(); i++) {
        std::cerr << box[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    sys_chg_ = chg;
    sys_chg_grad_ = chg_grad;
    polfac_ = polfac;
    pol_ = pol;
    sys_xyz_ = xyz;
    do_grads_ = do_grads;
    dip_method_ = dip_method;
    box_ = box;
    use_pbc_ = box.size();
    cutoff_ = cutoff;
    box_ABCabc_ = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
    if (use_pbc_) box_inverse_ = InvertUnitCell(box_);

    size_t nsites3 = nsites_ * 3;

    if (nsites_ > 0) {
        std::fill(sys_phi_.begin(), sys_phi_.end(), 0.0);
        std::fill(phi_.begin(), phi_.end(), 0.0);
        std::fill(sys_Efq_.begin(), sys_Efq_.end(), 0.0);
        std::fill(sys_Efd_.begin(), sys_Efd_.end(), 0.0);
        std::fill(Efq_.begin(), Efq_.end(), 0.0);
        std::fill(Efd_.begin(), Efd_.end(), 0.0);
        std::fill(sys_mu_.begin(), sys_mu_.end(), 0.0);
        std::fill(mu_.begin(), mu_.end(), 0.0);
        std::fill(grad_.begin(), grad_.end(), 0.0);
        std::fill(sys_grad_.begin(), sys_grad_.end(), 0.0);

        std::fill(mu_pred_.begin(), mu_pred_.end(), 0.0);
    }

    ReorderData();

    nsites_all_ = nsites_;
    if (nsites_ + external_charge_.size() > 0) {
        size_t nExtChg = external_charge_.size();

        // Define sizes.
        Efq_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        xyz_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        phi_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        chg_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        polfac_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        sites_all_ = std::vector<size_t>(nExtChg + mon_id_.size(), 1);
        mon_id_all_ = std::vector<std::string>(nExtChg + mon_id_.size(), "ext");
        islocal_all_ = std::vector<size_t>(nExtChg + nmon_total_, 1);
        islocal_atom_all_ = std::vector<size_t>(nExtChg + nsites_, 1);
        atom_tag_all_ = std::vector<int>(nExtChg + nsites_, 0);  // should only need nExtChg
        sys_xyz_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
        sys_chg_all_ = std::vector<double>(nExtChg + nsites_, 0.0);
        rec_phi_and_field_all_ = std::vector<double>((nExtChg + nsites_) * 4, 0.0);
        external_charge_grads_ = std::vector<double>(3 * nExtChg, 0.0);

        nn_first_neigh_ext = std::vector<size_t>(nExtChg, -1);
        nn_num_neighs_ext = std::vector<size_t>(nExtChg, 0);

        nsites_all_ = nsites_ + nExtChg;

        // Fill the vectors. NOTE. All of them in internal order
        for (size_t i = 0; i < 3 * nsites_; i++) {
            xyz_all_[i] = xyz_[i];
            sys_xyz_all_[i] = sys_xyz_[i];
        }

        for (size_t i = 0; i < nsites_; i++) {
            chg_all_[i] = chg_[i];
            sys_chg_all_[i] = sys_chg_[i];
            polfac_all_[i] = polfac_[i];
            islocal_atom_all_[i] = islocal_atom_[i];
            atom_tag_all_[i] = atom_tag_[i];
        }

        for (size_t i = 0; i < nmon_total_; ++i) {
            islocal_all_[i] = islocal_[i];
        }

        for (size_t i = 0; i < mon_id_.size(); i++) {
            sites_all_[i] = sites_[i];
            mon_id_all_[i] = mon_id_[i];
        }

        for (size_t i = 0; i < nExtChg; i++) {
            xyz_all_[3 * nsites_ + i] = external_charge_xyz_[3 * i];
            xyz_all_[3 * nsites_ + nExtChg + i] = external_charge_xyz_[3 * i + 1];
            xyz_all_[3 * nsites_ + 2 * nExtChg + i] = external_charge_xyz_[3 * i + 2];
            sys_xyz_all_[3 * nsites_ + 3 * i] = external_charge_xyz_[3 * i];
            sys_xyz_all_[3 * nsites_ + 3 * i + 1] = external_charge_xyz_[3 * i + 1];
            sys_xyz_all_[3 * nsites_ + 3 * i + 2] = external_charge_xyz_[3 * i + 2];
        }

        for (size_t i = 0; i < nExtChg; i++) {
            sys_chg_all_[nsites_ + i] = external_charge_[i];
            chg_all_[nsites_ + i] = external_charge_[i];
            islocal_all_[nmon_total_ + i] = external_islocal_.size() ? external_islocal_[i] : 1;
            islocal_atom_all_[nsites_ + i] = external_islocal_.size() ? external_islocal_[i] : 1;
            atom_tag_all_[nsites_ + i] = external_tag_.size() ? external_tag_[i] : 1;
        }
    }

    has_energy_ = false;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Internal xyz:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal charge:\n";
    for (size_t i = 0; i < chg_.size(); i++) {
        std::cerr << chg_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Internal pol_sqrt:\n";
    for (size_t i = 0; i < pol_sqrt_.size(); i++) {
        std::cerr << pol_sqrt_[i] << " , ";
    }
    std::cerr << std::endl;

#endif
}

void Electrostatics::SetBoxPMElocal(std::vector<double> box) {
    box_PMElocal_ = box;
    box_ABCabc_PMElocal_ = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
    box_inverse_PMElocal_ = InvertUnitCell(box);
}

void Electrostatics::ReorderData() {
    ////////////////////////////////////////////////////////////////////////////////
    // DATA ORGANIZATION ///////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Reorganize xyz and charge
    // XYZ will be reorganized from x1 y1 z1 x2 ... to
    // x1x2x3...y1y2y3...z1z2... for mon1, then x1x2x3...y1y2... for mon 2...
    // CHG will be reorganized from cA1cB1cC1... for mon_type 1 then mon2...
    // to cA1cA2...cB1cB2... for mon_type 1, then 2, ...
    // where A,B are the different sites, and 1,2 are the monomer index
    // POL_SQRT will be organized as charges, but three copies of each
    // and square rooted

    // Organize xyz so we have
    // x1_1 x1_2 ... y1_1 y1_2... z1_1 z1_2 ... x2_1 x2_2 ...
    // where xN_M is read as coordinate x of site N of monomer M
    // for the first monomer type. Then follows the second, and so on.
    // As an example, for a water dimer (OHHM), the system xyz would be
    // sys_xyz = {Ox, Oy, Oz, Hx, Hy, Hz, Hx, Hy, ...}
    // And after reordering,
    // xyz = {Ox, Ox, Oy, Oy, Oz, Oz, Hx, Hx, Hy, Hy, Hz, Hz, Hx, Hx, ... ...}

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                xyz_[inmon3 + m + fi_crd] = sys_xyz_[fi_crd + mns3 + 3 * i];
                xyz_[inmon3 + m + fi_crd + nmon] = sys_xyz_[fi_crd + mns3 + 3 * i + 1];
                xyz_[inmon3 + m + fi_crd + nmon2] = sys_xyz_[fi_crd + mns3 + 3 * i + 2];
                chg_[fi_sites + m + inmon] = sys_chg_[fi_sites + mns + i];
                pol_sqrt_[inmon3 + m + fi_crd] = sqrt(pol_[fi_sites + mns + i]);
                pol_sqrt_[inmon3 + m + fi_crd + nmon] = sqrt(pol_[fi_sites + mns + i]);
                pol_sqrt_[inmon3 + m + fi_crd + nmon2] = sqrt(pol_[fi_sites + mns + i]);

                islocal_atom_[fi_sites + m + inmon] = islocal_[fi_mon + m];
                islocal_atom_xyz_[inmon3 + m + fi_crd] = islocal_[fi_mon + m];
                islocal_atom_xyz_[inmon3 + m + fi_crd + nmon] = islocal_[fi_mon + m];
                islocal_atom_xyz_[inmon3 + m + fi_crd + nmon2] = islocal_[fi_mon + m];

                atom_tag_[fi_sites + m + inmon] = sys_atom_tag_[fi_sites + mns + i];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
}

////////////////////////////////////////////////////////////////////////////////
// PERMANENT ELECTRIC FIELD ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Electrostatics::CalculatePermanentElecFieldMPIlocal(bool use_ghost) {
    // MRR modification for external charges
    // MRR EXT
    size_t nExtChg = external_charge_.size();
    std::vector<std::pair<std::string, size_t>> mon_type_count_cp = mon_type_count_;
    if (nExtChg > 0) {
        mon_type_count_.push_back(std::make_pair("ext", nExtChg));
    }

    // Max number of monomers
    size_t maxnmon = (mon_type_count_.size() > 0) ? mon_type_count_.back().second : 1;
    if (nExtChg > maxnmon) maxnmon = nExtChg;
    //    size_t maxnmon = mon_type_count_.back().second > nExtChg ? mon_type_count_.back().second : nExtChg;

    // Max number of monomers
    ElectricFieldHolder elec_field(maxnmon);

    int me;
#if HAVE_MPI == 1
    MPI_Comm_rank(world_, &me);
#else
    me = 0;
#endif

    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    // This part looks at sites inside the same monomer
    // Reset first indexes
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    if (nsites_all_ > 0) {
        std::fill(phi_all_.begin(), phi_all_.end(), 0);
        std::fill(Efq_all_.begin(), Efq_all_.end(), 0);
    }

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Obtain excluded pairs for monomer type mt
        systools::GetExcluded(mon_id_all_[fi_mon], mon_j_, exc12, exc13, exc14);

        // Loop over each pair of sites
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = inmon * 3;
            for (size_t j = i + 1; j < ns; j++) {
                // Continue only if i and j are not bonded
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                double elec_scale_factor = (is12 || is13 || is14) ? 0 : 1;
                // Get a1a2 and check if is not 0.
                double A = polfac_all_[fi_sites + i] * polfac_all_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }
                for (size_t m = 0; m < nmon; m++) {
                    bool include_monomer = false;
                    if (!use_ghost) include_monomer = true;
                    if (use_ghost && islocal_all_[fi_mon + m]) include_monomer = true;

                    if (include_monomer) {
                        elec_field.CalcPermanentElecField(
                            xyz_all_.data() + fi_crd, xyz_all_.data() + fi_crd, chg_all_.data() + fi_sites,
                            chg_all_.data() + fi_sites, m, m, m + 1, nmon, nmon, i, j, Ai, Asqsqi, aCC_, aCC1_4_, g34_,
                            &ex, &ey, &ez, &phi1, phi_all_.data() + fi_sites, Efq_all_.data() + fi_crd,
                            elec_scale_factor, ewald_alpha_, simcell_periodic_, box_PMElocal_, box_inverse_PMElocal_,
                            cutoff_, use_ghost, islocal_all_, fi_mon + m, fi_mon, 0, &virial_);
                        phi_all_[fi_sites + inmon + m] += phi1;
                        Efq_all_[fi_crd + inmon3 + m] += ex;
                        Efq_all_[fi_crd + inmon3 + nmon + m] += ey;
                        Efq_all_[fi_crd + inmon3 + nmon2 + m] += ez;
                    }
                }
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") 1B LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " Efq_= " << Efq_all_[fi_crd + inmon3 + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Sites corresponding to different monomers
    // Declaring first indexes
    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;

    // Loop over all monomer types
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_all_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;

        // For each monomer type mt1, loop over all the other monomer types
        // mt2 >= mt1 to avoid double counting
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_all_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;

            // Check if monomer types 1 and 2 are the same
            // If so, same monomer won't be done, since it has been done in
            // previous loop.
            bool same = (mt1 == mt2);

            // TODO add neighbour list here
            // Loop over all pair of sites

            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> Efq_1_pool;
            std::vector<std::vector<double>> Efq_2_pool;
            std::vector<std::vector<double>> phi_1_pool;
            std::vector<std::vector<double>> phi_2_pool;
            std::vector<std::vector<double>> virial_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                Efq_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                Efq_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
                virial_pool.push_back(std::vector<double>(9, 0.0));
            }

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
            for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                double phi1_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon1 = i * nmon1;
                    size_t inmon13 = inmon1 * 3;

                    std::vector<double> xyz_sitei(3);
                    xyz_sitei[0] = xyz_all_[fi_crd1 + inmon13 + m1];
                    xyz_sitei[1] = xyz_all_[fi_crd1 + inmon13 + nmon1 + m1];
                    xyz_sitei[2] = xyz_all_[fi_crd1 + inmon13 + 2 * nmon1 + m1];

                    for (size_t j = 0; j < ns2; j++) {
                        size_t jnmon2 = j * nmon2;
                        size_t jnmon23 = jnmon2 * 3;
                        // If PBC is activated, get the xyz in vectorized form for
                        // all the monomer2 sites j
                        // What we are going to do here is to get all sites j of all m2
                        // that are close to site i of the monomer m1 we are looking at
                        size_t start_j = fi_crd2 + jnmon23;
                        size_t size_j = nmon2 - m2init;
                        std::vector<double> xyz_sitej(3 * size_j);
                        // Copy x
                        std::copy(xyz_all_.begin() + start_j + m2init, xyz_all_.begin() + start_j + nmon2,
                                  xyz_sitej.begin());
                        // Copy y
                        std::copy(xyz_all_.begin() + start_j + nmon2 + m2init, xyz_all_.begin() + start_j + 2 * nmon2,
                                  xyz_sitej.begin() + size_j);
                        // Copy y
                        std::copy(xyz_all_.begin() + start_j + 2 * nmon2 + m2init,
                                  xyz_all_.begin() + start_j + 3 * nmon2, xyz_sitej.begin() + 2 * size_j);

                        // Vector that will tell the original position of the new sites
                        std::vector<double> chg_sitej(size_j);
                        std::vector<double> phi_sitej(size_j, 0.0);
                        std::vector<double> Efq_sitej(3 * size_j, 0.0);
                        // declare temporary virial for each pair
                        std::vector<double> virial_thread(9, 0.0);

                        std::copy(chg_all_.begin() + fi_sites2 + nmon2 * j + m2init,
                                  chg_all_.begin() + fi_sites2 + nmon2 * (j + 1), chg_sitej.begin());

                        // Check if A = 0 and call the proper field calculation
                        double A = polfac_all_[fi_sites1 + i] * polfac_all_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        double elec_scale_factor = 1;
                        local_field->CalcPermanentElecField(
                            xyz_all_.data() + fi_crd1, xyz_sitej.data(), chg_all_.data() + fi_sites1, chg_sitej.data(),
                            m1, 0, size_j, nmon1, size_j, i, 0, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex_thread, &ey_thread,
                            &ez_thread, &phi1_thread, phi_sitej.data(), Efq_sitej.data(), elec_scale_factor,
                            ewald_alpha_, simcell_periodic_, box_PMElocal_, box_inverse_PMElocal_, cutoff_, use_ghost,
                            islocal_all_, fi_mon1 + m1, fi_mon2, m2init, &virial_thread);

                        // Put proper data in field and electric field of j
                        for (size_t ind = 0; ind < size_j; ind++) {
                            phi_2_pool[rank][jnmon2 + m2init + ind] += phi_sitej[ind];
                            for (size_t dim = 0; dim < 3; dim++) {
                                Efq_2_pool[rank][jnmon23 + nmon2 * dim + m2init + ind] += Efq_sitej[dim * size_j + ind];
                            }
                        }

                        phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                        Efq_1_pool[rank][inmon13 + m1] += ex_thread;
                        Efq_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        Efq_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;

                        // update virial_pool from virial_threads
                        for (size_t k = 0; k < 9; k++) {
                            virial_pool[rank][k] += virial_thread[k];
                        }
                    }
                }  // for(i)
            }      // for(m1)

            // Compress data in Efq and phi
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = Efq_1_pool[rank].size();
                size_t kend2 = Efq_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    Efq_all_[fi_crd1 + k] += Efq_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    Efq_all_[fi_crd2 + k] += Efq_2_pool[rank][k];
                }
                kend1 = phi_1_pool[rank].size();
                kend2 = phi_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    phi_all_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_all_[fi_sites2 + k] += phi_2_pool[rank][k];
                }
                for (size_t k = 0; k < 9; k++) {
                    virial_[k] += virial_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") 2B LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " chg_= " << sys_chg_all_[fi_sites + inmon + m] << " "
                                      << " Efq_= " << Efq_all_[fi_crd + inmon3 + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();
#endif

    bool compute_pme = (ewald_alpha_ > 0 && use_pbc_);

    if (!compute_pme && use_ghost && ewald_alpha_ > 0) compute_pme = true;

    if (!simcell_periodic_) compute_pme = false;

    if (compute_pme) {
#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        // Compute the reciprocal space terms, using PME
        double A, B, C, alpha, beta, gamma;
        if (use_ghost) {
            A = box_ABCabc_PMElocal_[0];
            B = box_ABCabc_PMElocal_[1];
            C = box_ABCabc_PMElocal_[2];
            alpha = box_ABCabc_PMElocal_[3];
            beta = box_ABCabc_PMElocal_[4];
            gamma = box_ABCabc_PMElocal_[5];
        } else {
            A = box_ABCabc_[0];
            B = box_ABCabc_[1];
            C = box_ABCabc_[2];
            alpha = box_ABCabc_[3];
            beta = box_ABCabc_[4];
            gamma = box_ABCabc_[5];
        }
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }

        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);

        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_all_.data(), nsites_all_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_all_.data(), nsites_all_, 1);
        auto result = helpme::Matrix<double>(rec_phi_and_field_all_.data(), nsites_all_, 4);
        std::fill(rec_phi_and_field_all_.begin(), rec_phi_and_field_all_.end(), 0);
#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(0, charges, coords, coords, 1, result);
        mbxt_ele_count_[ELE_PME_PRC]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRC] += MPI_Wtime() - _time1;
#endif

        // Resort phi from system order
        fi_mon = 0;
        fi_sites = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    const double *result_ptr = result[fi_sites + mns + i];
                    phi_all_[fi_sites + inmon + m] += result_ptr[0];
                    Efq_all_[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[1];
                    Efq_all_[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[2];
                    Efq_all_[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[3];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }
        // The Ewald self potential
        // double *phi_ptr = phi_.data();
        // for (const auto &q : chg_) {
        //     *phi_ptr -= 2 * ewald_alpha_ / PIQSRT * q;
        //     ++phi_ptr;
        // }

        for (int i = 0; i < nsites_all_; ++i)
            phi_all_[i] -= 2 * ewald_alpha_ / PIQSRT * chg_all_[i] * islocal_atom_all_[i];
    }

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") PERMDIP PME LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " Efq_= " << Efq_all_[fi_crd + inmon3 + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Copy back improtant imformation
    mon_type_count_ = mon_type_count_cp;
    for (size_t i = 0; i < nsites_; i++) phi_[i] = phi_all_[i];
    for (size_t i = 0; i < 3 * nsites_; i++) Efq_[i] = Efq_all_[i];

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();

    mbxt_ele_count_[ELE_PERMDIP_REAL]++;
    mbxt_ele_time_[ELE_PERMDIP_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_PERMDIP_PME]++;
    mbxt_ele_time_[ELE_PERMDIP_PME] += time3 - time2;
#endif
}

void Electrostatics::CalculatePermanentElecField(bool use_ghost) {
    // MRR modification for external charges
    // MRR EXT
    size_t nExtChg = external_charge_.size();
    std::vector<std::pair<std::string, size_t>> mon_type_count_cp = mon_type_count_;
    if (nExtChg > 0) {
        mon_type_count_.push_back(std::make_pair("ext", nExtChg));
    }

    // Max number of monomers
    size_t maxnmon = (mon_type_count_.size() > 0) ? mon_type_count_.back().second : 1;
    if (nExtChg > maxnmon) maxnmon = nExtChg;
    //    size_t maxnmon = mon_type_count_.back().second > nExtChg ? mon_type_count_.back().second : nExtChg;
    //    if (maxnmon == 0) maxnmon = 1;
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    // This part looks at sites inside the same monomer
    // Reset first indexes
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    std::fill(phi_all_.begin(), phi_all_.end(), 0);
    std::fill(Efq_all_.begin(), Efq_all_.end(), 0);

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Obtain excluded pairs for monomer type mt
        systools::GetExcluded(mon_id_all_[fi_mon], mon_j_, exc12, exc13, exc14);

        // Loop over each pair of sites
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = inmon * 3;
            for (size_t j = i + 1; j < ns; j++) {
                // Continue only if i and j are not bonded
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                double elec_scale_factor = (is12 || is13 || is14) ? 0 : 1;

                // Get a1a2 and check if is not 0.
                double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }
                //                for (size_t m = 0; m < nmon; m++) {
                size_t mstart = (mpi_rank_ < nmon) ? mpi_rank_ : nmon;
                for (size_t m = mstart; m < nmon; m += num_mpi_ranks_) {
                    elec_field.CalcPermanentElecField(
                        xyz_all_.data() + fi_crd, xyz_all_.data() + fi_crd, chg_all_.data() + fi_sites,
                        chg_all_.data() + fi_sites, m, m, m + 1, nmon, nmon, i, j, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex,
                        &ey, &ez, &phi1, phi_all_.data() + fi_sites, Efq_all_.data() + fi_crd, elec_scale_factor,
                        ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_, use_ghost, islocal_, fi_mon + m, fi_mon,
                        fi_mon, &virial_);
                    phi_all_[fi_sites + inmon + m] += phi1;
                    Efq_all_[fi_crd + inmon3 + m] += ex;
                    Efq_all_[fi_crd + inmon3 + nmon + m] += ey;
                    Efq_all_[fi_crd + inmon3 + nmon2 + m] += ez;
                }
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") 1B ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " Efq_= " << Efq_[fi_crd + inmon3 + m] << " "
                                      << Efq_[fi_crd + inmon3 + nmon + m] << " " << Efq_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Sites corresponding to different monomers
    // Declaring first indexes
    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;

    // Loop over all monomer types
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_all_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;

        // For each monomer type mt1, loop over all the other monomer types
        // mt2 >= mt1 to avoid double counting
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_all_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;

            // Check if monomer types 1 and 2 are the same
            // If so, same monomer won't be done, since it has been done in
            // previous loop.
            bool same = (mt1 == mt2);

            // TODO add neighbour list here
            // Loop over all pair of sites

            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> Efq_1_pool;
            std::vector<std::vector<double>> Efq_2_pool;
            std::vector<std::vector<double>> phi_1_pool;
            std::vector<std::vector<double>> phi_2_pool;
            std::vector<std::vector<double>> virial_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                Efq_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                Efq_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
                virial_pool.push_back(std::vector<double>(9, 0.0));
            }

            size_t m1start = (mpi_rank_ < nmon1) ? mpi_rank_ : nmon1;
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
            for (size_t m1 = m1start; m1 < nmon1; m1 += num_mpi_ranks_) {
                //	    for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                double phi1_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon1 = i * nmon1;
                    size_t inmon13 = inmon1 * 3;

                    std::vector<double> xyz_sitei(3);
                    xyz_sitei[0] = xyz_all_[fi_crd1 + inmon13 + m1];
                    xyz_sitei[1] = xyz_all_[fi_crd1 + inmon13 + nmon1 + m1];
                    xyz_sitei[2] = xyz_all_[fi_crd1 + inmon13 + 2 * nmon1 + m1];

                    for (size_t j = 0; j < ns2; j++) {
                        size_t jnmon2 = j * nmon2;
                        size_t jnmon23 = jnmon2 * 3;
                        // If PBC is activated, get the xyz in vectorized form for
                        // all the monomer2 sites j
                        // What we are going to do here is to get all sites j of all m2
                        // that are close to site i of the monomer m1 we are looking at
                        size_t start_j = fi_crd2 + jnmon23;
                        size_t size_j = nmon2 - m2init;
                        std::vector<double> xyz_sitej(3 * size_j);
                        // Copy x
                        std::copy(xyz_all_.begin() + start_j + m2init, xyz_all_.begin() + start_j + nmon2,
                                  xyz_sitej.begin());
                        // Copy y
                        std::copy(xyz_all_.begin() + start_j + nmon2 + m2init, xyz_all_.begin() + start_j + 2 * nmon2,
                                  xyz_sitej.begin() + size_j);
                        // Copy y
                        std::copy(xyz_all_.begin() + start_j + 2 * nmon2 + m2init,
                                  xyz_all_.begin() + start_j + 3 * nmon2, xyz_sitej.begin() + 2 * size_j);

                        // Vector that will tell the original position of the new sites
                        std::vector<double> chg_sitej(size_j);
                        std::vector<double> phi_sitej(size_j, 0.0);
                        std::vector<double> Efq_sitej(3 * size_j, 0.0);
                        // declare temporary virial for each pair
                        std::vector<double> virial_thread(9, 0.0);

                        std::copy(chg_all_.begin() + fi_sites2 + nmon2 * j + m2init,
                                  chg_all_.begin() + fi_sites2 + nmon2 * (j + 1), chg_sitej.begin());

                        // Check if A = 0 and call the proper field calculation
                        double A = polfac_all_[fi_sites1 + i] * polfac_all_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        double elec_scale_factor = 1;
                        local_field->CalcPermanentElecField(
                            xyz_all_.data() + fi_crd1, xyz_sitej.data(), chg_all_.data() + fi_sites1, chg_sitej.data(),
                            m1, 0, size_j, nmon1, size_j, i, 0, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex_thread, &ey_thread,
                            &ez_thread, &phi1_thread, phi_sitej.data(), Efq_sitej.data(), elec_scale_factor,
                            ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_, use_ghost, islocal_all_, fi_mon1 + m1,
                            fi_mon2, m2init, &virial_thread);

                        // Put proper data in field and electric field of j
                        for (size_t ind = 0; ind < size_j; ind++) {
                            phi_2_pool[rank][jnmon2 + m2init + ind] += phi_sitej[ind];
                            for (size_t dim = 0; dim < 3; dim++) {
                                Efq_2_pool[rank][jnmon23 + nmon2 * dim + m2init + ind] += Efq_sitej[dim * size_j + ind];
                            }
                        }

                        phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                        Efq_1_pool[rank][inmon13 + m1] += ex_thread;
                        Efq_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        Efq_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;

                        // update virial_pool from virial_threads
                        for (size_t k = 0; k < 9; k++) {
                            virial_pool[rank][k] += virial_thread[k];
                        }
                    }
                }
            }
            // Compress data in Efq and phi
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = Efq_1_pool[rank].size();
                size_t kend2 = Efq_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    Efq_all_[fi_crd1 + k] += Efq_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    Efq_all_[fi_crd2 + k] += Efq_2_pool[rank][k];
                }
                kend1 = phi_1_pool[rank].size();
                kend2 = phi_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    phi_all_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_all_[fi_sites2 + k] += phi_2_pool[rank][k];
                }
                for (size_t k = 0; k < 9; k++) {
                    virial_[k] += virial_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

#if HAVE_MPI == 1
    MPI_Allreduce(MPI_IN_PLACE, phi_.data(), phi_.size(), MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, Efq_.data(), Efq_.size(), MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, virial_.data(), virial_.size(), MPI_DOUBLE, MPI_SUM, world_);
    double time2 = MPI_Wtime();
#endif

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") 2B ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " chg_= " << sys_chg_[fi_sites + inmon + m] << " "
                                      << " Efq_= " << Efq_[fi_crd + inmon3 + m] << " "
                                      << Efq_[fi_crd + inmon3 + nmon + m] << " " << Efq_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    if (ewald_alpha_ > 0 && use_pbc_) {
#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        // Compute the reciprocal space terms, using PME
        double A, B, C, alpha, beta, gamma;
        A = box_ABCabc_[0];
        B = box_ABCabc_[1];
        C = box_ABCabc_[2];
        alpha = box_ABCabc_[3];
        beta = box_ABCabc_[4];
        gamma = box_ABCabc_[5];

        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);
        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_all_.data(), nsites_all_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_all_.data(), nsites_all_, 1);
        auto result = helpme::Matrix<double>(rec_phi_and_field_all_.data(), nsites_all_, 4);
        std::fill(rec_phi_and_field_all_.begin(), rec_phi_and_field_all_.end(), 0);

#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(0, charges, coords, coords, 1, result);
        mbxt_ele_count_[ELE_PME_PRC]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRC] += MPI_Wtime() - _time1;
#endif

#if HAVE_MPI == 1
        MPI_Allreduce(MPI_IN_PLACE, rec_phi_and_field_.data(), rec_phi_and_field_.size(), MPI_DOUBLE, MPI_SUM, world_);
#endif

        // Resort phi from system order
        fi_mon = 0;
        fi_sites = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    const double *result_ptr = result[fi_sites + mns + i];
                    phi_all_[fi_sites + inmon + m] += result_ptr[0];
                    Efq_all_[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[1];
                    Efq_all_[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[2];
                    Efq_all_[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[3];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }
        // The Ewald self potential
        double *phi_ptr = phi_all_.data();
        for (const auto &q : chg_all_) {
            *phi_ptr -= 2 * ewald_alpha_ / PIQSRT * q;
            ++phi_ptr;
        }
    }

#ifdef _DEBUG_PERM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") PERMDIP PME ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " Efq_= " << Efq_[fi_crd + inmon3 + m] << " "
                                      << Efq_[fi_crd + inmon3 + nmon + m] << " " << Efq_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();

    mbxt_ele_count_[ELE_PERMDIP_REAL]++;
    mbxt_ele_time_[ELE_PERMDIP_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_PERMDIP_PME]++;
    mbxt_ele_time_[ELE_PERMDIP_PME] += time3 - time2;
#endif

    // Copy back improtant imformation
    mon_type_count_ = mon_type_count_cp;
    for (size_t i = 0; i < nsites_; i++) phi_[i] = phi_all_[i];
    for (size_t i = 0; i < 3 * nsites_; i++) Efq_[i] = Efq_all_[i];
}

////////////////////////////////////////////////////////////////////////////////
// DIPOLE ELECTRIC FIELD ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Electrostatics::CalculateDipolesMPIlocal(bool use_ghost) {
#if DIRECT_ONLY
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            double p = pol_[fi_sites + i];
            size_t inmon3 = 3 * i * nmon;
            for (size_t m = 0; m < nmon; m++) {
                mu_[fi_crd + inmon3 + m] = p * Efq_[fi_crd + inmon3 + m];
                mu_[fi_crd + inmon3 + nmon + m] = p * Efq_[fi_crd + inmon3 + nmon + m];
                mu_[fi_crd + inmon3 + nmon2 + m] = p * Efq_[fi_crd + inmon3 + nmon2 + m];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    return;
#endif
    if (dip_method_ == "iter") {
        // CalculateDipolesIterative();

        std::string text = std::string("CalculateDipolesIterativeMPIlocal missing. ");
        throw CUException(__func__, __FILE__, __LINE__, text);

    } else if (dip_method_ == "cg") {
        CalculateDipolesCGMPIlocal(use_ghost);
    } else if (dip_method_ == "aspc") {
        CalculateDipolesAspcMPIlocal(use_ghost);
    }
}

void Electrostatics::CalculateDipoles() {
#if DIRECT_ONLY
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            double p = pol_[fi_sites + i];
            size_t inmon3 = 3 * i * nmon;
            for (size_t m = 0; m < nmon; m++) {
                mu_[fi_crd + inmon3 + m] = p * Efq_[fi_crd + inmon3 + m];
                mu_[fi_crd + inmon3 + nmon + m] = p * Efq_[fi_crd + inmon3 + nmon + m];
                mu_[fi_crd + inmon3 + nmon2 + m] = p * Efq_[fi_crd + inmon3 + nmon2 + m];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    return;
#endif
    if (dip_method_ == "iter")
        CalculateDipolesIterative();
    else if (dip_method_ == "cg")
        CalculateDipolesCG();
    else if (dip_method_ == "aspc")
        CalculateDipolesAspc();
}

void Electrostatics::DipolesCGIterationMPIlocal(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost) {
    // Apply sqrt(pol) to the dipoles
    int fi_sites = 0;
    int fi_crd = 0;
    int fi_mon = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon3 = 3 * i * nmon;
            double A = -std::sqrt(pol_[fi_sites + i]);
            for (size_t m = 0; m < nmon; m++) {
                in_v[fi_crd + inmon3 + m] *= A;
                in_v[fi_crd + inmon3 + nmon + m] *= A;
                in_v[fi_crd + inmon3 + nmon2 + m] *= A;
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    // Compute the field from the modified dipoles
    ComputeDipoleFieldMPIlocal(in_v, out_v, use_ghost);

    // Apply sqrt(pol) to the field product, and revert the changes to mu
    fi_sites = 0;
    fi_crd = 0;
    fi_mon = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon3 = 3 * i * nmon;
            double A = std::sqrt(pol_[fi_sites + i]);
            double Ai = A == 0 ? 1 : -1 / A;
            for (size_t m = 0; m < nmon; m++) {
                out_v[fi_crd + inmon3 + m] *= A;
                out_v[fi_crd + inmon3 + nmon + m] *= A;
                out_v[fi_crd + inmon3 + nmon2 + m] *= A;
                // Revert scaling of mu
                in_v[fi_crd + inmon3 + m] *= Ai;
                in_v[fi_crd + inmon3 + nmon + m] *= Ai;
                in_v[fi_crd + inmon3 + nmon2 + m] *= Ai;
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    for (size_t i = 0; i < in_v.size(); i++) {
        out_v[i] += in_v[i];
    }
}

void Electrostatics::DipolesCGIteration(std::vector<double> &in_v, std::vector<double> &out_v) {
    // Apply sqrt(pol) to the dipoles
    int fi_sites = 0;
    int fi_crd = 0;
    int fi_mon = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon3 = 3 * i * nmon;
            double A = -std::sqrt(pol_[fi_sites + i]);
            for (size_t m = 0; m < nmon; m++) {
                in_v[fi_crd + inmon3 + m] *= A;
                in_v[fi_crd + inmon3 + nmon + m] *= A;
                in_v[fi_crd + inmon3 + nmon2 + m] *= A;
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    // Compute the field from the modified dipoles
    ComputeDipoleField(in_v, out_v);

    // Apply sqrt(pol) to the field product, and revert the changes to mu
    fi_sites = 0;
    fi_crd = 0;
    fi_mon = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon3 = 3 * i * nmon;
            double A = std::sqrt(pol_[fi_sites + i]);
            double Ai = A == 0 ? 1 : -1 / A;
            for (size_t m = 0; m < nmon; m++) {
                out_v[fi_crd + inmon3 + m] *= A;
                out_v[fi_crd + inmon3 + nmon + m] *= A;
                out_v[fi_crd + inmon3 + nmon2 + m] *= A;
                // Revert scaling of mu
                in_v[fi_crd + inmon3 + m] *= Ai;
                in_v[fi_crd + inmon3 + nmon + m] *= Ai;
                in_v[fi_crd + inmon3 + nmon2 + m] *= Ai;
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    for (size_t i = 0; i < in_v.size(); i++) {
        out_v[i] += in_v[i];
    }
}

void Electrostatics::CalculateDipolesCGMPIlocal(bool use_ghost) {
    // Parallelization
    //    size_t nthreads = 1;
    //#   ifdef _OPENMP
    //#     pragma omp parallel // omp_get_num_threads() needs to be inside
    //                          // parallel region to get number of threads
    //      {
    //        if (omp_get_thread_num() == 0)
    //          nthreads = omp_get_num_threads();
    //      }
    //#   endif

    // proxy for sequence of reverse_comm(in_v) to accumulate and forward_comm(in_v) to update
    reverse_forward_comm(Efq_);

    size_t nsites3 = nsites_ * 3;
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    // Permanent electric field is computed
    // Now start computation of dipole through conjugate gradient
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            // TODO assuming pol not site dependant
            double p = pol_[fi_sites + i];
            size_t inmon3 = 3 * i * nmon;
#ifdef _OPENMP
#pragma omp simd
#endif
            for (size_t m = 0; m < nmon; m++) {
                mu_[fi_crd + inmon3 + m] = p * Efq_[fi_crd + inmon3 + m];
                mu_[fi_crd + inmon3 + nmon + m] = p * Efq_[fi_crd + inmon3 + nmon + m];
                mu_[fi_crd + inmon3 + nmon2 + m] = p * Efq_[fi_crd + inmon3 + nmon2 + m];
            }
        }

        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    // The Matrix is completed. Now proceed to CG algorithm
    // Following algorithm from:
    // https://en.wikipedia.org/wiki/Conjugate_gradient_method

#ifdef DEBUG
    for (size_t i = 0; i < nsites3; i++) {
        std::cerr << "mu[" << i << "] = " << mu_[i] << std::endl;
    }
#endif

    std::vector<double> ts2v(nsites3);

    DipolesCGIterationMPIlocal(mu_, ts2v, use_ghost);

    std::vector<double> rv(nsites3);
    std::vector<double> pv(nsites3);
    std::vector<double> r_new(nsites3);

#ifdef _DEBUG_DIPOLE
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") CALCDIP LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " Efq_= " << Efq_[fi_crd + inmon3 + m]
                                      << " " << Efq_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_[fi_crd + inmon3 + nmon2 + m]
                                      << " pol_sqrt_= " << pol_sqrt_[fi_crd + inmon3 + m] << " "
                                      << pol_sqrt_[fi_crd + inmon3 + nmon + m] << " "
                                      << pol_sqrt_[fi_crd + inmon3 + nmon2 + m]
                                      << " t2sv= " << ts2v[fi_crd + inmon3 + m] << " "
                                      << ts2v[fi_crd + inmon3 + nmon + m] << " " << ts2v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    //#   ifdef _OPENMP
    //#     pragma omp parallel for schedule(static)
    //#   endif
    for (size_t i = 0; i < nsites3; i++) {
        pv[i] = Efq_[i] * pol_sqrt_[i] - ts2v[i];
    }
    for (size_t i = 0; i < nsites3; i++) {
        rv[i] = pv[i];
    }

#ifdef DEBUG
    for (size_t i = 0; i < nsites3; i++) {
        std::cout << "ts2v[" << i << "] = " << ts2v[i] << std::endl;
    }
    for (size_t i = 0; i < nsites3; i++) {
        std::cout << "rv[" << i << "] = " << rv[i] << std::endl;
    }
#endif

    // Start iterations
    size_t iter = 1;
    double rvrv = DotProduct(rv, rv);
    double rvrv_global = 0.0;
#if HAVE_MPI == 1
    MPI_Allreduce(&rvrv, &rvrv_global, 1, MPI_DOUBLE, MPI_SUM, world_);
#else
    rvrv_global = rvrv;
#endif
    double residual = 0.0;
    double residual_global = 0.0;
    while (true) {
#ifdef DEBUG
        std::cout << "Iteration: " << iter << std::endl;
#endif

        DipolesCGIterationMPIlocal(pv, ts2v, use_ghost);
        double pvts2pv = DotProduct(pv, ts2v);

        double pvts2pv_global = 0.0;
#if HAVE_MPI == 1
        MPI_Allreduce(&pvts2pv, &pvts2pv_global, 1, MPI_DOUBLE, MPI_SUM, world_);
#else
        pvts2pv_global = pvts2pv;
#endif

        if (rvrv_global < tolerance_) break;
        double alphak = rvrv_global / pvts2pv_global;
        residual = 0.0;
        for (size_t i = 0; i < nsites3; i++) {
            mu_[i] = mu_[i] + alphak * pv[i];
        }
        for (size_t i = 0; i < nsites3; i++) {
            r_new[i] = rv[i] - alphak * ts2v[i];
        }
        for (size_t i = 0; i < nsites3; i++) {
            residual += r_new[i] * r_new[i];
        }

        residual_global = 0.0;
#if HAVE_MPI == 1
        MPI_Allreduce(&residual, &residual_global, 1, MPI_DOUBLE, MPI_SUM, world_);
#else
        residual_global = residual;
#endif
        // Check if converged
        int break_local = 0;
        if (residual_global < tolerance_) break_local = 1;

#if HAVE_MPI == 1
        MPI_Bcast(&break_local, 1, MPI_INT, 0, world_);
#endif
        // std::cout << "residual= " << residual << "  residual_global= " << residual_global << "  break_local= " <<
        // break_local << std::endl;
        if (break_local) break;

        double rvrv_new = residual_global;

#ifdef _DEBUG_ITERATION
        if (iter > _DEBUG_ITERATION) {
            std::cout << "Not converged" << std::endl;
            break;
        }
#endif
        if (iter > maxit_) {
            // Exit with error
            std::cerr << "Max number of iterations reached" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Prepare next iteration
        double betak = rvrv_new / rvrv_global;
        for (size_t i = 0; i < nsites3; i++) {
            pv[i] = r_new[i] + betak * pv[i];
        }
        rvrv_global = rvrv_new;
        rv = r_new;
        iter++;
    }

    // Dipoles are computed
    // Need to recalculate dipole and Efd due to the multiplication of polsqrt
    for (size_t i = 0; i < nsites3; i++) {
        mu_[i] *= pol_sqrt_[i];
#ifdef DEBUG
        std::cerr << "mu_final[" << i << "] = " << mu_[i] << std::endl;
#endif
    }

#ifdef _DEBUG_DIPOLE
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPOLES LOCAL mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " mu_= " << mu_[fi_crd + inmon3 + m]
                                      << " " << mu_[fi_crd + inmon3 + nmon + m] << " "
                                      << mu_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    //    Efd = Efq - 1/pol
}

void Electrostatics::CalculateDipolesCG() {
    // Parallelization
    //    size_t nthreads = 1;
    //#   ifdef _OPENMP
    //#     pragma omp parallel // omp_get_num_threads() needs to be inside
    //                          // parallel region to get number of threads
    //      {
    //        if (omp_get_thread_num() == 0)
    //          nthreads = omp_get_num_threads();
    //      }
    //#   endif

    size_t nsites3 = nsites_ * 3;
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    // Permanent electric field is computed
    // Now start computation of dipole through conjugate gradient
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            // TODO assuming pol not site dependant
            double p = pol_[fi_sites + i];
            size_t inmon3 = 3 * i * nmon;
#ifdef _OPENMP
#pragma omp simd
#endif
            for (size_t m = 0; m < nmon; m++) {
                mu_[fi_crd + inmon3 + m] = p * Efq_[fi_crd + inmon3 + m];
                mu_[fi_crd + inmon3 + nmon + m] = p * Efq_[fi_crd + inmon3 + nmon + m];
                mu_[fi_crd + inmon3 + nmon2 + m] = p * Efq_[fi_crd + inmon3 + nmon2 + m];
            }
        }

        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    // The Matrix is completed. Now proceed to CG algorithm
    // Following algorithm from:
    // https://en.wikipedia.org/wiki/Conjugate_gradient_method

#ifdef DEBUG
    for (size_t i = 0; i < nsites3; i++) {
        std::cerr << "mu[" << i << "] = " << mu_[i] << std::endl;
    }
#endif

    std::vector<double> ts2v(nsites3);

    DipolesCGIteration(mu_, ts2v);

    std::vector<double> rv(nsites3);
    std::vector<double> pv(nsites3);
    std::vector<double> r_new(nsites3);

#ifdef _DEBUG_DIPOLE
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") CALCDIP ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " Efq_= " << Efq_[fi_crd + inmon3 + m]
                                      << " " << Efq_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_[fi_crd + inmon3 + nmon2 + m]
                                      << " pol_sqrt_= " << pol_sqrt_[fi_crd + inmon3 + m] << " "
                                      << pol_sqrt_[fi_crd + inmon3 + nmon + m] << " "
                                      << pol_sqrt_[fi_crd + inmon3 + nmon2 + m]
                                      << " t2sv= " << ts2v[fi_crd + inmon3 + m] << " "
                                      << ts2v[fi_crd + inmon3 + nmon + m] << " " << ts2v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    //#   ifdef _OPENMP
    //#     pragma omp parallel for schedule(static)
    //#   endif
    for (size_t i = 0; i < nsites3; i++) {
        pv[i] = Efq_[i] * pol_sqrt_[i] - ts2v[i];
    }
    for (size_t i = 0; i < nsites3; i++) {
        rv[i] = pv[i];
    }

#ifdef DEBUG
    for (size_t i = 0; i < nsites3; i++) {
        std::cout << "ts2v[" << i << "] = " << ts2v[i] << std::endl;
    }
    for (size_t i = 0; i < nsites3; i++) {
        std::cout << "rv[" << i << "] = " << rv[i] << std::endl;
    }
#endif

    // Start iterations
    size_t iter = 1;
    double rvrv = DotProduct(rv, rv);
    double residual = 0.0;
    while (true) {
#ifdef DEBUG
        std::cout << "Iteration: " << iter << std::endl;
#endif
        DipolesCGIteration(pv, ts2v);
        double pvts2pv = DotProduct(pv, ts2v);

        if (rvrv < tolerance_) break;
        double alphak = rvrv / pvts2pv;
        residual = 0.0;
        for (size_t i = 0; i < nsites3; i++) {
            mu_[i] = mu_[i] + alphak * pv[i];
        }
        for (size_t i = 0; i < nsites3; i++) {
            r_new[i] = rv[i] - alphak * ts2v[i];
        }
        for (size_t i = 0; i < nsites3; i++) {
            residual += r_new[i] * r_new[i];
        }

        double rvrv_new = residual;

        // Check if converged
        if (residual < tolerance_) break;

#ifdef _DEBUG_ITERATION
        if (iter > _DEBUG_ITERATION) {
            std::cout << "Not converged" << std::endl;
            break;
        }
#endif

        if (iter > maxit_) {
            // Exit with error
            std::cerr << "Max number of iterations reached" << std::endl;
            //            std::exit(EXIT_FAILURE);
        }

        // Prepare next iteration
        double betak = rvrv_new / rvrv;
        for (size_t i = 0; i < nsites3; i++) {
            pv[i] = r_new[i] + betak * pv[i];
        }
        rvrv = rvrv_new;
        rv = r_new;
        iter++;
    }

    // Dipoles are computed
    // Need to recalculate dipole and Efd due to the multiplication of polsqrt
    for (size_t i = 0; i < nsites3; i++) {
        mu_[i] *= pol_sqrt_[i];
#ifdef DEBUG
        std::cerr << "mu_final[" << i << "] = " << mu_[i] << std::endl;
#endif
    }

#ifdef _DEBUG_DIPOLE
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPOLES ORIG mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " mu_= " << mu_[fi_crd + inmon3 + m]
                                      << " " << mu_[fi_crd + inmon3 + nmon + m] << " "
                                      << mu_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    //    Efd = Efq - 1/pol
}

void Electrostatics::SetAspcParameters(size_t k) {
    k_aspc_ = k;
    b_consts_aspc_ = std::vector<double>(k + 2, 0.0);
    mu_hist_ = std::vector<double>(mu_.size() * (k + 3), 0.0);

    std::vector<double> a(k + 4, 0.0);
    a[0] = 0.0;
    a[1] = -1.0;

    for (size_t i = 2; i < k + 4; i++) {
        double up_n = i;
        double down_n = 1.0;
        double sign_n = -1.0;
        for (size_t n = 0; n < i - 1; n++) {
            up_n *= k - n;
            down_n *= (k + 3 + n);
            sign_n *= -1;
        }
        a[i] = sign_n * up_n / down_n;
    }

    for (size_t i = 0; i < k + 2; i++) {
        b_consts_aspc_[i] = a[i + 2] - 2 * a[i + 1] + a[i];
    }

    omega_aspc_ = (double(k) + 2.0) / (2.0 * double(k) + 3.0);

    //    if (k == 0) {
    //        b_consts_aspc_[0] = 2.0;
    //        b_consts_aspc_[1] = -1.0;
    //        omega_aspc_ = 2.0 / 3.0;
    //    } else if (k == 1) {
    //        b_consts_aspc_[0] = 2.5;
    //        b_consts_aspc_[1] = -2.0;
    //        b_consts_aspc_[2] = 0.5;
    //        omega_aspc_ = 0.6;
    //    } else if (k == 2) {
    //        b_consts_aspc_[0] = 2.8;
    //        b_consts_aspc_[1] = -2.8;
    //        b_consts_aspc_[2] = 1.2;
    //        b_consts_aspc_[3] = -0.2;
    //        omega_aspc_ = 4.0 / 7.0;
    //    } else if (k == 3) {
    //        b_consts_aspc_[0] = 3.0;
    //        b_consts_aspc_[1] = -24.0 / 7.0;
    //        b_consts_aspc_[2] = 27.0 / 14.0;
    //        b_consts_aspc_[3] = -4.0 / 7.0;
    //        b_consts_aspc_[4] = 1.0 / 14.0;
    //        omega_aspc_ = 5.0 / 9.0;
    //    } else if (k == 4) {
    //        b_consts_aspc_[0] = 22.0 / 7.0;
    //        b_consts_aspc_[1] = -55.0 / 14.0;
    //        b_consts_aspc_[2] = 55.0 / 21.0;
    //        b_consts_aspc_[3] = -22.0 / 21.0;
    //        b_consts_aspc_[4] = 5.0 / 21.0;
    //        b_consts_aspc_[5] = -1.0 / 42.0;
    //        omega_aspc_ = 6.0 / 11.0;
    //    }

    // TODO add exception if k < 0 or k > 4
}

void Electrostatics::ResetAspcHistory() { hist_num_aspc_ = 0; }

std::vector<double> Electrostatics::GetDipoleHistory(size_t indx) {
    // Return selected history of dipoles
    // The internal dipole history needs to be reordered
    // external charges don't have dipoles

    std::vector<double> sys_mu_hist = std::vector<double>(mu_.size(), 0.0);

    if (indx >= hist_num_aspc_) {
        // Exit with error
        std::cerr << "GetDipoleHistory requested indx too large" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    size_t offset_h = indx * mu_.size();

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;

                sys_mu_hist[fi_crd + mns3 + 3 * i] = mu_hist_[offset_h + inmon3 + m + fi_crd];
                sys_mu_hist[fi_crd + mns3 + 3 * i + 1] = mu_hist_[offset_h + inmon3 + m + fi_crd + nmon];
                sys_mu_hist[fi_crd + mns3 + 3 * i + 2] = mu_hist_[offset_h + inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    return sys_mu_hist;
}

void Electrostatics::SetDipoleHistory(size_t indx, std::vector<double> mu_hist) {
    // Reorder dipoles for internal use
    // external charges don't have dipoles

    if (indx >= hist_num_aspc_) {
        // Exit with error
        std::cerr << "SetDipoleHistory requested indx too large" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    size_t offset_h = indx * mu_.size();

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                mu_hist_[offset_h + inmon3 + m + fi_crd] = mu_hist[fi_crd + mns3 + 3 * i];
                mu_hist_[offset_h + inmon3 + m + fi_crd + nmon] = mu_hist[fi_crd + mns3 + 3 * i + 1];
                mu_hist_[offset_h + inmon3 + m + fi_crd + nmon2] = mu_hist[fi_crd + mns3 + 3 * i + 2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
}

void Electrostatics::CalculateDipolesAspc() {
    if (hist_num_aspc_ < k_aspc_ + 2) {
        // TODO do we want to allow iteration?
        CalculateDipolesCG();
        std::copy(mu_.begin(), mu_.end(), mu_hist_.begin() + hist_num_aspc_ * nsites_ * 3);
        hist_num_aspc_++;
    } else {
        // If we have enough history of the dipoles,
        // we will use the predictor corrector step

        // First we get the predictor
        std::fill(mu_pred_.begin(), mu_pred_.end(), 0.0);
        for (size_t i = 0; i < b_consts_aspc_.size(); i++) {
            size_t shift = 3 * nsites_ * (b_consts_aspc_.size() - i - 1);
            for (size_t j = 0; j < 3 * nsites_; j++) {
                mu_pred_[j] += b_consts_aspc_[i] * mu_hist_[shift + j];
            }
        }

        // Now we get the corrector
        // First we set the dipoles to the predictor
        std::copy(mu_pred_.begin(), mu_pred_.end(), mu_.begin());

        // Now we run a single iteration to get the new Efd
        ComputeDipoleField(mu_, Efd_);

        // Now the Electric dipole field is computed, and we update
        // the dipoles to get the corrector
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;
        double alpha = 0.8;
        double alpha_i = 0.2;
        std::vector<double> mu_old = mu_;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t i = 0; i < ns; i++) {
                // TODO assuming pol not site dependant
                double p = pol_[fi_sites + i];
                size_t inmon3 = 3 * i * nmon;
                for (size_t m = 0; m < nmon; m++) {
                    mu_[fi_crd + inmon3 + m] = alpha_i * mu_old[fi_crd + inmon3 + m] +
                                               alpha * p * (Efq_[fi_crd + inmon3 + m] + Efd_[fi_crd + inmon3 + m]);
                    mu_[fi_crd + inmon3 + nmon + m] =
                        alpha_i * mu_old[fi_crd + inmon3 + nmon + m] +
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon + m] + Efd_[fi_crd + inmon3 + nmon + m]);
                    mu_[fi_crd + inmon3 + nmon2 + m] =
                        alpha_i * mu_old[fi_crd + inmon3 + nmon2 + m] +
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon2 + m] + Efd_[fi_crd + inmon3 + nmon2 + m]);
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        // Now we have the corrector in mu_
        // We get the final dipole

        for (size_t j = 0; j < 3 * nsites_; j++) {
            mu_[j] = omega_aspc_ * mu_[j] + (1 - omega_aspc_) * mu_pred_[j];
        }

        // And we update the history

        // Add the new dipole at the end
        std::copy(mu_.begin(), mu_.end(), mu_hist_.begin() + hist_num_aspc_ * nsites_ * 3);
        // Shift the dipoles one position in the history
        std::copy(mu_hist_.begin() + nsites_ * 3, mu_hist_.end(), mu_hist_.begin());

        // hist_num_aspc_ must not be touched here, so we are done

    }  // end if (hist_num_aspc_ < k_aspc_ + 2)
}

void Electrostatics::CalculateDipolesAspcMPIlocal(bool use_ghost) {
    if (hist_num_aspc_ < k_aspc_ + 2) {
        // TODO do we want to allow iteration?
        CalculateDipolesCGMPIlocal(use_ghost);
        std::copy(mu_.begin(), mu_.end(), mu_hist_.begin() + hist_num_aspc_ * nsites_ * 3);
        hist_num_aspc_++;
    } else {
        // If we have enough history of the dipoles,
        // we will use the predictor corrector step

        // First we get the predictor
        std::fill(mu_pred_.begin(), mu_pred_.end(), 0.0);
        for (size_t i = 0; i < b_consts_aspc_.size(); i++) {
            size_t shift = 3 * nsites_ * (b_consts_aspc_.size() - i - 1);
            for (size_t j = 0; j < 3 * nsites_; j++) {
                mu_pred_[j] += b_consts_aspc_[i] * mu_hist_[shift + j];
            }
        }

#ifdef _DEBUG_ASPC
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);
            size_t fi_mon = 0;
            size_t fi_crd = 0;
            size_t fi_sites = 0;

            MPI_Barrier(world_);
            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    // Loop over each monomer type
                    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                        size_t ns = sites_[fi_mon];
                        size_t nmon = mon_type_count_[mt].second;
                        size_t nmon2 = 2 * nmon;

                        // Loop over each pair of sites
                        for (size_t i = 0; i < ns; i++) {
                            size_t inmon = i * nmon;
                            size_t inmon3 = inmon * 3;
                            for (size_t m = 0; m < nmon; m++) {
                                std::cout << "(" << me << ") ASPC PREDICT: mt= " << mt << " i= " << i << " m= " << m
                                          << "  islocal= " << islocal_[fi_mon + m]
                                          << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                          << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                          << xyz_[fi_crd + inmon3 + nmon2 + m]
                                          << " mu_pred_= " << mu_pred_[fi_crd + inmon3 + m] << " "
                                          << mu_pred_[fi_crd + inmon3 + nmon + m] << " "
                                          << mu_pred_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                            }
                        }

                        // Update first indexes
                        fi_mon += nmon;
                        fi_sites += nmon * ns;
                        fi_crd += nmon * ns * 3;
                    }
                }
                MPI_Barrier(world_);
            }
        }  // debug print
#endif

        // Now we get the corrector
        // First we set the dipoles to the predictor
        std::copy(mu_pred_.begin(), mu_pred_.end(), mu_.begin());

        // Now we run a single iteration to get the new Efd
        reverse_forward_comm(Efq_);
        ComputeDipoleFieldMPIlocal(mu_, Efd_, use_ghost);

        // Now the Electric dipole field is computed, and we update
        // the dipoles to get the corrector
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;
        double alpha = 0.8;
        double alpha_i = 0.2;
        std::vector<double> mu_old = mu_;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t i = 0; i < ns; i++) {
                // TODO assuming pol not site dependant
                double p = pol_[fi_sites + i];
                size_t inmon3 = 3 * i * nmon;
                for (size_t m = 0; m < nmon; m++) {
                    mu_[fi_crd + inmon3 + m] = alpha_i * mu_old[fi_crd + inmon3 + m] +
                                               alpha * p * (Efq_[fi_crd + inmon3 + m] + Efd_[fi_crd + inmon3 + m]);
                    mu_[fi_crd + inmon3 + nmon + m] =
                        alpha_i * mu_old[fi_crd + inmon3 + nmon + m] +
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon + m] + Efd_[fi_crd + inmon3 + nmon + m]);
                    mu_[fi_crd + inmon3 + nmon2 + m] =
                        alpha_i * mu_old[fi_crd + inmon3 + nmon2 + m] +
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon2 + m] + Efd_[fi_crd + inmon3 + nmon2 + m]);
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        // Now we have the corrector in mu_
        // We get the final dipole

        for (size_t j = 0; j < 3 * nsites_; j++) {
            mu_[j] = omega_aspc_ * mu_[j] + (1 - omega_aspc_) * mu_pred_[j];
        }

        // And we update the history

        // Add the new dipole at the end
        std::copy(mu_.begin(), mu_.end(), mu_hist_.begin() + hist_num_aspc_ * nsites_ * 3);
        // Shift the dipoles one position in the history
        std::copy(mu_hist_.begin() + nsites_ * 3, mu_hist_.end(), mu_hist_.begin());

        // hist_num_aspc_ must not be touched here, so we are done

    }  // end if (hist_num_aspc_ < k_aspc_ + 2)
}

#if MBX_ELEC_P2P_COMM == 1

void Electrostatics::reverse_forward_comm(std::vector<double> &in_v) {
#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") REVFORCOMM IN LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m]
                                      << " tag= " << atom_tag_[fi_sites + m + inmon] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }

                std::cout << "atom_tag_= ";
                for (int i = 0; i < nsites_; ++i) std::cout << " " << atom_tag_[i];
                std::cout << std::endl;
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // setup proc neighbors and buffers

    if (nn_first) setup_comm();

    if (nn_first)
        reverse_comm_setup(in_v);
    else
        reverse_comm(in_v);

    // zero ghost monomers

    for (int i = 0; i < nsites_ * 3; ++i)
        if (!islocal_atom_xyz_[i]) in_v[i] = 0.0;

    // 2nd nearest-neighbor comm pass

    if (nn_first)
        forward_comm_setup(in_v);
    else
        forward_comm(in_v);

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") REVFORCOMM OUT LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m]
                                      << " tag= " << atom_tag_[fi_sites + m + inmon] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_REVFOR]++;
    mbxt_ele_time_[ELE_COMM_REVFOR] += time2 - time1;
#endif

    nn_first = false;
}

#else

void Electrostatics::reverse_forward_comm(std::vector<double> &in_v) {
#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    // poor-man's reverse_forward communication that doesn't scale...

    int local_size = nsites_;
    int global_size = 0;
#if HAVE_MPI == 1
    MPI_Allreduce(&local_size, &global_size, 1, MPI_INT, MPI_SUM, world_);
#else
    global_size = local_size;
#endif

    int offset = 0;
#if HAVE_MPI == 1
    MPI_Scan(&local_size, &offset, 1, MPI_INT, MPI_SUM, world_);
    offset -= local_size;
#endif

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") REVFORCOMM IN LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m]
                                      << " tag= " << atom_tag_[fi_sites + m + inmon] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }

                std::cout << "(" << mpi_rank_ << ") local_size= " << local_size << " global_size= " << global_size
                          << " offset= " << offset << std::endl;

                std::cout << "atom_tag_= ";
                for (int i = 0; i < nsites_; ++i) std::cout << " " << atom_tag_[i];
                std::cout << std::endl;
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    std::vector<double> in_all(global_size * 3, 0.0);
    std::vector<double> xyz_all(global_size * 3, 0.0);
    std::vector<int> tag_all(global_size, 0);

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;

    size_t indx = offset;
    size_t indx3 = offset * 3;
    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Loop over each pair of sites
        for (size_t i = 0; i < ns; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = inmon * 3;
            for (size_t m = 0; m < nmon; m++) {
                in_all[indx3] = in_v[fi_crd + inmon3 + m];
                in_all[indx3 + 1] = in_v[fi_crd + inmon3 + nmon + m];
                in_all[indx3 + 2] = in_v[fi_crd + inmon3 + nmon2 + m];

                xyz_all[indx3] = xyz_[fi_crd + inmon3 + m];
                xyz_all[indx3 + 1] = xyz_[fi_crd + inmon3 + nmon + m];
                xyz_all[indx3 + 2] = xyz_[fi_crd + inmon3 + nmon2 + m];

                tag_all[indx] = atom_tag_[fi_sites + m + inmon];

                indx++;
                indx3 += 3;
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#if HAVE_MPI == 1
    MPI_Allreduce(MPI_IN_PLACE, in_all.data(), global_size * 3, MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, xyz_all.data(), global_size * 3, MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, tag_all.data(), global_size, MPI_INT, MPI_SUM, world_);
#endif

    const double tolerance = 1e-6;

    for (int i = 0; i < nsites_ * 3; ++i) in_v[i] = 0.0;

    // record lookup table

    if (nn_first) {
        fi_mon = 0;
        fi_crd = 0;
        fi_sites = 0;

        indx = 0;
        int n_indx = 0;
        // Loop over each monomer type
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = 2 * nmon;

            // Loop over each pair of sites
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = inmon * 3;
                for (size_t m = 0; m < nmon; m++) {
                    // test for same position in global list and tally
                    int tagi = atom_tag_[fi_sites + m + inmon];

                    int num_neighs = 0;
                    for (int j = 0; j < global_size; ++j)
                        if (tagi == tag_all[j]) {
                            num_neighs++;
                            nn_neighs.push_back(j * 3);
                        }

                    nn_first_neigh[indx] = n_indx;
                    nn_num_neighs[indx] = num_neighs;

                    indx++;
                    n_indx += num_neighs;
                }
            }

            // Update first indexes
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        nn_first = false;
    }

    // now play back lookup table

    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;

    indx = 0;
    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Loop over each pair of sites
        for (size_t i = 0; i < ns; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = inmon * 3;
            for (size_t m = 0; m < nmon; m++) {
                for (int j = 0; j < nn_num_neighs[indx]; ++j) {
                    in_v[fi_crd + inmon3 + m] += in_all[nn_neighs[nn_first_neigh[indx] + j]];
                    in_v[fi_crd + inmon3 + nmon + m] += in_all[nn_neighs[nn_first_neigh[indx] + j] + 1];
                    in_v[fi_crd + inmon3 + nmon2 + m] += in_all[nn_neighs[nn_first_neigh[indx] + j] + 2];
                }
                indx++;
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") REVFORCOMM OUT LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m]
                                      << " tag= " << atom_tag_[fi_sites + m + inmon] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_REVFOR]++;
    mbxt_ele_time_[ELE_COMM_REVFOR] += time2 - time1;
#endif
}
#endif

// in_v is Efq_all and only updating nExtChg particles
void Electrostatics::reverse_forward_comm_ext(std::vector<double> &in_v) {
#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    // poor-man's reverse_forward communication that doesn't scale...

    int nExtChg = external_charge_.size();

    int local_size = nExtChg;
    int global_size = 0;
#if HAVE_MPI == 1
    MPI_Allreduce(&local_size, &global_size, 1, MPI_INT, MPI_SUM, world_);
#else
    global_size = local_size;
#endif

    int offset = 0;
#if HAVE_MPI == 1
    MPI_Scan(&local_size, &offset, 1, MPI_INT, MPI_SUM, world_);
    offset -= local_size;
#endif

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    if (mt == mon_type_count_.size() - 1) {
                        // Loop over each pair of sites
                        for (size_t i = 0; i < ns; i++) {
                            size_t inmon = i * nmon;
                            size_t inmon3 = inmon * 3;
                            for (size_t m = 0; m < nmon; m++) {
                                std::cout
                                    << "(" << me << ") REVFORCOMM EXT IN LOCAL: mt= " << mt << " i= " << i
                                    << " m= " << m << "  islocal= " << islocal_all_[fi_mon + m]
                                    << " tag= " << atom_tag_all_[fi_sites + m + inmon]
                                    << " indx= " << fi_crd + inmon3 + m << " " << fi_crd + inmon3 + nmon + m << " "
                                    << fi_crd + inmon3 + nmon2 + m << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                    << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                    << xyz_all_[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                    << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                    << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                            }
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }

                std::cout << "(" << mpi_rank_ << ") local_size= " << local_size << " global_size= " << global_size
                          << " offset= " << offset << std::endl;

                std::cout << "atom_tag_= ";
                for (int i = 0; i < nExtChg; ++i) std::cout << " " << atom_tag_all_[nsites_ + i];
                std::cout << std::endl;
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    std::vector<double> in_all(global_size * 3, 0.0);
    std::vector<double> xyz_all(global_size * 3, 0.0);
    std::vector<int> tag_all(global_size, 0);

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;

    size_t indx = offset;
    size_t indx3 = offset * 3;
    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        if (mt == mon_type_count_.size() - 1) {
            // Loop over each pair of sites
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = inmon * 3;
                for (size_t m = 0; m < nmon; m++) {
                    in_all[indx3] = in_v[fi_crd + inmon3 + m];
                    in_all[indx3 + 1] = in_v[fi_crd + inmon3 + nmon + m];
                    in_all[indx3 + 2] = in_v[fi_crd + inmon3 + nmon2 + m];

                    xyz_all[indx3] = xyz_all_[fi_crd + inmon3 + m];
                    xyz_all[indx3 + 1] = xyz_all_[fi_crd + inmon3 + nmon + m];
                    xyz_all[indx3 + 2] = xyz_all_[fi_crd + inmon3 + nmon2 + m];

                    tag_all[indx] = atom_tag_all_[fi_sites + m + inmon];

                    indx++;
                    indx3 += 3;
                }
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#if HAVE_MPI == 1
    MPI_Allreduce(MPI_IN_PLACE, in_all.data(), global_size * 3, MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, xyz_all.data(), global_size * 3, MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, tag_all.data(), global_size, MPI_INT, MPI_SUM, world_);
#endif

    const double tolerance = 1e-6;

    for (int i = nsites_ * 3; i < (nsites_ + nExtChg) * 3; ++i) in_v[i] = 0.0;

    // record lookup table

    if (nn_first_ext) {
        fi_mon = 0;
        fi_crd = 0;
        fi_sites = 0;

        indx = 0;
        int n_indx = 0;
        // Loop over each monomer type
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = 2 * nmon;

            if (mt == mon_type_count_.size() - 1) {
                // Loop over each pair of sites
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = inmon * 3;
                    for (size_t m = 0; m < nmon; m++) {
                        // test for same position in global list and tally
                        int tagi = atom_tag_all_[fi_sites + m + inmon];

                        int num_neighs = 0;
                        for (int j = 0; j < global_size; ++j)
                            if (tagi == tag_all[j]) {
                                num_neighs++;
                                nn_neighs_ext.push_back(j * 3);
                            }

                        nn_first_neigh_ext[indx] = n_indx;
                        nn_num_neighs_ext[indx] = num_neighs;

                        indx++;
                        n_indx += num_neighs;
                    }
                }
            }

            // Update first indexes
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        nn_first_ext = false;
    }

    // now play back lookup table

    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;

    indx = 0;
    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        if (mt == mon_type_count_.size() - 1) {
            // Loop over each pair of sites
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = inmon * 3;
                for (size_t m = 0; m < nmon; m++) {
                    for (int j = 0; j < nn_num_neighs_ext[indx]; ++j) {
                        in_v[fi_crd + inmon3 + m] += in_all[nn_neighs_ext[nn_first_neigh_ext[indx] + j]];
                        in_v[fi_crd + inmon3 + nmon + m] += in_all[nn_neighs_ext[nn_first_neigh_ext[indx] + j] + 1];
                        in_v[fi_crd + inmon3 + nmon2 + m] += in_all[nn_neighs_ext[nn_first_neigh_ext[indx] + j] + 2];
                    }
                    indx++;
                }
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_COMM
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    if (mt == mon_type_count_.size() - 1) {
                        // Loop over each pair of sites
                        for (size_t i = 0; i < ns; i++) {
                            size_t inmon = i * nmon;
                            size_t inmon3 = inmon * 3;
                            for (size_t m = 0; m < nmon; m++) {
                                std::cout
                                    << "(" << me << ") REVFORCOMM EXT OUT LOCAL: mt= " << mt << " i= " << i
                                    << " m= " << m << "  islocal= " << islocal_all_[fi_mon + m]
                                    << " tag= " << atom_tag_all_[fi_sites + m + inmon]
                                    << " indx= " << fi_crd + inmon3 + m << " " << fi_crd + inmon3 + nmon + m << " "
                                    << fi_crd + inmon3 + nmon2 + m << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                    << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                    << xyz_all_[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                    << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                    << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                            }
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_REVFOR]++;
    mbxt_ele_time_[ELE_COMM_REVFOR] += time2 - time1;
#endif
}

void Electrostatics::reverse_comm_setup(std::vector<double> &in_v) {
#if MBX_ELEC_P2P_COMM == 1

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    MPI_Request request[2];
    MPI_Status status[2];

    size_t fi_mon;
    size_t fi_crd;
    size_t fi_sites;

    size_t indx, indx_m;
    size_t offset_m = nncomm_max_send_size / 3;

    // accumulate ghost monomers on neighbor procs my local monomers

    int ncount;
    int *ptr_buf_int;
    double *ptr_buf_double;

    int idim_last = -1;

    for (int iswap = 0; iswap < nncomm_nswap; ++iswap) {
        int idim = nncomm_dim[iswap];

        // pack ghost monomers
        // -- on first transfer along a dimension

        if (idim != idim_last) {
            fi_mon = 0;
            fi_crd = 0;
            fi_sites = 0;

            indx = 0;
            indx_m = offset_m;

            int sl1 = idim * 2;
            int sl2 = sl1 + 1;

            int sl1_indx = 0;
            int sl2_indx = 0;

            // Loop over each monomer type
            for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                size_t ns = sites_[fi_mon];
                size_t nmon = mon_type_count_[mt].second;
                size_t nmon2 = 2 * nmon;

                // Loop over each pair of sites
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = inmon * 3;
                    for (size_t m = 0; m < nmon; m++) {
                        double x = xyz_[fi_crd + inmon3 + m];
                        double y = xyz_[fi_crd + inmon3 + nmon + m];
                        double z = xyz_[fi_crd + inmon3 + nmon2 + m];

                        double coord;
                        if (idim == 0)
                            coord = x * box_inverse_PMElocal_[0] + y * box_inverse_PMElocal_[3] +
                                    z * box_inverse_PMElocal_[6];
                        else if (idim == 1)
                            coord = x * box_inverse_PMElocal_[1] + y * box_inverse_PMElocal_[4] +
                                    z * box_inverse_PMElocal_[7];
                        else
                            coord = x * box_inverse_PMElocal_[2] + y * box_inverse_PMElocal_[5] +
                                    z * box_inverse_PMElocal_[8];

                        if (!islocal_[fi_mon + m] && (coord >= nncomm_cutlo[iswap]) && (coord < nncomm_cuthi[iswap])) {
                            // positive side of sub-domain

                            // pack data for MPI

                            nncomm_buf_send_d[indx * 3] = in_v[fi_crd + inmon3 + m];
                            nncomm_buf_send_d[indx * 3 + 1] = in_v[fi_crd + inmon3 + nmon + m];
                            nncomm_buf_send_d[indx * 3 + 2] = in_v[fi_crd + inmon3 + nmon2 + m];

                            nncomm_buf_send_i[indx] = atom_tag_[fi_sites + m + inmon];

                            // record mapping

                            nncomm_rev_sendlist[sl1].push_back(fi_crd + inmon3 + m);
                            nncomm_rev_sendlist[sl1].push_back(fi_crd + inmon3 + nmon + m);
                            nncomm_rev_sendlist[sl1].push_back(fi_crd + inmon3 + nmon2 + m);

                            indx++;
                        } else if (!islocal_[fi_mon + m] && (coord >= nncomm_cutlo[iswap + 1]) &&
                                   (coord < nncomm_cuthi[iswap + 1])) {
                            // negative side of sub-domain

                            // pack data for MPI

                            nncomm_buf_send_d[indx_m * 3] = in_v[fi_crd + inmon3 + m];
                            nncomm_buf_send_d[indx_m * 3 + 1] = in_v[fi_crd + inmon3 + nmon + m];
                            nncomm_buf_send_d[indx_m * 3 + 2] = in_v[fi_crd + inmon3 + nmon2 + m];

                            nncomm_buf_send_i[indx_m] = atom_tag_[fi_sites + m + inmon];

                            // record mapping

                            nncomm_rev_sendlist[sl2].push_back(fi_crd + inmon3 + m);
                            nncomm_rev_sendlist[sl2].push_back(fi_crd + inmon3 + nmon + m);
                            nncomm_rev_sendlist[sl2].push_back(fi_crd + inmon3 + nmon2 + m);

                            indx_m++;
                        }
                    }
                }

                // Update first indexes
                fi_mon += nmon;
                fi_sites += nmon * ns;
                fi_crd += nmon * ns * 3;
            }  // for(monomer types)

            indx_m -= offset_m;

            idim_last = idim;
        }

#ifdef _DEBUG_COMM
        for (int i = 0; i < num_mpi_ranks_; ++i) {
            if (mpi_rank_ == i) {
                std::cout << "(" << i << ") iswap= " << iswap << "  indx= " << indx << " " << indx_m << std::endl;
            }
            MPI_Barrier(world_);
        }
#endif

        // need to handle multiple cases: 1) send+recv, 2) only send, 3) only recv
        // exchange counts with neighbor procs

        int nrecv;
        if (nncomm_recvproc[iswap] == mpi_rank_) {
            if (nncomm_dir[iswap] == 1)
                nrecv = indx;
            else
                nrecv = indx_m;
        } else {
            MPI_Irecv(&nrecv, 1, MPI_INT, nncomm_recvproc[iswap], 1, world_, &(request[0]));
        }

        int nsend = 0;
        if (nncomm_sendproc[iswap] != mpi_rank_) {
            if (nncomm_send[iswap]) {
                if (nncomm_dir[iswap] == 1)
                    nsend = indx;
                else
                    nsend = indx_m;
            }

            MPI_Send(&nsend, 1, MPI_INT, nncomm_sendproc[iswap], 1, world_);
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) MPI_Wait(&(request[0]), &(status[0]));

        // exchange data with neighbor procs

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            if (nncomm_dir[iswap] == 1) {
                ptr_buf_int = nncomm_buf_send_i.data();
                ptr_buf_double = nncomm_buf_send_d.data();
            } else {
                ptr_buf_int = nncomm_buf_send_i.data() + offset_m;
                ptr_buf_double = nncomm_buf_send_d.data() + offset_m * 3;
            }

        } else {
            if (nrecv > 0) {
                MPI_Irecv(nncomm_buf_recv_i.data(), nrecv, MPI_INT, nncomm_recvproc[iswap], 2, world_, &(request[0]));
                MPI_Irecv(nncomm_buf_recv_d.data(), nrecv * 3, MPI_DOUBLE, nncomm_recvproc[iswap], 3, world_,
                          &(request[1]));
            }
        }

        if (nsend > 0) {
            if (nncomm_dir[iswap] == 1) {
                MPI_Send(nncomm_buf_send_i.data(), indx, MPI_INT, nncomm_sendproc[iswap], 2, world_);
                MPI_Send(nncomm_buf_send_d.data(), indx * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 3, world_);
            } else {
                MPI_Send(nncomm_buf_send_i.data() + offset_m, indx_m, MPI_INT, nncomm_sendproc[iswap], 2, world_);
                MPI_Send(nncomm_buf_send_d.data() + offset_m * 3, indx_m * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 3,
                         world_);
            }
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) {
            if (nrecv > 0) MPI_Waitall(2, request, status);
            ptr_buf_int = nncomm_buf_recv_i.data();
            ptr_buf_double = nncomm_buf_recv_d.data();
        }

#ifdef _DEBUG_COMM
        for (int i = 0; i < num_mpi_ranks_; ++i) {
            if (mpi_rank_ == i) {
                std::cout << "(" << i << ") iswap= " << iswap << "  comm finished" << std::endl;
            }
            MPI_Barrier(world_);
        }
#endif

        // unpack ghost monomers from neighbor procs and accumulate

        int rl_indx = 0;

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            fi_mon = 0;
            fi_crd = 0;
            fi_sites = 0;

            // Loop over each monomer type
            for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                size_t ns = sites_[fi_mon];
                size_t nmon = mon_type_count_[mt].second;
                size_t nmon2 = 2 * nmon;

                // Loop over each pair of sites
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = inmon * 3;
                    for (size_t m = 0; m < nmon; m++) {
                        // is monomer local?
                        if (islocal_[fi_mon + m]) {
                            // test for same position in global list and tally
                            int tagi = atom_tag_[fi_sites + m + inmon];

                            // loop over neighbor procs ghost monomers
                            for (int j = 0; j < nrecv; ++j)
                                if (tagi == ptr_buf_int[j]) {
                                    // accumulate from buffer

                                    in_v[fi_crd + inmon3 + m] += ptr_buf_double[j * 3];
                                    in_v[fi_crd + inmon3 + nmon + m] += ptr_buf_double[j * 3 + 1];
                                    in_v[fi_crd + inmon3 + nmon2 + m] += ptr_buf_double[j * 3 + 2];

                                    // record mapping

                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + m);
                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + nmon + m);
                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + nmon2 + m);
                                    nncomm_rev_recvlist[iswap].push_back(j);
                                }
                        }
                    }
                }

                // Update first indexes
                fi_mon += nmon;
                fi_sites += nmon * ns;
                fi_crd += nmon * ns * 3;
            }  // for(monomers)

        } else {
            if (nrecv > 0) {
                fi_mon = 0;
                fi_crd = 0;
                fi_sites = 0;

                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            // test for same position in global list and tally
                            int tagi = atom_tag_[fi_sites + m + inmon];

                            // loop over neighbor procs ghost monomers
                            for (int j = 0; j < nrecv; ++j)
                                if (tagi == ptr_buf_int[j]) {
                                    // accumulate from buffer

                                    in_v[fi_crd + inmon3 + m] += ptr_buf_double[j * 3];
                                    in_v[fi_crd + inmon3 + nmon + m] += ptr_buf_double[j * 3 + 1];
                                    in_v[fi_crd + inmon3 + nmon2 + m] += ptr_buf_double[j * 3 + 2];

                                    // record mapping

                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + m);
                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + nmon + m);
                                    nncomm_rev_recvlist[iswap].push_back(fi_crd + inmon3 + nmon2 + m);
                                    nncomm_rev_recvlist[iswap].push_back(j);
                                }
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }  // for(monomers)
            }      // if(ncount > 0)
        }

    }  // for(iswap < nswap)

#ifdef _DEBUG_COMM
    for (int i = 0; i < num_mpi_ranks_; ++i) {
        if (mpi_rank_ == i) {
            if (i == 0) {
                std::cout << "\nreverse_comm_setup()::sendlist" << std::endl;
            }

            std::cout << "\n" << std::endl;
            for (int idim = 0; idim < 6; ++idim) {
                std::cout << "(" << i << ") idim= " << idim
                          << " rev_sendlist.size()= " << nncomm_rev_sendlist[idim].size() << std::endl;

                for (int j = 0; j < nncomm_rev_sendlist[idim].size(); j += 3) {
                    std::cout << "   j= " << j / 4 << "  sendlist= " << nncomm_rev_sendlist[idim][j] << " "
                              << nncomm_rev_sendlist[idim][j + 1] << " " << nncomm_rev_sendlist[idim][j + 2] << " "
                              << std::endl;
                    ;
                }
            }
        }
        MPI_Barrier(world_);
    }

    for (int i = 0; i < num_mpi_ranks_; ++i) {
        if (mpi_rank_ == i) {
            if (i == 0) {
                std::cout << "\nreverse_comm_setup()::recvlist" << std::endl;
            }

            std::cout << "\n" << std::endl;
            for (int iswap = 0; iswap < nncomm_nswap; ++iswap) {
                std::cout << "(" << i << ") iswap= " << iswap
                          << " rev_recvlist.size()= " << nncomm_rev_recvlist[iswap].size() << std::endl;

                for (int j = 0; j < nncomm_rev_recvlist[iswap].size(); j += 4) {
                    std::cout << "   j= " << j / 4 << "  recvlist= " << nncomm_rev_recvlist[iswap][j] << " "
                              << nncomm_rev_recvlist[iswap][j + 1] << " " << nncomm_rev_recvlist[iswap][j + 2] << " "
                              << nncomm_rev_recvlist[iswap][j + 3] << std::endl;
                    ;
                }
            }
        }
        MPI_Barrier(world_);
    }
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_REVSET]++;
    mbxt_ele_time_[ELE_COMM_REVSET] += time2 - time1;
#endif

#endif
}

void Electrostatics::reverse_comm(std::vector<double> &in_v) {
#if MBX_ELEC_P2P_COMM == 1

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    MPI_Request request;
    MPI_Status status;

    size_t fi_mon;
    size_t fi_crd;
    size_t fi_sites;

    size_t indx, indx_m;
    size_t offset_m = nncomm_max_send_size / 3;

    // accumulate ghost monomers on neighbor procs my local monomers

    int ncount;
    int *ptr_buf_int;
    double *ptr_buf_double;

    int idim_last = -1;

    for (int iswap = 0; iswap < nncomm_nswap; ++iswap) {
        int idim = nncomm_dim[iswap];

        // pack ghost monomers
        // -- on first transfer along a dimension

        if (idim != idim_last) {
            int sl1 = idim * 2;
            int sl2 = sl1 + 1;

            indx = nncomm_rev_sendlist[sl1].size() / 3;
            for (int i = 0; i < nncomm_rev_sendlist[sl1].size(); ++i) {
                nncomm_buf_send_d[i] = in_v[nncomm_rev_sendlist[sl1][i]];
            }

            indx_m = nncomm_rev_sendlist[sl2].size() / 3;
            for (int i = 0; i < nncomm_rev_sendlist[sl2].size(); ++i) {
                nncomm_buf_send_d[offset_m * 3 + i] = in_v[nncomm_rev_sendlist[sl2][i]];
            }

            idim_last = idim;
        }

#ifdef _DEBUG_COMM
        for (int i = 0; i < num_mpi_ranks_; ++i) {
            if (mpi_rank_ == i) {
                std::cout << "(" << i << ") iswap= " << iswap << "  indx= " << indx << " " << indx_m << std::endl;
            }
            MPI_Barrier(world_);
        }
#endif

        // need to handle multiple cases: 1) send+recv, 2) only send, 3) only recv
        // exchange counts with neighbor procs

        int nrecv;
        if (nncomm_recvproc[iswap] == mpi_rank_) {
            if (nncomm_dir[iswap] == 1)
                nrecv = indx;
            else
                nrecv = indx_m;
        } else {
            MPI_Irecv(&nrecv, 1, MPI_INT, nncomm_recvproc[iswap], 1, world_, &request);
        }

        int nsend = 0;
        if (nncomm_sendproc[iswap] != mpi_rank_) {
            if (nncomm_send[iswap]) {
                if (nncomm_dir[iswap] == 1)
                    nsend = indx;
                else
                    nsend = indx_m;
            }

            MPI_Send(&nsend, 1, MPI_INT, nncomm_sendproc[iswap], 1, world_);
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) MPI_Wait(&request, &status);

        // exchange data with neighbor procs

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            if (nncomm_dir[iswap] == 1) {
                ptr_buf_double = nncomm_buf_send_d.data();
            } else {
                ptr_buf_double = nncomm_buf_send_d.data() + offset_m * 3;
            }

        } else {
            if (nrecv > 0) {
                MPI_Irecv(nncomm_buf_recv_d.data(), nrecv * 3, MPI_DOUBLE, nncomm_recvproc[iswap], 3, world_, &request);
            }
        }

        if (nsend > 0) {
            if (nncomm_dir[iswap] == 1) {
                MPI_Send(nncomm_buf_send_d.data(), indx * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 3, world_);
            } else {
                MPI_Send(nncomm_buf_send_d.data() + offset_m * 3, indx_m * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 3,
                         world_);
            }
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) {
            if (nrecv > 0) MPI_Wait(&request, &status);
            ptr_buf_double = nncomm_buf_recv_d.data();
        }

#ifdef _DEBUG_COMM
        for (int i = 0; i < num_mpi_ranks_; ++i) {
            if (mpi_rank_ == i) {
                std::cout << "(" << i << ") iswap= " << iswap << "  comm finished" << std::endl;
            }
            MPI_Barrier(world_);
        }
#endif

        // unpack ghost monomers from neighbor procs and accumulate

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            for (int i = 0; i < nncomm_rev_recvlist[iswap].size(); i += 4) {
                int j = nncomm_rev_recvlist[iswap][i + 3];
                in_v[nncomm_rev_recvlist[iswap][i]] += ptr_buf_double[j * 3];
                in_v[nncomm_rev_recvlist[iswap][i + 1]] += ptr_buf_double[j * 3 + 1];
                in_v[nncomm_rev_recvlist[iswap][i + 2]] += ptr_buf_double[j * 3 + 2];
            }

        } else {
            if (nrecv > 0) {
                for (int i = 0; i < nncomm_rev_recvlist[iswap].size(); i += 4) {
                    int j = nncomm_rev_recvlist[iswap][i + 3];
                    in_v[nncomm_rev_recvlist[iswap][i]] += ptr_buf_double[j * 3];
                    in_v[nncomm_rev_recvlist[iswap][i + 1]] += ptr_buf_double[j * 3 + 1];
                    in_v[nncomm_rev_recvlist[iswap][i + 2]] += ptr_buf_double[j * 3 + 2];
                }

            }  // if(ncount > 0)
        }

    }  // for(iswap < nswap)

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_REV]++;
    mbxt_ele_time_[ELE_COMM_REV] += time2 - time1;
#endif

#endif
}

void Electrostatics::forward_comm_setup(std::vector<double> &in_v) {
#if MBX_ELEC_P2P_COMM == 1

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    MPI_Request request[2];
    MPI_Status status[2];

    size_t fi_mon;
    size_t fi_crd;
    size_t fi_sites;

    size_t indx, indx_m;
    size_t offset_m = nncomm_max_send_size / 3;

    // accumulate ghost monomers on neighbor procs my local monomers

    int ncount;
    int *ptr_buf_int;
    double *ptr_buf_double;

    for (int iswap = 0; iswap < nncomm_nswap; ++iswap) {
        int idim = nncomm_dim[iswap];

        // need to pack ghost+local monomers in this version

        fi_mon = 0;
        fi_crd = 0;
        fi_sites = 0;

        indx = 0;

        // Loop over each monomer type
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = 2 * nmon;

            // Loop over each pair of sites
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = inmon * 3;
                for (size_t m = 0; m < nmon; m++) {
                    double x = xyz_[fi_crd + inmon3 + m];
                    double y = xyz_[fi_crd + inmon3 + nmon + m];
                    double z = xyz_[fi_crd + inmon3 + nmon2 + m];

                    double coord;
                    if (idim == 0)
                        coord =
                            x * box_inverse_PMElocal_[0] + y * box_inverse_PMElocal_[3] + z * box_inverse_PMElocal_[6];
                    else if (idim == 1)
                        coord =
                            x * box_inverse_PMElocal_[1] + y * box_inverse_PMElocal_[4] + z * box_inverse_PMElocal_[7];
                    else
                        coord =
                            x * box_inverse_PMElocal_[2] + y * box_inverse_PMElocal_[5] + z * box_inverse_PMElocal_[8];

                    if ((coord >= nncomm_cutlo[iswap]) && (coord < nncomm_cuthi[iswap])) {
                        nncomm_buf_send_d[indx * 3] = in_v[fi_crd + inmon3 + m];
                        nncomm_buf_send_d[indx * 3 + 1] = in_v[fi_crd + inmon3 + nmon + m];
                        nncomm_buf_send_d[indx * 3 + 2] = in_v[fi_crd + inmon3 + nmon2 + m];

                        nncomm_buf_send_i[indx] = atom_tag_[fi_sites + m + inmon];

                        // record mapping

                        nncomm_for_sendlist[iswap].push_back(fi_crd + inmon3 + m);
                        nncomm_for_sendlist[iswap].push_back(fi_crd + inmon3 + nmon + m);
                        nncomm_for_sendlist[iswap].push_back(fi_crd + inmon3 + nmon2 + m);

                        indx++;
                    }
                }
            }

            // Update first indexes
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        // need to handle multiple cases: 1) send+recv, 2) only send, 3) only recv
        // exchange counts with neighbor procs

        int nsend = 0;
        if (nncomm_send[iswap]) nsend = indx;

        int nrecv;
        if (nncomm_recvproc[iswap] == mpi_rank_)
            nrecv = nsend;
        else
            MPI_Irecv(&nrecv, 1, MPI_INT, nncomm_recvproc[iswap], 4, world_, &(request[0]));

        if (nncomm_sendproc[iswap] != mpi_rank_) MPI_Send(&nsend, 1, MPI_INT, nncomm_sendproc[iswap], 4, world_);

        if (nncomm_recvproc[iswap] != mpi_rank_) MPI_Wait(&(request[0]), &(status[0]));

        // exchange data with neighbor procs

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            ptr_buf_int = nncomm_buf_send_i.data();
            ptr_buf_double = nncomm_buf_send_d.data();
        } else {
            if (nrecv > 0) {
                MPI_Irecv(nncomm_buf_recv_i.data(), nrecv, MPI_INT, nncomm_recvproc[iswap], 5, world_, &(request[0]));
                MPI_Irecv(nncomm_buf_recv_d.data(), nrecv * 3, MPI_DOUBLE, nncomm_recvproc[iswap], 6, world_,
                          &(request[1]));
            }
        }

        if (nncomm_sendproc[iswap] != mpi_rank_) {
            if (nsend > 0) {
                MPI_Send(nncomm_buf_send_i.data(), nsend, MPI_INT, nncomm_sendproc[iswap], 5, world_);
                MPI_Send(nncomm_buf_send_d.data(), nsend * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 6, world_);
            }
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) {
            if (nrecv > 0) MPI_Waitall(2, request, status);
            ptr_buf_int = nncomm_buf_recv_i.data();
            ptr_buf_double = nncomm_buf_recv_d.data();
        }

        if (nrecv > 0) {
            // set values of ghost monomers on proc

            fi_mon = 0;
            fi_crd = 0;
            fi_sites = 0;

            // Loop over each monomer type
            for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                size_t ns = sites_[fi_mon];
                size_t nmon = mon_type_count_[mt].second;
                size_t nmon2 = 2 * nmon;

                // Loop over each pair of sites
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = inmon * 3;
                    for (size_t m = 0; m < nmon; m++) {
                        // is monomer local?
                        if (!islocal_[fi_mon + m]) {
                            // test for same position in global list and tally
                            int tagi = atom_tag_[fi_sites + m + inmon];

                            // loop over neighbor procs ghost monomers
                            for (int j = 0; j < nrecv; ++j)
                                if (tagi == ptr_buf_int[j] && fabs(in_v[fi_crd + inmon3 + m]) < 1e-9) {
                                    in_v[fi_crd + inmon3 + m] = ptr_buf_double[j * 3];
                                    in_v[fi_crd + inmon3 + nmon + m] = ptr_buf_double[j * 3 + 1];
                                    in_v[fi_crd + inmon3 + nmon2 + m] = ptr_buf_double[j * 3 + 2];

                                    // record mapping

                                    nncomm_for_recvlist[iswap].push_back(fi_crd + inmon3 + m);
                                    nncomm_for_recvlist[iswap].push_back(fi_crd + inmon3 + nmon + m);
                                    nncomm_for_recvlist[iswap].push_back(fi_crd + inmon3 + nmon2 + m);
                                    nncomm_for_recvlist[iswap].push_back(j);
                                }
                        }
                    }
                }

                // Update first indexes
                fi_mon += nmon;
                fi_sites += nmon * ns;
                fi_crd += nmon * ns * 3;
            }  // for(monomer types)
        }      // if(ncount > 0)

    }  // for(iswap < nncomm_nswap)

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_FORSET]++;
    mbxt_ele_time_[ELE_COMM_FORSET] += time2 - time1;
#endif
#endif
}

void Electrostatics::forward_comm(std::vector<double> &in_v) {
#if MBX_ELEC_P2P_COMM == 1

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    MPI_Request request;
    MPI_Status status;

    size_t fi_mon;
    size_t fi_crd;
    size_t fi_sites;

    size_t indx, indx_m;
    size_t offset_m = nncomm_max_send_size / 3;

    // accumulate ghost monomers on neighbor procs my local monomers

    int ncount;
    int *ptr_buf_int;
    double *ptr_buf_double;

    for (int iswap = 0; iswap < nncomm_nswap; ++iswap) {
        int idim = nncomm_dim[iswap];

        indx = nncomm_for_sendlist[iswap].size() / 3;
        for (int i = 0; i < nncomm_for_sendlist[iswap].size(); ++i) {
            nncomm_buf_send_d[i] = in_v[nncomm_for_sendlist[iswap][i]];
        }

        // need to handle multiple cases: 1) send+recv, 2) only send, 3) only recv
        // exchange counts with neighbor procs

        int nsend = 0;
        if (nncomm_send[iswap]) nsend = indx;

        int nrecv;
        if (nncomm_recvproc[iswap] == mpi_rank_)
            nrecv = nsend;
        else
            MPI_Irecv(&nrecv, 1, MPI_INT, nncomm_recvproc[iswap], 4, world_, &request);

        if (nncomm_sendproc[iswap] != mpi_rank_) MPI_Send(&nsend, 1, MPI_INT, nncomm_sendproc[iswap], 4, world_);

        if (nncomm_recvproc[iswap] != mpi_rank_) MPI_Wait(&request, &status);

        // exchange data with neighbor procs

        if (nncomm_recvproc[iswap] == mpi_rank_) {
            ptr_buf_double = nncomm_buf_send_d.data();
        } else {
            if (nrecv > 0) {
                MPI_Irecv(nncomm_buf_recv_d.data(), nrecv * 3, MPI_DOUBLE, nncomm_recvproc[iswap], 6, world_, &request);
            }
        }

        if (nncomm_sendproc[iswap] != mpi_rank_) {
            if (nsend > 0) {
                MPI_Send(nncomm_buf_send_d.data(), nsend * 3, MPI_DOUBLE, nncomm_sendproc[iswap], 6, world_);
            }
        }

        if (nncomm_recvproc[iswap] != mpi_rank_) {
            if (nrecv > 0) MPI_Wait(&request, &status);
            ptr_buf_double = nncomm_buf_recv_d.data();
        }

        if (nrecv > 0) {
            for (int i = 0; i < nncomm_for_recvlist[iswap].size(); i += 4) {
                int j = nncomm_for_recvlist[iswap][i + 3];
                in_v[nncomm_for_recvlist[iswap][i]] += ptr_buf_double[j * 3];
                in_v[nncomm_for_recvlist[iswap][i + 1]] += ptr_buf_double[j * 3 + 1];
                in_v[nncomm_for_recvlist[iswap][i + 2]] += ptr_buf_double[j * 3 + 2];
            }

        }  // if(nrecv > 0)

    }  // for(iswap < nncomm_nswap)

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();

    mbxt_ele_count_[ELE_COMM_FOR]++;
    mbxt_ele_time_[ELE_COMM_FOR] += time2 - time1;
#endif
#endif
}

void Electrostatics::ComputeDipoleFieldMPIlocal(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost) {
    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD IN LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Max number of monomers
    size_t maxnmon = (nsites_ == 0) ? 1 : mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    std::fill(out_v.begin(), out_v.end(), 0);
    double *in_ptr = in_v.data();
    double aDD = 0.0;

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    // Recalculate Electric field due to dipoles
    // Sites on the same monomer
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        //      TODO: Check why this makes shit fail
        //      if (ns == 1) continue;
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;
        // Get excluded pairs for this monomer
        systools::GetExcluded(mon_id_[fi_mon], mon_j_, exc12, exc13, exc14);
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon3 = 3 * i * nmon;
            for (size_t j = i + 1; j < ns; j++) {
                // Set the proper aDD
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                aDD = systools::GetAdd(is12, is13, is14, mon_id_[fi_mon]);
                double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }

                for (size_t m = 0; m < nmon; m++) {
                    bool include_monomer = false;
                    if (!use_ghost) include_monomer = true;
                    if (use_ghost && islocal_[fi_mon + m]) include_monomer = true;

                    // TODO. Slowest function
                    if (include_monomer) {
                        elec_field.CalcDipoleElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd, in_ptr + fi_crd,
                                                       in_ptr + fi_crd, m, m, m + 1, nmon, nmon, i, j, Asqsqi, aDD,
                                                       out_v.data() + fi_crd, &ex, &ey, &ez, ewald_alpha_,
                                                       simcell_periodic_, box_PMElocal_, box_inverse_PMElocal_, cutoff_,
                                                       use_ghost, islocal_, fi_mon + m, fi_mon);
                        out_v[fi_crd + inmon3 + m] += ex;
                        out_v[fi_crd + inmon3 + nmon + m] += ey;
                        out_v[fi_crd + inmon3 + nmon2 + m] += ez;
                    }  // if(include_monomer)
                }      // for(m)
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD 1B LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    size_t fi_mon1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites1 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = 2 * nmon1;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;
            bool same = (mt1 == mt2);
            // TODO add neighbour list here
            // Prepare for parallelization
            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> Efd_1_pool;
            std::vector<std::vector<double>> Efd_2_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                Efd_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                Efd_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
            }

// Parallel loop
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
            for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon13 = 3 * nmon1 * i;
                    for (size_t j = 0; j < ns2; j++) {
                        double A = polfac_[fi_sites1 + i] * polfac_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        local_field->CalcDipoleElecField(
                            xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, in_ptr + fi_crd1, in_ptr + fi_crd2, m1,
                            m2init, nmon2, nmon1, nmon2, i, j, Asqsqi, aDD, Efd_2_pool[rank].data(), &ex_thread,
                            &ey_thread, &ez_thread, ewald_alpha_, simcell_periodic_, box_PMElocal_,
                            box_inverse_PMElocal_, cutoff_, use_ghost, islocal_, fi_mon1 + m1, fi_mon2);

                        Efd_1_pool[rank][inmon13 + m1] += ex_thread;
                        Efd_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        Efd_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                    }
                }
            }

            // Compress data in Efd
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = Efd_1_pool[rank].size();
                size_t kend2 = Efd_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    out_v[fi_crd1 + k] += Efd_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    out_v[fi_crd2 + k] += Efd_2_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

    //   }
    // } // for(ip)

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD 2B LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                      << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();
#endif

    bool compute_pme = (ewald_alpha_ > 0 && use_pbc_);

    if (!compute_pme && use_ghost && ewald_alpha_ > 0) compute_pme = true;

    if (!simcell_periodic_) compute_pme = false;

    if (compute_pme) {
        // Sort the dipoles to the order helPME expects (for now)
        // int fi_mon = 0;
        // int fi_crd = 0;
        fi_mon = 0;
        fi_crd = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                size_t mns3 = mns * 3;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = 3 * inmon;
                    sys_mu_[fi_crd + mns3 + 3 * i] = in_ptr[inmon3 + m + fi_crd];
                    sys_mu_[fi_crd + mns3 + 3 * i + 1] = in_ptr[inmon3 + m + fi_crd + nmon];
                    sys_mu_[fi_crd + mns3 + 3 * i + 2] = in_ptr[inmon3 + m + fi_crd + nmon2];
                }
            }
            fi_mon += nmon;
            fi_crd += nmon * ns * 3;
        }

#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        // Compute the reciprocal space terms, using PME
        double A, B, C, alpha, beta, gamma;
        if (use_ghost) {
            A = box_ABCabc_PMElocal_[0];
            B = box_ABCabc_PMElocal_[1];
            C = box_ABCabc_PMElocal_[2];
            alpha = box_ABCabc_PMElocal_[3];
            beta = box_ABCabc_PMElocal_[4];
            gamma = box_ABCabc_PMElocal_[5];
        } else {
            A = box_ABCabc_[0];
            B = box_ABCabc_[1];
            C = box_ABCabc_[2];
            alpha = box_ABCabc_[3];
            beta = box_ABCabc_[4];
            gamma = box_ABCabc_[5];
        }
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);
        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto result = helpme::Matrix<double>(sys_Efd_.data(), nsites_, 3);
        std::fill(sys_Efd_.begin(), sys_Efd_.end(), 0.0);

#ifdef _DEBUG_DIPFIELD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);
            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (size_t i = 0; i < nsites_; i++) {
                        std::cout << "(" << me << ") DIPFIELD PRec LOCAL: i= " << i << " xyz= " << coords(i, 0) << " "
                                  << coords(i, 1) << " " << coords(i, 2) << " dipoles= " << dipoles(i, 0) << " "
                                  << dipoles(i, 1) << " " << dipoles(i, 2) << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }  // debug print
#endif
#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(-1, dipoles, coords, coords, -1, result);
        mbxt_ele_count_[ELE_PME_PRD]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRD] += MPI_Wtime() - _time1;
#endif

#ifdef _DEBUG_DIPFIELD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);
            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (size_t i = 0; i < nsites_; i++) {
                        std::cout << "(" << me << ") DIPFIELD PRec LOCAL (before Allreduce): i= " << i
                                  << " xyz= " << coords(i, 0) << " " << coords(i, 1) << " " << coords(i, 2)
                                  << " result= " << result(i, 0) << " " << result(i, 1) << " " << result(i, 2)
                                  << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }  // debug print
#endif

        // The Ewald self field due to induced dipoles
        double slf_prefactor = (4.0 / 3.0) * ewald_alpha_ * ewald_alpha_ * ewald_alpha_ / PIQSRT;

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    const double *result_ptr = result[fi_sites + mns + i];
                    out_v[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[0];
                    out_v[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[1];
                    out_v[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }

        for (int i = 0; i < nsites_ * 3; ++i) out_v[i] += slf_prefactor * in_v[i] * islocal_atom_xyz_[i];
    }

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD PME LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                      << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();

    mbxt_ele_count_[ELE_DIPFIELD_REAL]++;
    mbxt_ele_time_[ELE_DIPFIELD_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_DIPFIELD_PME]++;
    mbxt_ele_time_[ELE_DIPFIELD_PME] += time3 - time2;
#endif

    // proxy for reverse_forward_comm(out_v) to accumulate
    reverse_forward_comm(out_v);
}

void Electrostatics::ComputeDipoleField(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost) {
    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            //		std::cout << std::setprecision(15) <<
                            std::cout << "(" << me << ") DIPFIELD IN ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " indx= " << fi_crd + inmon3 + m
                                      << " " << fi_crd + inmon3 + nmon + m << " " << fi_crd + inmon3 + nmon2 + m
                                      << " xyz= " << xyz_[fi_crd + inmon3 + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon + m] << " " << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << " in_v= " << in_v[fi_crd + inmon3 + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon + m] << " " << in_v[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Max number of monomers
    size_t maxnmon = (nsites_ == 0) ? 1 : mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    std::fill(out_v.begin(), out_v.end(), 0);
    double *in_ptr = in_v.data();
    double aDD = 0.0;

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    // Recalculate Electric field due to dipoles
    // Sites on the same monomer
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        //      TODO: Check why this makes shit fail
        //      if (ns == 1) continue;
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;
        // Get excluded pairs for this monomer
        systools::GetExcluded(mon_id_[fi_mon], mon_j_, exc12, exc13, exc14);
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon3 = 3 * i * nmon;
            for (size_t j = i + 1; j < ns; j++) {
                // Set the proper aDD
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                aDD = systools::GetAdd(is12, is13, is14, mon_id_[fi_mon]);
                double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }

                //                for (size_t m = 0; m < nmon; m++) {
                size_t mstart = (mpi_rank_ < nmon) ? mpi_rank_ : nmon;
                for (size_t m = mstart; m < nmon; m += num_mpi_ranks_) {
                    // TODO. Slowest function
                    elec_field.CalcDipoleElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd, in_ptr + fi_crd,
                                                   in_ptr + fi_crd, m, m, m + 1, nmon, nmon, i, j, Asqsqi, aDD,
                                                   out_v.data() + fi_crd, &ex, &ey, &ez, ewald_alpha_, use_pbc_, box_,
                                                   box_inverse_, cutoff_, use_ghost, islocal_, fi_mon + m, fi_mon);
                    out_v[fi_crd + inmon3 + m] += ex;
                    out_v[fi_crd + inmon3 + nmon + m] += ey;
                    out_v[fi_crd + inmon3 + nmon2 + m] += ez;
                }  // for(m)
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD 1B ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    size_t fi_mon1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites1 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = 2 * nmon1;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;
            bool same = (mt1 == mt2);
            // TODO add neighbour list here
            // Prepare for parallelization
            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> Efd_1_pool;
            std::vector<std::vector<double>> Efd_2_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                Efd_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                Efd_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
            }

            // Parallel loop
            size_t m1start = (mpi_rank_ < nmon1) ? mpi_rank_ : nmon1;
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
            for (size_t m1 = m1start; m1 < nmon1; m1 += num_mpi_ranks_) {
                //            for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon13 = 3 * nmon1 * i;
                    for (size_t j = 0; j < ns2; j++) {
                        double A = polfac_[fi_sites1 + i] * polfac_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        local_field->CalcDipoleElecField(xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, in_ptr + fi_crd1,
                                                         in_ptr + fi_crd2, m1, m2init, nmon2, nmon1, nmon2, i, j,
                                                         Asqsqi, aDD, Efd_2_pool[rank].data(), &ex_thread, &ey_thread,
                                                         &ez_thread, ewald_alpha_, use_pbc_, box_, box_inverse_,
                                                         cutoff_, use_ghost, islocal_, fi_mon1 + m1, fi_mon2);
                        Efd_1_pool[rank][inmon13 + m1] += ex_thread;
                        Efd_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        Efd_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                    }
                }
            }

            // Compress data in Efd
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = Efd_1_pool[rank].size();
                size_t kend2 = Efd_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    out_v[fi_crd1 + k] += Efd_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    out_v[fi_crd2 + k] += Efd_2_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

#if HAVE_MPI == 1
    MPI_Allreduce(MPI_IN_PLACE, out_v.data(), out_v.size(), MPI_DOUBLE, MPI_SUM, world_);
    double time2 = MPI_Wtime();
#endif

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD 2B ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                      << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        // int fi_mon = 0;
        // int fi_crd = 0;
        fi_mon = 0;
        fi_crd = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                size_t mns3 = mns * 3;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = 3 * inmon;
                    sys_mu_[fi_crd + mns3 + 3 * i] = in_ptr[inmon3 + m + fi_crd];
                    sys_mu_[fi_crd + mns3 + 3 * i + 1] = in_ptr[inmon3 + m + fi_crd + nmon];
                    sys_mu_[fi_crd + mns3 + 3 * i + 2] = in_ptr[inmon3 + m + fi_crd + nmon2];
                }
            }
            fi_mon += nmon;
            fi_crd += nmon * ns * 3;
        }

#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        double A, B, C, alpha, beta, gamma;
        A = box_ABCabc_[0];
        B = box_ABCabc_[1];
        C = box_ABCabc_[2];
        alpha = box_ABCabc_[3];
        beta = box_ABCabc_[4];
        gamma = box_ABCabc_[5];

        // Compute the reciprocal space terms, using PME
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);
        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto result = helpme::Matrix<double>(sys_Efd_.data(), nsites_, 3);
        std::fill(sys_Efd_.begin(), sys_Efd_.end(), 0.0);

#ifdef _DEBUG_DIPFIELD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);
            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (size_t i = 0; i < nsites_; i++) {
                        std::cout << "(" << me << ") DIPFIELD PRec ORIG: i= " << i << " xyz= " << coords(i, 0) << " "
                                  << coords(i, 1) << " " << coords(i, 2) << " dipoles= " << dipoles(i, 0) << " "
                                  << dipoles(i, 1) << " " << dipoles(i, 2) << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }  // debug print
#endif

#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(-1, dipoles, coords, coords, -1, result);
        mbxt_ele_count_[ELE_PME_PRD]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRD] += MPI_Wtime() - _time1;
#endif

#ifdef _DEBUG_DIPFIELD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);
            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (size_t i = 0; i < nsites_; i++) {
                        std::cout << "(" << me << ") DIPFIELD PRec ORIG (before Allreduce): i= " << i
                                  << " xyz= " << coords(i, 0) << " " << coords(i, 1) << " " << coords(i, 2)
                                  << " result= " << result(i, 0) << " " << result(i, 1) << " " << result(i, 2)
                                  << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }  // debug print
#endif

#if HAVE_MPI == 1
        MPI_Allreduce(MPI_IN_PLACE, sys_Efd_.data(), sys_Efd_.size(), MPI_DOUBLE, MPI_SUM, world_);
#endif

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    const double *result_ptr = result[fi_sites + mns + i];
                    out_v[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[0];
                    out_v[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[1];
                    out_v[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }
        // The Ewald self field due to induced dipoles
        double slf_prefactor = (4.0 / 3.0) * ewald_alpha_ * ewald_alpha_ * ewald_alpha_ / PIQSRT;
        double *e_ptr = out_v.data();
        for (const auto &mu : in_v) {
            *e_ptr += slf_prefactor * mu;
            ++e_ptr;
        }
    }

#ifdef _DEBUG_DIPFIELD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") DIPFIELD PME ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << " out_v= " << out_v[fi_crd + inmon3 + m]
                                      << " " << out_v[fi_crd + inmon3 + nmon + m] << " "
                                      << out_v[fi_crd + inmon3 + nmon2 + m] << " in_v= " << in_v[fi_crd + inmon3 + m]
                                      << " " << in_v[fi_crd + inmon3 + nmon + m] << " "
                                      << in_v[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();

    mbxt_ele_count_[ELE_DIPFIELD_REAL]++;
    mbxt_ele_time_[ELE_DIPFIELD_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_DIPFIELD_PME]++;
    mbxt_ele_time_[ELE_DIPFIELD_PME] += time3 - time2;
#endif
}

void Electrostatics::CalculateDipolesIterative() {
    // Permanent electric field is computed
    // Now start computation of dipole through iteration
    double eps = 1.0E+50;
    std::vector<double> mu_old(3 * nsites_, 0.0);
    size_t iter = 0;

    while (true) {
        double max_eps = 0.0;
        //  Get new dipoles and check max difference
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;
        double alpha = 0.8;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t i = 0; i < ns; i++) {
                // TODO assuming pol not site dependant
                double p = pol_[fi_sites + i];
                size_t inmon3 = 3 * i * nmon;
                for (size_t m = 0; m < nmon; m++) {
                    mu_old[fi_crd + inmon3 + m] = mu_[fi_crd + inmon3 + m];
                    mu_old[fi_crd + inmon3 + nmon + m] = mu_[fi_crd + inmon3 + nmon + m];
                    mu_old[fi_crd + inmon3 + nmon2 + m] = mu_[fi_crd + inmon3 + nmon2 + m];
                    mu_[fi_crd + inmon3 + m] = alpha * p * (Efq_[fi_crd + inmon3 + m] + Efd_[fi_crd + inmon3 + m]) +
                                               (1 - alpha) * mu_old[fi_crd + inmon3 + m];
                    mu_[fi_crd + inmon3 + nmon + m] =
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon + m] + Efd_[fi_crd + inmon3 + nmon + m]) +
                        (1 - alpha) * mu_old[fi_crd + inmon3 + nmon + m];
                    mu_[fi_crd + inmon3 + nmon2 + m] =
                        alpha * p * (Efq_[fi_crd + inmon3 + nmon2 + m] + Efd_[fi_crd + inmon3 + nmon2 + m]) +
                        (1 - alpha) * mu_old[fi_crd + inmon3 + nmon2 + m];
                }

                // Check for max epsilon
                for (size_t m = 0; m < nmon; m++) {
                    double tmpeps = (mu_[fi_crd + inmon3 + m] - mu_old[fi_crd + inmon3 + m]) *
                                        (mu_[fi_crd + inmon3 + m] - mu_old[fi_crd + inmon3 + m]) +
                                    (mu_[fi_crd + inmon3 + nmon + m] - mu_old[fi_crd + inmon3 + nmon + m]) *
                                        (mu_[fi_crd + inmon3 + nmon + m] - mu_old[fi_crd + inmon3 + nmon + m]) +
                                    (mu_[fi_crd + inmon3 + nmon2 + m] - mu_old[fi_crd + inmon3 + nmon2 + m]) *
                                        (mu_[fi_crd + inmon3 + nmon2 + m] - mu_old[fi_crd + inmon3 + nmon2 + m]);
                    if (tmpeps > max_eps) max_eps = tmpeps;
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        // Check if convergence achieved
        if (max_eps < tolerance_) break;
        // Check if epsilon is increasing
        if (max_eps > eps && iter > 10) {
            // Exit with error
            std::cerr << "Dipoles diverged" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        eps = max_eps;

        // If not, check iter number
        if (iter > maxit_) {
            // Exit with error
            std::cerr << "Max number of iterations reached" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        iter++;
        // Perform next iteration
        ComputeDipoleField(mu_, Efd_);
    }
}

void Electrostatics::CalculateElecEnergyMPIlocal() {
    Eperm_ = 0.0;
    for (size_t i = 0; i < nsites_all_; i++) Eperm_ += phi_all_[i] * chg_all_[i];
    Eperm_ *= 0.5 * constants::COULOMB;

    // Induced Electrostatic energy (chg-dip, dip-dip, pol)
    Eind_ = 0.0;
    for (size_t i = 0; i < 3 * nsites_; i++) Eind_ -= mu_[i] * Efq_[i] * islocal_atom_xyz_[i];
    Eind_ *= 0.5 * constants::COULOMB;

#ifdef _DEBUG_PRINT_ENERGY
    {  // debug_output
        double Eperm_total = 0.0;
        double Eind_total = 0.0;

        MPI_Barrier(world_);
        int nprocs;
        MPI_Comm_size(world_, &nprocs);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == mpi_rank_) {
                std::cout << "(" << mpi_rank_ << ") Eperm(LOCAL)= " << Eperm_ << "  Eind_= " << Eind_ << std::endl;
            }
            MPI_Barrier(world_);
        }

        MPI_Reduce(&Eperm_, &Eperm_total, 1, MPI_DOUBLE, MPI_SUM, 0, world_);
        MPI_Reduce(&Eind_, &Eind_total, 1, MPI_DOUBLE, MPI_SUM, 0, world_);
        if (mpi_rank_ == 0) {
            std::cout << std::endl;
            std::cout << "(" << mpi_rank_ << ") TOTAL:: Eperm= " << Eperm_total << "  Eind_= " << Eind_total
                      << std::endl;
        }
    }  // debug_output
#endif

#ifdef PRINT_TERMS
    std::cerr << "E_ind = " << Eind_ << "   E_perm = " << Eperm_ << std::endl;
#endif
}

void Electrostatics::CalculateElecEnergy() {
    Eperm_ = 0.0;
    for (size_t i = 0; i < nsites_all_; i++) Eperm_ += phi_all_[i] * chg_all_[i];
    Eperm_ *= 0.5 * constants::COULOMB;

    // Induced Electrostatic energy (chg-dip, dip-dip, pol)
    Eind_ = 0.0;
    for (size_t i = 0; i < 3 * nsites_; i++) Eind_ -= mu_[i] * Efq_[i];
    Eind_ *= 0.5 * constants::COULOMB;

#ifdef _DEBUG_PRINT_ENERGY
    {  // debug_output
        if (0 == mpi_rank_)
            std::cout << "(" << mpi_rank_ << ") Eperm(ORIG)= " << Eperm_ << "  Eind_= " << Eind_ << std::endl;
    }  // debug_output
#endif

#ifdef PRINT_TERMS
    std::cerr << "E_ind = " << Eind_ << "   E_perm = " << Eperm_ << std::endl;
#endif
}

void Electrostatics::CalculateGradientsMPIlocal(std::vector<double> &grad, bool use_ghost) {
    // MRR EXT
    size_t nExtChg = external_charge_.size();
    std::vector<std::pair<std::string, size_t>> mon_type_count_cp = mon_type_count_;
    std::vector<double> grad_cp = grad;

    size_t maxnmon = (mon_type_count_.size() > 0) ? mon_type_count_.back().second : 1;
    if (nExtChg > maxnmon) maxnmon = nExtChg;
    //    size_t maxnmon = mon_type_count_.back().second > nExtChg ? mon_type_count_.back().second : nExtChg;

    if (nExtChg > 0) {
        mon_type_count_.push_back(std::make_pair("ext", nExtChg));
    }
    grad = std::vector<double>((nExtChg + nsites_) * 3, 0.0);

    sys_grad_all_ = std::vector<double>((nsites_ + nExtChg) * 3, 0.0);

    // FIx the mu vectors
    mu_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_mu_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_Efq_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_Efd_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    Efd_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_phi_all_ = std::vector<double>(nExtChg + nsites_, 0.0);

    for (size_t i = 0; i < 3 * nsites_; i++) {
        mu_all_[i] = mu_[i];
        sys_mu_all_[i] = sys_mu_[i];
        Efd_all_[i] = Efd_[i];
    }

    // Reset grad
    grad_ = std::vector<double>(3 * (nsites_ + nExtChg), 0.0);

    // Max number of monomers
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    for (size_t i = 0; i < nsites_ * 3; i++) Efq_all_[i] = Efq_[i];
    if (nExtChg > 0) reverse_forward_comm_ext(Efq_all_);

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    double aDD = 0.0;
    size_t nsites3 = 3 * nsites_;

    // Chg-Chg interactions
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t m = 0; m < nmon; m++) {
                if (islocal_atom_all_[fi_sites + m + inmon]) {
                    grad_[fi_crd + inmon3 + m] -= chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + m];
                    grad_[fi_crd + inmon3 + nmon + m] -=
                        chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + nmon + m];
                    grad_[fi_crd + inmon3 + nmon2 + m] -=
                        chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + nmon2 + m];
                }
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD CHG-CHG LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  chg_all_= " << chg_all_[fi_sites + inmon + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " Efq_all_= " << Efq_all_[fi_crd + inmon3 + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << Efq_all_[fi_crd + inmon3 + nmon2 + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        systools::GetExcluded(mon_id_all_[fi_mon], mon_j_, exc12, exc13, exc14);
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t j = i + 1; j < ns; j++) {
                // Set the proper aDD
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                // Don't do charge-dipole and modify phi if pair is excluded
                // TODO check this for distances more than 1-4
                double elec_scale_factor = (is12 || is13 || is14) ? 0 : 1;
                aDD = systools::GetAdd(is12, is13, is14, mon_id_all_[fi_mon]);
                double A = polfac_all_[fi_sites + i] * polfac_all_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }
                for (size_t m = 0; m < nmon; m++) {
                    bool include_monomer = false;
                    if (!use_ghost) include_monomer = true;
                    if (use_ghost && islocal_all_[fi_mon + m]) include_monomer = true;

                    if (include_monomer) {
                        elec_field.CalcElecFieldGrads(
                            xyz_all_.data() + fi_crd, xyz_all_.data() + fi_crd, chg_all_.data() + fi_sites,
                            chg_all_.data() + fi_sites, mu_all_.data() + fi_crd, mu_all_.data() + fi_crd, m, m, m + 1,
                            nmon, nmon, i, j, aDD, aCD_, Asqsqi, &ex, &ey, &ez, &phi1, phi_all_.data() + fi_sites,
                            grad_.data() + fi_crd, elec_scale_factor, ewald_alpha_, simcell_periodic_, box_PMElocal_,
                            box_inverse_PMElocal_, cutoff_, use_ghost, islocal_all_, fi_mon + m, fi_mon, &virial_);
                        phi_all_[fi_sites + inmon + m] += phi1;
                        grad_[fi_crd + inmon3 + m] += ex;
                        grad_[fi_crd + inmon3 + nmon + m] += ey;
                        grad_[fi_crd + inmon3 + nmon2 + m] += ez;
                    }
                }
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD DIP-DIP LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_all_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_all_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;
            bool same = (mt1 == mt2);
            // TODO add neighbour list here
            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> grad_1_pool;
            std::vector<std::vector<double>> grad_2_pool;
            std::vector<std::vector<double>> phi_1_pool;
            std::vector<std::vector<double>> phi_2_pool;
            std::vector<std::vector<double>> virial_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                grad_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                grad_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
                virial_pool.push_back(std::vector<double>(9, 0.0));
            }
#pragma omp parallel for schedule(dynamic)
            for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                double phi1_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon1 = i * nmon1;
                    size_t inmon13 = 3 * inmon1;
                    for (size_t j = 0; j < ns2; j++) {
                        double A = polfac_all_[fi_sites1 + i] * polfac_all_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        local_field->CalcElecFieldGrads(
                            xyz_all_.data() + fi_crd1, xyz_all_.data() + fi_crd2, chg_all_.data() + fi_sites1,
                            chg_all_.data() + fi_sites2, mu_all_.data() + fi_crd1, mu_all_.data() + fi_crd2, m1, m2init,
                            nmon2, nmon1, nmon2, i, j, aDD, aCD_, Asqsqi, &ex_thread, &ey_thread, &ez_thread,
                            &phi1_thread, phi_2_pool[rank].data(), grad_2_pool[rank].data(), 1, ewald_alpha_,
                            simcell_periodic_, box_PMElocal_, box_inverse_PMElocal_, cutoff_, use_ghost, islocal_all_,
                            fi_mon1 + m1, fi_mon2, &virial_pool[rank]);
                        grad_1_pool[rank][inmon13 + m1] += ex_thread;
                        grad_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        grad_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                        phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                    }
                }
            }
            // Compress data in grad and phi
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = grad_1_pool[rank].size();
                size_t kend2 = grad_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    grad_[fi_crd1 + k] += grad_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    grad_[fi_crd2 + k] += grad_2_pool[rank][k];
                }
                kend1 = phi_1_pool[rank].size();
                kend2 = phi_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    phi_all_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_all_[fi_sites2 + k] += phi_2_pool[rank][k];
                }
                for (size_t k = 0; k < 9; k++) {
                    virial_[k] += virial_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD INTER LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << phi_all_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();
#endif

    bool compute_pme = (ewald_alpha_ > 0 && use_pbc_);

    if (!compute_pme && use_ghost && ewald_alpha_ > 0) compute_pme = true;

    if (!simcell_periodic_) compute_pme = false;

    if (compute_pme) {
        // Sort the dipoles to the order helPME expects (for now)
        // int fi_mon = 0;
        // int fi_sites = 0;
        // int fi_crd = 0;
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                size_t mns3 = mns * 3;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = 3 * inmon;
                    sys_chg_all_[fi_sites + mns + i] = chg_all_[fi_sites + m + inmon];
                    sys_mu_all_[fi_crd + mns3 + 3 * i] = mu_all_[inmon3 + m + fi_crd];
                    sys_mu_all_[fi_crd + mns3 + 3 * i + 1] = mu_all_[inmon3 + m + fi_crd + nmon];
                    sys_mu_all_[fi_crd + mns3 + 3 * i + 2] = mu_all_[inmon3 + m + fi_crd + nmon2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        // Compute the reciprocal space terms, using PME

        double A, B, C, alpha, beta, gamma;
        if (use_ghost) {
            A = box_ABCabc_PMElocal_[0];
            B = box_ABCabc_PMElocal_[1];
            C = box_ABCabc_PMElocal_[2];
            alpha = box_ABCabc_PMElocal_[3];
            beta = box_ABCabc_PMElocal_[4];
            gamma = box_ABCabc_PMElocal_[5];
        } else {
            A = box_ABCabc_[0];
            B = box_ABCabc_[1];
            C = box_ABCabc_[2];
            alpha = box_ABCabc_[3];
            beta = box_ABCabc_[4];
            gamma = box_ABCabc_[5];
        }
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);
        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_all_.data(), nsites_all_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_all_.data(), nsites_all_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_all_.data(), nsites_all_, 1);
        auto result = helpme::Matrix<double>(nsites_all_, 10);

#ifdef _DEBUG_GRAD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);

            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (int i = 0; i < nsites_all_; ++i) {
                        std::cout << "(" << me << ") GRAD 1 LOCAL: i= " << i << " coords= " << coords(i, 0) << " "
                                  << coords(i, 1) << " " << coords(i, 2) << " charges= " << charges(0, i)
                                  << " dipoles= " << dipoles(i, 0) << " " << dipoles(i, 1) << " " << dipoles(i, 2)
                                  << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }
#endif

        if (calc_virial_) {
            std::vector<double> trecvir(6, 0.0);
            std::vector<double> tforcevec(sys_grad_all_.size(), 0.0);

            auto drecvirial = helpme::Matrix<double>(trecvir.data(), 6, 1);
            int ns_ = (nsites_ + nExtChg == 0) ? 1 : nsites_ + nExtChg;
            auto tmpforces2 = helpme::Matrix<double>(tforcevec.data(), ns_, 3);

#if HAVE_MPI == 1
            double _time1 = MPI_Wtime();
#endif
            double fulldummy_rec_energy = pme_solver_.computeEFVRecIsotropicInducedDipoles(
                0, charges, dipoles, PMEInstanceD::PolarizationType::Mutual, coords, tmpforces2, drecvirial);
            mbxt_ele_count_[ELE_PME_PRE]++;
#if HAVE_MPI == 1
            mbxt_ele_time_[ELE_PME_PRE] += MPI_Wtime() - _time1;
#endif

            virial_[0] += (*drecvirial[0]) * constants::COULOMB;
            virial_[1] += (*drecvirial[1]) * constants::COULOMB;
            virial_[2] += (*drecvirial[3]) * constants::COULOMB;
            virial_[4] += (*drecvirial[2]) * constants::COULOMB;
            virial_[5] += (*drecvirial[4]) * constants::COULOMB;
            virial_[8] += (*drecvirial[5]) * constants::COULOMB;

            virial_[3] = virial_[1];
            virial_[6] = virial_[2];
            virial_[7] = virial_[5];
        }

#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(-1, dipoles, coords, coords, 2, result);
        mbxt_ele_count_[ELE_PME_PRD]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRD] += MPI_Wtime() - _time1;
#endif

        double *ptr = result[0];

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        double fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double chg = sys_chg_all_[fi_sites + mns + i];
                    const double *mu = &sys_mu_all_[fi_crd + 3 * mns + 3 * i];
                    double Phi = result_ptr[0];
                    double Erec_x = result_ptr[1];
                    double Erec_y = result_ptr[2];
                    double Erec_z = result_ptr[3];
                    double Erec_xx = result_ptr[4];
                    double Erec_xy = result_ptr[5];
                    double Erec_yy = result_ptr[6];
                    double Erec_xz = result_ptr[7];
                    double Erec_yz = result_ptr[8];
                    double Erec_zz = result_ptr[9];
                    double Grad_x = chg * Erec_x;
                    double Grad_y = chg * Erec_y;
                    double Grad_z = chg * Erec_z;
                    phi_all_[fi_sites + i * nmon + m] += Phi;
#if !DIRECT_ONLY
                    Grad_x += Erec_xx * mu[0] + Erec_xy * mu[1] + Erec_xz * mu[2];
                    Grad_y += Erec_xy * mu[0] + Erec_yy * mu[1] + Erec_yz * mu[2];
                    Grad_z += Erec_xz * mu[0] + Erec_yz * mu[1] + Erec_zz * mu[2];
#endif
                    grad[fi_crd + 3 * mns + 3 * i] += fac * Grad_x;
                    grad[fi_crd + 3 * mns + 3 * i + 1] += fac * Grad_y;
                    grad[fi_crd + 3 * mns + 3 * i + 2] += fac * Grad_z;
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }
        // Now grid up the charges
        result.setZero();
#if HAVE_MPI == 1
        _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(0, charges, coords, coords, -2, result);
        mbxt_ele_count_[ELE_PME_PRC]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRC] += MPI_Wtime() - _time1;
#endif

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double *mu = &sys_mu_all_[fi_crd + 3 * mns + 3 * i];
                    double Erec_xx = result_ptr[0];
                    double Erec_xy = result_ptr[1];
                    double Erec_yy = result_ptr[2];
                    double Erec_xz = result_ptr[3];
                    double Erec_yz = result_ptr[4];
                    double Erec_zz = result_ptr[5];
                    double Grad_x = Erec_xx * mu[0] + Erec_xy * mu[1] + Erec_xz * mu[2];
                    double Grad_y = Erec_xy * mu[0] + Erec_yy * mu[1] + Erec_yz * mu[2];
                    double Grad_z = Erec_xz * mu[0] + Erec_yz * mu[1] + Erec_zz * mu[2];
                    grad[fi_crd + 3 * mns + 3 * i] += fac * Grad_x;
                    grad[fi_crd + 3 * mns + 3 * i + 1] += fac * Grad_y;
                    grad[fi_crd + 3 * mns + 3 * i + 2] += fac * Grad_z;
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }
    }

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // REVERT DATA ORGANIZATION ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Reorganize field and potential to initial order
    std::vector<double> tmp1(nsites3, 0.0);
    std::vector<double> tmp2(nsites_, 0.0);
    std::vector<double> tmp3(nsites3, 0.0);
    std::vector<double> tmp4(nsites3, 0.0);
    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                sys_Efq_all_[fi_crd + mns3 + 3 * i] = Efq_all_[inmon3 + m + fi_crd];
                sys_Efq_all_[fi_crd + mns3 + 3 * i + 1] = Efq_all_[inmon3 + m + fi_crd + nmon];
                sys_Efq_all_[fi_crd + mns3 + 3 * i + 2] = Efq_all_[inmon3 + m + fi_crd + nmon2];

                sys_Efd_all_[fi_crd + mns3 + 3 * i] = Efd_all_[inmon3 + m + fi_crd];
                sys_Efd_all_[fi_crd + mns3 + 3 * i + 1] = Efd_all_[inmon3 + m + fi_crd + nmon];
                sys_Efd_all_[fi_crd + mns3 + 3 * i + 2] = Efd_all_[inmon3 + m + fi_crd + nmon2];

                sys_mu_all_[fi_crd + mns3 + 3 * i] = mu_all_[inmon3 + m + fi_crd];
                sys_mu_all_[fi_crd + mns3 + 3 * i + 1] = mu_all_[inmon3 + m + fi_crd + nmon];
                sys_mu_all_[fi_crd + mns3 + 3 * i + 2] = mu_all_[inmon3 + m + fi_crd + nmon2];

                sys_phi_all_[fi_sites + mns + i] = phi_all_[fi_sites + m + inmon];

                grad[fi_crd + mns3 + 3 * i] += constants::COULOMB * grad_[inmon3 + m + fi_crd];
                grad[fi_crd + mns3 + 3 * i + 1] += constants::COULOMB * grad_[inmon3 + m + fi_crd + nmon];
                grad[fi_crd + mns3 + 3 * i + 2] += constants::COULOMB * grad_[inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // REDISTRIBUTION OF THE GRADIENTS AND GRADIENTS DUE TO SITE-DEPENDENT CHARGES /
    ////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG_PRINT_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD PME LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << sys_phi_all_[fi_sites + m + inmon]
                                      << " grad= " << grad[fi_crd + inmon3 + m] << " "
                                      << grad[fi_crd + inmon3 + nmon + m] << " " << grad[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        std::string id = mon_id_all_[fi_mon];

        // Redistribute gradients
        systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, grad);

        // // Gradients due to position dependant charges
        if (calc_virial_) {  // calculate virial if need be
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_all_, grad, sys_chg_grad_,
                                            xyz_all_.data() + fi_crd, &virial_);
        } else {
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_all_, grad, sys_chg_grad_);
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_PRINT_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_all_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD FINAL LOCAL: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_all_[fi_mon + m]
                                      << " xyz= " << xyz_all_[fi_crd + inmon3 + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_all_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << sys_phi_all_[fi_sites + m + inmon]
                                      << " grad= " << grad[fi_crd + inmon3 + m] << " "
                                      << grad[fi_crd + inmon3 + nmon + m] << " " << grad[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Reset mon_type_count_
    mon_type_count_ = mon_type_count_cp;

    // Add gradients to right place
    for (size_t i = 0; i < nsites_ * 3; i++) {
        grad_cp[i] += grad[i];
        mu_[i] = mu_all_[i];
        sys_mu_[i] = sys_mu_all_[i];
    }

    for (size_t i = 0; i < 3 * nExtChg; i++) {
        external_charge_grads_[i] = grad[3 * nsites_ + i];
    }

    for (size_t i = 0; i < nsites_; i++) {
        phi_[i] = phi_all_[i];
        sys_phi_[i] = sys_phi_all_[i];
    }

    grad = grad_cp;

#if HAVE_MPI == 1
    double time4 = MPI_Wtime();

    mbxt_ele_count_[ELE_GRAD_REAL]++;
    mbxt_ele_time_[ELE_GRAD_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_GRAD_PME]++;
    mbxt_ele_time_[ELE_GRAD_PME] += time3 - time2;

    mbxt_ele_count_[ELE_GRAD_FIN]++;
    mbxt_ele_time_[ELE_GRAD_FIN] += time4 - time3;
#endif
}

void Electrostatics::CalculateGradients(std::vector<double> &grad, bool use_ghost) {
    // MRR EXT
    size_t nExtChg = external_charge_.size();
    std::vector<std::pair<std::string, size_t>> mon_type_count_cp = mon_type_count_;
    std::vector<double> grad_cp = grad;

    //    size_t maxnmon = mon_type_count_.back().second > nExtChg ? mon_type_count_.back().second : nExtChg;
    size_t maxnmon = (mon_type_count_.size() > 0) ? mon_type_count_.back().second : 1;
    if (nExtChg > maxnmon) maxnmon = nExtChg;

    if (nExtChg > 0) {
        mon_type_count_.push_back(std::make_pair("ext", nExtChg));
    }
    grad = std::vector<double>((nExtChg + nsites_) * 3, 0.0);

    sys_grad_all_ = std::vector<double>((nsites_ + nExtChg) * 3, 0.0);

    // FIx the mu vectors
    mu_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_mu_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_Efq_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_Efd_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    Efd_all_ = std::vector<double>(3 * (nExtChg + nsites_), 0.0);
    sys_phi_all_ = std::vector<double>(nExtChg + nsites_, 0.0);

    for (size_t i = 0; i < 3 * nsites_; i++) {
        mu_all_[i] = mu_[i];
        sys_mu_all_[i] = sys_mu_[i];
        Efd_all_[i] = Efd_[i];
    }

    // Reset grad
    grad_ = std::vector<double>(3 * (nsites_ + nExtChg), 0.0);

    // Max number of monomers
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

#if HAVE_MPI == 1
    double time1 = MPI_Wtime();
#endif

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    double aDD = 0.0;
    size_t nsites3 = 3 * nsites_;

    // Chg-Chg interactions
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;
#if 1
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t m = 0; m < nmon; m++) {
                grad_[fi_crd + inmon3 + m] -= chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + m];
                grad_[fi_crd + inmon3 + nmon + m] -=
                    chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + nmon + m];
                grad_[fi_crd + inmon3 + nmon2 + m] -=
                    chg_all_[fi_sites + inmon + m] * Efq_all_[fi_crd + inmon3 + nmon2 + m];
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
#endif
#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD CHG-CHG ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        systools::GetExcluded(mon_id_all_[fi_mon], mon_j_, exc12, exc13, exc14);
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t j = i + 1; j < ns; j++) {
                // Set the proper aDD
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                // Don't do charge-dipole and modify phi if pair is excluded
                // TODO check this for distances more than 1-4
                double elec_scale_factor = (is12 || is13 || is14) ? 0 : 1;
                aDD = systools::GetAdd(is12, is13, is14, mon_id_all_[fi_mon]);
                double A = polfac_all_[fi_sites + i] * polfac_all_[fi_sites + j];
                double Ai = 0.0;
                double Asqsqi = 0.0;
                if (A > constants::EPS) {
                    A = std::pow(A, 1.0 / 6.0);
                    Ai = 1 / A;
                    Asqsqi = Ai * Ai * Ai * Ai;
                } else {
                    Ai = BIGNUM;
                    Asqsqi = Ai;
                }
                for (size_t m = 0; m < nmon; m++) {
                    elec_field.CalcElecFieldGrads(
                        xyz_all_.data() + fi_crd, xyz_all_.data() + fi_crd, chg_all_.data() + fi_sites,
                        chg_all_.data() + fi_sites, mu_all_.data() + fi_crd, mu_all_.data() + fi_crd, m, m, m + 1, nmon,
                        nmon, i, j, aDD, aCD_, Asqsqi, &ex, &ey, &ez, &phi1, phi_all_.data() + fi_sites,
                        grad_.data() + fi_crd, elec_scale_factor, ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_,
                        use_ghost, islocal_all_, fi_mon + m, fi_mon, &virial_);
                    phi_all_[fi_sites + inmon + m] += phi1;
                    grad_[fi_crd + inmon3 + m] += ex;
                    grad_[fi_crd + inmon3 + nmon + m] += ey;
                    grad_[fi_crd + inmon3 + nmon2 + m] += ez;
                }
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD DIP-DIP ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_all_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_all_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;
            bool same = (mt1 == mt2);
            // TODO add neighbour list here
            std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
            std::vector<std::vector<double>> grad_1_pool;
            std::vector<std::vector<double>> grad_2_pool;
            std::vector<std::vector<double>> phi_1_pool;
            std::vector<std::vector<double>> phi_2_pool;
            std::vector<std::vector<double>> virial_pool;
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                grad_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                grad_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
                virial_pool.push_back(std::vector<double>(9, 0.0));
            }
#pragma omp parallel for schedule(dynamic)
            for (size_t m1 = 0; m1 < nmon1; m1++) {
                int rank = 0;
#ifdef _OPENMP
                rank = omp_get_thread_num();
#endif
                std::shared_ptr<ElectricFieldHolder> local_field = field_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                double ex_thread = 0.0;
                double ey_thread = 0.0;
                double ez_thread = 0.0;
                double phi1_thread = 0.0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon1 = i * nmon1;
                    size_t inmon13 = 3 * inmon1;
                    for (size_t j = 0; j < ns2; j++) {
                        double A = polfac_all_[fi_sites1 + i] * polfac_all_[fi_sites2 + j];
                        double Ai = 0.0;
                        double Asqsqi = 0.0;
                        if (A > constants::EPS) {
                            A = std::pow(A, 1.0 / 6.0);
                            Ai = 1 / A;
                            Asqsqi = Ai * Ai * Ai * Ai;
                        } else {
                            Ai = BIGNUM;
                            Asqsqi = Ai;
                        }
                        local_field->CalcElecFieldGrads(
                            xyz_all_.data() + fi_crd1, xyz_all_.data() + fi_crd2, chg_all_.data() + fi_sites1,
                            chg_all_.data() + fi_sites2, mu_all_.data() + fi_crd1, mu_all_.data() + fi_crd2, m1, m2init,
                            nmon2, nmon1, nmon2, i, j, aDD, aCD_, Asqsqi, &ex_thread, &ey_thread, &ez_thread,
                            &phi1_thread, phi_2_pool[rank].data(), grad_2_pool[rank].data(), 1, ewald_alpha_, use_pbc_,
                            box_, box_inverse_, cutoff_, use_ghost, islocal_all_, fi_mon + m1, fi_mon2,
                            &virial_pool[rank]);
                        grad_1_pool[rank][inmon13 + m1] += ex_thread;
                        grad_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                        grad_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                        phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                    }
                }
            }
            // Compress data in grad and phi
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend1 = grad_1_pool[rank].size();
                size_t kend2 = grad_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    grad_[fi_crd1 + k] += grad_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    grad_[fi_crd2 + k] += grad_2_pool[rank][k];
                }
                kend1 = phi_1_pool[rank].size();
                kend2 = phi_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    phi_all_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_all_[fi_sites2 + k] += phi_2_pool[rank][k];
                }
                for (size_t k = 0; k < 9; k++) {
                    virial_[k] += virial_pool[rank][k];
                }
            }
            // Update first indexes
            fi_mon2 += nmon2;
            fi_sites2 += nmon2 * ns2;
            fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
    }

#ifdef _DEBUG_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD INTER ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m] << "  phi_= " << phi_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

#if HAVE_MPI == 1
    double time2 = MPI_Wtime();
#endif

    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        // int fi_mon = 0;
        // int fi_sites = 0;
        // int fi_crd = 0;
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                size_t mns3 = mns * 3;
                for (size_t i = 0; i < ns; i++) {
                    size_t inmon = i * nmon;
                    size_t inmon3 = 3 * inmon;
                    sys_chg_all_[fi_sites + mns + i] = chg_all_[fi_sites + m + inmon];
                    sys_mu_all_[fi_crd + mns3 + 3 * i] = mu_all_[inmon3 + m + fi_crd];
                    sys_mu_all_[fi_crd + mns3 + 3 * i + 1] = mu_all_[inmon3 + m + fi_crd + nmon];
                    sys_mu_all_[fi_crd + mns3 + 3 * i + 2] = mu_all_[inmon3 + m + fi_crd + nmon2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

#if HAVE_MPI == 1
        double _time0 = MPI_Wtime();
#endif
        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);
        // Compute the reciprocal space terms, using PME
        double A = box_ABCabc_[0];
        double B = box_ABCabc_[1];
        double C = box_ABCabc_[2];
        double alpha = box_ABCabc_[3];
        double beta = box_ABCabc_[4];
        double gamma = box_ABCabc_[5];
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);
        mbxt_ele_count_[ELE_PME_SETUP]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_SETUP] += MPI_Wtime() - _time0;
#endif

        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_all_.data(), nsites_all_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_all_.data(), nsites_all_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_all_.data(), nsites_all_, 1);
        auto result = helpme::Matrix<double>(nsites_all_, 10);

#ifdef _DEBUG_GRAD
        {  // debug print
            int me, nprocs;
            MPI_Comm_size(world_, &nprocs);
            MPI_Comm_rank(world_, &me);

            MPI_Barrier(world_);

            for (int ip = 0; ip < nprocs; ++ip) {
                if (ip == me) {
                    std::cout << "\n" << std::endl;
                    for (int i = 0; i < nsites_; ++i) {
                        std::cout << "(" << me << ") GRAD 1 ORIG: i= " << i << " coords= " << coords(i, 0) << " "
                                  << coords(i, 1) << " " << coords(i, 2) << " charges= " << charges(0, i)
                                  << " dipoles= " << dipoles(i, 0) << " " << dipoles(i, 1) << " " << dipoles(i, 2)
                                  << std::endl;
                    }
                }
                MPI_Barrier(world_);
            }
        }
#endif

        if (calc_virial_) {
            std::vector<double> trecvir(6, 0.0);
            std::vector<double> tforcevec(sys_grad_all_.size(), 0.0);

            auto drecvirial = helpme::Matrix<double>(trecvir.data(), 6, 1);
            auto tmpforces2 = helpme::Matrix<double>(tforcevec.data(), nsites_ + nExtChg, 3);

#if HAVE_MPI == 1
            double _time1 = MPI_Wtime();
#endif
            double fulldummy_rec_energy = pme_solver_.computeEFVRecIsotropicInducedDipoles(
                0, charges, dipoles, PMEInstanceD::PolarizationType::Mutual, coords, tmpforces2, drecvirial);
            mbxt_ele_count_[ELE_PME_PRE]++;
#if HAVE_MPI == 1
            mbxt_ele_time_[ELE_PME_PRE] += MPI_Wtime() - _time1;
#endif

#if HAVE_MPI == 1
            MPI_Allreduce(MPI_IN_PLACE, trecvir.data(), trecvir.size(), MPI_DOUBLE, MPI_SUM, world_);
#endif

            virial_[0] += (*drecvirial[0]) * constants::COULOMB;
            virial_[1] += (*drecvirial[1]) * constants::COULOMB;
            virial_[2] += (*drecvirial[3]) * constants::COULOMB;
            virial_[4] += (*drecvirial[2]) * constants::COULOMB;
            virial_[5] += (*drecvirial[4]) * constants::COULOMB;
            virial_[8] += (*drecvirial[5]) * constants::COULOMB;

            virial_[3] = virial_[1];
            virial_[6] = virial_[2];
            virial_[7] = virial_[5];
        }

#if HAVE_MPI == 1
        double _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(-1, dipoles, coords, coords, 2, result);
        mbxt_ele_count_[ELE_PME_PRD]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRD] += MPI_Wtime() - _time1;
#endif

        double *ptr = result[0];
#if HAVE_MPI == 1
        MPI_Allreduce(MPI_IN_PLACE, ptr, nsites_ * 10, MPI_DOUBLE, MPI_SUM, world_);
#endif

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        double fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double chg = sys_chg_all_[fi_sites + mns + i];
                    const double *mu = &sys_mu_all_[fi_crd + 3 * mns + 3 * i];
                    double Phi = result_ptr[0];
                    double Erec_x = result_ptr[1];
                    double Erec_y = result_ptr[2];
                    double Erec_z = result_ptr[3];
                    double Erec_xx = result_ptr[4];
                    double Erec_xy = result_ptr[5];
                    double Erec_yy = result_ptr[6];
                    double Erec_xz = result_ptr[7];
                    double Erec_yz = result_ptr[8];
                    double Erec_zz = result_ptr[9];
                    double Grad_x = chg * Erec_x;
                    double Grad_y = chg * Erec_y;
                    double Grad_z = chg * Erec_z;
                    phi_all_[fi_sites + i * nmon + m] += Phi;
#if !DIRECT_ONLY
                    Grad_x += Erec_xx * mu[0] + Erec_xy * mu[1] + Erec_xz * mu[2];
                    Grad_y += Erec_xy * mu[0] + Erec_yy * mu[1] + Erec_yz * mu[2];
                    Grad_z += Erec_xz * mu[0] + Erec_yz * mu[1] + Erec_zz * mu[2];
#endif
                    grad[fi_crd + 3 * mns + 3 * i] += fac * Grad_x;
                    grad[fi_crd + 3 * mns + 3 * i + 1] += fac * Grad_y;
                    grad[fi_crd + 3 * mns + 3 * i + 2] += fac * Grad_z;
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }
        // Now grid up the charges
        result.setZero();
#if HAVE_MPI == 1
        _time1 = MPI_Wtime();
#endif
        pme_solver_.computePRec(0, charges, coords, coords, -2, result);
        mbxt_ele_count_[ELE_PME_PRC]++;
#if HAVE_MPI == 1
        mbxt_ele_time_[ELE_PME_PRC] += MPI_Wtime() - _time1;
#endif

#if HAVE_MPI == 1
        MPI_Allreduce(MPI_IN_PLACE, ptr, nsites_ * 10, MPI_DOUBLE, MPI_SUM, world_);
#endif

        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_all_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double *mu = &sys_mu_all_[fi_crd + 3 * mns + 3 * i];
                    double Erec_xx = result_ptr[0];
                    double Erec_xy = result_ptr[1];
                    double Erec_yy = result_ptr[2];
                    double Erec_xz = result_ptr[3];
                    double Erec_yz = result_ptr[4];
                    double Erec_zz = result_ptr[5];
                    double Grad_x = Erec_xx * mu[0] + Erec_xy * mu[1] + Erec_xz * mu[2];
                    double Grad_y = Erec_xy * mu[0] + Erec_yy * mu[1] + Erec_yz * mu[2];
                    double Grad_z = Erec_xz * mu[0] + Erec_yz * mu[1] + Erec_zz * mu[2];
                    grad[fi_crd + 3 * mns + 3 * i] += fac * Grad_x;
                    grad[fi_crd + 3 * mns + 3 * i + 1] += fac * Grad_y;
                    grad[fi_crd + 3 * mns + 3 * i + 2] += fac * Grad_z;
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }
    }

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // REVERT DATA ORGANIZATION ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Reorganize field and potential to initial order
    std::vector<double> tmp1(nsites3, 0.0);
    std::vector<double> tmp2(nsites_, 0.0);
    std::vector<double> tmp3(nsites3, 0.0);
    std::vector<double> tmp4(nsites3, 0.0);
    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                sys_Efq_all_[fi_crd + mns3 + 3 * i] = Efq_all_[inmon3 + m + fi_crd];
                sys_Efq_all_[fi_crd + mns3 + 3 * i + 1] = Efq_all_[inmon3 + m + fi_crd + nmon];
                sys_Efq_all_[fi_crd + mns3 + 3 * i + 2] = Efq_all_[inmon3 + m + fi_crd + nmon2];

                sys_Efd_all_[fi_crd + mns3 + 3 * i] = Efd_all_[inmon3 + m + fi_crd];
                sys_Efd_all_[fi_crd + mns3 + 3 * i + 1] = Efd_all_[inmon3 + m + fi_crd + nmon];
                sys_Efd_all_[fi_crd + mns3 + 3 * i + 2] = Efd_all_[inmon3 + m + fi_crd + nmon2];

                sys_mu_all_[fi_crd + mns3 + 3 * i] = mu_all_[inmon3 + m + fi_crd];
                sys_mu_all_[fi_crd + mns3 + 3 * i + 1] = mu_all_[inmon3 + m + fi_crd + nmon];
                sys_mu_all_[fi_crd + mns3 + 3 * i + 2] = mu_all_[inmon3 + m + fi_crd + nmon2];

                sys_phi_all_[fi_sites + mns + i] = phi_all_[fi_sites + m + inmon];

                grad[fi_crd + mns3 + 3 * i] += constants::COULOMB * grad_[inmon3 + m + fi_crd];
                grad[fi_crd + mns3 + 3 * i + 1] += constants::COULOMB * grad_[inmon3 + m + fi_crd + nmon];
                grad[fi_crd + mns3 + 3 * i + 2] += constants::COULOMB * grad_[inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // REDISTRIBUTION OF THE GRADIENTS AND GRADIENTS DUE TO SITE-DEPENDENT CHARGES /
    ///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG_PRINT_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD PME ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << sys_phi_[fi_sites + m + inmon]
                                      << " grad= " << grad[fi_crd + inmon3 + m] << " "
                                      << grad[fi_crd + inmon3 + nmon + m] << " " << grad[fi_crd + inmon3 + nmon2 + m]
                                      << " sys_chg_grad_= " << sys_chg_grad_[fi_crd + inmon3 + m] << " "
                                      << sys_chg_grad_[fi_crd + inmon3 + nmon + m] << " "
                                      << sys_chg_grad_[fi_crd + inmon3 + nmon2 + m] << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_all_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        std::string id = mon_id_all_[fi_mon];

        // Redistribute gradients
        systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, grad);

        // // Gradients due to position dependant charges
        if (calc_virial_) {  // calculate virial if need be
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_all_, grad, sys_chg_grad_,
                                            xyz_all_.data() + fi_crd, &virial_);
        } else {
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_all_, grad, sys_chg_grad_);
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef _DEBUG_PRINT_GRAD
    {  // debug print
        int me, nprocs;
        MPI_Comm_size(world_, &nprocs);
        MPI_Comm_rank(world_, &me);
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        MPI_Barrier(world_);
        for (int ip = 0; ip < nprocs; ++ip) {
            if (ip == me) {
                std::cout << "\n" << std::endl;
                // Loop over each monomer type
                for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
                    size_t ns = sites_[fi_mon];
                    size_t nmon = mon_type_count_[mt].second;
                    size_t nmon2 = 2 * nmon;

                    // Loop over each pair of sites
                    for (size_t i = 0; i < ns; i++) {
                        size_t inmon = i * nmon;
                        size_t inmon3 = inmon * 3;
                        for (size_t m = 0; m < nmon; m++) {
                            std::cout << "(" << me << ") GRAD FINAL ORIG: mt= " << mt << " i= " << i << " m= " << m
                                      << "  islocal= " << islocal_[fi_mon + m] << " xyz= " << xyz_[fi_crd + inmon3 + m]
                                      << " " << xyz_[fi_crd + inmon3 + nmon + m] << " "
                                      << xyz_[fi_crd + inmon3 + nmon2 + m]
                                      << "  phi_= " << sys_phi_[fi_sites + inmon + m]
                                      << " grad_= " << grad_[fi_crd + inmon3 + m] << " "
                                      << grad_[fi_crd + inmon3 + nmon + m] << " " << grad_[fi_crd + inmon3 + nmon2 + m]
                                      << " grad= " << grad[fi_crd + inmon3 + m] << " "
                                      << grad[fi_crd + inmon3 + nmon + m] << " " << grad[fi_crd + inmon3 + nmon2 + m]
                                      << std::endl;
                        }
                    }

                    // Update first indexes
                    fi_mon += nmon;
                    fi_sites += nmon * ns;
                    fi_crd += nmon * ns * 3;
                }
            }
            MPI_Barrier(world_);
        }
    }  // debug print
#endif

    // Reset mon_type_count_
    mon_type_count_ = mon_type_count_cp;

    // Add gradients to right place
    for (size_t i = 0; i < nsites_ * 3; i++) {
        grad_cp[i] += grad[i];
        mu_[i] = mu_all_[i];
        sys_mu_[i] = sys_mu_all_[i];
    }

    for (size_t i = 0; i < 3 * nExtChg; i++) {
        external_charge_grads_[i] = grad[3 * nsites_ + i];
    }

    for (size_t i = 0; i < nsites_; i++) {
        phi_[i] = phi_all_[i];
        sys_phi_[i] = sys_phi_all_[i];
    }

    grad = grad_cp;

#if HAVE_MPI == 1
    double time4 = MPI_Wtime();

    mbxt_ele_count_[ELE_GRAD_REAL]++;
    mbxt_ele_time_[ELE_GRAD_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_GRAD_PME]++;
    mbxt_ele_time_[ELE_GRAD_PME] += time3 - time2;

    mbxt_ele_count_[ELE_GRAD_FIN]++;
    mbxt_ele_time_[ELE_GRAD_FIN] += time4 - time3;
#endif
}

std::vector<double> Electrostatics::GetInducedDipoles() {
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;
    // double db = constants::DEBYE;
    double db = 1.0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                sys_mu_[fi_crd + mns3 + 3 * i] = db * mu_[inmon3 + m + fi_crd];
                sys_mu_[fi_crd + mns3 + 3 * i + 1] = db * mu_[inmon3 + m + fi_crd + nmon];
                sys_mu_[fi_crd + mns3 + 3 * i + 2] = db * mu_[inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    return sys_mu_;
}

std::vector<double> Electrostatics::GetPermanentDipoles() {
    // TODO careful with pbc. Coords inside box?
    // double db = constants::DEBYE;
    double db = 1.0;
    for (size_t i = 0; i < sys_chg_.size(); i++) {
        double my_chg = sys_chg_[i];
        for (size_t j = 0; j < 3; j++) {
            sys_perm_mu_[3 * i + j] = db * my_chg * sys_xyz_[3 * i + j];
        }
    }

    return sys_perm_mu_;
}

std::vector<double> Electrostatics::GetMolecularInducedDipoles() {
    GetInducedDipoles();
    std::fill(sys_mol_mu_.begin(), sys_mol_mu_.end(), 0.0);

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                for (size_t j = 0; j < 3; j++) {
                    sys_mol_mu_[3 * fi_mon + 3 * m + j] += sys_mu_[fi_crd + mns3 + 3 * i + j];
                }
            }
        }
        fi_mon += nmon;
        fi_crd += nmon * ns * 3;
    }

    return sys_mol_mu_;
}

std::vector<double> Electrostatics::GetMolecularPermanentDipoles() {
    GetPermanentDipoles();
    std::fill(sys_mol_perm_mu_.begin(), sys_mol_perm_mu_.end(), 0.0);

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                for (size_t j = 0; j < 3; j++) {
                    sys_mol_perm_mu_[3 * fi_mon + 3 * m + j] += sys_perm_mu_[fi_crd + mns3 + 3 * i + j];
                }
            }
        }
        fi_mon += nmon;
        fi_crd += nmon * ns * 3;
    }

    return sys_mol_perm_mu_;
}

double Electrostatics::GetPermanentElectrostaticEnergy() { return Eperm_; }

double Electrostatics::GetInducedElectrostaticEnergy() { return Eind_; }

double Electrostatics::GetElectrostatics(std::vector<double> &grad, std::vector<double> *virial, bool use_ghost) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Grads:\n";
    for (size_t i = 0; i < grad.size(); i++) {
        std::cerr << grad[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Virial:\n";
    if (virial != 0) {
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
    }
    std::cerr << std::endl;

    std::cerr << "Use ghost: " << use_ghost << std::endl;
#endif

    std::fill(virial_.begin(), virial_.end(), 0.0);
    CalculatePermanentElecField(use_ghost);
    CalculateDipoles();
    CalculateElecEnergy();
    if (do_grads_) CalculateGradients(grad);
    // update viral
    if (virial != 0) {
        for (size_t k = 0; k < 9; k++) {
            (*virial)[k] += virial_[k];
        }
    }
    has_energy_ = true;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Grads (system atoms):\n";
    for (size_t i = 0; i < grad.size(); i++) {
        std::cerr << grad[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Grads (external charges):\n";
    for (size_t i = 0; i < external_charge_grads_.size(); i++) {
        std::cerr << external_charge_grads_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Virial:\n";
    if (virial != 0) {
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
    }
    std::cerr << std::endl;

    std::cerr << "Elec tot energy: " << Eperm_ + Eind_ << std::endl;
    std::cerr << "Eperm: " << Eperm_ << std::endl;
    std::cerr << "Eind: " << Eind_ << std::endl;

#endif

    return Eperm_ + Eind_;
}

double Electrostatics::GetElectrostaticsMPIlocal(std::vector<double> &grad, std::vector<double> *virial,
                                                 bool use_ghost) {
    std::fill(virial_.begin(), virial_.end(), 0.0);
    CalculatePermanentElecFieldMPIlocal(use_ghost);
    CalculateDipolesMPIlocal(use_ghost);
    CalculateElecEnergyMPIlocal();
    if (do_grads_) CalculateGradientsMPIlocal(grad, use_ghost);
    // update viral
    if (virial != 0) {
        for (size_t k = 0; k < 9; k++) {
            (*virial)[k] += virial_[k];
        }
    }
    has_energy_ = true;
    return Eperm_ + Eind_;
}

std::vector<size_t> Electrostatics::GetInfoCounts() { return mbxt_ele_count_; }
std::vector<double> Electrostatics::GetInfoTimings() { return mbxt_ele_time_; }

std::vector<int> Electrostatics::GetFFTDimension(int box_id) {
    double A, B, C, alpha, beta, gamma;
    bool compute_pme = true;
    if (box_id == 0) {
        if (box_ABCabc_.size()) {
            A = box_ABCabc_[0];
            B = box_ABCabc_[1];
            C = box_ABCabc_[2];
            alpha = box_ABCabc_[3];
            beta = box_ABCabc_[4];
            gamma = box_ABCabc_[5];
        } else
            compute_pme = false;

    } else if (box_id == 1) {
        if (box_ABCabc_PMElocal_.size()) {
            A = box_ABCabc_PMElocal_[0];
            B = box_ABCabc_PMElocal_[1];
            C = box_ABCabc_PMElocal_[2];
            alpha = box_ABCabc_PMElocal_[3];
            beta = box_ABCabc_PMElocal_[4];
            gamma = box_ABCabc_PMElocal_[5];
        } else
            compute_pme = false;
    }

    if (!simcell_periodic_) compute_pme = false;

    std::vector<int> fft_grid(3, -1);

    if (compute_pme) {
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;

        helpme::PMEInstance<double> pme_solver_;
        if (user_fft_grid_.size()) pme_solver_.SetFFTDimension(user_fft_grid_);

        if (mpi_initialized_) {
            pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0, world_,
                                      PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
        } else {
            pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        }
        pme_solver_.setLatticeVectors(A, B, C, alpha, beta, gamma, PMEInstanceD::LatticeType::XAligned);

        fft_grid = pme_solver_.GetFFTDimension();
    }

    return fft_grid;
}

void Electrostatics::SetFFTDimension(std::vector<int> grid) {
    // Easy things to check
    // If not, throw exception

    // vector of size 3

    if (grid.size() > 0 && grid.size() != 3) {
        std::string text = std::string("FFT grid != 3");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // elements are positive

    for (int i = 0; i < grid.size(); ++i)
        if (grid[i] < 1) {
            std::string text = std::string("FFT grid dimensions must be positive");
            throw CUException(__func__, __FILE__, __LINE__, text);
        }

    user_fft_grid_ = grid;
}

void Electrostatics::setup_comm() {
#if MBX_ELEC_P2P_COMM == 1
    nncomm_nswap = 0;
    nncomm_sendproc = std::vector<int>{};
    nncomm_recvproc = std::vector<int>{};
    nncomm_dim = std::vector<int>{};
    nncomm_dir = std::vector<int>{};
    nncomm_send = std::vector<int>{};

    nncomm_boxlo = std::vector<double>{};
    nncomm_boxhi = std::vector<double>{};

    nncomm_cutlo = std::vector<double>{};
    nncomm_cuthi = std::vector<double>{};

    // pairwise cutoff for ghost

    double padding = 2.0;  // FIXME:: hard-coded value, but set from LAMMPS

    double cutoff_ghost = cutoff_ + padding;

    // FIXME:: hard-coded for orthogonal lattices
    // -- cuts below should remain in fractional coordinates and used for tests

    std::vector<double> len_(3);
    len_[0] = box_ABCabc_PMElocal_[0];
    len_[1] = box_ABCabc_PMElocal_[1];
    len_[2] = box_ABCabc_PMElocal_[2];

    std::vector<double> cut_frac(3);
    cut_frac[0] = cutoff_ghost / box_ABCabc_PMElocal_[0];
    cut_frac[1] = cutoff_ghost / box_ABCabc_PMElocal_[1];
    cut_frac[2] = cutoff_ghost / box_ABCabc_PMElocal_[2];

    // # of neigh procs needed in each dimension
    // -- borrowing from LAMMPS since that's how data is distributed

    nncomm_maxneed = std::vector<int>(3, 0);
    nncomm_maxneed[0] = static_cast<int>(cutoff_ghost * proc_grid_x_ / len_[0]) + 1;
    nncomm_maxneed[1] = static_cast<int>(cutoff_ghost * proc_grid_y_ / len_[1]) + 1;
    nncomm_maxneed[2] = static_cast<int>(cutoff_ghost * proc_grid_z_ / len_[2]) + 1;

    if (nncomm_maxneed[0] > proc_grid_x_) nncomm_maxneed[0] = proc_grid_x_;
    if (nncomm_maxneed[1] > proc_grid_y_) nncomm_maxneed[1] = proc_grid_y_;
    if (nncomm_maxneed[2] > proc_grid_z_) nncomm_maxneed[2] = proc_grid_z_;

    // find my proc grid location ; order hardcoded to match PME solver order ZYX

    std::vector<int> myloc(3);
    myloc[0] = mpi_rank_ % proc_grid_x_;
    myloc[1] = (mpi_rank_ % (proc_grid_x_ * proc_grid_y_)) / proc_grid_x_;
    myloc[2] = mpi_rank_ / (proc_grid_x_ * proc_grid_y_);

    // bounding box of sub-domain

    double fracx = 1.0 / (double)proc_grid_x_;
    double fracy = 1.0 / (double)proc_grid_y_;
    double fracz = 1.0 / (double)proc_grid_z_;

    nncomm_boxlo.push_back((double)myloc[0] * fracx);
    nncomm_boxhi.push_back((double)(myloc[0] + 1) * fracx);

    nncomm_boxlo.push_back((double)myloc[1] * fracy);
    nncomm_boxhi.push_back((double)(myloc[1] + 1) * fracy);

    nncomm_boxlo.push_back((double)myloc[2] * fracz);
    nncomm_boxhi.push_back((double)(myloc[2] + 1) * fracz);

    std::vector<int> proc(3);
    std::vector<int> proc_grid(3);

    proc_grid[0] = proc_grid_x_;
    proc_grid[1] = proc_grid_y_;
    proc_grid[2] = proc_grid_z_;

    for (int idim = 0; idim < 3; ++idim) {
        for (int ineed = 0; ineed < nncomm_maxneed[idim]; ++ineed) {
            // neighboring proc in + direction

            proc[0] = myloc[0];
            proc[1] = myloc[1];
            proc[2] = myloc[2];

            proc[idim] += ineed + 1;

            nncomm_send.push_back(1);
            if (proc[idim] >= proc_grid[idim]) {
                if (!simcell_periodic_) nncomm_send[nncomm_nswap] = 0;
                proc[idim] -= proc_grid[idim];
            }

            int p = proc[2] * proc_grid[0] * proc_grid[1] + proc[1] * proc_grid[0] + proc[0];

            // neighboring proc in - direction

            proc[0] = myloc[0];
            proc[1] = myloc[1];
            proc[2] = myloc[2];

            proc[idim] -= ineed + 1;

            nncomm_send.push_back(1);
            if (proc[idim] < 0) {
                if (!simcell_periodic_) nncomm_send[nncomm_nswap + 1] = 0;
                proc[idim] += proc_grid[idim];
            }

            int m = proc[2] * proc_grid[0] * proc_grid[1] + proc[1] * proc_grid[0] + proc[0];

            bool include_swap = true;

            for (int iswap = 0; iswap < nncomm_nswap; ++iswap)
                if (p == nncomm_sendproc[iswap] && m == nncomm_recvproc[iswap] && nncomm_dim[idim] == idim &&
                    nncomm_dir[iswap] == 1)
                    include_swap = false;

            if (include_swap) {
#ifdef _DEBUG_COMM
                if (mpi_rank_ == 0)
                    std::cout << "idim= " << idim << " ineed= " << ineed << "  dir= 1  proc= " << proc[0] << " "
                              << proc[1] << " " << proc[2] << "  p= " << p << std::endl;
#endif

                nncomm_sendproc.push_back(p);
                nncomm_recvproc.push_back(m);
                nncomm_dim.push_back(idim);
                nncomm_dir.push_back(1);

                nncomm_cutlo.push_back(0.5 * (nncomm_boxlo[idim] + nncomm_boxhi[idim]));
                nncomm_cuthi.push_back((nncomm_boxhi[idim] + 100.0));

                nncomm_nswap++;

#ifdef _DEBUG_COMM
                if (mpi_rank_ == 0)
                    std::cout << "idim= " << idim << " ineed= " << ineed << "  dir= -1  proc= " << proc[0] << " "
                              << proc[1] << " " << proc[2] << "  m= " << m << std::endl;
#endif

                nncomm_sendproc.push_back(m);
                nncomm_recvproc.push_back(p);
                nncomm_dim.push_back(idim);
                nncomm_dir.push_back(-1);

                nncomm_cutlo.push_back((nncomm_boxlo[idim] - 100.0));
                nncomm_cuthi.push_back(0.5 * (nncomm_boxlo[idim] + nncomm_boxhi[idim]));

                nncomm_nswap++;
            }
        }
    }

    // maximum size of send arrays

    nncomm_max_send_size = 3 * nsites_;  // over-estimate

    // loop over neighbors to determine largest recv array

    MPI_Request request;
    MPI_Status status;

    nncomm_max_recv_size = 0;
    for (size_t i = 0; i < nncomm_nswap; ++i) {
        int nrecv = 0;
        if (nncomm_recvproc[i] == mpi_rank_)
            nrecv = nncomm_max_send_size;
        else
            MPI_Irecv(&nrecv, 1, MPI_INT, nncomm_recvproc[i], 0, world_, &request);

        if (nncomm_sendproc[i] != mpi_rank_) MPI_Send(&nncomm_max_send_size, 1, MPI_INT, nncomm_sendproc[i], 0, world_);

        if (nncomm_recvproc[i] != mpi_rank_) MPI_Wait(&request, &status);

        if (nrecv > nncomm_max_recv_size) nncomm_max_recv_size = nrecv;
    }

    // buffers for MPI comm

    nncomm_buf_send_i = std::vector<int>(nncomm_max_send_size / 3 * 2);
    nncomm_buf_send_d = std::vector<double>(nncomm_max_send_size * 2);

    nncomm_buf_recv_i = std::vector<int>(nncomm_max_recv_size / 3);
    nncomm_buf_recv_d = std::vector<double>(nncomm_max_recv_size);

    // maps for un/packing data

    nncomm_rev_sendlist = std::vector<std::vector<int>>(6);
    nncomm_rev_recvlist = std::vector<std::vector<int>>(nncomm_nswap);

    nncomm_for_sendlist = std::vector<std::vector<int>>(nncomm_nswap);
    nncomm_for_recvlist = std::vector<std::vector<int>>(nncomm_nswap);

#ifdef _DEBUG_COMM
    for (int i = 0; i < num_mpi_ranks_; ++i) {
        if (mpi_rank_ == i) {
            if (i == 0) {
                std::cout << "proc_grid_= " << proc_grid_x_ << " " << proc_grid_y_ << " " << proc_grid_z_ << std::endl;
                std::cout << "maxneed= " << nncomm_maxneed[0] << " " << nncomm_maxneed[1] << " " << nncomm_maxneed[2]
                          << "  cutoff_ghost= " << cutoff_ghost << std::endl;
            }

            std::cout << "(" << i << ") myloc_= " << myloc[0] << " " << myloc[1] << " " << myloc[2] << std::endl;

            std::cout << "  nswap= " << nncomm_nswap << std::endl;

            std::cout << "          boxlo= ";
            for (int j = 0; j < 3; ++j) std::cout << " " << nncomm_boxlo[j];
            std::cout << std::endl;

            std::cout << "          boxhi= ";
            for (int j = 0; j < 3; ++j) std::cout << " " << nncomm_boxhi[j];
            std::cout << std::endl;

            std::cout << "          cutlo= ";
            for (int j = 0; j < nncomm_nswap; ++j) std::cout << " " << nncomm_cutlo[j];
            std::cout << std::endl;

            std::cout << "          cuthi= ";
            for (int j = 0; j < nncomm_nswap; ++j) std::cout << " " << nncomm_cuthi[j];
            std::cout << std::endl;

            std::cout << "          sendproc= ";
            for (int j = 0; j < nncomm_nswap; ++j) std::cout << " " << nncomm_sendproc[j];
            std::cout << std::endl;

            std::cout << "          recvproc= ";
            for (int j = 0; j < nncomm_nswap; ++j) std::cout << " " << nncomm_recvproc[j];
            std::cout << std::endl;

            std::cout << "          nncomm_dim= ";
            for (int i = 0; i < nncomm_nswap; ++i) std::cout << " " << nncomm_dim[i];
            std::cout << std::endl;

            std::cout << "          nncomm_dir= ";
            for (int i = 0; i < nncomm_nswap; ++i) std::cout << " " << nncomm_dir[i];
            std::cout << std::endl;

            std::cout << "          nncomm_send= ";
            for (int i = 0; i < nncomm_nswap; ++i) std::cout << " " << nncomm_send[i];
            std::cout << std::endl;
        }
        MPI_Barrier(world_);
    }

    for (int i = 0; i < num_mpi_ranks_; ++i) {
        if (mpi_rank_ == i) {
            std::cout << "(" << i << ") max_send_size= " << nncomm_max_send_size
                      << "  nncomm_max_recv_size= " << nncomm_max_recv_size << std::endl;
        }
        MPI_Barrier(world_);
    }
#endif
#endif
}

}  // namespace elec

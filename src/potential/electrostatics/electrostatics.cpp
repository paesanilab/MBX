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

// When turning polarization off, don't set the 1/polarity value to max_dbl because it gets
// added to the potential and field values, generating inf values that result in NaN energies.
// Any large value will do; it doesn't enter the energy or forces below about 1e-12 anyway
const double BIGNUM = 1e50;

namespace elec {

const double PIQSRT = sqrt(M_PI);

void Electrostatics::SetCutoff(double cutoff) { cutoff_ = cutoff; }

void Electrostatics::SetEwaldAlpha(double alpha) { ewald_alpha_ = alpha; }

void Electrostatics::SetEwaldGridDensity(double density) { pme_grid_density_ = density; }

void Electrostatics::SetEwaldSplineOrder(int order) { pme_spline_order_ = order; }

void Electrostatics::SetDipoleTolerance(double tol) { tolerance_ = tol; }

void Electrostatics::SetDipoleMaxIt(size_t maxit) { maxit_ = maxit;}

void Electrostatics::Initialize(const std::vector<double> &chg, const std::vector<double> &chg_grad,
                                const std::vector<double> &polfac, const std::vector<double> &pol,
                                const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                                const std::vector<size_t> &sites, const std::vector<size_t> &first_ind,
                                const std::vector<std::pair<std::string, size_t>> &mon_type_count, 
				const std::vector<size_t> &islocal, const bool do_grads,
                                const double tolerance, const size_t maxit, const std::string dip_method,
                                const std::vector<double> &box) {
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
    mon_id_ = mon_id;
    sites_ = sites;
    first_ind_ = first_ind;
    mon_type_count_ = mon_type_count;
    do_grads_ = do_grads;
    tolerance_ = tolerance;
    maxit_ = maxit;
    dip_method_ = dip_method;
    box_ = box;
    use_pbc_ = box.size();
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
    virial_ = std::vector<double>(9,0.0);
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

    sys_mol_perm_mu_ = std::vector<double>(3*nmon_total_,0.0);
    sys_mol_mu_ = std::vector<double>(3*nmon_total_,0.0);
    
    calc_virial_ = true;
    if (do_grads == false) {
        calc_virial_=false;
    }
    
    if(!mpi_initialized_) {
      world_ = 0;
      mpi_rank_ = 0;
      proc_grid_x_ = 1;
      proc_grid_y_ = 1;
      proc_grid_z_ = 1;
      num_mpi_ranks_ = 1;
    }

    mbxt_ele_count_ = std::vector<size_t>(ELE_NUM_TIMERS, 0);
    mbxt_ele_time_  = std::vector<double>(ELE_NUM_TIMERS, 0.0);
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
    if (use_pbc_) box_inverse_ = InvertUnitCell(box_);

    size_t nsites3 = nsites_ * 3;

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

    ReorderData();

    has_energy_ = false;
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

void Electrostatics::CalculatePermanentElecField() {
    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
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

    std::fill(phi_.begin(), phi_.end(), 0);
    std::fill(Efq_.begin(), Efq_.end(), 0);

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Obtain excluded pairs for monomer type mt
        systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);

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
                for (size_t m = mstart; m < nmon; m+=num_mpi_ranks_) {
                    elec_field.CalcPermanentElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                                                      chg_.data() + fi_sites, chg_.data() + fi_sites, m, m, m + 1, nmon,
                                                      nmon, i, j, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex, &ey, &ez, &phi1,
                                                      phi_.data() + fi_sites, Efq_.data() + fi_crd, elec_scale_factor,
                                                      ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_, &virial_);
                    phi_[fi_sites + inmon + m] += phi1;
                    Efq_[fi_crd + inmon3 + m] += ex;
                    Efq_[fi_crd + inmon3 + nmon + m] += ey;
                    Efq_[fi_crd + inmon3 + nmon2 + m] += ez;
                }
            }
        }

        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    
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
        size_t ns1 = sites_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;

        // For each monomer type mt1, loop over all the other monomer types
        // mt2 >= mt1 to avoid double counting
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_[fi_mon2];
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
	    for (size_t m1 = m1start; m1 < nmon1; m1+=num_mpi_ranks_) {
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
                    xyz_sitei[0] = xyz_[fi_crd1 + inmon13 + m1];
                    xyz_sitei[1] = xyz_[fi_crd1 + inmon13 + nmon1 + m1];
                    xyz_sitei[2] = xyz_[fi_crd1 + inmon13 + 2 * nmon1 + m1];

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
                        std::copy(xyz_.begin() + start_j + m2init, xyz_.begin() + start_j + nmon2, xyz_sitej.begin());
                        // Copy y
                        std::copy(xyz_.begin() + start_j + nmon2 + m2init, xyz_.begin() + start_j + 2 * nmon2,
                                  xyz_sitej.begin() + size_j);
                        // Copy y
                        std::copy(xyz_.begin() + start_j + 2 * nmon2 + m2init, xyz_.begin() + start_j + 3 * nmon2,
                                  xyz_sitej.begin() + 2 * size_j);

                        // Vector that will tell the original position of the new sites
                        std::vector<double> chg_sitej(size_j);
                        std::vector<double> phi_sitej(size_j, 0.0);
                        std::vector<double> Efq_sitej(3 * size_j, 0.0);
                        // declare temporary virial for each pair
                        std::vector<double> virial_thread(9,0.0);

                        std::copy(chg_.begin() + fi_sites2 + nmon2 * j + m2init,
                                  chg_.begin() + fi_sites2 + nmon2 * (j + 1), chg_sitej.begin());

                        // Check if A = 0 and call the proper field calculation
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
                        double elec_scale_factor = 1;
                        local_field->CalcPermanentElecField(
                            xyz_.data() + fi_crd1, xyz_sitej.data(), chg_.data() + fi_sites1, chg_sitej.data(), m1, 0,
                            size_j, nmon1, size_j, i, 0, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex_thread, &ey_thread,
                            &ez_thread, &phi1_thread, phi_sitej.data(), Efq_sitej.data(), elec_scale_factor,
                            ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_, &virial_thread);
                        //                        local_field->CalcPermanentElecField(
                        //                            xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, chg_.data() +
                        //                            fi_sites1, chg_.data() + fi_sites2, m1, m2init, nmon2, nmon1,
                        //                            nmon2, i, 0, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex_thread,
                        //                            &ey_thread, &ez_thread, &phi1_thread, phi_2_pool[rank].data(),
                        //                            Efq_2_pool[rank].data());

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
                        for (size_t k = 0; k <9 ; k++) {
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
                    Efq_[fi_crd1 + k] += Efq_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    Efq_[fi_crd2 + k] += Efq_2_pool[rank][k];
                }
                kend1 = phi_1_pool[rank].size();
                kend2 = phi_2_pool[rank].size();
                for (size_t k = 0; k < kend1; k++) {
                    phi_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_[fi_sites2 + k] += phi_2_pool[rank][k];
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
    MPI_Allreduce(MPI_IN_PLACE, phi_.data(),    phi_.size(), MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, Efq_.data(),    Efq_.size(), MPI_DOUBLE, MPI_SUM, world_);
    MPI_Allreduce(MPI_IN_PLACE, virial_.data(), virial_.size(), MPI_DOUBLE, MPI_SUM, world_);
    double time2 = MPI_Wtime();
#endif
    
    if (ewald_alpha_ > 0 && use_pbc_) {
        helpme::PMEInstance<double> pme_solver_;
        // Compute the reciprocal space terms, using PME
        double A = box_[0], B = box_[4], C = box_[8];
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
	if(mpi_initialized_) {
	  pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0,
				    world_, PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
	} else {
	  pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
	}
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_.data(), nsites_, 1);
        auto result = helpme::Matrix<double>(rec_phi_and_field_.data(), nsites_, 4);
        std::fill(rec_phi_and_field_.begin(), rec_phi_and_field_.end(), 0);
        pme_solver_.computePRec(0, charges, coords, coords, 1, result);

#if HAVE_MPI == 1
	MPI_Allreduce(MPI_IN_PLACE, rec_phi_and_field_.data(), rec_phi_and_field_.size(), MPI_DOUBLE, MPI_SUM, world_);
#endif
	
        // Resort phi from system order
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
                    phi_[fi_sites + inmon + m] += result_ptr[0];
                    Efq_[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[1];
                    Efq_[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[2];
                    Efq_[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[3];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }
        // The Ewald self potential
        double *phi_ptr = phi_.data();
        for (const auto &q : chg_) {
            *phi_ptr -= 2 * ewald_alpha_ / PIQSRT * q;
            ++phi_ptr;
        }
    }

#if HAVE_MPI == 1
    double time3 = MPI_Wtime();

    mbxt_ele_count_[ELE_PERMDIP_REAL]++;
    mbxt_ele_time_[ELE_PERMDIP_REAL] += time2 - time1;

    mbxt_ele_count_[ELE_PERMDIP_PME]++;
    mbxt_ele_time_[ELE_PERMDIP_PME] += time3 - time2;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// DIPOLE ELECTRIC FIELD ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

        if (iter > maxit_) {
            // Exit with error
            std::cerr << "Max number of iterations reached" << std::endl;
            std::exit(EXIT_FAILURE);
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

void Electrostatics::ComputeDipoleField(std::vector<double> &in_v, std::vector<double> &out_v) {
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
    
    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
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
        systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);
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
		for (size_t m = mstart; m < nmon; m+=num_mpi_ranks_) {
                    // TODO. Slowest function
                    elec_field.CalcDipoleElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd, in_ptr + fi_crd,
                                                   in_ptr + fi_crd, m, m, m + 1, nmon, nmon, i, j, Asqsqi, aDD,
                                                   out_v.data() + fi_crd, &ex, &ey, &ez, ewald_alpha_, use_pbc_, box_,
                                                   box_inverse_, cutoff_);
                    out_v[fi_crd + inmon3 + m] += ex;
                    out_v[fi_crd + inmon3 + nmon + m] += ey;
                    out_v[fi_crd + inmon3 + nmon2 + m] += ez;
                }
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
    
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
            for (size_t m1 = m1start; m1 < nmon1; m1+=num_mpi_ranks_) {
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
                        local_field->CalcDipoleElecField(
                            xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, in_ptr + fi_crd1, in_ptr + fi_crd2, m1,
                            m2init, nmon2, nmon1, nmon2, i, j, Asqsqi, aDD, Efd_2_pool[rank].data(), &ex_thread,
                            &ey_thread, &ez_thread, ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_);
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
    
    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        //int fi_mon = 0;
        //int fi_crd = 0;
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

        helpme::PMEInstance<double> pme_solver_;
        // Compute the reciprocal space terms, using PME
        double A = box_[0], B = box_[4], C = box_[8];
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
	if(mpi_initialized_) {
	  pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0,
				    world_, PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
	} else {
	  pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
	}
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto result = helpme::Matrix<double>(sys_Efd_.data(), nsites_, 3);
        std::fill(sys_Efd_.begin(), sys_Efd_.end(), 0.0);
        pme_solver_.computePRec(-1, dipoles, coords, coords, -1, result);

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

void Electrostatics::CalculateElecEnergy() {
    Eperm_ = 0.0;
    for (size_t i = 0; i < nsites_; i++) Eperm_ += phi_[i] * chg_[i];
    Eperm_ *= 0.5 * constants::COULOMB;

    // Induced Electrostatic energy (chg-dip, dip-dip, pol)
    Eind_ = 0.0;
    for (size_t i = 0; i < 3 * nsites_; i++) Eind_ -= mu_[i] * Efq_[i];
    Eind_ *= 0.5 * constants::COULOMB;
#ifdef PRINT_TERMS
    std::cerr << "E_ind = " << Eind_ << "   E_perm = " << Eperm_ << std::endl;
#endif
}

void Electrostatics::CalculateGradients(std::vector<double> &grad) {
    // Reset grad
    grad_ = std::vector<double>(3 * nsites_, 0.0);

    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
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
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t i = 0; i < ns; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t m = 0; m < nmon; m++) {
                grad_[fi_crd + inmon3 + m] -= chg_[fi_sites + inmon + m] * Efq_[fi_crd + inmon3 + m];
                grad_[fi_crd + inmon3 + nmon + m] -= chg_[fi_sites + inmon + m] * Efq_[fi_crd + inmon3 + nmon + m];
                grad_[fi_crd + inmon3 + nmon2 + m] -= chg_[fi_sites + inmon + m] * Efq_[fi_crd + inmon3 + nmon2 + m];
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);
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
                    elec_field.CalcElecFieldGrads(xyz_.data() + fi_crd, xyz_.data() + fi_crd, chg_.data() + fi_sites,
                                                  chg_.data() + fi_sites, mu_.data() + fi_crd, mu_.data() + fi_crd, m,
                                                  m, m + 1, nmon, nmon, i, j, aDD, aCD_, Asqsqi, &ex, &ey, &ez, &phi1,
                                                  phi_.data() + fi_sites, grad_.data() + fi_crd, elec_scale_factor,
                                                  ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_, &virial_);
                    phi_[fi_sites + inmon + m] += phi1;
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

    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_[fi_mon2];
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
                        local_field->CalcElecFieldGrads(
                            xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, chg_.data() + fi_sites1,
                            chg_.data() + fi_sites2, mu_.data() + fi_crd1, mu_.data() + fi_crd2, m1, m2init, nmon2,
                            nmon1, nmon2, i, j, aDD, aCD_, Asqsqi, &ex_thread, &ey_thread, &ez_thread, &phi1_thread,
                            phi_2_pool[rank].data(), grad_2_pool[rank].data(), 1, ewald_alpha_, use_pbc_, box_,
                            box_inverse_, cutoff_, &virial_pool[rank]);
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
                    phi_[fi_sites1 + k] += phi_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    phi_[fi_sites2 + k] += phi_2_pool[rank][k];
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
    double time2 = MPI_Wtime();
#endif
    
    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        //int fi_mon = 0;
        //int fi_sites = 0;
        //int fi_crd = 0;
        fi_mon = 0;
        fi_sites = 0;
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
                    sys_chg_[fi_sites + mns + i] = chg_[fi_sites + m + inmon];
                    sys_mu_[fi_crd + mns3 + 3 * i] = mu_[inmon3 + m + fi_crd];
                    sys_mu_[fi_crd + mns3 + 3 * i + 1] = mu_[inmon3 + m + fi_crd + nmon];
                    sys_mu_[fi_crd + mns3 + 3 * i + 2] = mu_[inmon3 + m + fi_crd + nmon2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
            fi_crd += nmon * ns * 3;
        }

        helpme::PMEInstance<double> pme_solver_;
        // Compute the reciprocal space terms, using PME
        double A = box_[0], B = box_[4], C = box_[8];
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
	if(mpi_initialized_) {
	  pme_solver_.setupParallel(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0,
				    world_, PMEInstanceD::NodeOrder::ZYX, proc_grid_x_, proc_grid_y_, proc_grid_z_);
	} else {
	  pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
	}
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_.data(), nsites_, 1);
        auto result = helpme::Matrix<double>(nsites_, 10);
        
        if (calc_virial_) {
            std::vector<double> trecvir(6,0.0);
            std::vector<double> tforcevec(sys_grad_.size(), 0.0);

            auto drecvirial = helpme::Matrix<double>(trecvir.data(), 6,1);
            auto tmpforces2  = helpme::Matrix<double>(tforcevec.data(), nsites_, 3);

            double fulldummy_rec_energy = pme_solver_.computeEFVRecIsotropicInducedDipoles(0, charges, dipoles,
                                          PMEInstanceD::PolarizationType::Mutual, coords, tmpforces2, drecvirial);

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

        pme_solver_.computePRec(-1, dipoles, coords, coords, 2, result);

	double * ptr = result[0];
#if HAVE_MPI == 1
	MPI_Allreduce(MPI_IN_PLACE, ptr, nsites_*10, MPI_DOUBLE, MPI_SUM, world_);
#endif
	
        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        double fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double chg = sys_chg_[fi_sites + mns + i];
                    const double *mu = &sys_mu_[fi_crd + 3 * mns + 3 * i];
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
                    phi_[fi_sites + i * nmon + m] += Phi;
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
        pme_solver_.computePRec(0, charges, coords, coords, -2, result);

#if HAVE_MPI == 1
	MPI_Allreduce(MPI_IN_PLACE, ptr, nsites_*10, MPI_DOUBLE, MPI_SUM, world_);
#endif
	
        // Resort field from system order
        fi_mon = 0;
        fi_sites = 0;
        fi_crd = 0;
        fac = constants::COULOMB;
        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            for (size_t m = 0; m < nmon; m++) {
                size_t mns = m * ns;
                for (size_t i = 0; i < ns; i++) {
                    const double *result_ptr = result[fi_sites + mns + i];
                    const double *mu = &sys_mu_[fi_crd + 3 * mns + 3 * i];
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
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;
                sys_Efq_[fi_crd + mns3 + 3 * i] = Efq_[inmon3 + m + fi_crd];
                sys_Efq_[fi_crd + mns3 + 3 * i + 1] = Efq_[inmon3 + m + fi_crd + nmon];
                sys_Efq_[fi_crd + mns3 + 3 * i + 2] = Efq_[inmon3 + m + fi_crd + nmon2];

                sys_Efd_[fi_crd + mns3 + 3 * i] = Efd_[inmon3 + m + fi_crd];
                sys_Efd_[fi_crd + mns3 + 3 * i + 1] = Efd_[inmon3 + m + fi_crd + nmon];
                sys_Efd_[fi_crd + mns3 + 3 * i + 2] = Efd_[inmon3 + m + fi_crd + nmon2];

                sys_mu_[fi_crd + mns3 + 3 * i] = mu_[inmon3 + m + fi_crd];
                sys_mu_[fi_crd + mns3 + 3 * i + 1] = mu_[inmon3 + m + fi_crd + nmon];
                sys_mu_[fi_crd + mns3 + 3 * i + 2] = mu_[inmon3 + m + fi_crd + nmon2];

                sys_phi_[fi_sites + mns + i] = phi_[fi_sites + m + inmon];

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

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        std::string id = mon_id_[fi_mon];

        // Redistribute gradients
        systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, grad);

        // Gradients due to position dependant charges
        if (calc_virial_){ // calculate virial if need be
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_, grad, sys_chg_grad_,xyz_.data() + fi_crd, &virial_);
        } else {
            systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_, grad, sys_chg_grad_);
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

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
    //double db = constants::DEBYE;
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
    //double db = constants::DEBYE;
    double db = 1.0;
    for (size_t i = 0; i < sys_chg_.size(); i++) {
        double my_chg = sys_chg_[i];
        for (size_t j = 0; j < 3; j++) {
            sys_perm_mu_[3*i + j] = db * my_chg * sys_xyz_[3*i + j];
        }
    }

    return sys_perm_mu_;
}

std::vector<double> Electrostatics::GetMolecularInducedDipoles() {
    GetInducedDipoles();
    std::fill(sys_mol_mu_.begin(), sys_mol_mu_.end(),0.0);

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
                    sys_mol_mu_[3*fi_mon + 3*m + j] += sys_mu_[fi_crd + mns3 + 3*i + j];
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
    std::fill(sys_mol_perm_mu_.begin(), sys_mol_perm_mu_.end(),0.0);

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
                    sys_mol_perm_mu_[3*fi_mon + 3*m + j] += sys_perm_mu_[fi_crd + mns3 + 3*i + j];
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

  double Electrostatics::GetElectrostatics(std::vector<double> &grad, std::vector<double> *virial,
					   bool use_ghost) {
    std::fill(virial_.begin(), virial_.end(),0.0);
    CalculatePermanentElecField();
    CalculateDipoles();
    CalculateElecEnergy();
    if (do_grads_) CalculateGradients(grad);
    //update viral
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
  
}  // namespace elec


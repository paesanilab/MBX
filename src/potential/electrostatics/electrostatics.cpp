#include "potential/electrostatics/electrostatics.h"
#include <iomanip>

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

const double SQRTPI = sqrt(M_PI);

std::vector<double> InvertUnitCell(const std::vector<double> &box) {
    double determinant = box[0] * (box[4] * box[8] - box[7] * box[5]) - box[1] * (box[3] * box[8] - box[5] * box[6]) +
                         box[2] * (box[3] * box[7] - box[4] * box[6]);

    double determinant_inverse = 1 / determinant;
    std::vector<double> box_inverse(9);
    box_inverse[0] = (box[4] * box[8] - box[7] * box[5]) * determinant_inverse;
    box_inverse[1] = (box[2] * box[7] - box[1] * box[8]) * determinant_inverse;
    box_inverse[2] = (box[1] * box[5] - box[2] * box[4]) * determinant_inverse;
    box_inverse[3] = (box[5] * box[6] - box[3] * box[8]) * determinant_inverse;
    box_inverse[4] = (box[0] * box[8] - box[2] * box[6]) * determinant_inverse;
    box_inverse[5] = (box[3] * box[2] - box[0] * box[5]) * determinant_inverse;
    box_inverse[6] = (box[3] * box[7] - box[6] * box[4]) * determinant_inverse;
    box_inverse[7] = (box[6] * box[1] - box[0] * box[7]) * determinant_inverse;
    box_inverse[8] = (box[0] * box[4] - box[3] * box[1]) * determinant_inverse;
    return box_inverse;
}

void Electrostatics::SetCutoff(double cutoff) { cutoff_ = cutoff; }

void Electrostatics::SetEwaldAlpha(double alpha) { ewald_alpha_ = alpha; }

void Electrostatics::SetEwaldGridDensity(double density) { pme_grid_density_ = density; }

void Electrostatics::SetEwaldSplineOrder(int order) { pme_spline_order_ = order; }

void Electrostatics::Initialize(const std::vector<double> &chg, const std::vector<double> &chg_grad,
                                const std::vector<double> &polfac, const std::vector<double> &pol,
                                const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                                const std::vector<size_t> &sites, const std::vector<size_t> &first_ind,
                                const std::vector<std::pair<std::string, size_t>> &mon_type_count, const bool do_grads,
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
    xyz_ = std::vector<double>(nsites3, 0.0);
    grad_ = std::vector<double>(nsites3, 0.0);
    sys_grad_ = std::vector<double>(nsites3, 0.0);
    chg_ = std::vector<double>(nsites_, 0.0);
    pol_sqrt_ = std::vector<double>(nsites3, 0.0);

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
    SetAspcParameters(4);
    mu_pred_ = std::vector<double>(nsites3, 0.0);

    if (use_pbc_) box_inverse_ = InvertUnitCell(box_);
    ReorderData();
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
                for (size_t m = 0; m < nmon; m++) {
                    elec_field.CalcPermanentElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                                                      chg_.data() + fi_sites, chg_.data() + fi_sites, m, m, m + 1, nmon,
                                                      nmon, i, j, Ai, Asqsqi, aCC_, aCC1_4_, g34_, &ex, &ey, &ez, &phi1,
                                                      phi_.data() + fi_sites, Efq_.data() + fi_crd, elec_scale_factor,
                                                      ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_);
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
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                Efq_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                Efq_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
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
                            ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_);
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

    if (ewald_alpha_ > 0 && use_pbc_) {
        helpme::PMEInstance<double> pme_solver_;
        // Compute the reciprocal space terms, using PME
        double A = box_[0], B = box_[4], C = box_[8];
        int grid_A = pme_grid_density_ * A;
        int grid_B = pme_grid_density_ * B;
        int grid_C = pme_grid_density_ * C;
        pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_.data(), nsites_, 1);
        auto result = helpme::Matrix<double>(rec_phi_and_field_.data(), nsites_, 4);
        std::fill(rec_phi_and_field_.begin(), rec_phi_and_field_.end(), 0);
        pme_solver_.computePRec(0, charges, coords, coords, 1, result);

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
            *phi_ptr -= 2 * ewald_alpha_ / SQRTPI * q;
            ++phi_ptr;
        }
    }
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
    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

    size_t inv_size = in_v.size();
    std::vector<std::vector<double>> pv_pool(nthreads, std::vector<double>(inv_size, 0.0));

#ifdef DEBUG
    std::vector<double> ts2_all(inv_size * inv_size, 0.0);
#endif

    // Sites on the same monomer
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;

    double aDD = 0.055;

    ElectroTensorShort elec_tensor(maxnmon_);

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    std::fill(out_v.begin(), out_v.end(), 0.0);

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        size_t nmon3 = nmon * 3;
        size_t nmon32 = nmon3 * 2;
        std::vector<double> ts2(3 * nmon3);
        std::vector<double> ts1(nmon3);
        size_t fi_mon3 = 3 * fi_mon;
        // Get excluded pairs for this monomer
        systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);
        for (size_t i = 0; i < ns - 1; i++) {
            size_t inmon = i * nmon;
            size_t inmon3 = 3 * inmon;
            for (size_t j = i + 1; j < ns; j++) {
                size_t jnmon = j * nmon;
                size_t jnmon3 = 3 * jnmon;
                // Set the proper aDD
                bool is12 = systools::IsExcluded(exc12, i, j);
                bool is13 = systools::IsExcluded(exc13, i, j);
                bool is14 = systools::IsExcluded(exc14, i, j);
                aDD = systools::GetAdd(is12, is13, is14, mon_id_[fi_mon]);

                double pfipfj = sqrt(pol_[fi_sites + i] * pol_[fi_sites + j]);
                double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
                std::fill(ts1.begin(), ts1.end(), 0.0);
                std::fill(ts2.begin(), ts2.end(), 0.0);
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
                    // TODO. Slowest function
                    elec_tensor.CalcT1AndT2(xyz_.data() + fi_crd, xyz_.data() + fi_crd, m, m, m + 1, nmon, nmon, i, j,
                                            Asqsqi, aDD, nsites_, ts1.data(), ts2.data());
                }

                // ts2 for this monomer and pairs are calculated
                // this is due to mon2
                for (size_t kk = 0; kk < 3; kk++) {
                    size_t kknmon = kk * nmon;
#pragma omp simd
                    for (size_t k = 0; k < nmon; k++) {
#ifdef DEBUG
                        size_t row = fi_crd + inmon3 + k;
                        size_t col = fi_crd + jnmon3 + k;
#endif

                        out_v[fi_crd + inmon3 + k] -= pfipfj * in_v[fi_crd + jnmon3 + k + kknmon] * ts2[k + kknmon];

                        out_v[fi_crd + inmon3 + k + nmon] -=
                            pfipfj * in_v[fi_crd + jnmon3 + k + kknmon] * ts2[k + kknmon + nmon3];

                        out_v[fi_crd + inmon3 + k + nmon2] -=
                            pfipfj * in_v[fi_crd + jnmon3 + k + kknmon] * ts2[k + kknmon + nmon32];

#ifdef DEBUG
                        ts2_all[row * inv_size + col + kknmon] = -pfipfj * ts2[k + kknmon];
                        ts2_all[(row + nmon) * inv_size + col + kknmon] = -pfipfj * ts2[k + kknmon + nmon3];
                        ts2_all[(row + nmon2) * inv_size + col + kknmon] = -pfipfj * ts2[k + kknmon + nmon32];
#endif
                    }
                }

                // This is due to mon 1
                for (size_t kk = 0; kk < 3; kk++) {
                    size_t kknmon = kk * nmon;
#ifdef _OPENMP
#pragma omp simd
#endif
                    for (size_t k = 0; k < nmon; k++) {
#ifdef DEBUG
                        size_t row = fi_crd + jnmon3 + k;
                        size_t col = fi_crd + inmon3 + k;
#endif

                        out_v[fi_crd + jnmon3 + k + kknmon] -= pfipfj * in_v[fi_crd + inmon3 + k] * ts2[k + kknmon];

                        out_v[fi_crd + jnmon3 + k + kknmon] -=
                            pfipfj * in_v[fi_crd + inmon3 + k + nmon] * ts2[k + kknmon + nmon3];

                        out_v[fi_crd + jnmon3 + k + kknmon] -=
                            pfipfj * in_v[fi_crd + inmon3 + k + nmon2] * ts2[k + kknmon + nmon32];

#ifdef DEBUG
                        ts2_all[(row + kknmon) * inv_size + col] = -pfipfj * ts2[k + kknmon];
                        ts2_all[(row + kknmon) * inv_size + col + nmon] = -pfipfj * ts2[k + kknmon + nmon3];
                        ts2_all[(row + kknmon) * inv_size + col + nmon2] = -pfipfj * ts2[k + kknmon + nmon32];
#endif
                    }
                }
            }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef DEBUG
    std::cerr << "=== ts2 matrix after intramonomer =======" << std::endl;
    for (size_t i = 0; i < inv_size; i++) {
        for (size_t j = 0; j < inv_size; j++) {
            std::cerr << std::scientific << std::setprecision(3) << std::setw(11) << ts2_all[inv_size * i + j] << " ";
        }
        std::cerr << std::endl;
    }
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
        size_t nmon13 = nmon1 * 3;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = sites_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;
            size_t nmon23 = nmon2 * 3;
            size_t nmon232 = nmon23 * 2;

            bool same = (mt1 == mt2);
            // TODO add neighbour list here
            // Prepare for parallelization
            std::vector<std::shared_ptr<ElectroTensorShort>> elec_tensor_pool;
            std::vector<std::vector<double>> ts1_pool;
            std::vector<std::vector<double>> ts2_pool;
            for (size_t i = 0; i < nthreads; i++) {
                elec_tensor_pool.push_back(std::make_shared<ElectroTensorShort>(maxnmon_));
                ts1_pool.push_back(std::vector<double>(nmon23, 0.0));
                ts2_pool.push_back(std::vector<double>(3 * nmon23, 0.0));
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
                std::shared_ptr<ElectroTensorShort> local_elec_tensor = elec_tensor_pool[rank];
                size_t m2init = same ? m1 + 1 : 0;
                for (size_t i = 0; i < ns1; i++) {
                    size_t inmon1 = i * nmon1;
                    size_t inmon13 = 3 * inmon1;
                    size_t j2init = same ? i : 0;
                    for (size_t j = j2init; j < ns2; j++) {
                        size_t jnmon2 = j * nmon2;
                        size_t jnmon23 = 3 * jnmon2;
                        double pfipfj = sqrt(pol_[fi_sites1 + i] * pol_[fi_sites2 + j]);
                        std::fill(ts1_pool[rank].begin(), ts1_pool[rank].end(), 0.0);
                        std::fill(ts2_pool[rank].begin(), ts2_pool[rank].end(), 0.0);

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
                        if (same) {
                            local_elec_tensor->CalcT1AndT2(xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, m1, 0, m1,
                                                           nmon1, nmon2, i, j, Asqsqi, aDD, nsites_,
                                                           ts1_pool[rank].data(), ts2_pool[rank].data());
                        }
                        local_elec_tensor->CalcT1AndT2(xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, m1, m2init, nmon2,
                                                       nmon1, nmon2, i, j, Asqsqi, aDD, nsites_, ts1_pool[rank].data(),
                                                       ts2_pool[rank].data());

#ifdef DEBUG
                        std::cout << "Different monomers (m1 = " << m1 << "): i = " << i << ", j = " << j << std::endl;
                        for (size_t l = 0; l < ts2_pool[rank].size(); l++) {
                            std::cout << std::scientific << std::setprecision(3) << std::setw(11)
                                      << ts2_pool[rank][l] * (-pfipfj) << " ";
                            if ((l + 1) % nmon23 == 0) std::cout << std::endl;
                        }
#endif

// ts2 for this monomer and pairs are calculated
// this is due to mon2 (upper diagonal)
//#             ifdef _OPENMP
//#               pragma omp simd
//#             endif
#ifdef DEBUG
                        for (size_t k = 0; k < nmon23; k++) {
                            size_t row = fi_crd1 + inmon13 + m1;
                            size_t col = fi_crd2 + jnmon23 + k;
                            ts2_all[row * inv_size + col] += -pfipfj * ts2_pool[rank][k];
                            ts2_all[(row + nmon1) * inv_size + col] += -pfipfj * ts2_pool[rank][nmon23 + k];
                            ts2_all[(row + nmon12) * inv_size + col] += -pfipfj * ts2_pool[rank][nmon232 + k];
                        }
#endif
                        for (size_t k = 0; k < nmon23; k++) {
                            pv_pool[rank][fi_crd1 + inmon13 + m1] -=
                                pfipfj * in_v[fi_crd2 + jnmon23 + k] * ts2_pool[rank][k];
                        }
                        for (size_t k = 0; k < nmon23; k++) {
                            pv_pool[rank][nmon1 + fi_crd1 + inmon13 + m1] -=
                                pfipfj * in_v[fi_crd2 + jnmon23 + k] * ts2_pool[rank][nmon23 + k];
                        }
                        for (size_t k = 0; k < nmon23; k++) {
                            pv_pool[rank][nmon12 + fi_crd1 + inmon13 + m1] -=
                                pfipfj * in_v[fi_crd2 + jnmon23 + k] * ts2_pool[rank][nmon232 + k];
                        }
                        // This is due to mon 1 (lower diagonal)
                        if (!same || i != j) {
                            for (size_t k = 0; k < nmon23; k++) {
#ifdef DEBUG
                                size_t row = fi_crd2 + jnmon23 + k;
                                size_t col = fi_crd1 + inmon13 + m1;
#endif

                                pv_pool[rank][fi_crd2 + jnmon23 + k] -=
                                    pfipfj * in_v[fi_crd1 + inmon13 + m1] * ts2_pool[rank][k];

                                pv_pool[rank][fi_crd2 + jnmon23 + k] -=
                                    pfipfj * in_v[fi_crd1 + inmon13 + m1 + nmon1] * ts2_pool[rank][nmon23 + k];

                                pv_pool[rank][fi_crd2 + jnmon23 + k] -=
                                    pfipfj * in_v[fi_crd1 + inmon13 + m1 + nmon12] * ts2_pool[rank][nmon232 + k];

#ifdef DEBUG
                                ts2_all[row * inv_size + col] += -pfipfj * ts2_pool[rank][k];
                                ts2_all[row * inv_size + col + nmon1] += -pfipfj * ts2_pool[rank][nmon23 + k];
                                ts2_all[row * inv_size + col + nmon12] += -pfipfj * ts2_pool[rank][nmon232 + k];
#endif
                            }
                        }
                    }
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

    // Condense pv_pool
    for (size_t proc = 0; proc < nthreads; proc++) {
        for (size_t i = 0; i < inv_size; i++) {
            out_v[i] += pv_pool[proc][i];
        }
    }

    // Diagonal elements must be taken into account
    for (size_t i = 0; i < inv_size; i++) {
        out_v[i] += in_v[i];
    }

#ifdef DEBUG
    std::cerr << "=== ts2 matrix after intermonomer =======" << std::endl;
    for (size_t i = 0; i < inv_size; i++) {
        ts2_all[inv_size * i + i] = 1.0;
        for (size_t j = 0; j < inv_size; j++) {
            std::cerr << std::scientific << std::setprecision(3) << std::setw(11) << ts2_all[inv_size * i + j] << " ";
        }
        std::cerr << std::endl;
    }
#endif
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

// Initialize for first iteration
// for (size_t i = 0; i < nsites3; i++) {
//  mu_[i] *= pol_sqrt_[i];
//}

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

    if (k == 0) {
        b_consts_aspc_[0] = 2.0;
        b_consts_aspc_[1] = -1.0;
        omega_aspc_ = 2.0 / 3.0;
    } else if (k == 1) {
        b_consts_aspc_[0] = 2.5;
        b_consts_aspc_[1] = -2.0;
        b_consts_aspc_[2] = 0.5;
        omega_aspc_ = 0.6;
    } else if (k == 2) {
        b_consts_aspc_[0] = 2.8;
        b_consts_aspc_[1] = -2.8;
        b_consts_aspc_[2] = 1.2;
        b_consts_aspc_[3] = -0.2;
        omega_aspc_ = 4.0 / 7.0;
    } else if (k == 3) {
        b_consts_aspc_[0] = 3.0;
        b_consts_aspc_[1] = -24.0 / 7.0;
        b_consts_aspc_[2] = 27.0 / 14.0;
        b_consts_aspc_[3] = -4.0 / 7.0;
        b_consts_aspc_[4] = 1.0 / 14.0;
        omega_aspc_ = 5.0 / 9.0;
    } else if (k == 4) {
        b_consts_aspc_[0] = 22.0 / 7.0;
        b_consts_aspc_[1] = -55.0 / 14.0;
        b_consts_aspc_[2] = 55.0 / 21.0;
        b_consts_aspc_[3] = -22.0 / 21.0;
        b_consts_aspc_[4] = 5.0 / 21.0;
        b_consts_aspc_[5] = -1.0 / 42.0;
        omega_aspc_ = 6.0 / 11.0;
    }

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
        DipolesIterativeIteration();

        // Now the Electric dipole field is computed, and we update
        // the dipoles to get the corrector
        size_t fi_mon = 0;
        size_t fi_crd = 0;
        size_t fi_sites = 0;

        for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
            size_t ns = sites_[fi_mon];
            size_t nmon = mon_type_count_[mt].second;
            size_t nmon2 = nmon * 2;
            for (size_t i = 0; i < ns; i++) {
                // TODO assuming pol not site dependant
                double p = pol_[fi_sites + i];
                size_t inmon3 = 3 * i * nmon;
                for (size_t m = 0; m < nmon; m++) {
                    mu_[fi_crd + inmon3 + m] = p * (Efq_[fi_crd + inmon3 + m] + Efd_[fi_crd + inmon3 + m]);
                    mu_[fi_crd + inmon3 + nmon + m] =
                        p * (Efq_[fi_crd + inmon3 + nmon + m] + Efd_[fi_crd + inmon3 + nmon + m]);
                    mu_[fi_crd + inmon3 + nmon2 + m] =
                        p * (Efq_[fi_crd + inmon3 + nmon2 + m] + Efd_[fi_crd + inmon3 + nmon2 + m]);
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

void Electrostatics::DipolesIterativeIteration() {
    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    std::fill(Efd_.begin(), Efd_.end(), 0.0);

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
                for (size_t m = 0; m < nmon; m++) {
                    // TODO. Slowest function
                    elec_field.CalcDipoleElecField(xyz_.data() + fi_crd, xyz_.data() + fi_crd, mu_.data() + fi_crd,
                                                   mu_.data() + fi_crd, m, m, m + 1, nmon, nmon, i, j, Asqsqi, aDD,
                                                   Efd_.data() + fi_crd, &ex, &ey, &ez, ewald_alpha_, use_pbc_, box_,
                                                   box_inverse_, cutoff_);
                    Efd_[fi_crd + inmon3 + m] += ex;
                    Efd_[fi_crd + inmon3 + nmon + m] += ey;
                    Efd_[fi_crd + inmon3 + nmon2 + m] += ez;
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
                            xyz_.data() + fi_crd1, xyz_.data() + fi_crd2, mu_.data() + fi_crd1, mu_.data() + fi_crd2,
                            m1, m2init, nmon2, nmon1, nmon2, i, j, Asqsqi, aDD, Efd_2_pool[rank].data(), &ex_thread,
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
                    Efd_[fi_crd1 + k] += Efd_1_pool[rank][k];
                }
                for (size_t k = 0; k < kend2; k++) {
                    Efd_[fi_crd2 + k] += Efd_2_pool[rank][k];
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

    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        int fi_mon = 0;
        int fi_crd = 0;
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
                    sys_mu_[fi_crd + mns3 + 3 * i] = mu_[inmon3 + m + fi_crd];
                    sys_mu_[fi_crd + mns3 + 3 * i + 1] = mu_[inmon3 + m + fi_crd + nmon];
                    sys_mu_[fi_crd + mns3 + 3 * i + 2] = mu_[inmon3 + m + fi_crd + nmon2];
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
        pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto result = helpme::Matrix<double>(sys_Efd_.data(), nsites_, 3);
        std::fill(sys_Efd_.begin(), sys_Efd_.end(), 0.0);
        pme_solver_.computePRec(-1, dipoles, coords, coords, -1, result);

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
                    Efd_[3 * fi_sites + 3 * inmon + 0 * nmon + m] -= result_ptr[0];
                    Efd_[3 * fi_sites + 3 * inmon + 1 * nmon + m] -= result_ptr[1];
                    Efd_[3 * fi_sites + 3 * inmon + 2 * nmon + m] -= result_ptr[2];
                }
            }
            fi_mon += nmon;
            fi_sites += nmon * ns;
        }
        // The Ewald self field due to induced dipoles
        double slf_prefactor = (4.0 / 3.0) * ewald_alpha_ * ewald_alpha_ * ewald_alpha_ / SQRTPI;
        double *e_ptr = Efd_.data();
        for (const auto &mu : mu_) {
            *e_ptr += slf_prefactor * mu;
            ++e_ptr;
        }
    }
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
        DipolesIterativeIteration();
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
                    elec_field.CalcElecFieldGrads(xyz_.data() + fi_crd, xyz_.data() + fi_crd, chg_.data(),
                                                  chg_.data() + fi_sites, mu_.data() + fi_crd, mu_.data() + fi_crd, m,
                                                  m, m + 1, nmon, nmon, i, j, aDD, aCD_, Asqsqi, &ex, &ey, &ez, &phi1,
                                                  phi_.data() + fi_sites, grad_.data() + fi_crd, elec_scale_factor,
                                                  ewald_alpha_, use_pbc_, box_, box_inverse_, cutoff_);
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
            for (size_t i = 0; i < nthreads; i++) {
                field_pool.push_back(std::make_shared<ElectricFieldHolder>(maxnmon));
                grad_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
                grad_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
                phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
                phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
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
                            box_inverse_, cutoff_);
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
    if (ewald_alpha_ > 0 && use_pbc_) {
        // Sort the dipoles to the order helPME expects (for now)
        int fi_mon = 0;
        int fi_sites = 0;
        int fi_crd = 0;
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
        pme_solver_.setup(1, ewald_alpha_, pme_spline_order_, grid_A, grid_B, grid_C, 1, 0);
        pme_solver_.setLatticeVectors(A, B, C, 90, 90, 90, PMEInstanceD::LatticeType::XAligned);
        // N.B. these do not make copies; they just wrap the memory with some metadata
        auto coords = helpme::Matrix<double>(sys_xyz_.data(), nsites_, 3);
        auto dipoles = helpme::Matrix<double>(sys_mu_.data(), nsites_, 3);
        auto charges = helpme::Matrix<double>(sys_chg_.data(), nsites_, 1);
        std::vector<double> tmp(10 * nsites_);
        auto result = helpme::Matrix<double>(tmp.data(), nsites_, 10);
        std::fill(tmp.begin(), tmp.end(), 0);
        pme_solver_.computePRec(-1, dipoles, coords, coords, 2, result);

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
        std::fill(tmp.begin(), tmp.end(), 0);
        auto fieldgrad = helpme::Matrix<double>(tmp.data(), nsites_, 10);
        pme_solver_.computePRec(0, charges, coords, coords, -2, fieldgrad);

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
                    const double *result_ptr = fieldgrad[fi_sites + mns + i];
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
        systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites, sys_phi_, grad, sys_chg_grad_);
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
}

double Electrostatics::GetElectrostatics(std::vector<double> &grad) {
    CalculatePermanentElecField();
    CalculateDipoles();
    CalculateElecEnergy();
    if (do_grads_) CalculateGradients(grad);
    return Eperm_ + Eind_;
}

}  // namespace elec

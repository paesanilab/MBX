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

#include "potential/buckingham/buckingham.h"

namespace buck {

std::vector<size_t> Buckingham::GetIsLocal() { return islocal_; }
std::vector<size_t> Buckingham::GetEnforceTTMForIndex() { return force_ttm_for_idx_; }
std::vector<std::string> Buckingham::GetMonIds() { return mon_id_; }
std::vector<size_t> Buckingham::GetNumAtomsVector() { return num_atoms_; }
std::vector<std::pair<std::string, size_t> > Buckingham::GetMonTypeCount() { return mon_type_count_; }
bool Buckingham::GetDoGrads() { return do_grads_; }
std::vector<double> Buckingham::GetBox() { return box_; }
std::vector<double> Buckingham::GetBoxInverse() { return box_inverse_; }
std::vector<double> Buckingham::GetSystemXyz() { return sys_xyz_; }
std::vector<double> Buckingham::GetInternalXyz() { return xyz_; }
std::vector<double> Buckingham::GetInternalGrads() { return grad_; }
std::vector<double> Buckingham::GetSystemGrads() { return sys_grad_; }
std::vector<double> Buckingham::GetVirial() { return virial_; }
std::vector<std::pair<std::string, std::string> > Buckingham::GetBuckPairs() { return buck_pairs_; }
double Buckingham::GetCutoff() { return cutoff_; }

void Buckingham::Initialize(const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                            const std::vector<size_t> &num_atoms,
                            const std::vector<std::pair<std::string, size_t> > &mon_type_count,
                            const std::vector<size_t> force_ttm_for_idx, const std::vector<size_t> &islocal,
                            const bool do_grads = true, const std::vector<double> &box = {}) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Sys Xyz:\n";
    for (size_t i = 0; i < sys_xyz.size(); i++) {
        std::cerr << sys_xyz[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "mon_id:\n";
    for (size_t i = 0; i < mon_id.size(); i++) {
        std::cerr << mon_id[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "num_atoms:\n";
    for (size_t i = 0; i < num_atoms.size(); i++) {
        std::cerr << num_atoms[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "mon_type_count:\n";
    for (size_t i = 0; i < mon_type_count.size(); i++) {
        std::cerr << " { " << mon_type_count[i].first << " , " << mon_type_count[i].second << " } , ";
    }
    std::cerr << std::endl;

    std::cerr << "force_ttm_for_idx:\n";
    for (size_t i = 0; i < force_ttm_for_idx.size(); i++) {
        std::cerr << force_ttm_for_idx[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "islocal:\n";
    for (size_t i = 0; i < islocal.size(); i++) {
        std::cerr << islocal[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "box:\n";
    for (size_t i = 0; i < box.size(); i++) {
        std::cerr << box[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "doGrads = " << do_grads << std::endl;
#endif

    sys_xyz_ = sys_xyz;
    islocal_ = islocal;
    mon_id_ = mon_id;
    num_atoms_ = num_atoms;
    mon_type_count_ = mon_type_count;
    do_grads_ = do_grads;
    box_ = box;
    box_inverse_ = box.size() ? InvertUnitCell(box) : std::vector<double>{};
    use_pbc_ = box.size();
    force_ttm_for_idx_ = force_ttm_for_idx;

    natoms_ = sys_xyz_.size() / 3;
    size_t natoms3 = 3 * natoms_;
    xyz_ = std::vector<double>(natoms3, 0.0);
    grad_ = std::vector<double>(natoms3, 0.0);
    sys_grad_ = std::vector<double>(natoms3, 0.0);
    virial_ = std::vector<double>(9, 0.0);

    ReorderData();

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "xyz internal:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;
#endif
}

void Buckingham::SetJsonDispersionRepulsion(nlohmann::json repdisp_j) { repdisp_j_ = repdisp_j; }
void Buckingham::SetJsonMonomers(nlohmann::json mon_j) { mon_j_ = mon_j; }

nlohmann::json Buckingham::GetJsonDispersionRepulsion() { return repdisp_j_; }
nlohmann::json Buckingham::GetJsonMonomers() { return mon_j_; }

void Buckingham::SetNewParameters(const std::vector<double> &xyz,
                                  const std::vector<std::pair<std::string, std::string> > &buck_pairs,
                                  const std::vector<size_t> force_ttm_for_idx, bool do_grads = true,
                                  const double cutoff = 100.0, const std::vector<double> &box = {}) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "Sys Xyz:\n";
    for (size_t i = 0; i < xyz.size(); i++) {
        std::cerr << xyz[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "buck_pairs:\n";
    for (size_t i = 0; i < buck_pairs.size(); i++) {
        std::cerr << "{" << buck_pairs[i].first << " , " << buck_pairs[i].second << "} ,";
    }
    std::cerr << std::endl;

    std::cerr << "force_ttm_for_idx:\n";
    for (size_t i = 0; i < force_ttm_for_idx.size(); i++) {
        std::cerr << force_ttm_for_idx[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "box:\n";
    for (size_t i = 0; i < box.size(); i++) {
        std::cerr << box[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "doGrads = " << do_grads << std::endl;
    std::cerr << "cutoff = " << cutoff << std::endl;
#endif

    sys_xyz_ = xyz;
    box_ = box;
    box_inverse_ = box.size() ? InvertUnitCell(box) : std::vector<double>{};
    use_pbc_ = box.size();
    do_grads_ = do_grads;
    cutoff_ = cutoff;
    buck_pairs_ = buck_pairs;
    force_ttm_for_idx_ = force_ttm_for_idx;
    std::fill(grad_.begin(), grad_.end(), 0.0);

    ReorderData();

    // Put C6 and D6 in a vector
    size_t fi1 = 0;
    size_t fi2 = 0;
    size_t nmt = mon_type_count_.size();
    use_buck_all_ = std::vector<bool>(nmt * nmt);
    a_all_ = std::vector<std::vector<double> >(nmt * nmt);
    b_all_ = std::vector<std::vector<double> >(nmt * nmt);
    for (size_t mt1 = 0; mt1 < nmt; mt1++) {
        size_t ns1 = num_atoms_[fi1];
        fi2 = 0;
        for (size_t mt2 = 0; mt2 < nmt; mt2++) {
            size_t ns2 = num_atoms_[fi2];
            std::vector<double> av(ns1 * ns2), bv(ns1 * ns2);

            for (size_t i = 0; i < ns1; i++) {
                for (size_t j = 0; j < ns2; j++) {
                    double a = 0.0;
                    double b = 0.0;
                    bool do_buck = GetBuckParams(mon_id_[fi1], mon_id_[fi2], i, j, buck_pairs_, a, b, repdisp_j_);
                    if (i == 0 && j == 0) {
                        use_buck_all_[nmt * mt1 + mt2] = do_buck;
                    }
                    av[ns2 * i + j] = a;
                    bv[ns2 * i + j] = b;
                }
            }
            a_all_[nmt * mt1 + mt2] = av;
            b_all_[nmt * mt1 + mt2] = bv;
            fi2 += mon_type_count_[mt2].second;
        }
        fi1 += mon_type_count_[mt1].second;
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "xyz internal:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;
#endif
}

void Buckingham::ReorderData() {
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = num_atoms_[fi_mon];
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
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
}

double Buckingham::GetRepulsion(std::vector<double> &grad, std::vector<double> *virial, bool use_ghost) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "grad:\n";
    for (size_t i = 0; i < grad.size(); i++) {
        std::cerr << grad[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "virial:\n";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }

    std::cerr << "use_ghost = " << use_ghost << std::endl;
#endif

    calc_virial_ = false;

    if (virial != 0) {
        calc_virial_ = true;
    }

    std::fill(virial_.begin(), virial_.end(), 0.0);

    CalculateRepulsion(use_ghost);
    if (force_ttm_for_idx_.size() > 0) CalculateEnforcedRepulsion(use_ghost);

    if (calc_virial_) {
        (*virial)[0] += virial_[0];
        (*virial)[1] += virial_[1];
        (*virial)[2] += virial_[2];
        (*virial)[3] += virial_[3];
        (*virial)[4] += virial_[4];
        (*virial)[5] += virial_[5];
        (*virial)[6] += virial_[6];
        (*virial)[7] += virial_[7];
        (*virial)[8] += virial_[8];
    }

    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = num_atoms_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon * 2;
        for (size_t m = 0; m < nmon; m++) {
            size_t mns = m * ns;
            size_t mns3 = mns * 3;
            for (size_t i = 0; i < ns; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = 3 * inmon;

                grad[fi_crd + mns3 + 3 * i] += grad_[inmon3 + m + fi_crd];
                grad[fi_crd + mns3 + 3 * i + 1] += grad_[inmon3 + m + fi_crd + nmon];
                grad[fi_crd + mns3 + 3 * i + 2] += grad_[inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "grad:\n";
    for (size_t i = 0; i < grad.size(); i++) {
        std::cerr << grad[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "virial:\n";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }

    std::cerr << "Repulsion energy = " << rep_energy_ << std::endl;
#endif

    return rep_energy_;
}

void Buckingham::CalculateRepulsion(bool use_ghost) {
    rep_energy_ = 0.0;
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

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = num_atoms_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2 * nmon;

        // Check if buckingham needs to be done. Otherwise, skip.
        double dummy_a;
        double dummy_b;
        bool do_buck = use_buck_all_[mt * mon_type_count_.size() + mt];
        if (do_buck) {
            // Obtain excluded pairs for monomer type mt
            systools::GetExcluded(mon_id_[fi_mon], mon_j_, exc12, exc13, exc14);

            std::vector<std::vector<double> > grad_pool(nthreads, std::vector<double>(nmon * ns * 3, 0.0));
            std::vector<double> energy_pool(nthreads, 0.0);
            std::vector<std::vector<double> > virial_pool(nthreads, std::vector<double>(9, 0.0));

            // Loop over each pair of sites
            for (size_t i = 0; i < ns - 1; i++) {
                size_t inmon = i * nmon;
                size_t inmon3 = inmon * 3;
                for (size_t j = i + 1; j < ns; j++) {
                    // Continue only if i and j are not bonded
                    bool is12 = systools::IsExcluded(exc12, i, j);
                    bool is13 = systools::IsExcluded(exc13, i, j);
                    bool is14 = systools::IsExcluded(exc14, i, j);
                    bool is_excluded = (is12 || is13 || is14) ? true : false;

                    if (is_excluded) continue;

                    double a, b;
                    a = a_all_[mt * mon_type_count_.size() + mt][i * ns + j];
                    b = b_all_[mt * mon_type_count_.size() + mt][i * ns + j];

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
                    for (size_t m = 0; m < nmon; m++) {
                        int rank = 0;
#ifdef _OPENMP
                        rank = omp_get_thread_num();
#endif
                        bool include_monomer = false;
                        if (!use_ghost) include_monomer = true;
                        if (use_ghost && islocal_[fi_mon + m]) include_monomer = true;

                        if (include_monomer) {
                            double p1[3], g1[3];
                            p1[0] = xyz_[fi_crd + inmon3 + m];
                            p1[1] = xyz_[fi_crd + inmon3 + nmon + m];
                            p1[2] = xyz_[fi_crd + inmon3 + nmon2 + m];
                            std::fill(g1, g1 + 3, 0.0);
                            energy_pool[rank] +=
                                Repulsion(a, b, p1, xyz_.data() + fi_crd, g1, grad_pool[rank].data(), nmon, nmon, m,
                                          m + 1, i, j, do_grads_, cutoff_, box_, box_inverse_, use_ghost, islocal_,
                                          fi_mon + m, fi_mon, &virial_pool[rank]);

                            grad_pool[rank][inmon3 + m] += g1[0];
                            grad_pool[rank][inmon3 + nmon + m] += g1[1];
                            grad_pool[rank][inmon3 + nmon2 + m] += g1[2];
                        }
                    }
                }
            }

            // Compress data in phi and grad
            for (size_t rank = 0; rank < nthreads; rank++) {
                size_t kend = grad_pool[rank].size();
                for (size_t k = 0; k < kend; k++) {
                    grad_[fi_crd + k] += grad_pool[rank][k];
                }
                for (size_t k = 0; k < 9; k++) {
                    virial_[k] += virial_pool[rank][k];  // -= bc the atomindeces are switched relative to dlpoly
                }
                rep_energy_ += energy_pool[rank];
            }
        }  // if do_buck

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
        size_t ns1 = num_atoms_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;

        // For each monomer type mt1, loop over all the other monomer types
        // mt2 >= mt1 to avoid double counting
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = num_atoms_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;

            bool do_buck = use_buck_all_[mt1 * mon_type_count_.size() + mt2];
            if (do_buck) {
                // Check if monomer types 1 and 2 are the same
                // If so, same monomer won't be done, since it has been done in
                // previous loop.
                bool same = (mt1 == mt2);

                std::vector<std::vector<double> > grad1_pool(nthreads, std::vector<double>(nmon1 * ns1 * 3, 0.0));
                std::vector<std::vector<double> > grad2_pool(nthreads, std::vector<double>(nmon2 * ns2 * 3, 0.0));
                std::vector<double> energy_pool(nthreads, 0.0);
                std::vector<std::vector<double> > virial_pool(nthreads, std::vector<double>(9, 0.0));
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
                for (size_t m1 = 0; m1 < nmon1; m1++) {
                    int rank = 0;
#ifdef _OPENMP
                    rank = omp_get_thread_num();
#endif
                    size_t m2init = same ? m1 + 1 : 0;
                    for (size_t i = 0; i < ns1; i++) {
                        size_t inmon1 = i * nmon1;
                        size_t inmon13 = inmon1 * 3;
                        std::vector<double> xyz_sitei(3);
                        std::vector<double> g1(3, 0.0);
                        xyz_sitei[0] = xyz_[fi_crd1 + inmon13 + m1];
                        xyz_sitei[1] = xyz_[fi_crd1 + inmon13 + nmon1 + m1];
                        xyz_sitei[2] = xyz_[fi_crd1 + inmon13 + 2 * nmon1 + m1];

                        for (size_t j = 0; j < ns2; j++) {
                            double a, b;
                            a = a_all_[mt1 * mon_type_count_.size() + mt2][i * ns2 + j];
                            b = b_all_[mt1 * mon_type_count_.size() + mt2][i * ns2 + j];
                            energy_pool[rank] += Repulsion(a, b, xyz_sitei.data(), xyz_.data() + fi_crd2, g1.data(),
                                                           grad2_pool[rank].data(), nmon1, nmon2, m2init, nmon2, i, j,
                                                           do_grads_, cutoff_, box_, box_inverse_, use_ghost, islocal_,
                                                           fi_mon1 + m1, fi_mon2, &virial_pool[rank]);
                        }
                        grad1_pool[rank][inmon13 + m1] += g1[0];
                        grad1_pool[rank][inmon13 + nmon1 + m1] += g1[1];
                        grad1_pool[rank][inmon13 + nmon12 + m1] += g1[2];
                    }
                }

                // Compress data in Efq and phi
                for (size_t rank = 0; rank < nthreads; rank++) {
                    size_t kend1 = grad1_pool[rank].size();
                    size_t kend2 = grad2_pool[rank].size();
                    for (size_t k = 0; k < kend1; k++) {
                        grad_[fi_crd1 + k] += grad1_pool[rank][k];
                    }
                    for (size_t k = 0; k < kend2; k++) {
                        grad_[fi_crd2 + k] += grad2_pool[rank][k];
                    }
                    for (size_t k = 0; k < 9; k++) {
                        virial_[k] += virial_pool[rank][k];  // -= bc the atomindeces are switched relative to dlpoly
                    }

                    rep_energy_ += energy_pool[rank];
                }
            }  // if do_buck
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
}

void Buckingham::CalculateEnforcedRepulsion(bool use_ghost) {
    // Parallelization
    size_t nthreads = 1;
#ifdef _OPENMP
#pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
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
        size_t ns1 = num_atoms_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = nmon1 * 2;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;

        // For each monomer type mt1, loop over all the other monomer types
        // mt2 >= mt1 to avoid double counting
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
            size_t ns2 = num_atoms_[fi_mon2];
            size_t nmon2 = mon_type_count_[mt2].second;

            double dummy_a;
            double dummy_b;
            bool do_buck = use_buck_all_[mt1 * mon_type_count_.size() + mt2];
            if (!do_buck) {
                // Check if monomer types 1 and 2 are the same
                // If so, same monomer won't be done, since it has been done in
                // previous loop.
                bool same = (mt1 == mt2);

                std::vector<std::vector<double> > grad1_pool(nthreads, std::vector<double>(nmon1 * ns1 * 3, 0.0));
                std::vector<std::vector<double> > grad2_pool(nthreads, std::vector<double>(nmon2 * ns2 * 3, 0.0));
                std::vector<double> energy_pool(nthreads, 0.0);
                std::vector<std::vector<double> > virial_pool(nthreads, std::vector<double>(9, 0.0));
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
                for (size_t m1 = 0; m1 < nmon1; m1++) {
                    int rank = 0;
                    size_t mon1_idx = fi_mon1 + m1;
                    bool do_mon1 = (std::find(force_ttm_for_idx_.begin(), force_ttm_for_idx_.end(), mon1_idx) !=
                                    force_ttm_for_idx_.end());
#ifdef _OPENMP
                    rank = omp_get_thread_num();
#endif
                    size_t m2init = same ? m1 + 1 : 0;
                    for (size_t i = 0; i < ns1; i++) {
                        size_t inmon1 = i * nmon1;
                        size_t inmon13 = inmon1 * 3;
                        std::vector<double> xyz_sitei(3);
                        std::vector<double> g1(3, 0.0);
                        xyz_sitei[0] = xyz_[fi_crd1 + inmon13 + m1];
                        xyz_sitei[1] = xyz_[fi_crd1 + inmon13 + nmon1 + m1];
                        xyz_sitei[2] = xyz_[fi_crd1 + inmon13 + 2 * nmon1 + m1];

                        std::vector<std::pair<std::string, std::string> > buck_pairs = {
                            std::make_pair(mon_id_[fi_mon1], mon_id_[fi_mon2]),
                            std::make_pair(mon_id_[fi_mon2], mon_id_[fi_mon1])};
                        for (size_t j = 0; j < ns2; j++) {
                            double a, b;
                            a = a_all_[mt1 * mon_type_count_.size() + mt2][i * ns2 + j];
                            b = b_all_[mt1 * mon_type_count_.size() + mt2][i * ns2 + j];
                            for (size_t m2 = m2init; m2 < nmon2; m2++) {
                                size_t mon2_idx = fi_mon2 + m2;
                                bool do_mon2 = (std::find(force_ttm_for_idx_.begin(), force_ttm_for_idx_.end(),
                                                          mon2_idx) != force_ttm_for_idx_.end());
                                if (do_mon1 || do_mon2) {
                                    energy_pool[rank] +=
                                        Repulsion(a, b, xyz_sitei.data(), xyz_.data() + fi_crd2, g1.data(),
                                                  grad2_pool[rank].data(), nmon1, nmon2, m2init, m2init + 1, i, j,
                                                  do_grads_, cutoff_, box_, box_inverse_, use_ghost, islocal_,
                                                  fi_mon1 + m1, fi_mon2, &virial_pool[rank]);
                                }
                            }
                        }
                        grad1_pool[rank][inmon13 + m1] += g1[0];
                        grad1_pool[rank][inmon13 + nmon1 + m1] += g1[1];
                        grad1_pool[rank][inmon13 + nmon12 + m1] += g1[2];
                    }
                }

                // Compress data in Efq and phi
                for (size_t rank = 0; rank < nthreads; rank++) {
                    size_t kend1 = grad1_pool[rank].size();
                    size_t kend2 = grad2_pool[rank].size();
                    for (size_t k = 0; k < kend1; k++) {
                        grad_[fi_crd1 + k] += grad1_pool[rank][k];
                    }
                    for (size_t k = 0; k < kend2; k++) {
                        grad_[fi_crd2 + k] += grad2_pool[rank][k];
                    }
                    for (size_t k = 0; k < 9; k++) {
                        virial_[k] += virial_pool[rank][k];  // -= bc the atomindeces are switched relative to dlpoly
                    }

                    rep_energy_ += energy_pool[rank];
                }
            }  // if do_buck
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
}

}  // namespace buck

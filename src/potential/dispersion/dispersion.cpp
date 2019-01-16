#include "potential/dispersion/dispersion.h"

namespace disp {

void Dispersion::Initialize(const std::vector<double> sys_c6_long_range, const std::vector<double> &sys_xyz,
                            const std::vector<std::string> &mon_id, const std::vector<size_t> &num_atoms,
                            const std::vector<std::pair<std::string, size_t> > &mon_type_count,
                            const bool do_grads = true, const std::vector<double> &box = {}) {
    sys_c6_long_range_ = sys_c6_long_range;
    sys_xyz_ = sys_xyz;
    mon_id_ = mon_id;
    num_atoms_ = num_atoms;
    mon_type_count_ = mon_type_count;
    do_grads_ = do_grads;
    box_ = box;
    box_inverse_ = box.size() ? InvertUnitCell(box) : std::vector<double>{};

    natoms_ = sys_c6_long_range_.size();
    size_t natoms3 = 3 * natoms_;
    phi_ = std::vector<double>(natoms_, 0.0);
    xyz_ = std::vector<double>(natoms3, 0.0);
    grad_ = std::vector<double>(natoms3, 0.0);
    sys_grad_ = std::vector<double>(natoms3, 0.0);
    c6_long_range_ = std::vector<double>(natoms_, 0.0);
    sys_phi_ = std::vector<double>(natoms_, 0.0);

    ReorderData();
}

void Dispersion::SetNewParameters(const std::vector<double> &xyz, bool do_grads = true, const double cutoff = 100.0,
                                  const std::vector<double> &box = {}) {
    sys_xyz_ = xyz;
    box_ = box;
    box_inverse_ = box.size() ? InvertUnitCell(box) : std::vector<double>{};
    do_grads_ = do_grads;
    cutoff_ = cutoff;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(phi_.begin(), phi_.end(), 0.0);

    ReorderData();
}

void Dispersion::ReorderData() {
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
                c6_long_range_[fi_sites + m + inmon] = sys_c6_long_range_[fi_sites + mns + i];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }
}

double Dispersion::GetDispersion(std::vector<double> &grad) {
    CalculateDispersion();

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

                sys_phi_[fi_sites + mns + i] = phi_[fi_sites + m + inmon];

                grad[fi_crd + mns3 + 3 * i] += grad_[inmon3 + m + fi_crd];
                grad[fi_crd + mns3 + 3 * i + 1] += grad_[inmon3 + m + fi_crd + nmon];
                grad[fi_crd + mns3 + 3 * i + 2] += grad_[inmon3 + m + fi_crd + nmon2];
            }
        }
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
    }

    return disp_energy_;
}

void Dispersion::CalculateDispersion() {
    disp_energy_ = 0.0;
    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
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

    auto box_inverse = std::vector<double>();
    // auto box_inverse = box_.size() ? InvertUnitCell(box_) : std::vector<double>();

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = num_atoms_[fi_mon];
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
                double disp_scale_factor = (is12 || is13 || is14) ? 0 : 1;
                double c6, d6;
                GetC6(mon_id_[fi_mon], mon_id_[fi_mon], i, j, c6, d6);
                for (size_t m = 0; m < nmon; m++) {
                    double p1[3], g1[3];
                    p1[0] = xyz_[fi_crd + inmon3 + m];
                    p1[1] = xyz_[fi_crd + inmon3 + nmon + m];
                    p1[2] = xyz_[fi_crd + inmon3 + nmon2 + m];
                    std::fill(g1, g1 + 3, 0.0);
                    disp_energy_ += disp6(c6, d6, p1, xyz_.data() + fi_crd, g1, grad_.data() + fi_crd, nmon, nmon, m,
                                          m + 1, i, j, disp_scale_factor, do_grads_, cutoff_, box_, box_inverse_);

                    grad_[fi_crd + inmon3 + m] += g1[0];
                    grad_[fi_crd + inmon3 + nmon + m] += g1[1];
                    grad_[fi_crd + inmon3 + nmon2 + m] += g1[2];
                }
            }
        }
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

            // Check if monomer types 1 and 2 are the same
            // If so, same monomer won't be done, since it has been done in
            // previous loop.
            bool same = (mt1 == mt2);
            for (size_t m1 = 0; m1 < nmon1; m1++) {
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
                        size_t jnmon2 = j * nmon2;
                        size_t jnmon23 = jnmon2 * 3;
                        double c6, d6;
                        GetC6(mon_id_[fi_mon1], mon_id_[fi_mon2], i, j, c6, d6);
                        disp_energy_ +=
                            disp6(c6, d6, xyz_sitei.data(), xyz_.data() + fi_crd2, g1.data(), grad_.data() + fi_crd2,
                                  nmon1, nmon2, m2init, nmon2, i, j, 1.0, do_grads_, cutoff_, box_, box_inverse_);
                    }
                    grad_[fi_crd1 + inmon13 + m1] += g1[0];
                    grad_[fi_crd1 + inmon13 + nmon1 + m1] += g1[1];
                    grad_[fi_crd1 + inmon13 + nmon12 + m1] += g1[2];
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
}

}  // namespace disp

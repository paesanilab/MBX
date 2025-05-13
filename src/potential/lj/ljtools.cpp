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

#include "potential/lj/ljtools.h"

/**
 * @file ljtools.cpp
 * @brief Contains the helper functions implementation for the LennardJones class
 */

/**
 * @namespace lj
 * @brief Encloses the functions related to classical lennard-jones.
 */
namespace lj {

double lj(const double eps, const double sigma, double ljchgi, double ljchgj, const std::vector<double>& p1,
          const std::vector<double>& xyz2, std::vector<double>& grad1, std::vector<double>& grad2, double& phi1,
          std::vector<double>& phi2, const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2,
          const size_t atom_index1, const size_t atom_index2, const double lj_scale_factor, bool do_grads,
          const double cutoff, const double ewald_alpha, const std::vector<double>& box,
          const std::vector<double>& box_inverse, bool use_ghost, const std::vector<size_t>& islocal,
          const size_t isl1_offset, const size_t isl2_offset, std::vector<double>* virial) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "eps = " << eps << " , sigma = " << sigma << std::endl;

    std::cerr << "p1:\n ";
    for (size_t i = 0; i < p1.size(); i++) {
        std::cerr << p1[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "xyz2:\n ";
    for (size_t i = 0; i < xyz2.size(); i++) {
        std::cerr << xyz2[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "grad1:\n ";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "grad2:\n ";
    for (size_t i = 0; i < grad2.size(); i++) {
        std::cerr << grad2[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "phi1 = " << phi1 << std::endl;

    std::cerr << "phi2:\n ";
    for (size_t i = 0; i < phi2.size(); i++) {
        std::cerr << phi2[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "nmon1 = " << nmon1 << " , nmon2 = " << nmon2 << " , start2 = " << start2 << " , end2 = " << end2
              << std::endl;
    std::cerr << "atom_index1 = " << atom_index1 << " , atom_index2 = " << atom_index2
              << " , lj_scale_factor = " << lj_scale_factor << " , do_grads = " << do_grads << std::endl;
    std::cerr << "cutoff = " << cutoff << " , ewald_alpha = " << ewald_alpha << " , use_ghost = " << use_ghost
              << std::endl;

    std::cerr << "box:\n ";
    for (size_t i = 0; i < box.size(); i++) {
        std::cerr << box[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "box inverse:\n ";
    for (size_t i = 0; i < box_inverse.size(); i++) {
        std::cerr << box_inverse[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "islocal:\n ";
    for (size_t i = 0; i < islocal.size(); i++) {
        std::cerr << islocal[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "virial:\n ";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }

    std::cerr << "isl1_offset = " << isl1_offset << " , isl2_offset = " << isl2_offset << std::endl;

#endif

    size_t nmon22 = nmon2 * 2;

    size_t shift_phi = atom_index2 * nmon2;
    size_t shift2 = shift_phi * 3;

    bool use_pbc = box.size();

    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();

    const double sigma2 = sigma * sigma;
    const double sigma6 = sigma2 * sigma2 * sigma2;
    const double sigma12 = sigma6 * sigma6;

    double lj_energy = 0;

    double dx[end2];
    double dy[end2];
    double dz[end2];

    double rsq[end2];
    double r[end2];
    double inv_rsq[end2];
    double inv_r6[end2];
    double inv_r12[end2];
    double vscale[end2];

    #pragma omp simd simdlen(8)
    for (size_t nv = start2; nv < end2; nv++) {
        dx[nv] = p1[0] - xyz2[shift2 + nv];
        dy[nv] = p1[1] - xyz2[nmon2 + shift2 + nv];
        dz[nv] = p1[2] - xyz2[nmon22 + shift2 + nv];
    }

    // Apply minimum image convetion
    if (use_pbc) {
        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            double tmp1 = boxinv[0] * dx[nv] + boxinv[3] * dy[nv] + boxinv[6] * dz[nv];
            double tmp2 = boxinv[1] * dx[nv] + boxinv[4] * dy[nv] + boxinv[7] * dz[nv];
            double tmp3 = boxinv[2] * dx[nv] + boxinv[5] * dy[nv] + boxinv[8] * dz[nv];

            tmp1 -= std::floor(tmp1 + 0.5);
            tmp2 -= std::floor(tmp2 + 0.5);
            tmp3 -= std::floor(tmp3 + 0.5);

            dx[nv] = boxptr[0] * tmp1 + boxptr[3] * tmp2 + boxptr[6] * tmp3;
            dy[nv] = boxptr[1] * tmp1 + boxptr[4] * tmp2 + boxptr[7] * tmp3;
            dz[nv] = boxptr[2] * tmp1 + boxptr[5] * tmp2 + boxptr[8] * tmp3;
        }
    }

    #pragma omp simd simdlen(8)
    for (size_t nv = start2; nv < end2; nv++) {
        rsq[nv] = dx[nv] * dx[nv] + dy[nv] * dy[nv] + dz[nv] * dz[nv];
        r[nv] = std::sqrt(rsq[nv]);
        inv_rsq[nv] = 1.0 / rsq[nv];
        inv_r6[nv] = inv_rsq[nv] * inv_rsq[nv] * inv_rsq[nv];
        inv_r12[nv] = inv_r6[nv] * inv_r6[nv];

        size_t isls = islocal[isl1_offset] + islocal[isl2_offset + nv];
        vscale[nv] = (isls == 1) ? 0.5 : 1.0;
    }

    // Calculate contribution to lj field.
    #pragma omp simd simdlen(8)
    for (size_t nv = start2; nv < end2; nv++) {
        phi1 -= ljchgj * inv_r6[nv];
        phi2[shift_phi + nv] -= ljchgi * inv_r6[nv];
    }

    double ljsw_grad[end2];
    double ljsw[end2];

    // Loop not vectorized, since current switch_function is not vectorized
    for (size_t nv = start2; nv < end2; nv++) {
        ljsw[nv] = switch_function(r[nv], cutoff - 1.0, cutoff, ljsw_grad[nv]);
    }

    double chgsw[end2];
    double chgsw_grad[end2];
    double ljattr_e[end2];
    double chgterm[end2];

    #pragma omp simd simdlen(8) reduction(+ : lj_energy)
    for (size_t nv = start2; nv < end2; nv++) {
        const double ljrep_e = 4 * eps * sigma12 * inv_r12[nv];
        ljattr_e[nv] = 4 * eps * sigma6 * inv_r6[nv];

        chgsw[nv] = 1 - ljsw[nv];
        chgsw_grad[nv] = -ljsw_grad[nv];
        chgterm[nv] = ljchgi * ljchgj * inv_r6[nv];

        lj_energy += lj_scale_factor * ljrep_e * vscale[nv];
        lj_energy -= ljsw[nv] * (lj_scale_factor * ljattr_e[nv]) + chgsw[nv] * lj_scale_factor * chgterm[nv] * vscale[nv];
    }

    double ar2[end2];
    double ar4[end2];
    double ar6[end2];
    double expterm[end2];

    #pragma omp simd simdlen(8) reduction(+ : lj_energy)
    for (size_t nv = start2; nv < end2; nv++) {
        // Intermediates used in the dispersion PME terms
        ar2[nv] = ewald_alpha * ewald_alpha * rsq[nv];
        ar4[nv] = ar2[nv] * ar2[nv];
        expterm[nv] = ewald_alpha ? std::exp(-ar2[nv]) : 1;
        double pmeterm = ljchgi * ljchgj * (1 - (1 + ar2[nv] + ar4[nv] / 2) * expterm[nv]) * inv_r6[nv];
        lj_energy += pmeterm * vscale[nv];
    }

    
    if (do_grads) {
        double grad[nmon2];
        std::fill(grad, grad + nmon2, 0.0);

        for (size_t nv = start2; nv < end2; nv++) {

            double ar6 = ar4[nv] * ar2[nv];

            const double pmeterm_grad =
                6 * ljchgi * ljchgj * (1 - (1 + ar2[nv] + ar4[nv] / 2 + ar6 / 6) * expterm[nv]) * inv_r6[nv] * inv_rsq[nv];

            grad[nv] -= pmeterm_grad;
        }

        double gradx = 0.0;
        double grady = 0.0;
        double gradz = 0.0;

        #pragma omp simd simdlen(8) reduction(+ : gradx, grady, gradz)
        for (size_t nv = start2; nv < end2; nv++) {
            const double lj_attr_grad = 6 * sigma6 * inv_r6[nv] * 4 * eps * inv_rsq[nv];
            const double lj_rep_grad = -12 * sigma12 * inv_r12[nv] * 4 * eps * inv_rsq[nv];
            const double chg_attr_grad = 6 * ljchgi * ljchgj * inv_r6[nv] * inv_rsq[nv];

            const double lj_attr_sw_grad = ljsw[nv] * lj_attr_grad - ljsw_grad[nv] * ljattr_e[nv] / r[nv];
            const double chg_attr_sw_grad = chgsw[nv] * chg_attr_grad - chgsw_grad[nv] * chgterm[nv] / r[nv];

            grad[nv] += lj_scale_factor * (lj_attr_sw_grad + chg_attr_sw_grad + lj_rep_grad);

            gradx += dx[nv] * grad[nv];
            grad2[shift2 + nv] -= dx[nv] * grad[nv];

            grady += dy[nv] * grad[nv];
            grad2[shift2 + nv] -= dy[nv] * grad[nv];

            gradz += dz[nv] * grad[nv];
            grad2[shift2  + nv] -= dz[nv] * grad[nv];
        }


        if (virial != 0) {
            #pragma omp simd simdlen(8)
            for (size_t nv = start2; nv < end2; nv++) {

                    (*virial)[0] -= dx[nv] * dx[nv] * grad[nv] * vscale[nv];  //  update the virial for the atom pair
                    (*virial)[1] -= dx[nv] * dy[nv] * grad[nv] * vscale[nv];
                    (*virial)[2] -= dx[nv] * dz[nv] * grad[nv] * vscale[nv];

                    (*virial)[4] -= dy[nv] * dy[nv] * grad[nv] * vscale[nv];
                    (*virial)[5] -= dy[nv] * dz[nv] * grad[nv] * vscale[nv];

                    (*virial)[8] -= dz[nv] * dz[nv] * grad[nv] * vscale[nv];
            }

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }

        grad1[0] += gradx;
        grad1[1] += grady;
        grad1[2] += gradz;
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "grad1:\n ";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "grad2:\n ";
    for (size_t i = 0; i < grad2.size(); i++) {
        std::cerr << grad2[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "phi1 = " << phi1 << std::endl;

    std::cerr << "phi2:\n ";
    for (size_t i = 0; i < phi2.size(); i++) {
        std::cerr << phi2[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "virial:\n ";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }

    std::cerr << "lj_energy = " << lj_energy << std::endl;
#endif

    return lj_energy;
}

bool GetLjParams(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_epsilon,
                 double& out_sigma, std::vector<std::pair<std::string, std::string>>& use_lj, const nlohmann::json& lj_j) {
    // Order the two monomer names and corresponding xyz
    bool swaped = false;
    if (mon_id2 < mon_id1) {
        std::string tmp = mon_id1;
        mon_id1 = mon_id2;
        mon_id2 = tmp;
        size_t tmp1 = index1;
        index1 = index2;
        index2 = tmp1;
        swaped = true;
    }

    std::vector<double> eps, sigma;
    size_t nt2, i, j;

    out_epsilon = 0.0;
    out_sigma = 0.0;

    if (std::find(use_lj.begin(), use_lj.end(), std::make_pair(mon_id1, mon_id2)) == use_lj.end() &&
        std::find(use_lj.begin(), use_lj.end(), std::make_pair(mon_id2, mon_id1)) == use_lj.end()) {
        out_epsilon = 0.0;
        out_sigma = 0.0;
        return false;
    }

    bool done_with_it = false;

    // Check if pair is in json object
    try {
        std::vector<std::vector<std::string> > pairs = lj_j["pairs"];
        for (size_t k = 0; k < pairs.size(); k++) {
            if (mon_id1 == pairs[k][0] && mon_id2 == pairs[k][1]) {
                std::vector<std::vector<std::string> > types1 = lj_j["types1"];
                std::vector<std::vector<std::string> > types2 = lj_j["types2"];
                std::vector<std::vector<std::pair<std::vector<std::string>, double> > > eps_v = lj_j["epsilon"];
                std::vector<std::vector<std::pair<std::vector<std::string>, double> > > sigma_v = lj_j["sigma"];
                std::string si = types1[k][index1];
                std::string sj = types2[k][index2];

                for (size_t k2 = 0; k2 < eps_v[k].size(); k2++) {
                    if ((si == eps_v[k][k2].first[0] && sj == eps_v[k][k2].first[1]) ||
                        (si == eps_v[k][k2].first[1] && sj == eps_v[k][k2].first[0])) {
                        out_epsilon = eps_v[k][k2].second;
                        done_with_it = true;
                        break;
                    }
                }

                for (size_t k2 = 0; k2 < sigma_v[k].size(); k2++) {
                    if ((si == sigma_v[k][k2].first[0] && sj == sigma_v[k][k2].first[1]) ||
                        (si == sigma_v[k][k2].first[1] && sj == sigma_v[k][k2].first[0])) {
                        out_sigma = sigma_v[k][k2].second;
                        done_with_it = true;
                        break;
                    }
                }
            }
        }
    } catch (...) {
        out_sigma = 0.0;
        out_epsilon = 0.0;
    }

    if (done_with_it) return true;
    std::vector<size_t> types1, types2;

    // TODO Maybe add also hardcoded values, but don't see the need.

    return false;
}

}  // namespace lj

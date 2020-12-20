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

    double lj_rep = 0.0;
    double lj_attr = 0.0;
    double lj_lr_below_cutoff = 0.0;

    size_t nmon22 = nmon2 * 2;

    size_t shift_phi = atom_index2 * nmon2;
    size_t shift2 = shift_phi * 3;

    bool use_pbc = box.size();
    double g1[3], g2[3 * nmon2];
    std::fill(g1, g1 + 3, 0.0);
    std::fill(g2, g2 + 3 * nmon2, 0.0);
    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();
    double lj_energy = 0;
    for (size_t nv = start2; nv < end2; nv++) {
        double dx = p1[0] - xyz2[shift2 + nv];
        double dy = p1[1] - xyz2[nmon2 + shift2 + nv];
        double dz = p1[2] - xyz2[nmon22 + shift2 + nv];

        // Apply minimum image convetion
        if (use_pbc) {
            double tmp1 = boxinv[0] * dx + boxinv[3] * dy + boxinv[6] * dz;
            double tmp2 = boxinv[1] * dx + boxinv[4] * dy + boxinv[7] * dz;
            double tmp3 = boxinv[2] * dx + boxinv[5] * dy + boxinv[8] * dz;

            tmp1 -= std::floor(tmp1 + 0.5);
            tmp2 -= std::floor(tmp2 + 0.5);
            tmp3 -= std::floor(tmp3 + 0.5);

            dx = boxptr[0] * tmp1 + boxptr[3] * tmp2 + boxptr[6] * tmp3;
            dy = boxptr[1] * tmp1 + boxptr[4] * tmp2 + boxptr[7] * tmp3;
            dz = boxptr[2] * tmp1 + boxptr[5] * tmp2 + boxptr[8] * tmp3;
        }

        const double rsq = dx * dx + dy * dy + dz * dz;
        const double r = std::sqrt(rsq);
        const double inv_rsq = 1.0 / rsq;
        const double inv_r6 = inv_rsq * inv_rsq * inv_rsq;
        const double inv_r12 = inv_r6 * inv_r6;
        const double sigma2 = sigma * sigma;
        const double sigma6 = sigma2 * sigma2 * sigma2;
        const double sigma12 = sigma6 * sigma6;

        // Update phi for long range interactions
        // phi1 is a double value passed by reference
        // phi2 is a double array

        phi1 -= ljchgj * inv_r6;
        phi2[shift_phi + nv] -= ljchgi * inv_r6;

        bool include_pair = false;
        size_t isls = islocal[isl1_offset] + islocal[isl2_offset + nv];
        if (!use_ghost) include_pair = true;
        if (use_ghost && isls) include_pair = true;

        // If using cutoff, check for distances and get proper dispersion
        if (r <= cutoff && include_pair) {
            // Intermediates used in the dispersion PME terms
            double ar2 = ewald_alpha * ewald_alpha * rsq;
            double ar4 = ar2 * ar2;
            double ar6 = ar4 * ar2;
            double expterm = ewald_alpha ? std::exp(-ar2) : 1;

            const double ljrep_e = 4 * eps * sigma12 * inv_r12;
            const double ljattr_e = 4 * eps * sigma6 * inv_r6;

            // lj will refer to the real space, in which the full functional form of LJ is accounted for
            // chg will refer to the reciprocal space. We will transition from cutoff -1 to cutoff from full expression
            // to the "charge like" expression.

            double ljsw_grad = 0;
            const double ljsw = switch_function(r, cutoff - 1.0, cutoff, ljsw_grad);
            const double chgsw = 1 - ljsw;
            const double chgsw_grad = -ljsw_grad;

            // The idea here is quite simple.  At short range we want the full term (ljattr_e + ljrep_e) to model
            // lennard.  At long range this becomes chgi*chgj / Rij^6, which is handled by PME.  The reciprocal space
            // part of PME always includes extra terms that contribute below the cutoff, even if that pair shouldn't
            // contribute.  For intermonomer pairs, this means there is the LJ contribution that we want, but we have to
            // remove the part of the reciprocal space from chgi chgj / Rij^6 that was added in the reciprocal space
            // term.  Similarly for intramonomer terms, there should be no LJ contribution or chgi chgj / Rij^6 term, so
            // we use the scale factor to prevent LJ contributing, and then back out the reciprocal space chgi chgj /
            // Rij^6 contribution. See http://dx.doi.org/10.1021/acs.jctc.5b00726 for more details of this trick.
            double chgterm = ljchgi * ljchgj * inv_r6;
            double pmeterm = ljchgi * ljchgj * (1 - (1 + ar2 + ar4 / 2) * expterm) * inv_r6;
            double attr_pair_energy = ljsw * (lj_scale_factor * ljattr_e) + chgsw * lj_scale_factor * chgterm - pmeterm;
            // The repulsion part does not have a switch
            double rep_pair_energy = lj_scale_factor * ljrep_e;

            if (isls == 1) {
                attr_pair_energy *= 0.5;
                rep_pair_energy *= 0.5;
            }

            lj_energy += rep_pair_energy - attr_pair_energy;

            if (do_grads) {
                const double lj_attr_grad = 6 * sigma6 * inv_r6 * 4 * eps * inv_rsq;
                const double lj_rep_grad = -12 * sigma12 * inv_r12 * 4 * eps * inv_rsq;
                const double chg_attr_grad = 6 * ljchgi * ljchgj * inv_r6 * inv_rsq;

                // const double e6term_grad = 6 * e6 * inv_rsq - C6 * std::pow(d6, 7) * if6 * std::exp(-d6r) / r;
                // const double c6term_grad = 6 * c6term * inv_rsq;
                const double pmeterm_grad =
                    6 * ljchgi * ljchgj * (1 - (1 + ar2 + ar4 / 2 + ar6 / 6) * expterm) * inv_r6 * inv_rsq;

                const double lj_attr_sw_grad = ljsw * lj_attr_grad - ljsw_grad * ljattr_e / r;
                const double chg_attr_sw_grad = chgsw * chg_attr_grad - chgsw_grad * chgterm / r;

                // const double ttgrad = ttsw * e6term_grad - ttsw_grad * e6 / r;
                // const double c6grad = c6sw * c6term_grad - c6sw_grad * c6term / r;
                const double grad = lj_scale_factor * (lj_attr_sw_grad + chg_attr_sw_grad + lj_rep_grad) - pmeterm_grad;

                g1[0] += dx * grad;
                g2[nv] -= dx * grad;

                g1[1] += dy * grad;
                g2[nmon2 + nv] -= dy * grad;

                g1[2] += dz * grad;
                g2[nmon22 + nv] -= dz * grad;

                if (virial != 0) {
                    const double vscale = (isls == 1) ? 0.5 : 1.0;

                    (*virial)[0] -= dx * dx * grad * vscale;  //  update the virial for the atom pair
                    (*virial)[1] -= dx * dy * grad * vscale;
                    (*virial)[2] -= dx * dz * grad * vscale;

                    (*virial)[4] -= dy * dy * grad * vscale;
                    (*virial)[5] -= dy * dz * grad * vscale;

                    (*virial)[8] -= dz * dz * grad * vscale;

                    (*virial)[3] = (*virial)[1];
                    (*virial)[6] = (*virial)[2];
                    (*virial)[7] = (*virial)[5];
                }
            }
        }
    }

    if (do_grads) {
        grad1[0] += g1[0];
        grad1[1] += g1[1];
        grad1[2] += g1[2];
        for (size_t i = start2; i < end2; i++) {
            grad2[shift2 + i] += g2[i];
            grad2[shift2 + nmon2 + i] += g2[nmon2 + i];
            grad2[shift2 + nmon22 + i] += g2[nmon22 + i];
        }
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
                 double& out_sigma, std::vector<std::pair<std::string, std::string> > use_lj, nlohmann::json lj_j) {
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

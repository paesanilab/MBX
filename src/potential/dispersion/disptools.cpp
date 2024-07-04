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

#include "potential/dispersion/disptools.h"

namespace disp {

double tang_toennies(int n, const double& x) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "n = " << n << " , x = " << x << std::endl;
#endif

    assert(n >= 0);
    int nn = n;
    double sum = 1.0 + x / nn;

    while (--nn != 0) sum = 1.0 + sum * x / nn;

    double tt = 1.0 - sum * std::exp(-x);

    if (std::fabs(tt) < 1.0e-8) {
        double term(1);
        for (nn = n; nn != 0; --nn) term *= x / nn;

        sum = 0.0;
        for (nn = n + 1; nn < 1000; ++nn) {
            term *= x / nn;
            sum += term;

            if (std::fabs(term / sum) < 1.0e-8) break;
        }

        tt = sum * std::exp(-x);
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "tt = " << tt << std::endl;
#endif

    return tt;
}

double tang_toennies(const double& x) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "x = " << x << std::endl;
#endif

    int n = 6;
    double one_over_6 = 1.0 / 6.0;
    double one_over_5 = 0.2;
    double one_over_4 = 0.25;
    double one_over_3 = one_over_6 * 2.0;
    double one_over_2 = 0.5;

    double sum6 = 1.0 + x * one_over_6;
    double sum5 = 1.0 + sum6 * x * one_over_5;
    double sum4 = 1.0 + sum5 * x * one_over_4;
    double sum3 = 1.0 + sum4 * x * one_over_3;
    double sum2 = 1.0 + sum3 * x * one_over_2;
    double sum = 1.0 + sum2 * x;

    double tt = 1.0 - sum * std::exp(-x);

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "tt = " << tt << std::endl;
#endif

    return tt;
}

//----------------------------------------------------------------------------//

double disp6(const double C6, const double d6, const double c6i, const double c6j, const std::vector<double>& p1,
             const std::vector<double>& xyz2, std::vector<double>& grad1, std::vector<double>& grad2, double& phi1,
             std::vector<double>& phi2, const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2,
             const size_t atom_index1, const size_t atom_index2, const double disp_scale_factor, bool do_grads,
             const double cutoff, const double ewald_alpha, const std::vector<double>& box,
             const std::vector<double>& box_inverse, bool use_ghost, const std::vector<size_t>& islocal,
             const size_t isl1_offset, const size_t isl2_offset, std::vector<double>* virial, const size_t xyz2_offset) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "C6 = " << C6 << " , d6 = " << d6 << " , c6i = " << c6i << " , c6j = " << c6j << std::endl;

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
              << " , dip_scale_factor = " << disp_scale_factor << " , do_grads = " << do_grads << std::endl;
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

    double disp = 0.0;

    size_t nmon22 = nmon2 * 2;

    size_t shift_phi = atom_index2 * nmon2;
    size_t shift2 = shift_phi * 3;

    bool use_pbc = box.size();
    double g1[3];
    std::fill(g1, g1 + 3, 0.0);
    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();
    double dispersion_energy = 0;

    const std::vector<double>& x1 = p1;

    double x1_r, y1_r, z1_r;

    if (use_pbc) {
        x1_r = boxinv[0] * p1[0] + boxinv[3] * p1[1] + boxinv[6] * p1[2];
        y1_r = boxinv[1] * p1[0] + boxinv[4] * p1[1] + boxinv[7] * p1[2];
        z1_r = boxinv[2] * p1[0] + boxinv[5] * p1[1] + boxinv[8] * p1[2];
    }

    const size_t N = end2 - start2;
    double dx[N];
    double dy[N];
    double dz[N];

    #pragma omp simd
    for (size_t nv = start2; nv < end2; nv++) {
        double x2[3];
        x2[0] = xyz2[xyz2_offset + shift2 + nv];
        x2[1] = xyz2[xyz2_offset + nmon2 + shift2 + nv];
        x2[2] = xyz2[xyz2_offset + nmon22 + shift2 + nv];

        // Apply minimum image convetion
        if (use_pbc) {
            double tmp1 = boxinv[0] * x2[0] + boxinv[3] * x2[1] + boxinv[6] * x2[2];
            double tmp2 = boxinv[1] * x2[0] + boxinv[4] * x2[1] + boxinv[7] * x2[2];
            double tmp3 = boxinv[2] * x2[0] + boxinv[5] * x2[1] + boxinv[8] * x2[2];

            tmp1 -= std::round(tmp1 - x1_r);
            tmp2 -= std::round(tmp2 - y1_r);
            tmp3 -= std::round(tmp3 - z1_r);

            x2[0] = boxptr[0] * tmp1 + boxptr[3] * tmp2 + boxptr[6] * tmp3;
            x2[1] = boxptr[1] * tmp1 + boxptr[4] * tmp2 + boxptr[7] * tmp3;
            x2[2] = boxptr[2] * tmp1 + boxptr[5] * tmp2 + boxptr[8] * tmp3;
        }

        dx[nv - start2] = x1[0] - x2[0];
        dy[nv - start2] = x1[1] - x2[1];
        dz[nv - start2] = x1[2] - x2[2];
    }

    double rsq[N], r[N], inv_rsq[N], inv_r6[N];

    #pragma omp simd reduction(+: phi1)
    for (size_t i = 0; i < N; i++) {
        rsq[i] = dx[i] * dx[i] + dy[i] * dy[i] + dz[i] * dz[i];
        r[i] = std::sqrt(rsq[i]);

        inv_rsq[i] = 1.0 / rsq[i];
        inv_r6[i] = inv_rsq[i] * inv_rsq[i] * inv_rsq[i];

        // Update phi for long range interactions
        // phi2 is a double array
        // phi1 is a double value passed by reference
        phi2[shift_phi + start2 + i] -= c6i * inv_r6[i];
        phi1 -= c6j * inv_r6[i];
    }

    // Figure out which are the pairs to be included
    // Kinda buzzed right now, so hopefully nothing breaks...

    std::vector<size_t> indexes_to_include, iisls;
    for (size_t i = 0; i < N; i++) {
        bool include_pair = false;
        size_t isls = islocal[isl1_offset] + islocal[isl2_offset + i + start2];
        if (!use_ghost) include_pair = true;
        if (use_ghost && isls) include_pair = true;

        // If using cutoff, check for distances and get proper dispersion
        if (r[i] <= cutoff && include_pair) {
            indexes_to_include.push_back(i);
            iisls.push_back(isls);
        }
    }

    size_t n_idxs = indexes_to_include.size();

    double idx[n_idxs];
    double idy[n_idxs];
    double idz[n_idxs];

    double irsq[n_idxs], ir[n_idxs], iinv_rsq[n_idxs], iinv_r6[n_idxs];

    for (size_t i = 0; i < n_idxs; i++) {
        idx[i] = dx[indexes_to_include[i]];
        idy[i] = dy[indexes_to_include[i]];
        idz[i] = dz[indexes_to_include[i]];

        irsq[i] = rsq[indexes_to_include[i]];
        ir[i] = r[indexes_to_include[i]];
        iinv_rsq[i] = inv_rsq[indexes_to_include[i]];
        iinv_r6[i] = inv_r6[indexes_to_include[i]];
    }

    double tt6[n_idxs], ttsw_grad[n_idxs], ttsw[n_idxs];
    double ipair_energy[n_idxs];
    double d6r[n_idxs];
    for (size_t i = 0; i < n_idxs; i++) {
        d6r[i] = d6 * ir[i];
        tt6[i] = disp::tang_toennies(d6r[i]);
        ttsw[i] = switch_function(ir[i], cutoff - 1.0, cutoff, ttsw_grad[i]);
    }

    double pmeterm[n_idxs], pmeterm2[n_idxs], e6[n_idxs], expterm[n_idxs], c6term[n_idxs];
    for (size_t i = 0; i < n_idxs; i++) {
        // Intermediates used in the dispersion PME terms
        double ar2 = ewald_alpha * ewald_alpha * irsq[i];
        double ar4 = ar2 * ar2;
        double ar6 = ar4 * ar2;
        expterm[i] = ewald_alpha ? std::exp(-ar2) : 1;

        e6[i] = C6 * tt6[i] * iinv_r6[i];

        // The idea here is quite simple.  At short range we want the TT term (e6) to model dispersion.  At long
        // range this becomes C6i C6j / Rij^6, which is handled by PME.  The reciprocal space part of PME always
        // includes extra terms that contribute below the cutoff, even if that pair shouldn't contribute.  For
        // intermonomer pairs, this means there is the TT contribution that we want, but we have to remove the
        // part of the reciprocal space from C6i C6j / Rij^6 that was added in the reciprocal space term.  Similarly
        // for intramonomer terms, there should be no TT contribution or C6i C6j / Rij^6 term, so we use the scale
        // factor to prevent TT contributing, and then back out the reciprocal space C6i C6j / Rij^6 contribution.
        // See http://dx.doi.org/10.1021/acs.jctc.5b00726 for more details of this trick.
        c6term[i] = c6i * c6j * iinv_r6[i];
        pmeterm[i] = (1 - (1 + ar2 + ar4 / 2) * expterm[i]) * c6term[i];
        pmeterm2[i] = (1 - (1 + ar2 + ar4 / 2 + ar6 / 6) * expterm[i]) * c6term[i];
        ipair_energy[i] =
            ttsw[i] * (disp_scale_factor * e6[i]) + (1 - ttsw[i]) * disp_scale_factor * c6term[i] - pmeterm[i];
    }

    for (size_t i = 0; i < n_idxs; i++) {
        if (iisls[i] == 1) ipair_energy[i] *= 0.5;
        dispersion_energy -= ipair_energy[i];
    }

    if (do_grads) {
        double grad[n_idxs];
        for (size_t i = 0; i < n_idxs; i++) {
            const double c6sw = 1 - ttsw[i];
            const double c6sw_grad = -ttsw_grad[i];
            const double e6term_grad = 6 * e6[i] * iinv_rsq[i] - C6 * std::pow(d6, 7) * if6 * std::exp(-d6r[i]) / ir[i];
            const double c6term_grad = 6 * c6term[i] * iinv_rsq[i];
            const double pmeterm_grad = 6 * pmeterm2[i] * iinv_rsq[i];
            const double ttgrad = ttsw[i] * e6term_grad - ttsw_grad[i] * e6[i] / ir[i];
            const double c6grad = c6sw * c6term_grad - c6sw_grad * c6term[i] / ir[i];
            grad[i] = disp_scale_factor * (ttgrad + c6grad) - pmeterm_grad;
        }

        // #pragma omp simd
        for (size_t i = 0; i < n_idxs; i++) {
            const double vscale = (iisls[i] == 1) ? 0.5 : 1.0;

            size_t index = indexes_to_include[i] + start2;
            g1[0] += idx[i] * grad[i] * vscale;

            g1[1] += idy[i] * grad[i] * vscale;

            g1[2] += idz[i] * grad[i] * vscale;

            grad2[shift2 + index] -= idx[i] * grad[i] * vscale;
            grad2[shift2 + nmon2 + index] -= idy[i] * grad[i] * vscale;
            grad2[shift2 + nmon22 + index] -= idz[i] * grad[i] * vscale;

            if (virial != 0) {

                (*virial)[0] -= idx[i] * idx[i] * grad[i] * vscale;  //  update the virial for the atom pair
                (*virial)[1] -= idx[i] * idy[i] * grad[i] * vscale;
                (*virial)[2] -= idx[i] * idz[i] * grad[i] * vscale;

                (*virial)[4] -= idy[i] * idy[i] * grad[i] * vscale;
                (*virial)[5] -= idy[i] * idz[i] * grad[i] * vscale;

                (*virial)[8] -= idz[i] * idz[i] * grad[i] * vscale;

                (*virial)[3] = (*virial)[1];
                (*virial)[6] = (*virial)[2];
                (*virial)[7] = (*virial)[5];
            }
        }

        grad1[0] += g1[0];
        grad1[1] += g1[1];
        grad1[2] += g1[2];
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

    std::cerr << "dispersion_energy = " << dispersion_energy << std::endl;
#endif

    return dispersion_energy;
}

bool GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_C6, double& out_d6,
           std::vector<std::pair<std::string, std::string> > ignore_disp, const nlohmann::json& repdisp_j) {
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

    std::vector<double> C6, d6;
    size_t nt2, i, j;

    out_C6 = 0.0;
    out_d6 = 0.0;

    if (std::find(ignore_disp.begin(), ignore_disp.end(), std::make_pair(mon_id1, mon_id2)) != ignore_disp.end() ||
        std::find(ignore_disp.begin(), ignore_disp.end(), std::make_pair(mon_id2, mon_id1)) != ignore_disp.end()) {
        out_C6 = 0.0;
        out_d6 = 0.0;
        return false;
    }

    bool done_with_it = false;

    // Check if pair is in json object
    if (repdisp_j.size() > 0) {
        try {
            std::vector<std::vector<std::string> > pairs = repdisp_j["pairs"];
            for (size_t k = 0; k < pairs.size(); k++) {
                if (mon_id1 == pairs[k][0] && mon_id2 == pairs[k][1]) {
                    std::vector<std::vector<std::string> > types1 = repdisp_j["types1"];
                    std::vector<std::vector<std::string> > types2 = repdisp_j["types2"];
                    std::vector<std::vector<std::pair<std::vector<std::string>, double> > > c6_v = repdisp_j["c6"];
                    std::vector<std::vector<std::pair<std::vector<std::string>, double> > > d6_v = repdisp_j["d6"];
                    std::string si = types1[k][index1];
                    std::string sj = types2[k][index2];

                    for (size_t k2 = 0; k2 < c6_v[k].size(); k2++) {
                        if ((si == c6_v[k][k2].first[0] && sj == c6_v[k][k2].first[1]) ||
                            (si == c6_v[k][k2].first[1] && sj == c6_v[k][k2].first[0])) {
                            out_C6 = c6_v[k][k2].second;
                            done_with_it = true;
                            break;
                        }
                    }

                    for (size_t k2 = 0; k2 < d6_v[k].size(); k2++) {
                        if ((si == d6_v[k][k2].first[0] && sj == d6_v[k][k2].first[1]) ||
                            (si == d6_v[k][k2].first[1] && sj == d6_v[k][k2].first[0])) {
                            out_d6 = d6_v[k][k2].second;
                            done_with_it = true;
                            break;
                        }
                    }
                }
            }
        } catch (...) {
            out_C6 = 0.0;
            out_d6 = 0.0;
        }
    } else {
        out_C6 = 0.0;
        out_d6 = 0.0;
    }

    if (done_with_it) return true;

    if (mon_id1 == "co2cm5100" || mon_id1 == "co2cm595" || mon_id1 == "co2cm590" || mon_id1 == "co2cm585" ||
        mon_id1 == "co2cm580" || mon_id1 == "co2cm5875")
        mon_id1 = "co2cm5";
    if (mon_id2 == "co2cm5100" || mon_id2 == "co2cm595" || mon_id2 == "co2cm590" || mon_id2 == "co2cm585" ||
        mon_id2 == "co2cm580" || mon_id2 == "co2cm5875")
        mon_id2 = "co2cm5";

    std::vector<size_t> types1, types2;

    // Monomers here have to be in alphabetical order: mon1 < mon2 ALWAYS
    if (mon_id1 == "h2o" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(2.373212214147944e+02);  // kcal/mol * A^(-6) O -- O
        C6.push_back(8.349556669872743e+01);  // kcal/mol * A^(-6) O -- H
        C6.push_back(8.349556669872743e+01);  // kcal/mol * A^(-6) H -- O
        C6.push_back(2.009358600184719e+01);  // kcal/mol * A^(-6) H -- H

        d6.push_back(9.295485815062264e+00);  // A^(-1)
        d6.push_back(9.775202425217957e+00);  // A^(-1)
        d6.push_back(9.775202425217957e+00);  // A^(-1)
        d6.push_back(9.406475169954112e+00);  // A^(-1)

    } else if (mon_id1 == "f-" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(3.488640000000000e+02);  // kcal/mol * A^(-6) F -- O
        C6.push_back(1.286780000000000e+02);  // kcal/mol * A^(-6) F -- H

        d6.push_back(3.586190000000000e+00);  // A^(-1)
        d6.push_back(2.697680000000000e+00);  // A^(-1)
    } else if (mon_id1 == "cl-" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(7.461990000000000e+02);  // kcal/mol * A^(-6) Cl -- O
        C6.push_back(3.068900000000000e+02);  // kcal/mol * A^(-6) Cl -- H

        d6.push_back(3.275420000000000e+00);  // A^(-1)
        d6.push_back(2.782260000000000e+00);  // A^(-1)
    } else if (mon_id1 == "br-" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(9.426500000000000e+02);  // kcal/mol * A^(-6) Br -- O
        C6.push_back(3.941680000000000e+02);  // kcal/mol * A^(-6) Br -- H

        d6.push_back(3.058250000000000e+00);  // A^(-1)
        d6.push_back(2.798040000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "i-") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(1.294680000000000e+03);  // kcal/mol * A^(-6) I -- O
        C6.push_back(5.681559999999999e+02);  // kcal/mol * A^(-6) I -- H

        d6.push_back(2.723140000000000e+00);  // A^(-1)
        d6.push_back(2.799110000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "li+") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(4.314700000000000e+01);  // kcal/mol * A^(-6) Li -- O
        C6.push_back(1.871220000000000e+01);  // kcal/mol * A^(-6) Li -- H

        d6.push_back(4.023330000000000e+00);  // A^(-1)
        d6.push_back(4.006630000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "na+") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(1.762550000000000e+02);  // kcal/mol * A^(-6) Na -- O
        C6.push_back(8.578690000000000e+01);  // kcal/mol * A^(-6) Na -- H

        d6.push_back(3.769530000000000e+00);  // A^(-1)
        d6.push_back(3.822550000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "k+") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(4.095830000000000e+02);  // kcal/mol * A^(-6) K -- O
        C6.push_back(2.181920000000000e+02);  // kcal/mol * A^(-6) K -- H

        d6.push_back(3.401250000000000e+00);  // A^(-1)
        d6.push_back(3.321390000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "rb+") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(5.318040000000000e+02);  // kcal/mol * A^(-6) Rb -- O
        C6.push_back(2.861310000000000e+02);  // kcal/mol * A^(-6) Rb -- H

        d6.push_back(3.236530000000000e+00);  // A^(-1)
        d6.push_back(3.313640000000000e+00);  // A^(-1)
    } else if (mon_id1 == "cs+" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(7.045400000000000e+02);  // kcal/mol * A^(-6) Cs -- O
        C6.push_back(3.845670000000000e+02);  // kcal/mol * A^(-6) Cs -- H

        d6.push_back(3.028640000000000e+00);  // A^(-1)
        d6.push_back(3.271530000000000e+00);  // A^(-1)
    } else if (mon_id1 == "he" and mon_id2 == "he") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(24.348011);  // kcal/mol * A^(-6) He -- He

        d6.push_back(4.02693);  // A^(-1)
        d6.push_back(4.02693);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "he") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(63.246969);  // kcal/mol * A^(-6) He -- O
        C6.push_back(26.208221);  // kcal/mol * A^(-6) He -- H

        d6.push_back(3.71873);  // A^(-1)
        d6.push_back(3.9707);   // A^(-1)
    } else if (mon_id1 == "ar" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(382.1275);  // kcal/mol * A^(-6) Cs -- O
        C6.push_back(170.8771);  // kcal/mol * A^(-6) Cs -- H

        d6.push_back(3.43864);  // A^(-1)
        d6.push_back(3.45707);  // A^(-1)
    } else if (mon_id1 == "co2_archive" && mon_id2 == "co2_archive") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(321.00932864);  // kcal/mol * A^(-6) C -- C
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) C -- O
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) O -- C
        C6.push_back(170.09525896);  // kcal/mol * A^(-6) O -- O

        d6.push_back(3.12663);  // A^(-1)
        d6.push_back(3.64236);  // A^(-1)
        d6.push_back(3.64236);  // A^(-1)
        d6.push_back(3.52744);  // A^(-1)
    } else if (mon_id1 == "co2cm5" && mon_id2 == "co2cm5") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(321.00932864);  // kcal/mol * A^(-6) C -- C
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) C -- O
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) O -- C
        C6.push_back(170.09525896);  // kcal/mol * A^(-6) O -- O

        d6.push_back(3.43054);  // A^(-1)
        d6.push_back(3.82894);  // A^(-1)
        d6.push_back(3.82894);  // A^(-1)
        d6.push_back(3.92759);  // A^(-1)
    } else if (mon_id1 == "co2" && mon_id2 == "co2") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(321.00932864);  // kcal/mol * A^(-6) C -- C
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) C -- O
        C6.push_back(219.55020747);  // kcal/mol * A^(-6) O -- C
        C6.push_back(170.09525896);  // kcal/mol * A^(-6) O -- O

        d6.push_back(3.43054);  // A^(-1)
        d6.push_back(3.82894);  // A^(-1)
        d6.push_back(3.82894);  // A^(-1)
        d6.push_back(3.92759);  // A^(-1)

    } else if (mon_id1 == "nh3pbe0d3bj" and mon_id2 == "nh3pbe0d3bj") {
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(243.7007);  // kcal/mol * A^(-6)  A--A
        C6.push_back(98.6092);   // kcal/mol * A^(-6)  A--B
        C6.push_back(98.6092);   // kcal/mol * A^(-6)  A--B
        C6.push_back(39.9906);   // kcal/mol * A^(-6)  B--B
        d6.push_back(3.09382);   // A^(-1) A--A
        d6.push_back(3.44698);   // A^(-1) A--B
        d6.push_back(3.44698);   // A^(-1) A--B
        d6.push_back(3.83901);   // A^(-1) B--B

    } else if (mon_id1 == "nh3" and mon_id2 == "nh3") {
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(243.7007);  // kcal/mol * A^(-6)  A--A
        C6.push_back(98.6092);   // kcal/mol * A^(-6)  A--B
        C6.push_back(98.6092);   // kcal/mol * A^(-6)  A--B
        C6.push_back(39.9906);   // kcal/mol * A^(-6)  B--B
        d6.push_back(3.11493);   // A^(-1) A--A
        d6.push_back(3.4174);    // A^(-1) A--B
        d6.push_back(3.4174);    // A^(-1) A--B
        d6.push_back(3.78007);   // A^(-1) B--B

    } else if (mon_id1 == "ch4" && mon_id2 == "ch4") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(303.247);      // kcal/mol * A^(-6) C -- C
        C6.push_back(104.10825);    // kcal/mol * A^(-6) C -- O
        C6.push_back(104.10825);    // kcal/mol * A^(-6) O -- C
        C6.push_back(36.78116875);  // kcal/mol * A^(-6) O -- O

        d6.push_back(3.37925);  // A^(-1)
        d6.push_back(3.25885);  // A^(-1)
        d6.push_back(3.25885);  // A^(-1)
        d6.push_back(4.05972);  // A^(-1)
    } else if (mon_id1 == "ch4" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(222.003);  // kcal/mol * A^(-6) C -- O
        C6.push_back(105.979);  // kcal/mol * A^(-6) C -- H
        C6.push_back(80.628);   // kcal/mol * A^(-6) O -- O
        C6.push_back(37.176);   // kcal/mol * A^(-6) O -- H

        d6.push_back(2.87176);  // A^(-1)
        d6.push_back(3.68542);  // A^(-1)
        d6.push_back(3.79757);  // A^(-1)
        d6.push_back(4.01558);  // A^(-1)
    } else if (mon_id1 == "co2_archive" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(225.5857);  // kcal/mol * A^(-6) C -- O
        C6.push_back(130.8452);  // kcal/mol * A^(-6) C -- H
        C6.push_back(208.0754);  // kcal/mol * A^(-6) O -- O
        C6.push_back(94.19873);  // kcal/mol * A^(-6) O -- H

        d6.push_back(2.93819);  // A^(-1)
        d6.push_back(3.7359);   // A^(-1)
        d6.push_back(3.53045);  // A^(-1)
        d6.push_back(3.89503);  // A^(-1)
    } else if (mon_id1 == "co2" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(223.8723);  // kcal/mol * A^(-6) C -- O
        C6.push_back(106.2522);  // kcal/mol * A^(-6) C -- H
        C6.push_back(176.5128);  // kcal/mol * A^(-6) O -- O
        C6.push_back(77.9633);   // kcal/mol * A^(-6) O -- H

        d6.push_back(3.54868);  // A^(-1)
        d6.push_back(3.1436);   // A^(-1)
        d6.push_back(3.87591);  // A^(-1)
        d6.push_back(3.71826);  // A^(-1)
    } else if (mon_id1 == "co2cm5" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(223.8723);  // kcal/mol * A^(-6) C -- O
        C6.push_back(106.2522);  // kcal/mol * A^(-6) C -- H
        C6.push_back(176.5128);  // kcal/mol * A^(-6) O -- O
        C6.push_back(77.9633);   // kcal/mol * A^(-6) O -- H

        d6.push_back(3.54868);  // A^(-1)
        d6.push_back(3.1436);   // A^(-1)
        d6.push_back(3.87591);  // A^(-1)
        d6.push_back(3.71826);  // A^(-1)

    } else if (mon_id1 == "ar" and mon_id2 == "cs+") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(1500.0914);  // kcal/mol * A^(-6)  A--B
        d6.push_back(3.28039);    // A^(-1) A--B
    } else if (mon_id1 == "ar" and mon_id2 == "ar") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(774.5257);  // kcal/mol * A^(-6)  A--B
        d6.push_back(3.41808);   // A^(-1) A--B
    } else if (mon_id1 == "h2" and mon_id2 == "h2") {
        types1.push_back(0);
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(45.4166);  // kcal/mol * A^(-6)  A--A
        d6.push_back(3.11276);  // A^(-1) A--A
    } else if (mon_id1 == "h2" and mon_id2 == "h2o") {
        types1.push_back(0);
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(91.2878);  // kcal/mol * A^(-6)  A--C
        C6.push_back(40.6489);  // kcal/mol * A^(-6)  B--C
        d6.push_back(3.05339);  // A^(-1) A--C
        d6.push_back(3.62823);  // A^(-1) B--C
    } else if (mon_id1 == "ar" and mon_id2 == "h2o") {
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(382.031);   // kcal/mol * A^(-6)  A--B
        C6.push_back(170.8082);  // kcal/mol * A^(-6)  A--C
        d6.push_back(3.48054);   // A^(-1) A--B
        d6.push_back(3.46238);   // A^(-1) A--C
    } else if (mon_id1 == "cs+" and mon_id2 == "h2") {
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(372.8086);  // kcal/mol * A^(-6)  A--B
        d6.push_back(3.24781);   // A^(-1) A--B
    } else if (mon_id1 == "na+" and mon_id2 == "na+") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(251.48);   // kcal/mol * A^(-6)  A--A
        d6.push_back(4.42822);  // A^(-1) A--A
    } else if (mon_id1 == "ch4" and mon_id2 == "co2") {
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 1;

        C6.push_back(306.4929);  // kcal/mol * A^(-6)  A--C
        C6.push_back(215.0259);  // kcal/mol * A^(-6)  A--D
        C6.push_back(104.5401);  // kcal/mol * A^(-6)  B--C
        C6.push_back(77.8082);   // kcal/mol * A^(-6)  B--D
        d6.push_back(3.2644);    // A^(-1) A--C
        d6.push_back(3.55762);   // A^(-1) A--D
        d6.push_back(3.37636);   // A^(-1) B--C
        d6.push_back(3.5529);    // A^(-1) B--D

    } else if (mon_id1 == "cl-" and mon_id2 == "cl-") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(3066.2919);  // kcal/mol * A^(-6)  A--A
        d6.push_back(1.82786);    // A^(-1) A--A
    } else if (mon_id1 == "cl-" and mon_id2 == "na+") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(784.5853);  // kcal/mol * A^(-6)  A--B
        d6.push_back(2.85113);   // A^(-1) A--B

    } else if (mon_id1 == "n2o5" and mon_id2 == "n2o5") {
        // Define the type of atom in each mon
        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(2);
        types2.push_back(2);
        types2.push_back(2);
        types2.push_back(2);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);
        types1.push_back(2);
        types1.push_back(2);
        types1.push_back(2);
        types1.push_back(2);

        // Set the number of different types
        nt2 = 3;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(169.526699905518);    // kcal/mol * A^(-6)  A--A
        C6.push_back(165.351572694970);    // kcal/mol * A^(-6)  A--B
        C6.push_back(174.39423682672617);  // kcal/mol * A^(-6)  A--C
        C6.push_back(165.351572694970);    // kcal/mol * A^(-6)  B--A
        C6.push_back(171.35934650288723);  // kcal/mol * A^(-6)  B--B
        C6.push_back(165.351572694970);    // kcal/mol * A^(-6)  B--C
        C6.push_back(174.39423682672617);  // kcal/mol * A^(-6)  C--A
        C6.push_back(165.351572694970);    // kcal/mol * A^(-6)  C--B
        C6.push_back(179.62003548877342);  // kcal/mol * A^(-6)  C--C

        d6.push_back(3.86891);  // A^(-1) A--A
        d6.push_back(3.19945);  // A^(-1) A--B
        d6.push_back(3.60907);  // A^(-1) A--C
        d6.push_back(3.19945);  // A^(-1) B--A
        d6.push_back(2.33813);  // A^(-1) B--B
        d6.push_back(4.19428);  // A^(-1) B--C
        d6.push_back(3.60907);  // A^(-1) C--A
        d6.push_back(4.19428);  // A^(-1) C--B
        d6.push_back(3.56601);  // A^(-1) C--C
    } else if (mon_id1 == "h2o" and mon_id2 == "n2o5") {
        // Define the type of atom in each mon
        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);
        types2.push_back(2);
        types2.push_back(2);
        types2.push_back(2);
        types2.push_back(2);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 3;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(176.36123157691006);  // kcal/mol * A^(-6)  D--A
        C6.push_back(170.03653392132748);  // kcal/mol * A^(-6)  D--B
        C6.push_back(181.7351576894966);   // kcal/mol * A^(-6)  D--C
        C6.push_back(78.55577632783532);   // kcal/mol * A^(-6)  E--A
        C6.push_back(79.65812014580179);   // kcal/mol * A^(-6)  E--B
        C6.push_back(80.37292121526443);   // kcal/mol * A^(-6)  E--C

        d6.push_back(4.1869);   // A^(-1) D--A
        d6.push_back(3.22662);  // A^(-1) D--B
        d6.push_back(4.455);    // A^(-1) D--C
        d6.push_back(3.24055);  // A^(-1) E--A
        d6.push_back(6.4749);   // A^(-1) E--B
        d6.push_back(3.03227);  // A^(-1) E--C
    } else if (mon_id1 == "mbpbe" and mon_id2 == "mbpbe") {
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(185.9792);  // kcal/mol * A^(-6)  A--A
        C6.push_back(84.0262);   // kcal/mol * A^(-6)  A--B
        C6.push_back(84.0262);   // kcal/mol * A^(-6)  A--B
        C6.push_back(39.5741);   // kcal/mol * A^(-6)  B--B
        d6.push_back(4.21118);   // A^(-1) A--A
        d6.push_back(3.44966);   // A^(-1) A--B
        d6.push_back(3.44966);   // A^(-1) A--B
        d6.push_back(3.56401);   // A^(-1) B--B

        // =====>> BEGIN SECTION DISPERSION <<=====
        // ======>> PASTE CODE BELOW <<======

        // =====>> END SECTION DISPERSION <<=====
    } else {
        out_C6 = 0.0;
        out_d6 = 0.0;
        return false;
    }

    i = types1[index1];
    j = types2[index2];

    out_C6 = C6[i * nt2 + j];
    out_d6 = d6[i * nt2 + j];

    return true;
}

}  // namespace disp

//----------------------------------------------------------------------------//

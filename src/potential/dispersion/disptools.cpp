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
#include <string>
#include <vector>

namespace disp {

double tang_toennies(int n, const double& x) {
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

    return tt;
}

//----------------------------------------------------------------------------//

/* Block commented since C8 is not used for now.

double disp68(const double& C6, const double& d6,
               const double& C8, const double& d8,
               const double* p1, const double* p2,
                     double* g1,       double* g2, bool do_grads) {

  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = disp::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = disp::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  const double grd = (6*e6 + 8*e8)*inv_rsq
      - (C6*std::pow(d6, 7)*if6*std::exp(-d6r)
      +  C8*std::pow(d8, 9)*if8*std::exp(-d8r))/r;

  g1[0] += dx*grd;
  g2[0] -= dx*grd;

  g1[1] += dy*grd;
  g2[1] -= dy*grd;

  g1[2] += dz*grd;
  g2[2] -= dz*grd;

  return - (e6 + e8);
}

//----------------------------------------------------------------------------//

double disp68(const double& C6, const double& d6,
              const double& C8, const double& d8,
              const double* p1, const double* p2) {

  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = disp::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = disp::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  return - (e6 + e8);
}

******************************************************************************/

//----------------------------------------------------------------------------//

double disp6(const double C6, const double d6, const double c6i, const double c6j, const double* p1, const double* xyz2,
             double* grad1, double* grad2, double& phi1, double* phi2, const size_t nmon1, const size_t nmon2,
             const size_t start2, const size_t end2, const size_t atom_index1, const size_t atom_index2,
             const double disp_scale_factor, bool do_grads, const double cutoff, const double ewald_alpha,
             const std::vector<double>& box, const std::vector<double>& box_inverse,
	     bool use_ghost, const std::vector<size_t>& islocal, const size_t isl1_offset, const size_t isl2_offset,
	     std::vector<double> *virial) {
    double disp = 0.0;
    double disp_lr_below_cutoff = 0.0;

    size_t nmon22 = nmon2 * 2;

    size_t shift_phi = atom_index2 * nmon2;
    size_t shift2 = shift_phi * 3;

    bool use_pbc = box.size();
    double g1[3], g2[3 * nmon2];
    std::fill(g1, g1 + 3, 0.0);
    std::fill(g2, g2 + 3 * nmon2, 0.0);
    //    #pragma simd
    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();
    double dispersion_energy = 0;
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

        // Update phi for long range interactions
        // phi1 is a double value passed by reference
        // phi2 is a double array

        phi1 -= c6j * inv_r6;
        phi2[shift_phi + nv] -= c6i * inv_r6;

	bool include_pair = false;
	size_t isls = islocal[isl1_offset] + islocal[isl2_offset+nv];
	if(!use_ghost) include_pair = true;
	if(use_ghost && isls) include_pair = true;

        // If using cutoff, check for distances and get proper dispersion
        if (r <= cutoff && include_pair) {
            const double d6r = d6 * r;
            const double tt6 = disp::tang_toennies(6, d6r);

            const double inv_rsq = 1.0 / rsq;
            const double inv_r6 = inv_rsq * inv_rsq * inv_rsq;

            // Intermediates used in the dispersion PME terms
            double ar2 = ewald_alpha * ewald_alpha * rsq;
            double ar4 = ar2 * ar2;
            double ar6 = ar4 * ar2;
            double expterm = ewald_alpha ? std::exp(-ar2) : 1;

            const double e6 = C6 * tt6 * inv_r6;

            double ttsw_grad = 0;
            const double ttsw = switch_function(r, cutoff - 1.0, cutoff, ttsw_grad);
            const double c6sw = 1 - ttsw;
            const double c6sw_grad = -ttsw_grad;

            // The idea here is quite simple.  At short range we want the TT term (e6) to model dispersion.  At long
            // range this becomes C6i C6j / Rij^6, which is handled by PME.  The reciprocal space part of PME always
            // includes extra terms that contribute below the cutoff, even if that pair shouldn't contribute.  For
            // intermonomer pairs, this means there is the TT contribution that we want, but we have to remove the
            // part of the reciprocal space from C6i C6j / Rij^6 that was added in the reciprocal space term.  Similarly
            // for intramonomer terms, there should be no TT contribution or C6i C6j / Rij^6 term, so we use the scale
            // factor to prevent TT contributing, and then back out the reciprocal space C6i C6j / Rij^6 contribution.
            // See http://dx.doi.org/10.1021/acs.jctc.5b00726 for more details of this trick.
            double c6term = c6i * c6j * inv_r6;
            double pmeterm = c6i * c6j * (1 - (1 + ar2 + ar4 / 2) * expterm) * inv_r6;
            double pair_energy = ttsw * (disp_scale_factor * e6) + c6sw * disp_scale_factor * c6term - pmeterm;

	    if(isls == 1) pair_energy *= 0.5;
            dispersion_energy -= pair_energy;

            if (do_grads) {
                const double e6term_grad = 6 * e6 * inv_rsq - C6 * std::pow(d6, 7) * if6 * std::exp(-d6r) / r;
                const double c6term_grad = 6 * c6term * inv_rsq;
                const double pmeterm_grad =
                    6 * c6i * c6j * (1 - (1 + ar2 + ar4 / 2 + ar6 / 6) * expterm) * inv_r6 * inv_rsq;
                const double ttgrad = ttsw * e6term_grad - ttsw_grad * e6 / r;
                const double c6grad = c6sw * c6term_grad - c6sw_grad * c6term / r;
                const double grad = disp_scale_factor * (ttgrad + c6grad) - pmeterm_grad;

                g1[0] += dx * grad;
                g2[nv] -= dx * grad;

                g1[1] += dy * grad;
                g2[nmon2 + nv] -= dy * grad;

                g1[2] += dz * grad;
                g2[nmon22 + nv] -= dz * grad;


                if (virial != 0 ) {

                    (*virial)[0] -= dx* dx * grad; //  update the virial for the atom pair
                    (*virial)[1] -= dx* dy * grad; 
                    (*virial)[2] -= dx* dz * grad; 

                    (*virial)[4] -= dy* dy * grad; 
                    (*virial)[5] -= dy* dz * grad; 

                    (*virial)[8] -= dz* dz * grad; 

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

    return dispersion_energy;
}

void GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_C6, double& out_d6) {
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
    std::vector<size_t> types1, types2;
    size_t nt2, i, j;

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

    } else if (mon_id1 == "f" and mon_id2 == "h2o") {
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
    } else if (mon_id1 == "cl" and mon_id2 == "h2o") {
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
    } else if (mon_id1 == "br" and mon_id2 == "h2o") {
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
    } else if (mon_id1 == "h2o" and mon_id2 == "i") {
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
    } else if (mon_id1 == "h2o" and mon_id2 == "li") {
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
    } else if (mon_id1 == "h2o" and mon_id2 == "na") {
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
    } else if (mon_id1 == "h2o" and mon_id2 == "k") {
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
    } else if (mon_id1 == "h2o" and mon_id2 == "rb") {
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
    } else if (mon_id1 == "cs" and mon_id2 == "h2o") {
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
        d6.push_back(3.9707);  // A^(-1)
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
        // =====>> BEGIN SECTION DISPERSION <<=====
        // ======>> PASTE CODE BELOW <<======
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

        d6.push_back(3.12663);  // A^(-1)
        d6.push_back(3.64236);  // A^(-1)
        d6.push_back(3.64236);  // A^(-1)
        d6.push_back(3.52744);  // A^(-1)

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
        C6.push_back(303.247);  // kcal/mol * A^(-6) C -- C
        C6.push_back(104.10825);  // kcal/mol * A^(-6) C -- O
        C6.push_back(104.10825);  // kcal/mol * A^(-6) O -- C
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
        C6.push_back(80.628);  // kcal/mol * A^(-6) O -- O
        C6.push_back(37.176);  // kcal/mol * A^(-6) O -- H

        d6.push_back(2.87176);  // A^(-1)
        d6.push_back(3.68542);  // A^(-1)
        d6.push_back(3.79757);  // A^(-1)
        d6.push_back(4.01558);  // A^(-1)
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
        C6.push_back(225.5857);  // kcal/mol * A^(-6) C -- O
        C6.push_back(130.8452);  // kcal/mol * A^(-6) C -- H
        C6.push_back(208.0754);  // kcal/mol * A^(-6) O -- O
        C6.push_back(94.19873);  // kcal/mol * A^(-6) O -- H

        d6.push_back(2.93819);  // A^(-1)
        d6.push_back(3.7359);  // A^(-1)
        d6.push_back(3.53045);  // A^(-1)
        d6.push_back(3.89503);  // A^(-1)

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
        C6.push_back(243.38373570927124);  // kcal/mol * A^(-6)  A--A
        C6.push_back(98.66666135686104);  // kcal/mol * A^(-6)  A--B
        C6.push_back(98.66666135686104);  // kcal/mol * A^(-6)  B--A
        C6.push_back(40.09775637853022);  // kcal/mol * A^(-6)  B--B
        d6.push_back(3.10660);  // A^(-1) A--A
        d6.push_back(3.90008);  // A^(-1) A--B
        d6.push_back(3.90008);  // A^(-1) B--A
        d6.push_back(3.33535);  // A^(-1) B--B
    } else if (mon_id1 == "ar" and mon_id2 == "cs") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(1857.467);  // kcal/mol * A^(-6)  A--B
        d6.push_back(3.19908);  // A^(-1) A--B
        // =====>> END SECTION DISPERSION <<=====
    } else {
        out_C6 = 0.0;
        out_d6 = 0.0;
        return;
    }

    i = types1[index1];
    j = types2[index2];

    out_C6 = C6[i * nt2 + j];
    out_d6 = d6[i * nt2 + j];
}

}  // namespace disp

//----------------------------------------------------------------------------//

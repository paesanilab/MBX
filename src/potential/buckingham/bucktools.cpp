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

#include "potential/buckingham/bucktools.h"
#include <string>
#include <vector>

namespace buck {

//----------------------------------------------------------------------------//

double Repulsion(const double a, const double b, const double* p1, const double* xyz2,
		 double* grad1, double* grad2, const size_t nmon1, const size_t nmon2,
		 const size_t start2, const size_t end2, const size_t atom_index1, const size_t atom_index2,
		 bool do_grads, const double cutoff,                                 
		 const std::vector<double>& box, const std::vector<double>& box_inverse,
		 bool use_ghost, const std::vector<size_t>& islocal, const size_t isl1_offset, const size_t isl2_offset,
		 std::vector<double> *virial) {

    size_t nmon22 = nmon2 * 2;

    size_t shift2 = atom_index2 * nmon2 * 3;

    bool use_pbc = box.size();
    double g1[3], g2[3 * nmon2];
    std::fill(g1, g1 + 3, 0.0);
    std::fill(g2, g2 + 3 * nmon2, 0.0);
    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();
    double repulsion_energy = 0;
    for (size_t nv = start2; nv < end2; nv++) {
        double dx = p1[0] - xyz2[shift2 + nv];
        double dy = p1[1] - xyz2[nmon2 + shift2 + nv];
        double dz = p1[2] - xyz2[nmon22 + shift2 + nv];

        // Apply minimum image convetion
        if (use_pbc) {
            double tmp1 = boxinv[0] * dx + boxinv[1] * dy + boxinv[2] * dz;
            double tmp2 = boxinv[3] * dx + boxinv[4] * dy + boxinv[5] * dz;
            double tmp3 = boxinv[6] * dx + boxinv[7] * dy + boxinv[8] * dz;

            tmp1 -= std::floor(tmp1 + 0.5);
            tmp2 -= std::floor(tmp2 + 0.5);
            tmp3 -= std::floor(tmp3 + 0.5);

            dx = boxptr[0] * tmp1 + boxptr[1] * tmp2 + boxptr[2] * tmp3;
            dy = boxptr[3] * tmp1 + boxptr[4] * tmp2 + boxptr[5] * tmp3;
            dz = boxptr[6] * tmp1 + boxptr[7] * tmp2 + boxptr[8] * tmp3;
        }

        const double rsq = dx * dx + dy * dy + dz * dz;
        const double r = std::sqrt(rsq);

        const double inv_r = 1.0 / r;

	bool include_pair = false;
	size_t isls = islocal[isl1_offset] + islocal[isl2_offset+nv];
	if(!use_ghost) include_pair = true;
	if(use_ghost && isls) include_pair = true;
	
        // If using cutoff, check for distances and get proper dispersion
        if (r <= cutoff && include_pair) {
            const double br = -b * r;
            const double fac = a * exp(br);

            double ttsw_grad = 0;
            const double ttsw = switch_function(r, cutoff - 1.0, cutoff, ttsw_grad);

	    double pair_energy = ttsw * fac;

	    if(isls == 1) pair_energy *= 0.5;
            repulsion_energy += pair_energy;

            if (do_grads) {
                // TODO check that this is correct and gradients are properly calculated
                // in switch area
                // Complciated due to the small energy/grad in this area.
                const double grad = ttsw * (b * inv_r * fac) - ttsw_grad * fac / r ;

                g1[0] -= dx * grad;
                g2[nv] += dx * grad;

                g1[1] -= dy * grad;
                g2[nmon2 + nv] += dy * grad;

                g1[2] -= dz * grad;
                g2[nmon22 + nv] += dz * grad;

                if (virial != 0) {

		    (*virial)[0] -= dx * dx * grad;
                    (*virial)[1] -= dx * dy * grad;
                    (*virial)[2] -= dx * dz * grad;
                    (*virial)[4] -= dy * dy * grad;
                    (*virial)[5] -= dy * dz * grad;
                    (*virial)[8] -= dz * dz * grad;
                    
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

    return repulsion_energy;
}

bool GetBuckParams(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, std::vector<std::pair<std::string,std::string> > buck_pairs, double& out_a, double& out_b) {
    // Order the two monomer names and corresponding xyz
    if (mon_id2 < mon_id1) {
        std::string tmp = mon_id1;
        mon_id1 = mon_id2;
        mon_id2 = tmp;
        size_t tmp1 = index1;
        index1 = index2;
        index2 = tmp1;
    }

    if (std::find(buck_pairs.begin(), buck_pairs.end(), std::make_pair(mon_id1,mon_id2)) == buck_pairs.end()) {
        out_a = 0.0;
        out_b = 0.0;
        return false;
    }

    std::vector<double> a, b;
    std::vector<size_t> types1, types2;
    size_t nt2, i, j;

    // Monomers here have to be in alphabetical order: mon1 < mon2 ALWAYS
    if (mon_id1 == "f" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(35920.3);  // kcal/mol F -- O
        a.push_back(800.553);  // kcal/mol F -- H

        b.push_back(3.586190000000000e+00);  // A^(-1)
        b.push_back(2.697680000000000e+00);  // A^(-1)
    } else if (mon_id1 == "cl" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(50180.4);  // kcal/mol * A^(-6) Cl -- O
        a.push_back(2594.28);  // kcal/mol * A^(-6) Cl -- H

        b.push_back(3.275420000000000e+00);  // A^(-1)
        b.push_back(2.782260000000000e+00);  // A^(-1)
    } else if (mon_id1 == "br" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(37682.2);  // kcal/mol * A^(-6) Br -- O
        a.push_back(3804.53);  // kcal/mol * A^(-6) Br -- H

        b.push_back(3.058250000000000e+00);  // A^(-1)
        b.push_back(2.798040000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "i") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(22210.0);  // kcal/mol * A^(-6) I -- O
        a.push_back(6215.10);  // kcal/mol * A^(-6) I -- H

        b.push_back(2.723140000000000e+00);  // A^(-1)
        b.push_back(2.799110000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "li") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(32318.0);  // kcal/mol * A^(-6) Li -- O
        a.push_back(3245.78);  // kcal/mol * A^(-6) Li -- H

        b.push_back(4.023330000000000e+00);  // A^(-1)
        b.push_back(4.006630000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "na") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(47827.7);  // kcal/mol * A^(-6) Na -- O
        a.push_back(4992.61);  // kcal/mol * A^(-6) Na -- H

        b.push_back(3.769530000000000e+00);  // A^(-1)
        b.push_back(3.822550000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "k") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(49986.5);  // kcal/mol * A^(-6) K -- O
        a.push_back(4951.5);  // kcal/mol * A^(-6) K -- H

        b.push_back(3.401250000000000e+00);  // A^(-1)
        b.push_back(3.321390000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "rb") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(48456.3);  // kcal/mol * A^(-6) Rb -- O
        a.push_back(6794.51);  // kcal/mol * A^(-6) Rb -- H

        b.push_back(3.236530000000000e+00);  // A^(-1)
        b.push_back(3.313640000000000e+00);  // A^(-1)
    } else if (mon_id1 == "cs" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(42431.0);  // kcal/mol * A^(-6) Cs -- O
        a.push_back(9403.73);  // kcal/mol * A^(-6) Cs -- H

        b.push_back(3.028640000000000e+00);  // A^(-1)
        b.push_back(3.271530000000000e+00);  // A^(-1)
    } else if (mon_id1 == "he" and mon_id2 == "he") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(6440.78);  // kcal/mol * A^(-6) He -- O

        b.push_back(4.26243);  // A^(-1)
        b.push_back(2.697680000000000e+00);  // A^(-1)
    } else if (mon_id1 == "h2o" and mon_id2 == "he") {
        // Define the type of atom in each mon
        types2.push_back(0);

        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        // Set the number of different types
        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(12373.7);  // kcal/mol * A^(-6) He -- O
        a.push_back(1629.41);  // kcal/mol * A^(-6) He -- H

        b.push_back(3.71873);  // A^(-1)
        b.push_back(3.9707 );  // A^(-1)
    } else if (mon_id1 == "ar" and mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(52941.3);  // kcal/mol * A^(-6) Cs -- O
        a.push_back(5351.94);  // kcal/mol * A^(-6) Cs -- H

        b.push_back(3.43864);  // A^(-1)
        b.push_back(3.45707);  // A^(-1)
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
        a.push_back(9038.48);  // kcal/mol * A^(-6) C -- C
        a.push_back(12608.9);  // kcal/mol * A^(-6) C -- O
        a.push_back(12608.9);  // kcal/mol * A^(-6) O -- C
        a.push_back(24274.7);  // kcal/mol * A^(-6) O -- O

        b.push_back(3.12663);  // A^(-1)
        b.push_back(3.64236);  // A^(-1)
        b.push_back(3.64236);  // A^(-1)
        b.push_back(3.52744);  // A^(-1)
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
        a.push_back(42713.9);  // kcal/mol * A^(-6) C -- C
        a.push_back(3258.86);  // kcal/mol * A^(-6) C -- O
        a.push_back(3258.86);  // kcal/mol * A^(-6) O -- C
        a.push_back(2594.4);  // kcal/mol * A^(-6) O -- O

        b.push_back(3.37925);  // A^(-1)
        b.push_back(3.25885);  // A^(-1)
        b.push_back(3.25885);  // A^(-1)
        b.push_back(4.05972);  // A^(-1)
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
        a.push_back(11447.5);  // kcal/mol * A^(-6) C -- O
        a.push_back(4887.62);  // kcal/mol * A^(-6) C -- H
        a.push_back(6182.32);  // kcal/mol * A^(-6) O -- O
        a.push_back(1480.08);  // kcal/mol * A^(-6) O -- H

        b.push_back(2.87176);  // A^(-1)
        b.push_back(3.68542);  // A^(-1)
        b.push_back(3.79757);  // A^(-1)
        b.push_back(4.01558);  // A^(-1)
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
        a.push_back(4735.44);  // kcal/mol * A^(-6) C -- O
        a.push_back(4956.27);  // kcal/mol * A^(-6) C -- H
        a.push_back(30678.4);  // kcal/mol * A^(-6) O -- O
        a.push_back(4559.97);  // kcal/mol * A^(-6) O -- H

        b.push_back(2.93819);  // A^(-1)
        b.push_back(3.7359);  // A^(-1)
        b.push_back(3.53045);  // A^(-1)
        b.push_back(3.89503);  // A^(-1)
        // =====>> BEGIN SECTION BUCKINGHAM <<=====
        // ======>> PASTE CODE BELOW <<======
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
        a.push_back(21244.3);  // kcal/mol A--A
        a.push_back(7082.49);  // kcal/mol A--B
        a.push_back(7082.49);  // kcal/mol B--A
        a.push_back(554.617);  // kcal/mol B--B
        b.push_back(3.1066);  // A^(-1) A--A
        b.push_back(3.90008);  // A^(-1) A--B
        b.push_back(3.90008);  // A^(-1) B--A
        b.push_back(3.33535);  // A^(-1) B--B
    } else if (mon_id1 == "ar" and mon_id2 == "cs") {
        types1.push_back(0);

        types2.push_back(0);

        nt2 = 1;

        // Fill in (in order) the C6 and d6 coefficients
        a.push_back(127779.0);  // kcal/mol A--B
        b.push_back(3.19908);  // A^(-1) A--B
        // =====>> END SECTION BUCKINGHAM <<=====
    } else {
        out_a = 0.0;
        out_b = 0.0;
        return false;
    }

    i = types1[index1];
    j = types2[index2];

    out_a = a[i * nt2 + j];
    out_b = b[i * nt2 + j];
    
    return true;
}

}  // namespace buck

//----------------------------------------------------------------------------//

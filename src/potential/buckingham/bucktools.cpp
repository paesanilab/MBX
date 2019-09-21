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
             const std::vector<double>& box, const std::vector<double>& box_inverse) {

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

        // If using cutoff, check for distances and get proper dispersion
        if (r <= cutoff) {
            const double br = -b * r;
            const double fac = a * exp(br);

            double ttsw_grad = 0;
            const double ttsw = switch_function(r, cutoff - 1.0, cutoff, ttsw_grad);

            repulsion_energy += ttsw * fac;

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
        a.push_back(35920.3);  // kcal/mol * A^(-6) F -- O
        a.push_back(800.553);  // kcal/mol * A^(-6) F -- H

        b.push_back(3.586190000000000e+00);  // A^(-1)
        b.push_back(2.697680000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "cl" and mon_id2 == "h2o") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(7.461990000000000e+02);  // kcal/mol * A^(-6) Cl -- O
//        C6.push_back(3.068900000000000e+02);  // kcal/mol * A^(-6) Cl -- H
//
//        d6.push_back(3.275420000000000e+00);  // A^(-1)
//        d6.push_back(2.782260000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "br" and mon_id2 == "h2o") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(9.426500000000000e+02);  // kcal/mol * A^(-6) Br -- O
//        C6.push_back(3.941680000000000e+02);  // kcal/mol * A^(-6) Br -- H
//
//        d6.push_back(3.058250000000000e+00);  // A^(-1)
//        d6.push_back(2.798040000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "i") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        // Set the number of different types
//        nt2 = 1;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(1.294680000000000e+03);  // kcal/mol * A^(-6) I -- O
//        C6.push_back(5.681559999999999e+02);  // kcal/mol * A^(-6) I -- H
//
//        d6.push_back(2.723140000000000e+00);  // A^(-1)
//        d6.push_back(2.799110000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "li") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        // Set the number of different types
//        nt2 = 1;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(4.314700000000000e+01);  // kcal/mol * A^(-6) Li -- O
//        C6.push_back(1.871220000000000e+01);  // kcal/mol * A^(-6) Li -- H
//
//        d6.push_back(4.023330000000000e+00);  // A^(-1)
//        d6.push_back(4.006630000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "na") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        // Set the number of different types
//        nt2 = 1;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(1.762550000000000e+02);  // kcal/mol * A^(-6) Na -- O
//        C6.push_back(8.578690000000000e+01);  // kcal/mol * A^(-6) Na -- H
//
//        d6.push_back(3.769530000000000e+00);  // A^(-1)
//        d6.push_back(3.822550000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "k") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        // Set the number of different types
//        nt2 = 1;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(4.095830000000000e+02);  // kcal/mol * A^(-6) K -- O
//        C6.push_back(2.181920000000000e+02);  // kcal/mol * A^(-6) K -- H
//
//        d6.push_back(3.401250000000000e+00);  // A^(-1)
//        d6.push_back(3.321390000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "rb") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        // Set the number of different types
//        nt2 = 1;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(5.318040000000000e+02);  // kcal/mol * A^(-6) Rb -- O
//        C6.push_back(2.861310000000000e+02);  // kcal/mol * A^(-6) Rb -- H
//
//        d6.push_back(3.236530000000000e+00);  // A^(-1)
//        d6.push_back(3.313640000000000e+00);  // A^(-1)
//    } else if (mon_id1 == "cs" and mon_id2 == "h2o") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(7.045400000000000e+02);  // kcal/mol * A^(-6) Cs -- O
//        C6.push_back(3.845670000000000e+02);  // kcal/mol * A^(-6) Cs -- H
//
//        d6.push_back(3.028640000000000e+00);  // A^(-1)
//        d6.push_back(3.271530000000000e+00);  // A^(-1)
//
//        // =====>> SECTION BUCKINGHAM <<=====
//        // ======>> PASTE CODE BELOW <<======
//    } else if (mon_id1 == "co2" && mon_id2 == "co2") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(321.00932864);  // kcal/mol * A^(-6) C -- C
//        C6.push_back(219.55020747);  // kcal/mol * A^(-6) C -- O
//        C6.push_back(219.55020747);  // kcal/mol * A^(-6) O -- C
//        C6.push_back(170.09525896);  // kcal/mol * A^(-6) O -- O
//
//        d6.push_back(3.12663);  // A^(-1)
//        d6.push_back(3.64236);  // A^(-1)
//        d6.push_back(3.64236);  // A^(-1)
//        d6.push_back(3.52744);  // A^(-1)
//
//    } else if (mon_id1 == "ch4" && mon_id2 == "ch4") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(303.247);  // kcal/mol * A^(-6) C -- C
//        C6.push_back(104.10825);  // kcal/mol * A^(-6) C -- O
//        C6.push_back(104.10825);  // kcal/mol * A^(-6) O -- C
//        C6.push_back(36.78116875);  // kcal/mol * A^(-6) O -- O
//
//        d6.push_back(3.37925);  // A^(-1)
//        d6.push_back(3.25885);  // A^(-1)
//        d6.push_back(3.25885);  // A^(-1)
//        d6.push_back(4.05972);  // A^(-1)
//    } else if (mon_id1 == "nh4+" and mon_id2 == "nh4+") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back( 121.02357191499344);  // kcal/mol * A^(-6)  A--A
//        C6.push_back( 42.87084004560211);  // kcal/mol * A^(-6)  A--B
//        C6.push_back( 42.87084004560211);  // kcal/mol * A^(-6)  B--A
//        C6.push_back( 15.18478609248808);  // kcal/mol * A^(-6)  B--B
//
//        d6.push_back( 3.62294);  // A^(-1) A--A
//        d6.push_back( 4.17516);  // A^(-1) A--B
//        d6.push_back( 4.17516);  // A^(-1) B--A
//        d6.push_back( 6.65926);  // A^(-1) B--B
//    } else if (mon_id1 == "ch4" && mon_id2 == "h2o") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(222.003);  // kcal/mol * A^(-6) C -- O
//        C6.push_back(105.979);  // kcal/mol * A^(-6) C -- H
//        C6.push_back(80.628);  // kcal/mol * A^(-6) O -- O
//        C6.push_back(37.176);  // kcal/mol * A^(-6) O -- H
//
//        d6.push_back(2.87176);  // A^(-1)
//        d6.push_back(3.68542);  // A^(-1)
//        d6.push_back(3.79757);  // A^(-1)
//        d6.push_back(4.01558);  // A^(-1)
//    } else if (mon_id1 == "pf6-" and mon_id2 == "pf6-") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back( 1343.1094871080675);  // kcal/mol * A^(-6)  A--A
//        C6.push_back( 316.586254871107);  // kcal/mol * A^(-6)  A--B
//        C6.push_back( 316.586254871107);  // kcal/mol * A^(-6)  B--A
//        C6.push_back( 128.05063099625954);  // kcal/mol * A^(-6)  B--B
//
//        d6.push_back( 2.97498);  // A^(-1) A--A
//        d6.push_back( 2.97061);  // A^(-1) A--B
//        d6.push_back( 2.97061);  // A^(-1) B--A
//        d6.push_back( 4.29032);  // A^(-1) B--B
//    } else if (mon_id1 == "co2" && mon_id2 == "h2o") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back(225.5857);  // kcal/mol * A^(-6) C -- O
//        C6.push_back(130.8452);  // kcal/mol * A^(-6) C -- H
//        C6.push_back(208.0754);  // kcal/mol * A^(-6) O -- O
//        C6.push_back(94.19873);  // kcal/mol * A^(-6) O -- H
//
//        d6.push_back(2.93819);  // A^(-1)
//        d6.push_back(3.7359);  // A^(-1)
//        d6.push_back(3.53045);  // A^(-1)
//        d6.push_back(3.89503);  // A^(-1)
//    } else if (mon_id1 == "h2o" and mon_id2 == "nh4+") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back( 146.8735344463071);  // kcal/mol * A^(-6)  C--A
//        C6.push_back( 52.113303994364706);  // kcal/mol * A^(-6)  C--B
//        C6.push_back( 67.0087248346366);  // kcal/mol * A^(-6)  D--A
//        C6.push_back( 23.700392086279034);  // kcal/mol * A^(-6)  D--B
//
//        d6.push_back( 3.03002);  // A^(-1) C--A
//        d6.push_back( 4.00557);  // A^(-1) C--B
//        d6.push_back( 3.50204);  // A^(-1) D--A
//        d6.push_back( 3.82525);  // A^(-1) D--B
//
//    } else if (mon_id1 == "h2o" and mon_id2 == "pf6-") {
//        // Define the type of atom in each mon
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back( 407.63296458627474);  // kcal/mol * A^(-6)  C--A
//        C6.push_back( 152.50926721566066);  // kcal/mol * A^(-6)  C--B
//        C6.push_back( 203.69935826247843);  // kcal/mol * A^(-6)  D--A
//        C6.push_back( 65.76858803942432);  // kcal/mol * A^(-6)  D--B
//
//        d6.push_back( 2.70377);  // A^(-1) C--A
//        d6.push_back( 3.66078);  // A^(-1) C--B
//        d6.push_back( 3.70149);  // A^(-1) D--A
//        d6.push_back( 4.39919);  // A^(-1) D--B
//    } else if (mon_id1 == "nh4+" and mon_id2 == "pf6-") {
//        // Define the type of atom in each mon
//        types1.push_back(0);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//        types1.push_back(1);
//
//        types2.push_back(0);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//        types2.push_back(1);
//
//
//        // Set the number of different types
//        nt2 = 2;
//
//        // Fill in (in order) the C6 and d6 coefficients
//        C6.push_back( 359.40542255024184);  // kcal/mol * A^(-6)  A--C
//        C6.push_back( 119.75587652433201);  // kcal/mol * A^(-6)  A--D
//        C6.push_back( 126.31137741680135);  // kcal/mol * A^(-6)  B--C
//        C6.push_back( 42.61190407586103);  // kcal/mol * A^(-6)  B--D
//
//        d6.push_back( 2.85598);  // A^(-1) A--C
//        d6.push_back( 3.38009);  // A^(-1) A--D
//        d6.push_back( 3.39886);  // A^(-1) B--C
//        d6.push_back( 5.20723);  // A^(-1) B--D

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

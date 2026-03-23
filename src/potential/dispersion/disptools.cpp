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

double koideC6(const double& x) {
    // x is in Bohr

    std::array<double, 10> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----
    const double polyA =
        (89.0/1728.0) * xp[3] +
        (89.0/864.0) * xp[4] +
        (119.0/1296.0) * xp[5] +
        (5.0/108.0) * xp[6] +
        (11.0/810.0) * xp[7] +
        (1.0/486.0) * xp[8] +
        (1.0/8505.0) * xp[9];

    // ---- B(x) ----
    const double polyB =
        1.0 +
        2.0 * x +
        2.0 * xp[2] +
        (4.0/3.0) * xp[3] +
        (2.0/3.0) * xp[4] +
        (1699.0/6480.0) * xp[5] +
        (259.0/3240.0) * xp[6] +
        (197.0/11340.0) * xp[7] +
        (19.0/8505.0) * xp[8] +
        (1.0/8505.0) * xp[9];

    return (e4 / 2.0) * (polyA * polyA) + (1.0 - e2 * polyB) * (1.0 - e2 * polyB);
}

double koideC8(const double& x) {
    // x is in Bohr

    std::array<double, 12> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----
    const double polyA =
        (47.0/8640.0) * xp[5] +
        (47.0/4320.0) * xp[6] +
        (7.0/720.0) * xp[7] +
        (2.0/405.0) * xp[8] +
        (209.0/141750.0) * xp[9] +
        (11.0/47250.0) * xp[10] +
        (1.0/70875.0) * xp[11];

    // ---- B(x) ----
    const double polyB =
        1.0 +
        2.0 * x +
        2.0 * xp[2] +
        (4.0/3.0) * xp[3] +
        (2.0/3.0) * xp[4] +
        (4.0/15.0) * xp[5] +
        (4.0/45.0) * xp[6] +
        (109.0/4320.0) * xp[7] +
        (13.0/2160.0) * xp[8] +
        (211.0/189000.0) * xp[9] +
        (19.0/141750.0) * xp[10] +
        (1.0/141750.0) * xp[11];

    return (e4 / 6.0) * (polyA * polyA) + (1.0 - e2 * polyB) * (1.0 - e2 * polyB);
}

// default for C10 is chi13
double koideC10(const double& x) {
    // x is in Bohr

    std::array<double, 14> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----
    const double polyA =
        (11.0/80640.0) * xp[7] +
        (11.0/40320.0) * xp[8] +
        (241.0/987840.0) * xp[9] +
        (23.0/185220.0) * xp[10] +
        (23.0/617400.0) * xp[11] +
        (11.0/1852200.0) * xp[12] +
        (1.0/2778300.0) * xp[13];

    // ---- B(x) ----
    const double polyB =
        1.0 +
        2.0 * x +
        2.0 * xp[2] +
        (4.0/3.0) * xp[3] +
        (2.0/3.0) * xp[4] +
        (4.0/15.0) * xp[5] +
        (4.0/45.0) * xp[6] +
        (8.0/315.0) * xp[7] +
        (2.0/315.0) * xp[8] +
        (6253.0/4445280.0) * xp[9] +
        (3041.0/11113200.0) * xp[10] +
        (3.0/68600.0) * xp[11] +
        (2.0/416745.0) * xp[12] +
        (1.0/4167450.0) * xp[13];

    return (e4 / 3.0) * (polyA * polyA) + (1.0 - e2 * polyB) * (1.0 - e2 * polyB);
}

// Also implement chi22 just in case
double koideC10chi22(const double& x) {
    // x is in Bohr

    std::array<double, 14> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A1(x) ----
    const double polyA1 =
        (-13.0/7200.0) * xp[5] +
        (-13.0/3600.0) * xp[6] +
        (-67.0/21600.0) * xp[7] +
        (-1.0/720.0) * xp[8] +
        (-7.0/27000.0) * xp[9] +
        (1.0/20250.0) * xp[10] +
        (2.0/50625.0) * xp[11] +
        (1.0/118125.0) * xp[12] +
        (2.0/3189375.0) * xp[13];

    // ---- A2(x) ----
    const double polyA2 =
        (11.0/75600.0) * xp[7] +
        (11.0/37800.0) * xp[8] +
        (23.0/88200.0) * xp[9] +
        (53.0/396900.0) * xp[10] +
        (121.0/2976750.0) * xp[11] +
        (2.0/297675.0) * xp[12] +
        (2.0/4465125.0) * xp[13];

    // ---- B(x) ----
    const double polyB =
        1.0 +
        2.0 * x +
        2.0 * xp[2] +
        (4.0/3.0) * xp[3] +
        (2.0/3.0) * xp[4] +
        (4.0/15.0) * xp[5] +
        (4.0/45.0) * xp[6] +
        (8.0/315.0) * xp[7] +
        (2.0/315.0) * xp[8] +
        (698.0/496125.0) * xp[9] +
        (136.0/496125.0) * xp[10] +
        (73.0/1653750.0) * xp[11] +
        (37.0/7441875.0) * xp[12] +
        (2.0/7441875.0) * xp[13];

    return (e4 / 14.0) * (polyA1 * polyA1) + (e4 / 5.0) * (polyA2 * polyA2) + (1.0 - e2 * polyB) * (1.0 - e2 * polyB);
}

double koideC6grad(const double& x) {
    // x is in Bohr

    std::array<double, 19> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----  (multiplies exp(-2x))
    const double polyA =
        (-29.0/648.0) * xp[4] +
        (-29.0/324.0) * xp[5] +
        (-31.0/405.0) * xp[6] +
        (-41.0/1215.0) * xp[7] +
        (-58.0/8505.0) * xp[8] +
        (-4.0/8505.0) * xp[9];

    // ---- B(x) ----  (multiplies exp(-4x))
    const double polyB =
        (29.0/648.0) * xp[4] +
        (170255.0/995328.0) * xp[5] +
        (389707.0/1244160.0) * xp[6] +
        (2051701.0/5598720.0) * xp[7] +
        (3038533.0/9797760.0) * xp[8] +
        (4028711.0/19595520.0) * xp[9] +
        (332345.0/2939328.0) * xp[10] +
        (1951531.0/36741600.0) * xp[11] +
        (194311.0/9185400.0) * xp[12] +
        (23689.0/3444525.0) * xp[13] +
        (4763.0/2755620.0) * xp[14] +
        (1517.0/4822335.0) * xp[15] +
        (919.0/24111675.0) * xp[16] +
        (13.0/4822335.0) * xp[17] +
        (2.0/24111675.0) * xp[18];

    // ---- d chi(r) / d x ---- 
    // Chain rule: d/dx = (dr/dx) d/dr = bohr_per_ang * d/dr  (r: Ang / x: Bohr)
    const double dchi_dr = -(e2 * polyA) - (e4 * polyB);
    return constants::bohr_per_ang * dchi_dr;
}

double koideC8grad(const double& x) {
    // x is in Bohr

    std::array<double, 23> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----  (multiplies exp(-2x))
    const double polyA =
        (-1.0/432.0) * xp[6] +
        (-1.0/216.0) * xp[7] +
        (-94.0/23625.0) * xp[8] +
        (-253.0/141750.0) * xp[9] +
        (-1.0/2625.0) * xp[10] +
        (-2.0/70875.0) * xp[11];

    // ---- B(x) ----  (multiplies exp(-4x))
    const double polyB =
        (1.0/432.0) * xp[6] +
        (1.0/108.0) * xp[7] +
        (3377.0/189000.0) * xp[8] +
        (172745201.0/7838208000.0) * xp[9] +
        (38068337.0/1959552000.0) * xp[10] +
        (636817.0/48988800.0) * xp[11] +
        (2522557.0/367416000.0) * xp[12] +
        (43404469.0/14696640000.0) * xp[13] +
        (3933989.0/3674160000.0) * xp[14] +
        (1882543.0/5511240000.0) * xp[15] +
        (5497.0/55112400.0) * xp[16] +
        (366899.0/13778100000.0) * xp[17] +
        (7019.0/1148175000.0) * xp[18] +
        (293.0/267907500.0) * xp[19] +
        (82.0/602791875.0) * xp[20] +
        (61.0/6027918750.0) * xp[21] +
        (1.0/3013959375.0) * xp[22];

    // ---- d chi(r) / d x ---- 
    // Chain rule: d/dx = (dr/dx) d/dr = bohr_per_ang * d/dr  (r: Ang / x: Bohr)
    const double dchi_dr = -(e2 * polyA) - (e4 * polyB);
    return constants::bohr_per_ang * dchi_dr;
}

// default for C10 is chi13
double koideC10grad(const double& x) {
    // x is in Bohr

    std::array<double, 27> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----  (multiplies exp(-2x))
    const double polyA =
        (-19.0/246960.0) * xp[8] +
        (-19.0/123480.0) * xp[9] +
        (-92.0/694575.0) * xp[10] +
        (-83.0/1389150.0) * xp[11] +
        (-1.0/77175.0) * xp[12] +
        (-2.0/2083725.0) * xp[13];

    // ---- B(x) ----  (multiplies exp(-4x))
    const double polyB =
        (19.0/246960.0) * xp[8] +
        (19.0/61740.0) * xp[9] +
        (3301.0/5556600.0) * xp[10] +
        (1021.0/1389150.0) * xp[11] +
        (173.0/264600.0) * xp[12] +
        (30451927.0/68279500800.0) * xp[13] +
        (4153559.0/17069875200.0) * xp[14] +
        (10886347.0/99574272000.0) * xp[15] +
        (9244633.0/224042112000.0) * xp[16] +
        (13856807.0/1045529856000.0) * xp[17] +
        (574649.0/156829478400.0) * xp[18] +
        (3138241.0/3528663264000.0) * xp[19] +
        (868319.0/4410829080000.0) * xp[20] +
        (26087.0/630118440000.0) * xp[21] +
        (26977.0/3308121810000.0) * xp[22] +
        (1.0/735138180.0) * xp[23] +
        (137.0/827030452500.0) * xp[24] +
        (61.0/4962182715000.0) * xp[25] +
        (1.0/2481091357500.0) * xp[26];

    // ---- d chi(r) / d x ---- 
    // Chain rule: d/dx = (dr/dx) d/dr = bohr_per_ang * d/dr  (r: Ang / x: Bohr)
    const double dchi_dr = -(e2 * polyA) - (e4 * polyB);
    return constants::bohr_per_ang * dchi_dr;
}

// Also implement chi22 just in case
double koideC10chi22grad(const double& x) {
    // x is in Bohr

    std::array<double, 27> xp;
    xp[0] = 1.0;
    for (size_t i = 1; i < xp.size(); ++i) xp[i] = xp[i - 1] * x;

    const double e2 = std::exp(-2.0 * x);
    const double e4 = e2 * e2; // exp(-4x)

    // ---- A(x) ----  (multiplies exp(-2x))
    const double polyA =
        (-4.0/55125.0) * xp[8] +
        (-8.0/55125.0) * xp[9] +
        (-311.0/2480625.0) * xp[10] +
        (-142.0/2480625.0) * xp[11] +
        (-32.0/2480625.0) * xp[12] +
        (-8.0/7441875.0) * xp[13];

    // ---- B(x) ----  (multiplies exp(-4x))
    const double polyB =
        (4.0/55125.0) * xp[8] +
        (146273.0/508032000.0) * xp[9] +
        (38909.0/70560000.0) * xp[10] +
        (1063.0/1568000.0) * xp[11] +
        (286469.0/476280000.0) * xp[12] +
        (47072843.0/114307200000.0) * xp[13] +
        (6504203.0/28576800000.0) * xp[14] +
        (52367501.0/500094000000.0) * xp[15] +
        (15309733.0/375070500000.0) * xp[16] +
        (406649.0/30005640000.0) * xp[17] +
        (2151547.0/562605750000.0) * xp[18] +
        (776291.0/843908625000.0) * xp[19] +
        (204949.0/1054885781250.0) * xp[20] +
        (24401.0/632931468750.0) * xp[21] +
        (11884.0/1582328671875.0) * xp[22] +
        (2096.0/1582328671875.0) * xp[23] +
        (844.0/4746986015625.0) * xp[24] +
        (212.0/14240958046875.0) * xp[25] +
        (8.0/14240958046875.0) * xp[26];

    // ---- d chi(r) / d x ---- 
    // Chain rule: d/dx = (dr/dx) d/dr = bohr_per_ang * d/dr  (r: Ang / x: Bohr)
    const double dchi_dr = -(e2 * polyA) - (e4 * polyB);
    return constants::bohr_per_ang * dchi_dr;
}

//----------------------------------------------------------------------------//

double disp6(const double C6, const double d6, const double C8, const double C10, const bool use_koide, const double c6i, const double c6j, const std::vector<double>& p1,
             const std::vector<double>& xyz2, std::vector<double>& grad1, std::vector<double>& grad2, double& phi1,
             std::vector<double>& phi2, const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2,
             const size_t atom_index1, const size_t atom_index2, const double disp_scale_factor, bool do_grads, bool do_field,
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

    size_t nmon22 = nmon2 * 2;

    size_t shift_phi = atom_index2 * nmon2;
    size_t shift2 = shift_phi * 3;

    bool use_pbc = box.size();

    const double* boxinv = box_inverse.data();
    const double* boxptr = box.data();

    double dispersion_energy = 0;

    double x1_r, y1_r, z1_r;

    if (use_pbc) {
        x1_r = boxinv[0] * p1[0] + boxinv[3] * p1[1] + boxinv[6] * p1[2];
        y1_r = boxinv[1] * p1[0] + boxinv[4] * p1[1] + boxinv[7] * p1[2];
        z1_r = boxinv[2] * p1[0] + boxinv[5] * p1[1] + boxinv[8] * p1[2];
    }

    const size_t N = end2 - start2;

    double x[end2];
    double y[end2];
    double z[end2];

    double dx[end2];
    double dy[end2];
    double dz[end2];

    #pragma omp simd simdlen(8)
    for (size_t nv = start2; nv < end2; nv++) {
        x[nv] = xyz2[xyz2_offset + shift2 + nv];
        y[nv] = xyz2[xyz2_offset + nmon2 + shift2 + nv];
        z[nv] = xyz2[xyz2_offset + nmon22 + shift2 + nv];
    }

    // Apply minimum image convetion
    if (use_pbc) {
        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            double tmp1 = boxinv[0] * x[nv] + boxinv[3] * y[nv] + boxinv[6] * z[nv];
            double tmp2 = boxinv[1] * x[nv] + boxinv[4] * y[nv] + boxinv[7] * z[nv];
            double tmp3 = boxinv[2] * x[nv] + boxinv[5] * y[nv] + boxinv[8] * z[nv];

            tmp1 -= std::round(tmp1 - x1_r);
            tmp2 -= std::round(tmp2 - y1_r);
            tmp3 -= std::round(tmp3 - z1_r);

            x[nv] = boxptr[0] * tmp1 + boxptr[3] * tmp2 + boxptr[6] * tmp3;
            y[nv] = boxptr[1] * tmp1 + boxptr[4] * tmp2 + boxptr[7] * tmp3;
            z[nv] = boxptr[2] * tmp1 + boxptr[5] * tmp2 + boxptr[8] * tmp3;
        }
    }



    // If use_koide = false, use Tang-Toennies damping function                                                       
    if (!use_koide) {

        double rsq[end2];
        double r[end2];
        double inv_rsq[end2];
        double inv_r6[end2];

        double vscale[end2];

        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            dx[nv] = p1[0] - x[nv];
            dy[nv] = p1[1] - y[nv];
            dz[nv] = p1[2] - z[nv];

            rsq[nv] = dx[nv] * dx[nv] + dy[nv] * dy[nv] + dz[nv] * dz[nv];
            r[nv] = std::sqrt(rsq[nv]);

            inv_rsq[nv] = 1.0 / rsq[nv];
            inv_r6[nv] = inv_rsq[nv] * inv_rsq[nv] * inv_rsq[nv];

            size_t isls = islocal[isl1_offset] + islocal[isl2_offset + nv];
            vscale[nv] = (isls == 0) ? 0.0 : ((isls == 1) ? 0.5 : 1.0);
            vscale[nv] = r[nv] > cutoff ? 0.0 : vscale[nv];
        }

        if (do_field) {
            #pragma omp simd simdlen(8) reduction(+: phi1)
            for (size_t nv = start2; nv < end2; nv++) {
                phi2[shift_phi + start2 + nv] -= c6i * inv_r6[nv];
                phi1 -= c6j * inv_r6[nv];
            }
        }

        double tt6[end2];
        double ttsw_grad[end2];
        double ttsw[end2];
        double ipair_energy[end2];
        double d6r[end2];

        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            d6r[nv] = d6 * r[nv];
        }
        
        // Loop not vectorized because this functions are not yet vectorizable.
        for (size_t nv = start2; nv < end2; nv++) {
            tt6[nv] = disp::tang_toennies(d6r[nv]);
        }

        if (ewald_alpha > 0.0) {
            // Loop not vectorized because this functions are not yet vectorizable.
            for (size_t nv = start2; nv < end2; nv++) {
                ttsw[nv] = switch_function(r[nv], cutoff - 1.0, cutoff, ttsw_grad[nv]);
            }
        } else {
            #pragma omp simd simdlen(8)
            for (size_t nv = start2; nv < end2; nv++) {
                ttsw[nv] = 1.0;
                ttsw_grad[nv] = 0.0;
            }
        }
        

        // The idea here is quite simple.  At short range we want the TT term (e6) to model dispersion.  At long
        // range this becomes C6i C6j / Rij^6, which is handled by PME.  The reciprocal space part of PME always
        // includes extra terms that contribute below the cutoff, even if that pair shouldn't contribute.  For
        // intermonomer pairs, this means there is the TT contribution that we want, but we have to remove the
        // part of the reciprocal space from C6i C6j / Rij^6 that was added in the reciprocal space term.  Similarly
        // for intramonomer terms, there should be no TT contribution or C6i C6j / Rij^6 term, so we use the scale
        // factor to prevent TT contributing, and then back out the reciprocal space C6i C6j / Rij^6 contribution.
        // See http://dx.doi.org/10.1021/acs.jctc.5b00726 for more details of this trick.

        double e6[end2];
        double expterm[end2];
        double ar2[end2];
        double ar4[end2];
        double c6term[end2];

        #pragma omp simd simdlen(8) reduction(+ : dispersion_energy)
        for (size_t nv = start2; nv < end2; nv++) {
            e6[nv] = C6 * tt6[nv] * inv_r6[nv];
            dispersion_energy -= disp_scale_factor * ttsw[nv] * e6[nv] * vscale[nv];
        }
        
        if (ewald_alpha > 0.0) {
            #pragma omp simd simdlen(8) reduction(+ : dispersion_energy)
            for (size_t nv = start2; nv < end2; nv++) {
                c6term[nv] = c6i * c6j * inv_r6[nv];
                
                // Intermediates used in the dispersion PME terms
                ar2[nv] = ewald_alpha * ewald_alpha * rsq[nv];
                ar4[nv] = ar2[nv] * ar2[nv];
                expterm[nv] = ewald_alpha ? std::exp(-ar2[nv]) : 1;

                double pmeterm = c6i * c6j * (1 - (1 + ar2[nv] + ar4[nv] / 2) * expterm[nv]) * inv_r6[nv];

                dispersion_energy -= disp_scale_factor * (1 - ttsw[nv]) * c6term[nv] * vscale[nv] - pmeterm * vscale[nv];
            }
        }

        if (do_grads) {
            double grad[end2];
            std::fill(grad, grad + end2, 0.0);
            
            if (ewald_alpha > 0.0) {
                #pragma omp simd simdlen(8)
                for (size_t nv = start2; nv < end2; nv++) {
                    const double c6sw = 1 - ttsw[nv];
                    const double c6sw_grad = -ttsw_grad[nv];
                    const double c6term_grad = 6 * c6term[nv] * inv_rsq[nv];
                    double ar6 = ar4[nv] * ar2[nv];
                    double pmeterm2 = c6i * c6j * (1 - (1 + ar2[nv] + ar4[nv] / 2 + ar6 / 6) * expterm[nv]) * inv_r6[nv];
                    const double pmeterm_grad = 6 * pmeterm2 * inv_rsq[nv];
                    const double c6grad = c6sw * c6term_grad - c6sw_grad * c6term[nv] / r[nv];
                    grad[nv] += disp_scale_factor * c6grad - pmeterm_grad;
                }
            }

            double gradx = 0.0;
            double grady = 0.0;
            double gradz = 0.0;

            double C6_d6_7 = C6 * std::pow(d6, 7);

            #pragma omp simd simdlen(8) reduction(+ : gradx, grady, gradz)
            for (size_t nv = start2; nv < end2; nv++) {

                // Note: e6term_grad = - d(e6)/dr * (1/r) , where e6 = tt6*C6/r^6. The extra factor of (1/r) is there because (dx,dy,dz) will soon be 
                // multiplied to yield the x,y,z components of the gradient. See calculations for getting grad1 and grad2, right below.
                const double e6term_grad = 6 * e6[nv] * inv_rsq[nv] - C6_d6_7 * if6 * std::exp(-d6r[nv]) / r[nv];
                const double ttgrad = ttsw[nv] * e6term_grad - ttsw_grad[nv] * e6[nv] / r[nv];
                
                grad[nv] += disp_scale_factor * ttgrad;

                gradx += dx[nv] * grad[nv] * vscale[nv];

                grady += dy[nv] * grad[nv] * vscale[nv];

                gradz += dz[nv] * grad[nv] * vscale[nv];

                grad2[shift2 + nv] -= dx[nv] * grad[nv] * vscale[nv];
                grad2[shift2 + nmon2 + nv] -= dy[nv] * grad[nv] * vscale[nv];
                grad2[shift2 + nmon22 + nv] -= dz[nv] * grad[nv] * vscale[nv];
            }

            if (virial != 0) {

                double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

                #pragma omp simd simdlen(8) reduction(+ : v[0:6])
                for (size_t nv = start2; nv < end2; nv++) {
                    v[0] -= dx[nv] * dx[nv] * grad[nv] * vscale[nv];  //  update the virial for the atom pair
                    v[1] -= dx[nv] * dy[nv] * grad[nv] * vscale[nv];
                    v[2] -= dx[nv] * dz[nv] * grad[nv] * vscale[nv];

                    v[3] -= dy[nv] * dy[nv] * grad[nv] * vscale[nv];
                    v[4] -= dy[nv] * dz[nv] * grad[nv] * vscale[nv];

                    v[5] -= dz[nv] * dz[nv] * grad[nv] * vscale[nv];
                }

                (*virial)[0] += v[0];
                (*virial)[1] += v[1];
                (*virial)[2] += v[2];
                (*virial)[4] += v[3];
                (*virial)[5] += v[4];
                (*virial)[8] += v[5];

                (*virial)[3] += v[1];
                (*virial)[6] += v[2];
                (*virial)[7] += v[4];
            }

            grad1[0] += gradx;
            grad1[1] += grady;
            grad1[2] += gradz;
        }
    }

    // If use_koide = true, use Koide damping function // inv_r10 terms are all commented out for now
    else {

        double rsq[end2];
        double r[end2];
        double inv_rsq[end2];
        double inv_r6[end2];
        double inv_r8[end2];
        //double inv_r10[end2];

        double vscale[end2];

        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            dx[nv] = p1[0] - x[nv];
            dy[nv] = p1[1] - y[nv];
            dz[nv] = p1[2] - z[nv];

            rsq[nv] = dx[nv] * dx[nv] + dy[nv] * dy[nv] + dz[nv] * dz[nv];
            r[nv] = std::sqrt(rsq[nv]);

            inv_rsq[nv] = 1.0 / rsq[nv];
            inv_r6[nv] = inv_rsq[nv] * inv_rsq[nv] * inv_rsq[nv];
            inv_r8[nv] = inv_r6[nv] * inv_rsq[nv];
            //inv_r10[nv] = inv_r8[nv] * inv_rsq[nv];

            size_t isls = islocal[isl1_offset] + islocal[isl2_offset + nv];
            vscale[nv] = (isls == 0) ? 0.0 : ((isls == 1) ? 0.5 : 1.0);
            vscale[nv] = r[nv] > cutoff ? 0.0 : vscale[nv];
        }

        if (do_field) {
            #pragma omp simd simdlen(8) reduction(+: phi1)
            for (size_t nv = start2; nv < end2; nv++) {
                phi2[shift_phi + start2 + nv] -= c6i * inv_r6[nv];
                phi1 -= c6j * inv_r6[nv];
            }
        }

        double k6[end2], k8[end2]; //, k10[end2];
        double k6g[end2], k8g[end2]; //, k10g[end2];
        double ttsw_grad[end2];
        double ttsw[end2];
        double ipair_energy[end2];
        double r_bohr[end2];

        #pragma omp simd simdlen(8)
        for (size_t nv = start2; nv < end2; nv++) {
            r_bohr[nv] = constants::bohr_per_ang * r[nv];
        }

        // Loop not vectorized because this functions are not yet vectorizable.
        for (size_t nv = start2; nv < end2; nv++) {
            k6[nv] = disp::koideC6(r_bohr[nv]);
            k8[nv] = disp::koideC8(r_bohr[nv]);
            // k10[nv] = disp::koideC10(r_bohr[nv]);
            k6g[nv] = disp::koideC6grad(r_bohr[nv]);
            k8g[nv] = disp::koideC8grad(r_bohr[nv]);
            // k10g[nv] = disp::koideC10grad(r_bohr[nv]);
        }

        if (ewald_alpha > 0.0) {
            // Loop not vectorized because this functions are not yet vectorizable.
            for (size_t nv = start2; nv < end2; nv++) {
                ttsw[nv] = switch_function(r[nv], cutoff - 1.0, cutoff, ttsw_grad[nv]);
            }
        } else {
            #pragma omp simd simdlen(8)
            for (size_t nv = start2; nv < end2; nv++) {
                ttsw[nv] = 1.0;
                ttsw_grad[nv] = 0.0;
            }
        }
        

        // The idea here is quite simple.  At short range we want the TT term (e6) to model dispersion.  At long
        // range this becomes C6i C6j / Rij^6, which is handled by PME.  The reciprocal space part of PME always
        // includes extra terms that contribute below the cutoff, even if that pair shouldn't contribute.  For
        // intermonomer pairs, this means there is the TT contribution that we want, but we have to remove the
        // part of the reciprocal space from C6i C6j / Rij^6 that was added in the reciprocal space term.  Similarly
        // for intramonomer terms, there should be no TT contribution or C6i C6j / Rij^6 term, so we use the scale
        // factor to prevent TT contributing, and then back out the reciprocal space C6i C6j / Rij^6 contribution.
        // See http://dx.doi.org/10.1021/acs.jctc.5b00726 for more details of this trick.

        double e6[end2];
        double expterm[end2];
        double ar2[end2];
        double ar4[end2];
        double c6term[end2];
        // e6 holds all C_N contributions
        // ekN (N=6,8,10) holds Koide damped C_N contributions
        double ek6[end2], ek8[end2]; // , ek10[end2];

        #pragma omp simd simdlen(8) reduction(+ : dispersion_energy)
        for (size_t nv = start2; nv < end2; nv++) {
            ek6[nv] = C6 * k6[nv] * inv_r6[nv];
            ek8[nv] = C8 * k8[nv] * inv_r8[nv];
            //ek10[nv] = C10 * k10[nv] * inv_r10[nv];
            e6[nv] = ek6[nv] + ek8[nv] ; // + ek10[nv] ;
            dispersion_energy -= disp_scale_factor * ttsw[nv] * e6[nv] * vscale[nv];
        }
        
        if (ewald_alpha > 0.0) {
            #pragma omp simd simdlen(8) reduction(+ : dispersion_energy)
            for (size_t nv = start2; nv < end2; nv++) {
                c6term[nv] = c6i * c6j * inv_r6[nv];
                
                // Intermediates used in the dispersion PME terms
                ar2[nv] = ewald_alpha * ewald_alpha * rsq[nv];
                ar4[nv] = ar2[nv] * ar2[nv];
                expterm[nv] = ewald_alpha ? std::exp(-ar2[nv]) : 1;

                double pmeterm = c6i * c6j * (1 - (1 + ar2[nv] + ar4[nv] / 2) * expterm[nv]) * inv_r6[nv];

                dispersion_energy -= disp_scale_factor * (1 - ttsw[nv]) * c6term[nv] * vscale[nv] - pmeterm * vscale[nv];
            }
        }

        if (do_grads) {
            double grad[end2];
            std::fill(grad, grad + end2, 0.0);
            
            if (ewald_alpha > 0.0) {
                #pragma omp simd simdlen(8)
                for (size_t nv = start2; nv < end2; nv++) {
                    const double c6sw = 1 - ttsw[nv];
                    const double c6sw_grad = -ttsw_grad[nv];
                    const double c6term_grad = 6 * c6term[nv] * inv_rsq[nv];
                    double ar6 = ar4[nv] * ar2[nv];
                    double pmeterm2 = c6i * c6j * (1 - (1 + ar2[nv] + ar4[nv] / 2 + ar6 / 6) * expterm[nv]) * inv_r6[nv];
                    const double pmeterm_grad = 6 * pmeterm2 * inv_rsq[nv];
                    const double c6grad = c6sw * c6term_grad - c6sw_grad * c6term[nv] / r[nv];
                    grad[nv] += disp_scale_factor * c6grad - pmeterm_grad;
                }
            }

            double gradx = 0.0;
            double grady = 0.0;
            double gradz = 0.0;

            #pragma omp simd simdlen(8) reduction(+ : gradx, grady, gradz)
            for (size_t nv = start2; nv < end2; nv++) {

                // Note: e6term_grad = - d(e6)/dr * (1/r) , where e6 = k6*C6/r^6 (+ k8*C8/r^8 + k10*C10/r^10). 
                // The extra factor of (1/r) is there because (dx,dy,dz) will soon be multiplied to yield the x,y,z components of the gradient. 
                // See calculations for getting grad1 and grad2, right below.
                const double e6term_grad = (6 * ek6[nv] + 8 * ek8[nv]) * inv_rsq[nv] - (C6 * k6g[nv] * inv_r6[nv] + C8 * k8g[nv] * inv_r8[nv]) / r[nv];
                //const double e6term_grad = (6 * ek6[nv] + 8 * ek8[nv] + 10 * ek10[nv]) * inv_rsq[nv] - (C6 * k6g[nv] * inv_r6[nv] + C8 * k8g[nv] * inv_r8[nv] + C10 * k10g[nv] * inv_r10[nv])/r[nv];

                const double ttgrad = ttsw[nv] * e6term_grad - ttsw_grad[nv] * e6[nv] / r[nv];
                
                grad[nv] += disp_scale_factor * ttgrad;

                gradx += dx[nv] * grad[nv] * vscale[nv];

                grady += dy[nv] * grad[nv] * vscale[nv];

                gradz += dz[nv] * grad[nv] * vscale[nv];

                grad2[shift2 + nv] -= dx[nv] * grad[nv] * vscale[nv];
                grad2[shift2 + nmon2 + nv] -= dy[nv] * grad[nv] * vscale[nv];
                grad2[shift2 + nmon22 + nv] -= dz[nv] * grad[nv] * vscale[nv];
            }

            if (virial != 0) {

                double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

                #pragma omp simd simdlen(8) reduction(+ : v[0:6])
                for (size_t nv = start2; nv < end2; nv++) {
                    v[0] -= dx[nv] * dx[nv] * grad[nv] * vscale[nv];  //  update the virial for the atom pair
                    v[1] -= dx[nv] * dy[nv] * grad[nv] * vscale[nv];
                    v[2] -= dx[nv] * dz[nv] * grad[nv] * vscale[nv];

                    v[3] -= dy[nv] * dy[nv] * grad[nv] * vscale[nv];
                    v[4] -= dy[nv] * dz[nv] * grad[nv] * vscale[nv];

                    v[5] -= dz[nv] * dz[nv] * grad[nv] * vscale[nv];
                }

                (*virial)[0] += v[0];
                (*virial)[1] += v[1];
                (*virial)[2] += v[2];
                (*virial)[4] += v[3];
                (*virial)[5] += v[4];
                (*virial)[8] += v[5];

                (*virial)[3] += v[1];
                (*virial)[6] += v[2];
                (*virial)[7] += v[4];
            }

            grad1[0] += gradx;
            grad1[1] += grady;
            grad1[2] += gradz;
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

    std::cerr << "dispersion_energy = " << dispersion_energy << std::endl;
#endif

    return dispersion_energy;
}

bool GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_C6, double& out_d6, double& out_C8, double& out_C10,
           std::vector<std::pair<std::string, std::string>>& ignore_disp, const nlohmann::json& repdisp_j) {
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

    std::vector<double> C6, d6, C8, C10;
    size_t nt2, i, j;

    out_C6 = 0.0;
    out_d6 = 0.0;
    out_C8 = 0.0;
    out_C10 = 0.0;

    bool use_koide = false;
    bool use_C8 = false;
    bool use_C10 = false;

    if (std::find(ignore_disp.begin(), ignore_disp.end(), std::make_pair(mon_id1, mon_id2)) != ignore_disp.end() ||
        std::find(ignore_disp.begin(), ignore_disp.end(), std::make_pair(mon_id2, mon_id1)) != ignore_disp.end()) {
        out_C6 = 0.0;
        out_d6 = 0.0;
        out_C8 = 0.0;
        out_C10 = 0.0;
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

    } else if (mon_id1 == "f-"  and mon_id2 == "h2o") {
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

        d6.push_back(3.57999);  // A^(-1)
        d6.push_back(2.70536);  // A^(-1)
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

        d6.push_back(3.27293);  // A^(-1)
        d6.push_back(2.79252);  // A^(-1)
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

        d6.push_back(3.05463);  // A^(-1)
        d6.push_back(2.81519);  // A^(-1)
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

        d6.push_back(2.72259);  // A^(-1)
        d6.push_back(2.80139);  // A^(-1)
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
    } else if (mon_id1 == "nma" and mon_id2 == "nma") {
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(2);
        types1.push_back(3);
        types1.push_back(3);
        types1.push_back(3);
        types1.push_back(4);
        types1.push_back(5);
        types1.push_back(6);
        types1.push_back(7);
        types1.push_back(7);
        types1.push_back(7);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(2);
        types2.push_back(3);
        types2.push_back(3);
        types2.push_back(3);
        types2.push_back(4);
        types2.push_back(5);
        types2.push_back(6);
        types2.push_back(7);
        types2.push_back(7);
        types2.push_back(7);

        nt2 = 8;

        use_koide = true;
        use_C8 = true;
        use_C10 = false;

        // Fill in (in order) dispersion parameters (Koide: C6, C8, C10; Tang-Toennies: C6, d6)
        C6.push_back(285.259);  // kcal/mol * A^(-6)  A--A
        C6.push_back(217.0515);  // kcal/mol * A^(-6)  A--B
        C6.push_back(282.765);  // kcal/mol * A^(-6)  A--C
        C6.push_back(96.8133);  // kcal/mol * A^(-6)  A--D
        C6.push_back(213.7169);  // kcal/mol * A^(-6)  A--E
        C6.push_back(93.1343);  // kcal/mol * A^(-6)  A--F
        C6.push_back(282.1311);  // kcal/mol * A^(-6)  A--G
        C6.push_back(97.4931);  // kcal/mol * A^(-6)  A--H
        C6.push_back(217.0515);  // kcal/mol * A^(-6)  A--B
        C6.push_back(201.2604);  // kcal/mol * A^(-6)  B--B
        C6.push_back(222.4943);  // kcal/mol * A^(-6)  B--C
        C6.push_back(83.3556);  // kcal/mol * A^(-6)  B--D
        C6.push_back(178.8002);  // kcal/mol * A^(-6)  B--E
        C6.push_back(79.0243);  // kcal/mol * A^(-6)  B--F
        C6.push_back(221.8054);  // kcal/mol * A^(-6)  B--G
        C6.push_back(83.9664);  // kcal/mol * A^(-6)  B--H
        C6.push_back(282.765);  // kcal/mol * A^(-6)  A--C
        C6.push_back(222.4943);  // kcal/mol * A^(-6)  B--C
        C6.push_back(282.0071);  // kcal/mol * A^(-6)  C--C
        C6.push_back(98.0627);  // kcal/mol * A^(-6)  C--D
        C6.push_back(215.4393);  // kcal/mol * A^(-6)  C--E
        C6.push_back(94.0988);  // kcal/mol * A^(-6)  C--F
        C6.push_back(281.3319);  // kcal/mol * A^(-6)  C--G
        C6.push_back(98.7608);  // kcal/mol * A^(-6)  C--H
        C6.push_back(96.8133);  // kcal/mol * A^(-6)  A--D
        C6.push_back(83.3556);  // kcal/mol * A^(-6)  B--D
        C6.push_back(98.0627);  // kcal/mol * A^(-6)  C--D
        C6.push_back(35.5292);  // kcal/mol * A^(-6)  D--D
        C6.push_back(77.0538);  // kcal/mol * A^(-6)  D--E
        C6.push_back(33.8787);  // kcal/mol * A^(-6)  D--F
        C6.push_back(97.7871);  // kcal/mol * A^(-6)  D--G
        C6.push_back(35.791);  // kcal/mol * A^(-6)  D--H
        C6.push_back(213.7169);  // kcal/mol * A^(-6)  A--E
        C6.push_back(178.8002);  // kcal/mol * A^(-6)  B--E
        C6.push_back(215.4393);  // kcal/mol * A^(-6)  C--E
        C6.push_back(77.0538);  // kcal/mol * A^(-6)  D--E
        C6.push_back(167.7905);  // kcal/mol * A^(-6)  E--E
        C6.push_back(73.609);  // kcal/mol * A^(-6)  E--F
        C6.push_back(214.8606);  // kcal/mol * A^(-6)  E--G
        C6.push_back(77.605);  // kcal/mol * A^(-6)  E--H
        C6.push_back(93.1343);  // kcal/mol * A^(-6)  A--F
        C6.push_back(79.0243);  // kcal/mol * A^(-6)  B--F
        C6.push_back(94.0988);  // kcal/mol * A^(-6)  C--F
        C6.push_back(33.8787);  // kcal/mol * A^(-6)  D--F
        C6.push_back(73.609);  // kcal/mol * A^(-6)  E--F
        C6.push_back(32.3262);  // kcal/mol * A^(-6)  F--F
        C6.push_back(93.8508);  // kcal/mol * A^(-6)  F--G
        C6.push_back(34.1129);  // kcal/mol * A^(-6)  F--H
        C6.push_back(282.1311);  // kcal/mol * A^(-6)  A--G
        C6.push_back(221.8054);  // kcal/mol * A^(-6)  B--G
        C6.push_back(281.3319);  // kcal/mol * A^(-6)  C--G
        C6.push_back(97.7871);  // kcal/mol * A^(-6)  D--G
        C6.push_back(214.8606);  // kcal/mol * A^(-6)  E--G
        C6.push_back(93.8508);  // kcal/mol * A^(-6)  F--G
        C6.push_back(280.6705);  // kcal/mol * A^(-6)  G--G
        C6.push_back(98.4829);  // kcal/mol * A^(-6)  G--H
        C6.push_back(97.4931);  // kcal/mol * A^(-6)  A--H
        C6.push_back(83.9664);  // kcal/mol * A^(-6)  B--H
        C6.push_back(98.7608);  // kcal/mol * A^(-6)  C--H
        C6.push_back(35.791);  // kcal/mol * A^(-6)  D--H
        C6.push_back(77.605);  // kcal/mol * A^(-6)  E--H
        C6.push_back(34.1129);  // kcal/mol * A^(-6)  F--H
        C6.push_back(98.4829);  // kcal/mol * A^(-6)  G--H
        C6.push_back(36.0451);  // kcal/mol * A^(-6)  H--H
        C8.push_back(3056.355584838995);  // kcal/mol * A^(-8)  A--A
        C8.push_back(1886.2613218245347);  // kcal/mol * A^(-8)  A--B
        C8.push_back(2908.799001723657);  // kcal/mol * A^(-8)  A--C
        C8.push_back(754.6070387729116);  // kcal/mol * A^(-8)  A--D
        C8.push_back(1934.3394323764846);  // kcal/mol * A^(-8)  A--E
        C8.push_back(732.029748415136);  // kcal/mol * A^(-8)  A--F
        C8.push_back(2903.2831382377813);  // kcal/mol * A^(-8)  A--G
        C8.push_back(759.1125931138287);  // kcal/mol * A^(-8)  A--H
        C8.push_back(1886.2613218245347);  // kcal/mol * A^(-8)  A--B
        C8.push_back(1341.7226994747691);  // kcal/mol * A^(-8)  B--B
        C8.push_back(1838.5941517841247);  // kcal/mol * A^(-8)  B--C
        C8.push_back(481.04349007146646);  // kcal/mol * A^(-8)  B--D
        C8.push_back(1256.3551137724146);  // kcal/mol * A^(-8)  B--E
        C8.push_back(461.29905253580614);  // kcal/mol * A^(-8)  B--F
        C8.push_back(1833.4988753006696);  // kcal/mol * A^(-8)  B--G
        C8.push_back(483.8120329543737);  // kcal/mol * A^(-8)  B--H
        C8.push_back(2908.799001723657);  // kcal/mol * A^(-8)  A--C
        C8.push_back(1838.5941517841247);  // kcal/mol * A^(-8)  B--C
        C8.push_back(2780.5161073889763);  // kcal/mol * A^(-8)  C--C
        C8.push_back(722.5427461831623);  // kcal/mol * A^(-8)  C--D
        C8.push_back(1857.902567931954);  // kcal/mol * A^(-8)  C--E
        C8.push_back(699.569306599865);  // kcal/mol * A^(-8)  C--F
        C8.push_back(2774.8073140854767);  // kcal/mol * A^(-8)  C--G
        C8.push_back(726.8309329295561);  // kcal/mol * A^(-8)  C--H
        C8.push_back(754.6070387729116);  // kcal/mol * A^(-8)  A--D
        C8.push_back(481.04349007146646);  // kcal/mol * A^(-8)  B--D
        C8.push_back(722.5427461831623);  // kcal/mol * A^(-8)  C--D
        C8.push_back(173.271984140003);  // kcal/mol * A^(-8)  D--D
        C8.push_back(472.4220996212332);  // kcal/mol * A^(-8)  D--E
        C8.push_back(167.42063713792518);  // kcal/mol * A^(-8)  D--F
        C8.push_back(720.7935158367028);  // kcal/mol * A^(-8)  D--G
        C8.push_back(174.2004052957035);  // kcal/mol * A^(-8)  D--H
        C8.push_back(1934.3394323764846);  // kcal/mol * A^(-8)  A--E
        C8.push_back(1256.3551137724146);  // kcal/mol * A^(-8)  B--E
        C8.push_back(1857.902567931954);  // kcal/mol * A^(-8)  C--E
        C8.push_back(472.4220996212332);  // kcal/mol * A^(-8)  D--E
        C8.push_back(1239.3965828032478);  // kcal/mol * A^(-8)  E--E
        C8.push_back(456.18255377241246);  // kcal/mol * A^(-8)  E--F
        C8.push_back(1853.5944451044058);  // kcal/mol * A^(-8)  E--G
        C8.push_back(475.1314773600689);  // kcal/mol * A^(-8)  E--H
        C8.push_back(732.029748415136);  // kcal/mol * A^(-8)  A--F
        C8.push_back(461.29905253580614);  // kcal/mol * A^(-8)  B--F
        C8.push_back(699.569306599865);  // kcal/mol * A^(-8)  C--F
        C8.push_back(167.42063713792518);  // kcal/mol * A^(-8)  D--F
        C8.push_back(456.18255377241246);  // kcal/mol * A^(-8)  E--F
        C8.push_back(161.91056154657812);  // kcal/mol * A^(-8)  F--F
        C8.push_back(697.9216859573544);  // kcal/mol * A^(-8)  F--G
        C8.push_back(168.31904698866185);  // kcal/mol * A^(-8)  F--H
        C8.push_back(2903.2831382377813);  // kcal/mol * A^(-8)  A--G
        C8.push_back(1833.4988753006696);  // kcal/mol * A^(-8)  B--G
        C8.push_back(2774.8073140854767);  // kcal/mol * A^(-8)  C--G
        C8.push_back(720.7935158367028);  // kcal/mol * A^(-8)  D--G
        C8.push_back(1853.5944451044058);  // kcal/mol * A^(-8)  E--G
        C8.push_back(697.9216859573544);  // kcal/mol * A^(-8)  F--G
        C8.push_back(2769.1216723600915);  // kcal/mol * A^(-8)  G--G
        C8.push_back(725.0714129928233);  // kcal/mol * A^(-8)  G--H
        C8.push_back(759.1125931138287);  // kcal/mol * A^(-8)  A--H
        C8.push_back(483.8120329543737);  // kcal/mol * A^(-8)  B--H
        C8.push_back(726.8309329295561);  // kcal/mol * A^(-8)  C--H
        C8.push_back(174.2004052957035);  // kcal/mol * A^(-8)  D--H
        C8.push_back(475.1314773600689);  // kcal/mol * A^(-8)  E--H
        C8.push_back(168.31904698866185);  // kcal/mol * A^(-8)  F--H
        C8.push_back(725.0714129928233);  // kcal/mol * A^(-8)  G--H
        C8.push_back(175.1318275819003);  // kcal/mol * A^(-8)  H--H

        // =====>> END SECTION DISPERSION <<=====
    } else {
        out_C6 = 0.0;
        out_d6 = 0.0;
        out_C8 = 0.0;
        out_C10 = 0.0;
        return false;
    }

    i = types1[index1];
    j = types2[index2];

    if (!use_koide) {
        out_C6 = C6[i * nt2 + j];
        out_d6 = d6[i * nt2 + j];
        out_C8 = 0.0;
        out_C10 = 0.0;
    } else {
        out_C6 = C6[i * nt2 + j];
        out_d6 = 0.0;
        if (use_C8) { out_C8 = C8[i * nt2 + j]; } else { out_C8 = 0.0; }
        if (use_C10) { out_C10 = C10[i * nt2 + j]; } else { out_C10 = 0.0; }
    }

    return true;
}

}  // namespace disp

//----------------------------------------------------------------------------//

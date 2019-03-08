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

#include "potential/electrostatics/electrostatic_tensors.h"

namespace elec {

ElectroTensor::ElectroTensor(size_t n) { maxnmon = n; }

////////////////////////////////////////////////////////////////////////////////

void ElectroTensor::CalcT0AndT1(double* xyz1, double* xyz2, size_t mon1_index, size_t mon2_index_start,
                                size_t mon2_index_end, size_t nmon1, size_t nmon2, size_t site_i, size_t site_j,
                                double Ai, double Asqsqi, double aCC, double aCC1_4, double g34, double* ts0_mon1,
                                double* ts0_mon2, double* ts1_mon1, double* ts1_mon2) {
    // Shifts that will be useful in the loops
    const size_t nmon12 = nmon1 * 2;
    const size_t nmon22 = nmon2 * 2;
    const size_t site_i3 = site_i * 3;
    const size_t site_j3 = site_j * 3;
    const size_t site_inmon1 = site_i * nmon1;
    const size_t site_jnmon2 = site_j * nmon2;
    const size_t site_inmon13 = nmon1 * site_i3;
    const size_t site_jnmon23 = nmon2 * site_j3;

    // Coordinates x, y and z of site i of monomer 1
    const double xyzmon1_x = xyz1[site_inmon13 + mon1_index];
    const double xyzmon1_y = xyz1[site_inmon13 + nmon1 + mon1_index];
    const double xyzmon1_z = xyz1[site_inmon13 + nmon12 + mon1_index];

    // Fill vectors with zeros in the desired range
    double v0_[maxnmon];
    double v1_[maxnmon];
    double v2_[maxnmon];
    double v3_[maxnmon];
    double v4_[maxnmon];
    double v5_[maxnmon];
    double v6_[maxnmon];

// Store rijx, rijy and rijz in vectors
#ifdef _OPENMP
#pragma omp simd
#endif
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v0_[m] = xyzmon1_x - xyz2[site_jnmon23 + m];           // rijx
        v1_[m] = xyzmon1_y - xyz2[site_jnmon23 + nmon2 + m];   // rijy
        v2_[m] = xyzmon1_z - xyz2[site_jnmon23 + nmon22 + m];  // rijz
    }

    // Store r^2 in vector
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v3_[m] = v0_[m] * v0_[m] + v1_[m] * v1_[m] + v2_[m] * v2_[m];  // rsq
    }

    // Store 1/r, a*(r/A)^4 in vector
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v4_[m] = 1.0 / std::sqrt(v3_[m]);  // 1/r

        v5_[m] = aCC * v3_[m] * v3_[m] * Asqsqi;  // a*(r/A)^4
    }

    // Compute gammq and store result in vector. This loop is not vectorizable
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v6_[m] = gammq(0.75, v5_[m]);  // gammq
    }

// Finalize computation of electric field
#ifdef _OPENMP
#pragma omp simd
#endif
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        const double exp1 = std::exp(-v5_[m]);

        // Screening functions
        const double s1r = v4_[m] - exp1 * v4_[m];
        const double s0r = s1r + aCC1_4 * Ai * g34 * v6_[m];
        const double s1r3 = s1r * v4_[m] * v4_[m];

        // Compute electrostatic tensors
        // Ts0
        ts0_mon1[site_inmon1 + m] = s0r;
        ts0_mon2[site_jnmon2 + m] = s0r;

        const double ts1x = s1r3 * v0_[m];
        const double ts1y = s1r3 * v1_[m];
        const double ts1z = s1r3 * v2_[m];

        // Ts1
        ts1_mon1[site_inmon13 + m] = ts1x;
        ts1_mon2[site_jnmon23 + m] = -ts1x;

        ts1_mon1[site_inmon13 + nmon1 + m] = ts1y;
        ts1_mon2[site_jnmon23 + nmon2 + m] = -ts1y;

        ts1_mon1[site_inmon13 + nmon12 + m] = ts1z;
        ts1_mon2[site_jnmon23 + nmon22 + m] = -ts1z;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ElectroTensor::CalcT1AndT2(double* xyz1, double* xyz2, size_t mon1_index, size_t mon2_index_start,
                                size_t mon2_index_end, size_t nmon1, size_t nmon2, size_t site_i, size_t site_j,
                                double Asqsqi, double aDD, size_t nsites, double* ts1_mon1, double* ts1_mon2,
                                double* ts2) {
    // Shifts that will be useful in the loops
    const size_t nmon12 = nmon1 * 2;
    const size_t nmon22 = nmon2 * 2;

    const size_t nsites3 = 3 * nsites;

    const size_t site_i3 = site_i * 3;
    const size_t site_j3 = site_j * 3;
    const size_t site_inmon13 = nmon1 * site_i3;
    const size_t site_jnmon23 = nmon2 * site_j3;

    // Coordinates x, y and z of site i of monomer 1
    const double xyzmon1_x = xyz1[site_inmon13 + mon1_index];
    const double xyzmon1_y = xyz1[site_inmon13 + nmon1 + mon1_index];
    const double xyzmon1_z = xyz1[site_inmon13 + nmon12 + mon1_index];

#pragma omp simd
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        // Distances between sites i and j from mon1 and mon2
        const double rijx = xyzmon1_x - xyz2[site_jnmon23 + m];
        const double rijy = xyzmon1_y - xyz2[site_jnmon23 + nmon2 + m];
        const double rijz = xyzmon1_z - xyz2[site_jnmon23 + nmon22 + m];

        const double rsq = rijx * rijx + rijy * rijy + rijz * rijz;
        const double r = std::sqrt(rsq);
        const double ri = 1.0 / r;
        const double risq = ri * ri;
        const double rsqsq = rsq * rsq;

        // Some values that will be used in the screening functions
        const double rA4 = rsqsq * Asqsqi;
        const double arA4 = aDD * rA4;
#if NO_THOLE
        const double exp1 = 0;
#else
        const double exp1 = std::exp(-arA4);
#endif
        const double exp1r = exp1 * ri;
        // Get screening functions
        const double s1r = ri - exp1r;
        const double s1r3 = s1r * risq;
        const double s2r5_3 = (3.0 * s1r3 - 4.0 * aDD * rA4 * exp1r * risq) * risq;
        const double ts2x = s2r5_3 * rijx;
        const double ts2y = s2r5_3 * rijy;
        const double ts2z = s2r5_3 * rijz;

        // Obtaining the 3 components of the tensor Tij_alpha
        const double ts1x = s1r3 * rijx;
        const double ts1y = s1r3 * rijy;
        const double ts1z = s1r3 * rijz;

        // Obtaining the 9 components of the tensor Tij_alpha_beta
        const double ts2xx = ts2x * rijx - s1r3;
        const double ts2xy = ts2x * rijy;
        const double ts2xz = ts2x * rijz;

        const double ts2yy = ts2y * rijy - s1r3;
        const double ts2yz = ts2y * rijz;

        const double ts2zz = ts2z * rijz - s1r3;

        // Store the tensors in the pointers
        // Ts1
        ts1_mon1[site_inmon13 + m] = ts1x;
        ts1_mon2[site_jnmon23 + m] = -ts1x;

        ts1_mon1[site_inmon13 + nmon1 + m] = ts1y;
        ts1_mon2[site_jnmon23 + nmon2 + m] = -ts1y;

        ts1_mon1[site_inmon13 + nmon12 + m] = ts1z;
        ts1_mon2[site_jnmon23 + nmon22 + m] = -ts1z;

        // Ts2
        // Order will be xx xy xz yx yy yz zx zy zz
        ts2[mon1_index * nsites3 + m] = ts2xx;           // ts2xx
        ts2[mon1_index * nsites3 + nmon2 + m] = ts2xy;   // ts2xy
        ts2[mon1_index * nsites3 + nmon22 + m] = ts2xz;  // ts2xz

        ts2[(mon1_index + nmon1) * nsites3 + m] = ts2xy;           // ts2yx
        ts2[(mon1_index + nmon1) * nsites3 + nmon2 + m] = ts2yy;   // ts2yy
        ts2[(mon1_index + nmon1) * nsites3 + nmon22 + m] = ts2yz;  // ts2yz

        ts2[(mon1_index + nmon12) * nsites3 + m] = ts2xz;           // ts2zx
        ts2[(mon1_index + nmon12) * nsites3 + nmon2 + m] = ts2yz;   // ts2zy
        ts2[(mon1_index + nmon12) * nsites3 + nmon22 + m] = ts2zz;  // ts2zz
    }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace elec

////////////////////////////////////////////////////////////////////////////////

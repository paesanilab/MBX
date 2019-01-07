#include "fields.h"

namespace elec {

ElectricFieldHolder::ElectricFieldHolder(size_t n) {
    maxnmon = n;
    v0_ = std::vector<double>(maxnmon);
    v1_ = std::vector<double>(maxnmon);
    v2_ = std::vector<double>(maxnmon);
    v3_ = std::vector<double>(maxnmon);
    v4_ = std::vector<double>(maxnmon);
    v5_ = std::vector<double>(maxnmon);
    v6_ = std::vector<double>(maxnmon);
    v7_ = std::vector<double>(maxnmon);
    v8_ = std::vector<double>(maxnmon);
    v9_ = std::vector<double>(maxnmon);
    v10_ = std::vector<double>(maxnmon);
}

////////////////////////////////////////////////////////////////////////////////

void ElectricFieldHolder::CalcPermanentElecField(double *xyz1, double *xyz2, double *chg1, double *chg2,
                                                 size_t mon1_index, size_t mon2_index_start, size_t mon2_index_end,
                                                 size_t nmon1, size_t nmon2, size_t site_i, size_t site_j, double Ai,
                                                 double Asqsqi, double aCC, double aCC1_4, double g34,
                                                 double *Efqx_mon1, double *Efqy_mon1, double *Efqz_mon1, double *phi1,
                                                 double *phi2, double *Efq2, double elec_scale_factor,
                                                 double ewald_alpha, bool use_pbc, const std::vector<double> &box,
                                                 const std::vector<double> &box_inverse, double cutoff) {
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
    std::fill(v0_.begin() + mon2_index_start, v0_.begin() + mon2_index_end, 0.0);
    std::fill(v1_.begin() + mon2_index_start, v1_.begin() + mon2_index_end, 0.0);
    std::fill(v2_.begin() + mon2_index_start, v2_.begin() + mon2_index_end, 0.0);
    std::fill(v3_.begin() + mon2_index_start, v3_.begin() + mon2_index_end, 0.0);
    std::fill(v4_.begin() + mon2_index_start, v4_.begin() + mon2_index_end, 0.0);
    std::fill(v5_.begin() + mon2_index_start, v5_.begin() + mon2_index_end, 0.0);
    std::fill(v6_.begin() + mon2_index_start, v6_.begin() + mon2_index_end, 0.0);
    std::fill(v7_.begin() + mon2_index_start, v7_.begin() + mon2_index_end, 0.0);
    std::fill(v8_.begin() + mon2_index_start, v8_.begin() + mon2_index_end, 0.0);
    std::fill(v9_.begin() + mon2_index_start, v9_.begin() + mon2_index_end, 0.0);
    std::fill(v10_.begin() + mon2_index_start, v10_.begin() + mon2_index_end, 0.0);

// Store rijx, rijy and rijz in vectors
#pragma omp simd
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v0_[m] = xyzmon1_x - xyz2[site_jnmon23 + m];           // rijx
        v1_[m] = xyzmon1_y - xyz2[site_jnmon23 + nmon2 + m];   // rijy
        v2_[m] = xyzmon1_z - xyz2[site_jnmon23 + nmon22 + m];  // rijz
    }

    // Apply the minimum image convention via fractional coordinates
    // It is probably a good idea to identify orthorhombic cases and write a faster version for them
    if (use_pbc) {
        // Convert to fractional coordinates
        for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
            v3_[m] = box_inverse[0] * v0_[m] + box_inverse[1] * v1_[m] + box_inverse[2] * v2_[m];
            v4_[m] = box_inverse[3] * v0_[m] + box_inverse[4] * v1_[m] + box_inverse[5] * v2_[m];
            v5_[m] = box_inverse[6] * v0_[m] + box_inverse[7] * v1_[m] + box_inverse[8] * v2_[m];
        }
        // Put in the range 0 to 1
        for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
            v3_[m] -= std::floor(v3_[m] + 0.5);
            v4_[m] -= std::floor(v4_[m] + 0.5);
            v5_[m] -= std::floor(v5_[m] + 0.5);
        }
        // Convert back to cartesian coordinates
        for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
            v0_[m] = box[0] * v3_[m] + box[1] * v4_[m] + box[2] * v5_[m];
            v1_[m] = box[3] * v3_[m] + box[4] * v4_[m] + box[5] * v5_[m];
            v2_[m] = box[6] * v3_[m] + box[7] * v4_[m] + box[8] * v5_[m];
        }
    }

    // Store r2 in vector
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v3_[m] = v0_[m] * v0_[m] + v1_[m] * v1_[m] + v2_[m] * v2_[m];  // r2
    }

    // Store a*(r/A)^4 in vector
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v5_[m] = aCC * v3_[m] * v3_[m] * Asqsqi;  // a*(r/A)^4
    }

    // Convert r2 -> 1/r
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v3_[m] = 1 / std::sqrt(v3_[m]);
    }

    // Cheesy way to apply cutoffs, for now!
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v3_[m] *= (v3_[m] < 1.0 / cutoff ? 0 : 1);
    }

    // Store the attenuated coulomb operator in vector
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v4_[m] = (elec_scale_factor - erf(ewald_alpha / (v3_[m] + 1e-30))) * v3_[m];  // (1-erf(alpha r))/r
    }

    if (!use_pbc) {
        // Rescale v3 to ensure right behavior in no PBC conditions
        for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
            v3_[m] *= elec_scale_factor;
            v4_[m] = v3_[m];
        }
    }

    // Compute gammq and store result in vector. This loop is not vectorizable
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        v6_[m] = gammq(0.75, v5_[m]) * elec_scale_factor;  // gammq
    }

    // Finalize computation of electric field
    const double SQRTPI = sqrt(M_PI);
#pragma omp simd
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        const double exp1 = std::exp(-v5_[m]);
        // Terms needed for the Ewald direct space field, see equation 2.8 of
        // A. Y. Toukmaji, C. Sagui, J. Board and T. A. Darden, J. Chem. Phys., 113 10913 (2000).
        const double exp_alpha2r2 = std::exp(-ewald_alpha * ewald_alpha / (v3_[m] * v3_[m]));
        const double ewaldterm = use_pbc ? 2 * exp_alpha2r2 * ewald_alpha / SQRTPI : 0;

        // Screening functions
        const double s1r = v4_[m] - exp1 * v3_[m];
        const double s0r = constants::COULOMB * (s1r + aCC1_4 * Ai * g34 * v6_[m]);
        const double s1r3 = constants::COULOMB * (s1r + ewaldterm) * v3_[m] * v3_[m];

        // Compute contribution to the field phi
        // Storing the contrib to mon 1 in vector to make it vectorizable

        // Assuming phi will be at1mon1_index at1m2 at1m3 .. for same type of mons
        // phi[finsts1 + site_inmon1 + mon1_index]
        //                           += s0r *chg[finsts2 + site_jnmon2 + m];
        v7_[m] = s0r * chg2[site_jnmon2 + m];
        phi2[site_jnmon2 + m] += s0r * chg1[site_inmon1 + mon1_index];

        // Field will be as xyz xxxxyyyyzzzzat1 xxxxxyyyyzzzz at2...
        const double s1r3ci = s1r3 * chg1[site_inmon1 + mon1_index];
        const double s1r3cj = s1r3 * chg2[site_jnmon2 + m];

        // Compute the three components of the permanent electric field
        // Storing contributions to mon1 in vectors to make the loop vectorizable

        // Efq[fincrd1 + site_inmon13 + mon1_index] += s1r3cj * v0_[m];
        v8_[m] = s1r3cj * v0_[m];
        Efq2[site_jnmon23 + m] -= s1r3ci * v0_[m];

        // Efq[fincrd1 + site_inmon13 + nmon1 + mon1_index] += s1r3cj * v1_[m];
        v9_[m] = s1r3cj * v1_[m];
        Efq2[site_jnmon23 + nmon2 + m] -= s1r3ci * v1_[m];

        // Efq[fincrd1 + site_inmon13 + nmon12 + mon1_index] += s1r3cj * v2_[m];
        v10_[m] = s1r3cj * v2_[m];
        Efq2[site_jnmon23 + nmon22 + m] -= s1r3ci * v2_[m];
    }

    // Add up the contributions to the mon1 site
    *phi1 = 0.0;
    *Efqx_mon1 = 0.0;
    *Efqy_mon1 = 0.0;
    *Efqz_mon1 = 0.0;
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        *phi1 += v7_[m];
        *Efqx_mon1 += v8_[m];
        *Efqy_mon1 += v9_[m];
        *Efqz_mon1 += v10_[m];
    }
}

////////////////////////////////////////////////////////////////////////////////

void ElectricFieldHolder::CalcDipoleElecField(double *xyz1, double *xyz2, double *mu1, double *mu2, size_t mon1_index,
                                              size_t mon2_index_start, size_t mon2_index_end, size_t nmon1,
                                              size_t nmon2, size_t site_i, size_t site_j, double Asqsqi, double aDD,
                                              double *Efd2, double *Efdx_mon1, double *Efdy_mon1, double *Efdz_mon1,
                                              double ewald_alpha, bool use_pbc, const std::vector<double> &box,
                                              const std::vector<double> &box_inverse, double cutofff) {
    // Shifts that will be useful in the loops
    const size_t nmon12 = nmon1 * 2;
    const size_t nmon22 = nmon2 * 2;
    const size_t site_i3 = site_i * 3;
    const size_t site_j3 = site_j * 3;
    const size_t site_inmon13 = nmon1 * site_i3;
    const size_t site_jnmon23 = nmon2 * site_j3;

    // Coordinates x, y and z of site i of monomer 1
    const double xyzmon1_x = xyz1[site_inmon13 + mon1_index];
    const double xyzmon1_y = xyz1[site_inmon13 + nmon1 + mon1_index];
    const double xyzmon1_z = xyz1[site_inmon13 + nmon12 + mon1_index];

    // Fill vectors with zeros in the desired range
    std::fill(v0_.begin() + mon2_index_start, v0_.begin() + mon2_index_end, 0.0);
    std::fill(v1_.begin() + mon2_index_start, v1_.begin() + mon2_index_end, 0.0);
    std::fill(v2_.begin() + mon2_index_start, v2_.begin() + mon2_index_end, 0.0);

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

        // Now build the Ewald generalization of the Coulomb operator and its derivatives, see
        // Toukmaji, Sagui, Board, and Darden, JCP, 113 10913 (2000)
        // particularly equations 2.8 and 2.9.  When alpha is zero these fall out to just be
        // r^-1, r^-3, r^-5
        double r_alpha = ewald_alpha * r;
        double alpha_pi_term = ewald_alpha == 0 ? 0 : 1 / (std::sqrt(M_PI) * ewald_alpha);
        double exp_alpha2_r2 = exp(-r_alpha * r_alpha);
        double two_alpha_squared = 2.0 * ewald_alpha * ewald_alpha;
        double bn0 = erfc(r_alpha) * ri;
        alpha_pi_term *= two_alpha_squared;
        double bn1 = (bn0 + alpha_pi_term * exp_alpha2_r2) * risq;
        alpha_pi_term *= two_alpha_squared;
        double bn2 = (3 * bn1 + alpha_pi_term * exp_alpha2_r2) * risq;

        // Some values that will be used in the screening functions
        const double rA4 = rsqsq * Asqsqi;
        const double arA4 = aDD * rA4;
        // TODO look at the exponential function intel vec
        const double exp1 = std::exp(-arA4);

        // Get screening functions
        const double s1r3 = bn1 - exp1 * ri * risq;
        const double s2r5_3 = bn2 - (3.0 + 4.0 * aDD * rA4) * exp1 * risq * ri * risq;
        const double ts2x = s2r5_3 * rijx;
        const double ts2y = s2r5_3 * rijy;
        const double ts2z = s2r5_3 * rijz;

        // Contributions to the dipole electric field to site i of mon1
        // Stored in vectors to make the loop vectorizable

        // Component x
        // Efd[fincrd1 + site_inmon13 + mon1_index] +=
        v0_[m] = ((ts2x * rijx - s1r3) * mu2[site_jnmon23 + m] + ts2x * rijy * mu2[site_jnmon23 + nmon2 + m] +
                  ts2x * rijz * mu2[site_jnmon23 + nmon22 + m]);

        // Component y
        // Efd[fincrd1 + site_inmon13 + nmon1 + mon1_index] +=
        v1_[m] = (ts2y * rijx * mu2[site_jnmon23 + m] + (ts2y * rijy - s1r3) * mu2[site_jnmon23 + nmon2 + m] +
                  ts2y * rijz * mu2[site_jnmon23 + nmon22 + m]);

        // Component z
        // Efd[fincrd1 + site_inmon13 + nmon12 + mon1_index] +=
        v2_[m] = (ts2z * rijx * mu2[site_jnmon23 + m] + ts2z * rijy * mu2[site_jnmon23 + nmon2 + m] +
                  (ts2z * rijz - s1r3) * mu2[site_jnmon23 + nmon22 + m]);

        // Contributions to the dipole electric field to site j of mon2
        // Component x
        Efd2[site_jnmon23 + m] += ((ts2x * rijx - s1r3) * mu1[site_inmon13 + mon1_index] +
                                   ts2x * rijy * mu1[site_inmon13 + nmon1 + mon1_index] +
                                   ts2x * rijz * mu1[site_inmon13 + nmon12 + mon1_index]);

        // Component y
        Efd2[site_jnmon23 + nmon2 + m] += ((ts2y * rijx) * mu1[site_inmon13 + mon1_index] +
                                           (ts2y * rijy - s1r3) * mu1[site_inmon13 + nmon1 + mon1_index] +
                                           ts2y * rijz * mu1[site_inmon13 + nmon12 + mon1_index]);

        // Component z
        Efd2[site_jnmon23 + nmon22 + m] +=
            ((ts2z * rijx) * mu1[site_inmon13 + mon1_index] + ts2z * rijy * mu1[site_inmon13 + nmon1 + mon1_index] +
             (ts2z * rijz - s1r3) * mu1[site_inmon13 + nmon12 + mon1_index]);
    }

    // Setting the values to the output
    *Efdx_mon1 = 0.0;
    *Efdy_mon1 = 0.0;
    *Efdz_mon1 = 0.0;
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        *Efdx_mon1 += v0_[m];
        *Efdy_mon1 += v1_[m];
        *Efdz_mon1 += v2_[m];
    }
}

////////////////////////////////////////////////////////////////////////////////

void ElectricFieldHolder::CalcElecFieldGrads(double *xyz1, double *xyz2, double *chg1, double *chg2, double *mu1,
                                             double *mu2, size_t mon1_index, size_t mon2_index_start,
                                             size_t mon2_index_end, size_t nmon1, size_t nmon2, size_t site_i,
                                             size_t site_j, double aDD, double aCD, double Asqsqi, double *grdx,
                                             double *grdy, double *grdz, double *phi1, double *phi2, double *grd2,
                                             double ewald_alpha, bool use_pbc, const std::vector<double> &box,
                                             const std::vector<double> &box_inverse, double cutoff) {
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
    std::fill(v0_.begin() + mon2_index_start, v0_.begin() + mon2_index_end, 0.0);
    std::fill(v1_.begin() + mon2_index_start, v1_.begin() + mon2_index_end, 0.0);
    std::fill(v2_.begin() + mon2_index_start, v2_.begin() + mon2_index_end, 0.0);
    std::fill(v3_.begin() + mon2_index_start, v3_.begin() + mon2_index_end, 0.0);

#pragma omp simd
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        // Distances between sites i and j from mon1 and mon2
        const double rijx = xyzmon1_x - xyz2[site_jnmon23 + m];
        const double rijy = xyzmon1_y - xyz2[site_jnmon23 + nmon2 + m];
        const double rijz = xyzmon1_z - xyz2[site_jnmon23 + nmon22 + m];

        const double rijx2 = rijx * rijx;
        const double rijy2 = rijy * rijy;
        const double rijz2 = rijz * rijz;
        const double rsq = rijx2 + rijy2 + rijz2;
        const double r = std::sqrt(rsq);
        const double ri = 1.0 / r;
        const double risq = ri * ri;
        const double rsqsq = rsq * rsq;
        // Some values that will be used in the screening functions
        const double rA4 = rsqsq * Asqsqi;
        // TODO look at the exponential function intel vec

        const double adrA4 = aDD * 4.0 * rA4;
        const double acrA4 = aCD * 4.0 * rA4;
        const double exp1d = std::exp(-aDD * rA4);
        const double exp1c = std::exp(-aCD * rA4);

        // Now build the Ewald generalization of the Coulomb operator and its derivatives, see
        // Toukmaji, Sagui, Board, and Darden, JCP, 113 10913 (2000)
        // particularly equations 2.8 and 2.9.  When alpha is zero these fall out to just be
        // r^-1, r^-3, r^-5
        double r_alpha = ewald_alpha * r;
        double alpha_pi_term = ewald_alpha == 0 ? 0 : 1 / (std::sqrt(M_PI) * ewald_alpha);
        double exp_alpha2_r2 = exp(-r_alpha * r_alpha);
        double two_alpha_squared = 2.0 * ewald_alpha * ewald_alpha;
        double bn0 = erfc(r_alpha) * ri;
        alpha_pi_term *= two_alpha_squared;
        double bn1 = (bn0 + alpha_pi_term * exp_alpha2_r2) * risq;
        alpha_pi_term *= two_alpha_squared;
        double bn2 = (3 * bn1 + alpha_pi_term * exp_alpha2_r2) * risq;
        alpha_pi_term *= two_alpha_squared;
        double bn3 = (5 * bn2 + alpha_pi_term * exp_alpha2_r2) * risq;

        // Get screening functions
        const double s2r5_3d = bn2 - (3.0 + adrA4) * exp1d * risq * ri * risq;
        const double s3r7_15d = bn3 - ((15.0 + 5.0 * adrA4) + adrA4 * (adrA4 - 1.0)) * exp1d * ri * risq * risq * risq;
        const double s3r7_15x2 = s3r7_15d * rijx2;
        const double s3r7_15y2 = s3r7_15d * rijy2;
        const double s3r7_15z2 = s3r7_15d * rijz2;

        const double s1r3c = bn1 - exp1c * ri * risq;
        const double rxs1r3c = rijx * s1r3c;
        const double rys1r3c = rijy * s1r3c;
        const double rzs1r3c = rijz * s1r3c;
        const double s2r5_3c = bn2 - (3.0 + acrA4) * exp1c * ri * risq * risq;

        // Tensors
        const double ts2x = s2r5_3c * rijx;
        const double ts2y = s2r5_3c * rijy;
        const double ts2z = s2r5_3c * rijz;

        // T_alpha_beta_gamma tensor
        const double t3_0 = s3r7_15x2 * rijx - s2r5_3d * 3.0 * rijx;         // x x x
        const double t3_1 = s3r7_15x2 * rijy - s2r5_3d * rijy;               // x x y
        const double t3_2 = s3r7_15x2 * rijz - s2r5_3d * rijz;               // x x z
        const double t3_3 = s3r7_15y2 * rijx - s2r5_3d * rijx;               // x y y
        const double t3_4 = s3r7_15d * rijx * rijy * rijz;                   // x y z
        const double t3_5 = s3r7_15z2 * rijx - s2r5_3d * rijx;               // x z z
        const double t3_6 = s3r7_15d * rijy2 * rijy - s2r5_3d * 3.0 * rijy;  // y y y
        const double t3_7 = s3r7_15y2 * rijz - s2r5_3d * rijz;               // y y z
        const double t3_8 = s3r7_15z2 * rijy - s2r5_3d * rijy;               // y z z
        const double t3_9 = s3r7_15z2 * rijz - s2r5_3d * 3.0 * rijz;         // z z z

        // T_alpha_beta tensor
        const double t2_0 = ts2x * rijx - s1r3c;  // alpha = x, beta = x
        const double t2_1 = ts2x * rijy;
        const double t2_2 = ts2x * rijz;
        const double t2_3 = ts2y * rijy - s1r3c;
        const double t2_4 = ts2y * rijz;
        const double t2_5 = ts2z * rijz - s1r3c;

        // Charge times the dipole component
        const double ci_mjx = chg1[site_inmon1 + mon1_index] * mu2[site_jnmon23 + m];
        const double cj_mix = chg2[site_jnmon2 + m] * mu1[site_inmon13 + mon1_index];
        const double ci_mjy = chg1[site_inmon1 + mon1_index] * mu2[site_jnmon23 + nmon2 + m];
        const double cj_miy = chg2[site_jnmon2 + m] * mu1[site_inmon13 + nmon1 + mon1_index];
        const double ci_mjz = chg1[site_inmon1 + mon1_index] * mu2[site_jnmon23 + nmon22 + m];
        const double cj_miz = chg2[site_jnmon2 + m] * mu1[site_inmon13 + nmon12 + mon1_index];

        // Dipole times dipole
        const double mu1xmu2x = mu1[site_inmon13 + mon1_index] * mu2[site_jnmon23 + m];
        const double mu1xmu2y = mu1[site_inmon13 + mon1_index] * mu2[site_jnmon23 + nmon2 + m];
        const double mu1xmu2z = mu1[site_inmon13 + mon1_index] * mu2[site_jnmon23 + nmon22 + m];
        const double mu1ymu2x = mu1[site_inmon13 + nmon1 + mon1_index] * mu2[site_jnmon23 + m];
        const double mu1ymu2y = mu1[site_inmon13 + nmon1 + mon1_index] * mu2[site_jnmon23 + nmon2 + m];
        const double mu1ymu2z = mu1[site_inmon13 + nmon1 + mon1_index] * mu2[site_jnmon23 + nmon22 + m];
        const double mu1zmu2x = mu1[site_inmon13 + nmon12 + mon1_index] * mu2[site_jnmon23 + m];
        const double mu1zmu2y = mu1[site_inmon13 + nmon12 + mon1_index] * mu2[site_jnmon23 + nmon2 + m];
        const double mu1zmu2z = mu1[site_inmon13 + nmon12 + mon1_index] * mu2[site_jnmon23 + nmon22 + m];

        if (phi2 != 0) {
            // Gradients Charge-Dipole
            // Site site_i
            // Note. Sign changed
            v0_[m] = (cj_mix - ci_mjx) * t2_0     // x x
                     + (cj_miy - ci_mjy) * t2_1   // x y
                     + (cj_miz - ci_mjz) * t2_2;  // x z
            v1_[m] = (cj_mix - ci_mjx) * t2_1     // y x
                     + (cj_miy - ci_mjy) * t2_3   // y y
                     + (cj_miz - ci_mjz) * t2_4;  // y z
            v2_[m] = (cj_mix - ci_mjx) * t2_2     // z x
                     + (cj_miy - ci_mjy) * t2_4   // z y
                     + (cj_miz - ci_mjz) * t2_5;  // z z

            // Site site_j
            grd2[site_jnmon23 + m] -= v0_[m];
            grd2[site_jnmon23 + nmon2 + m] -= v1_[m];
            grd2[site_jnmon23 + nmon22 + m] -= v2_[m];

            // Update field
            // Site site_i
            v3_[m] = rxs1r3c * mu2[site_jnmon23 + m] + rys1r3c * mu2[site_jnmon23 + nmon2 + m] +
                     rzs1r3c * mu2[site_jnmon23 + nmon22 + m];

            // Site site_j
            phi2[site_jnmon2 + m] -=
                (rxs1r3c * mu1[site_inmon13 + mon1_index] + rys1r3c * mu1[site_inmon13 + nmon1 + mon1_index] +
                 rzs1r3c * mu1[site_inmon13 + nmon12 + mon1_index]);
        }
        // Gradients Dipole-Dipole
        const double gx = mu1xmu2x * t3_0     // x x x
                          + mu1xmu2y * t3_1   // x x y
                          + mu1xmu2z * t3_2   // x x z
                          + mu1ymu2x * t3_1   // x y x
                          + mu1ymu2y * t3_3   // x y y
                          + mu1ymu2z * t3_4   // x y z
                          + mu1zmu2x * t3_2   // x z x
                          + mu1zmu2y * t3_4   // x z y
                          + mu1zmu2z * t3_5;  // x z z

        const double gy = mu1xmu2x * t3_1     // y x x
                          + mu1xmu2y * t3_3   // y x y
                          + mu1xmu2z * t3_4   // y x z
                          + mu1ymu2x * t3_3   // y y x
                          + mu1ymu2y * t3_6   // y y y
                          + mu1ymu2z * t3_7   // y y z
                          + mu1zmu2x * t3_4   // y z x
                          + mu1zmu2y * t3_7   // y z y
                          + mu1zmu2z * t3_8;  // y z z

        const double gz = mu1xmu2x * t3_2     // z x x
                          + mu1xmu2y * t3_4   // z x y
                          + mu1xmu2z * t3_5   // z x z
                          + mu1ymu2x * t3_4   // z y x
                          + mu1ymu2y * t3_7   // z y y
                          + mu1ymu2z * t3_8   // z y z
                          + mu1zmu2x * t3_5   // z z x
                          + mu1zmu2y * t3_8   // z z y
                          + mu1zmu2z * t3_9;  // z z z
        // Site site_i
        v0_[m] += gx;
        v1_[m] += gy;
        v2_[m] += gz;

        // Site site_j
        grd2[site_jnmon23 + m] -= gx;
        grd2[site_jnmon23 + nmon2 + m] -= gy;
        grd2[site_jnmon23 + nmon22 + m] -= gz;
    }

    // Compress vectors to double
    *grdx = 0.0;
    *grdy = 0.0;
    *grdz = 0.0;
    *phi1 = 0.0;
    for (size_t m = mon2_index_start; m < mon2_index_end; m++) {
        *grdx += v0_[m];
        *grdy += v1_[m];
        *grdz += v2_[m];
        *phi1 += v3_[m];
    }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace elec

////////////////////////////////////////////////////////////////////////////////

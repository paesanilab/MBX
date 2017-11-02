#include "fields.h"

namespace elec {

Field::Field(size_t n) {
  maxnmon = n;
  v0_  = std::vector<double> (maxnmon);
  v1_  = std::vector<double> (maxnmon);
  v2_  = std::vector<double> (maxnmon);
  v3_  = std::vector<double> (maxnmon);
  v4_  = std::vector<double> (maxnmon);
  v5_  = std::vector<double> (maxnmon);
  v6_  = std::vector<double> (maxnmon);
  v7_  = std::vector<double> (maxnmon);
  v8_  = std::vector<double> (maxnmon);
  v9_  = std::vector<double> (maxnmon);
  v10_ = std::vector<double> (maxnmon);
  v11_ = std::vector<double> (maxnmon);
  v12_ = std::vector<double> (maxnmon);
  v13_ = std::vector<double> (maxnmon);
  v14_ = std::vector<double> (maxnmon);
  v15_ = std::vector<double> (maxnmon);
}

void Field::DoEfqWA(double * xyz1, double * xyz2, 
                    double * chg1, double * chg2,
                    size_t m1, 
                    size_t m2in, size_t m2fi,
                    size_t nmon1, size_t nmon2,
                    size_t i, size_t j,
                    double Ai, double Asqsq,
                    double aCC, double aCC1_4,
                    double g34, 
                    double &ex, double &ey, 
                    double &ez, double &phi1,
                    double * phi2,
                    double * Efq2) {
             
  size_t nmon12 = nmon1 * 2;
  size_t nmon22 = nmon2 * 2;
  size_t i3 = i * 3;
  size_t j3 = j * 3;
  size_t inmon1 = i * nmon1;
  size_t jnmon2 = j * nmon2;
  size_t inmon13 = nmon1 * i3;
  size_t jnmon23 = nmon2 * j3;

  std::fill(v0_.begin() + m2in, v0_.begin() + m2fi, 0.0);
  std::fill(v1_.begin() + m2in, v1_.begin() + m2fi, 0.0);
  std::fill(v2_.begin() + m2in, v2_.begin() + m2fi, 0.0);
  std::fill(v3_.begin() + m2in, v3_.begin() + m2fi, 0.0);
  std::fill(v4_.begin() + m2in, v4_.begin() + m2fi, 0.0);
  std::fill(v5_.begin() + m2in, v5_.begin() + m2fi, 0.0);
  std::fill(v6_.begin() + m2in, v6_.begin() + m2fi, 0.0);
  std::fill(v7_.begin() + m2in, v7_.begin() + m2fi, 0.0);
  std::fill(v8_.begin() + m2in, v8_.begin() + m2fi, 0.0);
  std::fill(v9_.begin() + m2in, v9_.begin() + m2fi, 0.0);
  std::fill(v10_.begin() + m2in, v10_.begin() + m2fi, 0.0);

  # pragma ivdep
  for (size_t m = m2in; m < m2fi; m++) {
    v0_[m] = xyz1[inmon13 + m1]
           - xyz2[jnmon23 + m];                     // rijx
    v1_[m] = xyz1[inmon13 + nmon1 + m1]
           - xyz2[jnmon23 + nmon2 + m];             // rijy
    v2_[m] = xyz1[inmon13 + nmon12 + m1]
           - xyz2[jnmon23 + nmon22 + m];            // rijz
  }
  for (size_t m = m2in; m < m2fi; m++) {
    v3_[m] = v0_[m]*v0_[m] + v1_[m]*v1_[m] + v2_[m]*v2_[m];  // rsq
  }
  for (size_t m = m2in; m < m2fi; m++) {
    v4_[m] = 1.0 / std::sqrt(v3_[m]);                        // 1/r
    
    v5_[m] = aCC * v3_[m] * v3_[m] / Asqsq;                  // a*(r/A)^4
  }
  for (size_t m = m2in; m < m2fi; m++) {
    v6_[m] = gammq(0.75, v5_[m]);                            // gammq
  }
  #pragma ivdep
  for (size_t m = m2in; m < m2fi; m++) {
    const double exp1 = std::exp(-v5_[m]);
    const double s1r = v4_[m] - exp1 * v4_[m];
    const double s0r = s1r + aCC1_4 * Ai * g34 * v6_[m];
    const double s1r3 = s1r * v4_[m] * v4_[m];

    // Assuming phi will be at1m1 at1m2 at1m3 .. for same type of mons
    //phi[finsts1 + inmon1 + m1] += s0r *chg[finsts2 + jnmon2 + m];
    v7_[m] = s0r *chg2[jnmon2 + m];
    phi2[jnmon2 + m] += s0r *chg1[inmon1 + m1];

    // Field will be as xyz xxxxyyyyzzzzat1 xxxxxyyyyzzzz at2...
    const double s1r3ci = s1r3 * chg1[inmon1 + m1];
    const double s1r3cj = s1r3 * chg2[jnmon2 + m];
    //Efq[fincrd1 + inmon13 + m1] += s1r3cj * v0_[m];
    v8_[m] = s1r3cj * v0_[m];
    Efq2[jnmon23 + m] -= s1r3ci * v0_[m];
    //Efq[fincrd1 + inmon13 + nmon1 + m1] += s1r3cj * v1_[m];
    v9_[m] = s1r3cj * v1_[m];
    Efq2[jnmon23 + nmon2 + m] -= s1r3ci * v1_[m];
    //Efq[fincrd1 + inmon13 + nmon12 + m1] += s1r3cj * v2_[m];
    v10_[m] = s1r3cj * v2_[m];
    Efq2[jnmon23 + nmon22 + m] -= s1r3ci * v2_[m];
  }
  phi1 = 0.0;
  ex = 0.0;
  ey = 0.0;
  ez = 0.0;
  for (size_t m = m2in; m < m2fi; m++) {
    phi1 += v7_[m];
    ex += v8_[m];
    ey += v9_[m];
    ez += v10_[m];
  }
}

void Field::DoEfqWA(double * xyz1, double * xyz2,
                    double * chg1, double * chg2,
                    size_t m1,
                    size_t m2in, size_t m2fi,
                    size_t nmon1, size_t nmon2,
                    size_t i, size_t j,
                    double &ex, double &ey,
                    double &ez, double &phi1,
                    double * phi2,
                    double * Efq2) {

  size_t nmon12 = nmon1 * 2;
  size_t nmon22 = nmon2 * 2;
  size_t i3 = i * 3;
  size_t j3 = j * 3;
  size_t inmon1 = i * nmon1;
  size_t jnmon2 = j * nmon2;
  size_t inmon13 = nmon1 * i3;
  size_t jnmon23 = nmon2 * j3;

  std::fill(v0_.begin() + m2in, v0_.begin() + m2fi, 0.0);
  std::fill(v1_.begin() + m2in, v1_.begin() + m2fi, 0.0);
  std::fill(v2_.begin() + m2in, v2_.begin() + m2fi, 0.0);
  std::fill(v3_.begin() + m2in, v3_.begin() + m2fi, 0.0);

  #pragma ivdep
  for (size_t m = m2in; m < m2fi; m++) {
    const double rijx  = xyz1[inmon13 + m1]
                       - xyz2[jnmon23 + m];             // rijx
    const double rijy  = xyz1[inmon13 + nmon1 + m1]
                       - xyz2[jnmon23 + nmon2 + m];     // rijy
    const double rijz  = xyz1[inmon13 + nmon12 + m1]
                       - xyz2[jnmon23 + nmon22 + m];    // rijz

    const double risq = rijx * rijx + rijy * rijy + rijz * rijz;  // rsq
    const double ri = 1.0 / std::sqrt(risq);                      // 1/r

    const double s0r = ri;
    const double s1r3 = ri * risq;

    // Assuming phi will be at1m1 at1m2 at1m3 .. for same type of mons
    //phi[finsts1 + inmon1 + m1] += s0r *chg[finsts2 + jnmon2 + m];
    v0_[m] = s0r *chg2[jnmon2 + m];
    phi2[jnmon2 + m] += s0r *chg1[inmon1 + m1];

    // Field will be as xyz xxxxyyyyzzzzat1 xxxxxyyyyzzzz at2...
    const double s1r3ci = s1r3 * chg1[inmon1 + m1];
    const double s1r3cj = s1r3 * chg2[jnmon2 + m];
    //Efq[fincrd1 + inmon13 + m1] += s1r3cj * rijx;
    v1_[m] = s1r3cj * rijx;
    Efq2[jnmon23 + m] -= s1r3ci * rijx;
    //Efq[fincrd1 + inmon13 + nmon1 + m1] += s1r3cj * rijy;
    v2_[m] = s1r3cj * rijy;
    Efq2[jnmon23 + nmon2 + m] -= s1r3ci * rijy;
    //Efq[fincrd1 + inmon13 + nmon12 + m1] += s1r3cj * rijz;
    v3_[m] = s1r3cj * rijz;
    Efq2[jnmon23 + nmon22 + m] -= s1r3ci * rijz;
  }
  phi1 = 0.0;
  ex = 0.0;
  ey = 0.0;
  ez = 0.0;
  for (size_t m = m2in; m < m2fi; m++) {
    phi1 += v0_[m];
    ex += v1_[m];
    ey += v2_[m];
    ez += v3_[m];
  }
}

void Field::DoEfdWA( double * xyz1, double * xyz2,
                     double * mu1, double * mu2,
                     size_t m1,
                     size_t m2in, size_t m2fi,
                     size_t nmon1, size_t nmon2,
                     size_t i, size_t j,
                     double Asqsq,
                     double aDD, double * Efd2,
                     double &Efdx, double &Efdy, double &Efdz) {

  size_t nmon12 = nmon1 * 2;
  size_t nmon22 = nmon2 * 2;
  size_t i3 = i * 3;
  size_t j3 = j * 3;
  size_t inmon1 = i * nmon1;
  size_t jnmon2 = j * nmon2;
  size_t inmon13 = nmon1 * i3;
  size_t jnmon23 = nmon2 * j3;

  std::fill(v0_.begin() + m2in, v0_.begin() + m2fi, 0.0);
  std::fill(v1_.begin() + m2in, v1_.begin() + m2fi, 0.0);
  std::fill(v2_.begin() + m2in, v2_.begin() + m2fi, 0.0);

  #pragma ivdep
  for (size_t m = m2in; m < m2fi; m++) {
    const double rijx = xyz1[inmon13 + m1]
                      - xyz2[jnmon23 + m];
    const double rijy = xyz1[inmon13 + m1 + nmon1]
                      - xyz2[jnmon23 + nmon2 + m];
    const double rijz = xyz1[inmon13 + m1 + nmon12]
                      - xyz2[jnmon23 + nmon22 + m];

    const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
    const double r = std::sqrt(rsq);
    const double ri = 1.0/r;
    const double risq = ri*ri;
    const double rsqsq = rsq * rsq;

    // Some values that will be used in the screening functions
    const double rA4 = rsqsq/Asqsq;
    const double arA4 = aDD*rA4;
    // TODO look at the exponential function intel vec
    const double exp1 = std::exp(-arA4);

    // Get screening functions
    const double s1r = ri - exp1*ri;
    const double s1r3 = s1r * risq;
    const double s2r5_3 = (3.0*s1r3
                          -4.0*aDD*rA4*exp1*risq*ri)*risq;
    const double ts2x = s2r5_3 * rijx;
    const double ts2y = s2r5_3 * rijy;
    const double ts2z = s2r5_3 * rijz;

    //Efd[fincrd1 + inmon13 + m1] += 
    v0_[m] = 
              ((ts2x * rijx - s1r3) * mu2[jnmon23 + m] +
                ts2x * rijy * mu2[jnmon23 + nmon2 + m] +
                ts2x * rijz * mu2[jnmon23 + nmon22 + m]);
    //Efd[fincrd1 + inmon13 + nmon1 + m1] += 
    v1_[m] = 
              ( ts2y * rijx * mu2[jnmon23 + m] +
               (ts2y * rijy - s1r3) * mu2[jnmon23 + nmon2 + m] +
                ts2y * rijz * mu2[jnmon23 + nmon22 + m]);
    //Efd[fincrd1 + inmon13 + nmon12 + m1] +=
    v2_[m] =
              ( ts2z * rijx * mu2[jnmon23 + m] +
                ts2z * rijy * mu2[jnmon23 + nmon2 + m] +
               (ts2z * rijz - s1r3) * mu2[jnmon23 + nmon22 + m]);

    Efd2[jnmon23 + m] +=
              ((ts2x * rijx - s1r3) * mu1[inmon13 + m1] +
               ts2x * rijy * mu1[inmon13 + nmon1 + m1] +
               ts2x * rijz * mu1[inmon13 + nmon12 + m1]);

    Efd2[jnmon23 + nmon2 + m] +=
              ((ts2y * rijx) * mu1[inmon13 + m1] +
               (ts2y * rijy - s1r3) * mu1[inmon13 + nmon1 + m1] +
               ts2y * rijz * mu1[inmon13 + nmon12 + m1]);

    Efd2[jnmon23 + nmon22 + m] +=
              ((ts2z * rijx) * mu1[inmon13 + m1] +
               ts2z * rijy * mu1[inmon13 + nmon1 + m1] +
               (ts2z * rijz - s1r3) * mu1[inmon13 + nmon12 + m1]);

  }

  // Setting the values to the output
  Efdx = 0.0;
  Efdy = 0.0;
  Efdz = 0.0;
  for (size_t m = m2in; m < m2fi; m++) {
    Efdx += v0_[m];
    Efdy += v1_[m];
    Efdz += v2_[m];
  }
}

void Field::DoEfdWoA(double * xyz1, double * xyz2,
                     double * mu1, double * mu2,
                     size_t m1,
                     size_t m2in, size_t m2fi,
                     size_t nmon1, size_t nmon2,
                     size_t i, size_t j, double * Efd2,
                     double &Efdx, double &Efdy, double &Efdz) {

  size_t nmon12 = nmon1 * 2;
  size_t nmon22 = nmon2 * 2;
  size_t i3 = i * 3;
  size_t j3 = j * 3;
  size_t inmon1 = i * nmon1;
  size_t jnmon2 = j * nmon2;
  size_t inmon13 = nmon1 * i3;
  size_t jnmon23 = nmon2 * j3;

  std::fill(v0_.begin() + m2in, v0_.begin() + m2fi, 0.0);
  std::fill(v1_.begin() + m2in, v1_.begin() + m2fi, 0.0);
  std::fill(v2_.begin() + m2in, v2_.begin() + m2fi, 0.0);

  #pragma ivdep
  for (size_t m = m2in; m < m2fi; m++) {
    const double rijx = xyz1[inmon13 + m1]
                      - xyz2[jnmon23 + m];
    const double rijy = xyz1[inmon13 + m1 + nmon1]
                      - xyz2[jnmon23 + m + nmon2];
    const double rijz = xyz1[inmon13 + m1 + nmon12]
                      - xyz2[jnmon23 + m + nmon22];

    const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
    const double r = std::sqrt(rsq);
    const double ri = 1.0/r;
    const double risq = ri*ri;

    // Some values that will be used in the screening functions

    // Get screening functions
    const double s1r = ri;
    const double s1r3 = s1r * risq;
    const double s2r5_3 = 3.0*s1r3*risq;
                          
    const double ts2x = s2r5_3 * rijx;
    const double ts2y = s2r5_3 * rijy;
    const double ts2z = s2r5_3 * rijz;

    //Efd[fincrd1 + inmon13 + m1] += 
    v0_[m] =
              ((ts2x * rijx - s1r3) * mu2[jnmon23 + m] +
                ts2x * rijy * mu2[jnmon23 + nmon2 + m] +
                ts2x * rijz * mu2[jnmon23 + nmon22 + m]);
    //Efd[fincrd1 + inmon13 + nmon1 + m1] += 
    v1_[m] =
              ( ts2y * rijx * mu2[jnmon23 + m] +
               (ts2y * rijy - s1r3) * mu2[jnmon23 + nmon2 + m] +
                ts2y * rijz * mu2[jnmon23 + nmon22 + m]);
    //Efd[fincrd1 + inmon13 + nmon12 + m1] +=
    v2_[m] =
              ( ts2z * rijx * mu2[jnmon23 + m] +
                ts2z * rijy * mu2[jnmon23 + nmon2 + m] +
               (ts2z * rijz - s1r3) * mu2[jnmon23 + nmon22 + m]);
    
    Efd2[jnmon23 + m] +=
              ((ts2x * rijx - s1r3) * mu1[inmon13 + m1] +
               ts2x * rijy * mu1[inmon13 + nmon1 + m1] +
               ts2x * rijz * mu1[inmon13 + nmon12 + m1]);

    Efd2[jnmon23 + nmon2 + m] +=
              ((ts2y * rijx) * mu1[inmon13 + m1] +
               (ts2y * rijy - s1r3) * mu1[inmon13 + nmon1 + m1] +
               ts2y * rijz * mu1[inmon13 + nmon12 + m1]);

    Efd2[jnmon23 + nmon22 + m] +=
              ((ts2z * rijx) * mu1[inmon13 + m1] +
               ts2z * rijy * mu1[inmon13 + nmon1 + m1] +
               (ts2z * rijz - s1r3) * mu1[inmon13 + nmon12 + m1]);
  }

  // Setting the values to the output
  Efdx = 0.0;
  Efdy = 0.0;
  Efdz = 0.0;
  for (size_t m = m2in; m < m2fi; m++) {
    Efdx += v0_[m];
    Efdy += v1_[m];
    Efdz += v2_[m];
  }
  
}

} // namespace elec







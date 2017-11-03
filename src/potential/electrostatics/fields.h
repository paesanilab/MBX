#ifndef FIELDS_H
#define FIELDS_H

#include <vector>
#include <cmath>
#include <cstddef>

#include "potential/electrostatics/gammq.h"

namespace elec {

////////////////////////////////////////////////////////////////////////////////

class Field {
 public:
  Field() {maxnmon = 0;}
  Field(size_t n);
  ~Field() {}

////////////////////////////////////////////////////////////////////////////////

  void DoEfqWA(
    double * xyz1, double * xyz2,   // xyz all sites ordered
    double * chg1, double * chg2,   // charges all sites ordered
    size_t m1,                      // mon 1 index
    size_t m2in, size_t m2fi,       // initial and final index m2
    size_t nmon1, size_t nmon2,     // Number of monomers 1 and 2
    size_t i, size_t j,             // Site i and site j
    double Ai, double Asqsq,
    double aCC, double aCC1_4,
    double g34,
    double &ex, double &ey,
    double &ez, double &phi1,
    double * phi2,
    double * Efq2);

////////////////////////////////////////////////////////////////////////////////

  void DoEfqWoA(
    double * xyz1, double * xyz2,
    double * chg1, double * chg2,
    size_t m1,
    size_t m2in, size_t m2fi,
    size_t nmon1, size_t nmon2,
    size_t i, size_t j,
    double &ex, double &ey,
    double &ez, double &phi1,
    double * phi2,
    double * Efq2);

////////////////////////////////////////////////////////////////////////////////

  void DoEfdWA(
    double * xyz1, double * xyz2,
    double * mu1, double * mu2,
    size_t m1,
    size_t m2in, size_t m2fi,
    size_t nmon1, size_t nmon2,
    size_t i, size_t j,
    double Asqsq,
    double aDD, double * Efd2,
    double &Efdx, double &Efdy, double &Efdz);

////////////////////////////////////////////////////////////////////////////////

  void DoEfdWoA(
    double * xyz1, double * xyz2,
    double * mu1, double * mu2,
    size_t m1,
    size_t m2in, size_t m2fi,
    size_t nmon1, size_t nmon2,
    size_t i, size_t j, double * Efd2,
    double &Efdx, double &Efdy, double &Efdz);

////////////////////////////////////////////////////////////////////////////////

  void DoGrdWA(
    double * xyz1, double * xyz2,
    double * chg1, double * chg2,
    double * mu1, double * mu2,
    size_t m1,
    size_t m2in, size_t m2fi,
    size_t nmon1, size_t nmon2,
    size_t i, size_t j,
    double aDD, double aCD, double Asqsq,
    double &grdx, double &grdy, double &grdz,
    double &phi1, double * phi2,
    double * grd2);

////////////////////////////////////////////////////////////////////////////////

  void DoGrdWoA(
    double * xyz1, double * xyz2,
    double * chg1, double * chg2,
    double * mu1, double * mu2,
    size_t m1,
    size_t m2in, size_t m2fi,
    size_t nmon1, size_t nmon2,
    size_t i, size_t j,
    double &grdx, double &grdy, double &grdz,
    double &phi1, double * phi2,
    double * grd2);

////////////////////////////////////////////////////////////////////////////////

 private:
  // Vectors that will be reused:
  size_t maxnmon;
  std::vector<double> v0_;
  std::vector<double> v1_;
  std::vector<double> v2_;
  std::vector<double> v3_;
  std::vector<double> v4_;
  std::vector<double> v5_;
  std::vector<double> v6_;
  std::vector<double> v7_;
  std::vector<double> v8_;
  std::vector<double> v9_;
  std::vector<double> v10_;
};

} // namespace elec
#endif

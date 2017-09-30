#ifndef DISPERSION_H
#define DISPERSION_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>

template <int N>
inline int factorial()
{
    return N*factorial<N-1>();
}

template<>
inline int factorial<0>()
{
    return 1;
}

//----------------------------------------------------------------------------//

const double if6 = 1.0/x2o::factorial<6>();

//----------------------------------------------------------------------------//

double tang_toennies(int n, const double& x) {

  assert(n >= 0);
  int nn = n;
  double sum = 1.0 + x/nn;

  while (--nn != 0)
      sum = 1.0 + sum*x/nn;

  double tt = 1.0 - sum*std::exp(-x);

  if (std::fabs(tt) < 1.0e-8) {

    double term(1);
    for (nn = n; nn != 0; --nn)
      term *= x/nn;

    sum = 0.0;
    for (nn = n + 1; nn < 1000; ++nn) {
      term *= x/nn;
      sum += term;

      if (std::fabs(term/sum) < 1.0e-8)
        break;
    }

    tt = sum*std::exp(-x);
  }

  return tt;
}

//----------------------------------------------------------------------------//

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
  const double tt6 = x2o::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = x2o::tang_toennies(8, d8r);


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
  const double tt6 = x2o::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = x2o::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  return - (e6 + e8);
}

//----------------------------------------------------------------------------//

double disp6(const double& C6, const double& d6,
             const double* p1, const double* p2) {

  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double tt6 = x2o::tang_toennies(6, d6*r);

  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;

  return - C6*tt6*inv_r6;
}

//----------------------------------------------------------------------------//


double disp6(const double& C6, const double& d6,
              const double* p1, const double* p2,
                    double* g1,       double* g2) {
  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = x2o::tang_toennies(6, d6r);

  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;

  const double e6 = C6*tt6*inv_r6;

  const double grd = 6*e6*inv_rsq - C6*std::pow(d6, 7)*if6*std::exp(-d6r)/r;

  g1[0] += dx*grd;
  g2[0] -= dx*grd;

  g1[1] += dy*grd;
  g2[1] -= dy*grd;

  g1[2] += dz*grd;
  g2[2] -= dz*grd;

  return - e6;
}

//----------------------------------------------------------------------------//
#endif // DISPERSION_H
//----------------------------------------------------------------------------//

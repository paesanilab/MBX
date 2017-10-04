#include "potential/dispersion/disptools.h"

namespace disp {

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
  
  //----------------------------------------------------------------------------//
  
  double disp6(const double& C6, const double& d6,
               const double* p1a, const double* p2a, size_t n) {
  
    // p1a and p2a are an array of coordinates of the atoms involved xyzxyzxyz...
    // rearrange them in xxxx...yyyy...zzzz...
    double p1[3*n], p2[3*n];
    for (size_t i = 0; i < n; i++) {
      p1[i] = p1a[3*i];
      p1[i + n] = p1a[3*i + 1];
      p1[i + 2*n] = p1a[3*i + 2];

      p2[i] = p2a[3*i];
      p2[i + n] = p2a[3*i + 1];
      p2[i + 2*n] = p2a[3*i + 2];
    }

    // Main loop
    size_t n2 = 2*n;
    double disp = 0.0;
//    #pragma simd
    for (size_t nv = 0; nv < n; nv++) {
      const double dx = p1[nv] - p2[nv];
      const double dy = p1[nv + n] - p2[nv + n];
      const double dz = p1[nv + n2] - p2[nv + n2];
  
      const double rsq = dx*dx + dy*dy + dz*dz;
      const double r = std::sqrt(rsq);
  
      const double tt6 = disp::tang_toennies(6, d6*r);
  
      const double inv_rsq = 1.0/rsq;
      const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
      disp += C6*tt6*inv_r6;
    }
  
    return -disp;

    // TODO shift values
//    double p1[3*n], p2[3*n];
//    for (size_t i = 0; i < n; i++) {
//      p1[i] = p1a[3*i];
//      p1[i + n] = p1a[3*i + 1];
//      p1[i + 2*n] = p1a[3*i + 2];
//
//      p2[i] = p2a[3*i];
//      p2[i + n] = p2a[3*i + 1];
//      p2[i + 2*n] = p2a[3*i + 2];
//    }
//
//    // Main loop
//    size_t n2 = 2*n;
//    double r[n];
//    double tt6[n];
//    double inv_r6[n];
//    double disp = 0.0;
//    for (size_t nv = 0; nv < n; nv++) {
//      const double dx = p1[nv] - p2[nv];
//      const double dy = p1[nv + n] - p2[nv + n];
//      const double dz = p1[nv + n2] - p2[nv + n2];
//
//      const double rsq = dx*dx + dy*dy + dz*dz;
//      r[nv] = std::sqrt(rsq);
//
//      tt6[nv] = disp::tang_toennies(6, d6*r[nv]);
//
//      const double inv_rsq = 1.0/rsq;
//      inv_r6[nv] = inv_rsq*inv_rsq*inv_rsq;
//    }
//    
//    // TODO shift values
//    for (size_t nv = 0; nv < n; nv++) {
//      disp += C6*tt6[nv]*inv_r6[nv];
//    }
//    return -disp;
  }
  
  //----------------------------------------------------------------------------//
  
  
  double disp6(const double& C6, const double& d6,
               const double* p1a, const double* p2a,
                     double* g1a,       double* g2a, size_t n) {
    double p1[3*n], p2[3*n];
    for (size_t i = 0; i < n; i++) {
      p1[i] = p1a[3*i];
      p1[i + n] = p1a[3*i + 1];
      p1[i + 2*n] = p1a[3*i + 2];

      p2[i] = p2a[3*i];
      p2[i + n] = p2a[3*i + 1];
      p2[i + 2*n] = p2a[3*i + 2];
    }
    
    size_t n2 = 2*n;
    double disp = 0.0;
    double g1[3*n], g2[3*n];
    std::fill(g1, g1 + 3*n, 0.0);
    std::fill(g2, g2 + 3*n, 0.0);
//    #pragma simd
    for (size_t nv = 0; nv < n; nv++) {
      const double dx = p1[nv] - p2[nv];
      const double dy = p1[nv + n] - p2[nv + n];
      const double dz = p1[nv + n2] - p2[nv + n2];
 
      const double rsq = dx*dx + dy*dy + dz*dz;
      const double r = std::sqrt(rsq);
    
      const double d6r = d6*r;
      const double tt6 = disp::tang_toennies(6, d6r);
    
      const double inv_rsq = 1.0/rsq;
      const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
    
      const double e6 = C6*tt6*inv_r6;
    
      const double grd = 6*e6*inv_rsq - C6*std::pow(d6, 7)*if6*std::exp(-d6r)/r;
    
      g1[nv] += dx*grd;
      g2[nv] -= dx*grd;
    
      g1[nv + n] += dy*grd;
      g2[nv + n] -= dy*grd;
    
      g1[nv + n2] += dz*grd;
      g2[nv + n2] -= dz*grd;
    
      disp -= e6;
    }

    for (size_t i = 0; i < n; i++) {
      g1a[3*i] += g1[i];
      g1a[3*i + 1] += g1[i + n];
      g1a[3*i + 2] += g1[i + n2];
  
      g2a[3*i] += g2[i];
      g2a[3*i + 1] += g2[i + n];
      g2a[3*i + 2] += g2[i + n2];
    }
    return disp;
  }
} // namespace disp

//----------------------------------------------------------------------------//

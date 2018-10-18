#ifndef DISPTOOLS_H
#define DISPTOOLS_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace disp {

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
  
  const double if6 = 1.0/disp::factorial<6>();
  const double if8 = 1.0/disp::factorial<8>();
  
  //----------------------------------------------------------------------------//
  
  double tang_toennies(int n, const double& x);
  
//  double disp68(const double& C6, const double& d6,
//                const double& C8, const double& d8,
//                const double* p1, const double* p2,
//                      double* g1,       double* g2, bool do_grads); 
//  double disp68(const double& C6, const double& d6,
//                const double& C8, const double& d8,
//                const double* p1, const double* p2);
  
  double disp6(const double& C6, const double& d6,
               const double* p1a, const double* p2a, size_t n,
               const double cutoff, const bool use_cutoff);
  double disp6(const double& C6, const double& d6,
               const double* p1a, const double* p2a,
                     double* g1a,       double* g2a, size_t n,
               const double cutoff, const bool use_cutoff);
} // namespace disp

//----------------------------------------------------------------------------//
#endif // DISPERSION_H
//----------------------------------------------------------------------------//

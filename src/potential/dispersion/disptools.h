#ifndef DISPTOOLS_NEW_H
#define DISPTOOLS_NEW_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace disp {

template <int N>
inline int factorial() {
    return N * factorial<N - 1>();
}

template <>
inline int factorial<0>() {
    return 1;
}

//----------------------------------------------------------------------------//

const double if6 = 1.0 / disp::factorial<6>();
const double if8 = 1.0 / disp::factorial<8>();

//----------------------------------------------------------------------------//

double tang_toennies(int n, const double& x);

//  double disp68(const double& C6, const double& d6,
//                const double& C8, const double& d8,
//                const double* p1, const double* p2,
//                      double* g1,       double* g2, bool do_grads);
//  double disp68(const double& C6, const double& d6,
//                const double& C8, const double& d8,
//                const double* p1, const double* p2);

double disp6(const double C6, const double d6, const double* p1, const double* xyz2, double* grad1, double* grad2,
             const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2, const size_t atom_index1,
             const size_t atom_index2, const double disp_scale_factor, bool do_grads, const double cutoff,
             const std::vector<double>& box, const std::vector<double>& box_inverse);

void GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_c6, double& out_d6);

}  // namespace disp

//----------------------------------------------------------------------------//
#endif  // DISPERSION_H
//----------------------------------------------------------------------------//

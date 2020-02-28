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

#ifndef DISPTOOLS_NEW_H
#define DISPTOOLS_NEW_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>

#include "tools/math_tools.h"

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

double disp6(const double C6, const double d6, const double c6i, const double c6j, const double* p1, const double* xyz2,
             double* grad1, double* grad2, double& phi1, double* phi2, const size_t nmon1, const size_t nmon2,
             const size_t start2, const size_t end2, const size_t atom_index1, const size_t atom_index2,
             const double disp_scale_factor, bool do_grads, const double cutoff, const double ewald_alpha,
             const std::vector<double>& box, const std::vector<double>& box_inverse,
	     bool use_ghost, const std::vector<size_t>& islocal, const size_t isl1_offset, const size_t isl2_offset,
	     std::vector<double> *virial=0);

void GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_c6, double& out_d6);

}  // namespace disp

//----------------------------------------------------------------------------//
#endif  // DISPERSION_H
//----------------------------------------------------------------------------//

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

#ifndef X2B_V9X_H
#define X2B_V9X_H

#include <vector>
#include <cmath>
#include <algorithm>

#include "potential/2b/poly-2b-v6x.h"

////////////////////////////////////////////////////////////////////////////////

namespace x2o {

//----------------------------------------------------------------------------//

//
//  x2b_v9<4> with gradients and no fitting interface
//

//----------------------------------------------------------------------------//

struct x2b_v9x {
    static double eval(const double* w1, const double* w2, const size_t nd);

    static double eval(const double* w1, const double* w2, double* g1, double* g2, const size_t nd,
                       std::vector<double>* virial = 0);

    static const double k_HH_intra;
    static const double k_OH_intra;

    static const double k_HH_coul;
    static const double k_OH_coul;
    static const double k_OO_coul;

    static const double k_XH_main;
    static const double k_XO_main;
    static const double k_XX_main;

    static const double in_plane_gamma;
    static const double out_of_plane_gamma;

    static const double r2i;
    static const double r2f;

    static double f_switch(const double&, double&);  // O-O separation
};

//----------------------------------------------------------------------------//

}  // namespace x2o

////////////////////////////////////////////////////////////////////////////////

#endif  // X2B_V9X_H

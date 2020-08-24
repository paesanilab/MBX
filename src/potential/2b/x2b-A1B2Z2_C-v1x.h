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

#ifndef X2B_A1B2Z2_C_V1X_H
#define X2B_A1B2Z2_C_V1X_H

////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <string>
#include "poly-2b-A1B2Z2_C-v1x.h"

namespace h2o_ion {

//----------------------------------------------------------------------------//

//
//  x2b_h2o_ion_v1<4> with gradients and no fitting interface
//

//----------------------------------------------------------------------------//

struct x2b_h2o_ion_v2x {
    // TODO think what to do with default constructor. Error?
    x2b_h2o_ion_v2x(){};
    x2b_h2o_ion_v2x(std::string mon1, std::string mon2);

    ~x2b_h2o_ion_v2x(){};

    double eval(const double* w1, const double* x, double* g1, double* g2, const size_t nd,
                std::vector<double>* virial = 0);
    double eval(const double* w1, const double* x, const size_t nd);

    double k_HH_intra;
    double k_OH_intra;

    double k_XH_coul;
    double k_XO_coul;

    double k_xlp_main;

    double d_HH_intra;
    double d_OH_intra;

    double d_XH_coul;
    double d_XO_coul;

    double d_xlp_main;

    double in_plane_gamma;
    double out_of_plane_gamma;

    double r2i;
    double r2f;

    // ADDED MRR

    std::vector<double> twobodyfit;

    // END ADDED MRR

    double f_switch(const double&, double&);  // O-X separation
};

//----------------------------------------------------------------------------//

}  // namespace h2o_ion

////////////////////////////////////////////////////////////////////////////////

#endif  // X2B_H2O_ION_V2X_H

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

#ifndef X2B_A1B4_A1B4_DEG4_EXP0_V1X_H
#define X2B_A1B4_A1B4_DEG4_EXP0_V1X_H

#include "poly_2b_A1B4_A1B4_deg4_exp0_v1x.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B4_A1B4_deg4_exp0 {

//----------------------------------------------------------------------------//

struct x2b_A1B4_A1B4_v1x {
    x2b_A1B4_A1B4_v1x(){};
    x2b_A1B4_A1B4_v1x(const std::string mon1, const std::string mon2);

    ~x2b_A1B4_A1B4_v1x(){};

    typedef poly_2b_A1B4_A1B4_deg4_exp0_v1x polynomial;

    // returns 2B contribution only
    // XYZ is for the real sites

    double eval(const double* xyz1, const double* xyz2, const size_t ndim) const;
    double eval(const double* xyz1, const double* xyz2, double* grad1, double* grad2, const size_t ndim,
                std::vector<double>* virial = 0) const;

   private:
    double m_k_intra_AB;
    double m_d_intra_AB;
    double m_k_intra_BB;
    double m_d_intra_BB;
    double m_k_AA;
    double m_d_AA;
    double m_k_AB;
    double m_d_AB;
    double m_k_BB;
    double m_d_BB;

   protected:
    double m_r2i = 8.0;
    double m_r2f = 9.0;

    double f_switch(const double&, double&) const;  // At1_a -- At1_b separation

   private:
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x2b_A1B4_A1B4_deg4_exp0

////////////////////////////////////////////////////////////////////////////////

#endif

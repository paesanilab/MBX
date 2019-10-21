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

#include "testutils.h"

#include "tools/custom_exceptions.h"
#include "tools/math_tools.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test math tools") {
    
    // Prepare three vectors:
    SECTION("Dot Product Doubles") {
        std::vector<double> a = {0.0,1.0,2.0,3.0};
        std::vector<double> b = {0.0,1.0,2.0,3.0};
        std::vector<double> c = {0.0,1.0,2.0};
        double a_dot_b = 14.0;

        double a_dot_b_calc = DotProduct(a,b);
        REQUIRE(a_dot_b_calc == Approx(a_dot_b).margin(TOL));

        double tmp = 0.0;
        bool fails_if_not_different = false;
        try {
            tmp = DotProduct(a,c);
        } catch (CUException &e) {
            fails_if_not_different = true;
        }

        REQUIRE(fails_if_not_different);
    }

    SECTION("Dot Product Integers") {        
        std::vector<int> a = {0,1,2,3};
        std::vector<int> b = {0,1,2,3};
        std::vector<int> c = {0,1,2};
        int a_dot_b = 14;

        int a_dot_b_calc = DotProduct(a,b);
        REQUIRE(a_dot_b_calc == a_dot_b);

        int tmp = 0.0;
        bool fails_if_not_different = false;
        try {
            tmp = DotProduct(a,c);
        } catch (CUException &e) {
            fails_if_not_different = true;
        }

        REQUIRE(fails_if_not_different);
    }
}

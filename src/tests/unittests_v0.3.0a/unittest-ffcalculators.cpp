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

#include "tools/testutils.h"

#include "potential/force_field/calculators.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("calculators::CalculateDistance(p1,p2)") {
    SECTION("Normal behavior") {
        std::vector<double> p1 = {1.1,3.2,-5.8};
        std::vector<double> p2 = {2.1,-9.2,2.1};

        double expected_d = sqrt((p2[0]-p1[0])*(p2[0]-p1[0]) + (p2[1]-p1[1])*(p2[1]-p1[1]) + (p2[2]-p1[2])*(p2[2]-p1[2]));

        double d = eff::CalculateDistance(p1,p2);

        REQUIRE(d == Approx(expected_d).margin(TOL));
    }

    SECTION("Exceptions") {
        std::vector<double> p1 = {1.1,3.2,-5.8};
        std::vector<double> p2 = {2.1,-9.2};

        bool sizes_are_not_3 = false;
        try {
            double d = eff::CalculateDistance(p1,p2);
        } catch (...) {
            sizes_are_not_3 = true;
        }
  
        REQUIRE(sizes_are_not_3);
    }
}

TEST_CASE("calculators::CalculateDistance(p1)") {
    SECTION("Normal behavior") {
        std::vector<double> p = {2.1,-5.1,8.2};

        double expected_d = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);

        double d = eff::CalculateDistance(p);

        REQUIRE(d == Approx(expected_d).margin(TOL));
    }

    SECTION("Exceptions") {
        std::vector<double> p = {2.1,-5.1};
        bool sizes_are_not_3 = false;
        try {
            double d = eff::CalculateDistance(p);
        } catch (...) {
            sizes_are_not_3 = true;
        }

        REQUIRE(sizes_are_not_3);
    }
}

TEST_CASE("calculators::CalculateAngle") {
    SECTION("Normal behavior") {
        std::vector<double> p1 = {1.0,0.0,0.0};
        std::vector<double> p2 = {0.0,0.0,0.0};
        std::vector<double> p3 = {1.0,1.0,0.0};
        double expected_angle = pi/4;
 
        double angle = eff::CalculateAngle(p1,p2,p3);

        REQUIRE(angle == Approx(expected_angle).margin(TOL));
    }

    
}





























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

#include "potential/electrostatics/gammq.h"

#include <vector>
#include <iostream>

constexpr double TOL = 1E-6;

TEST_CASE("test the gamma function") {
    // Reference numebrs obtained from matlab
    std::vector<double> gammln_v = {elec::gammln(0.1), elec::gammln(0.75), elec::gammln(2.8), elec::gammln(15.293),
                                    elec::gammln(1003.92)};
    std::vector<double> gammln_vref = {2.252712651734206, 0.2032809514312953, 0.5167027919877466, 25.9777441762294,
                                       5932.304540589239};

    std::vector<double> gammq_v = {elec::gammq(0.75, 0.5),  elec::gammq(0.75, 5.0),  elec::gammq(0.75, 200.0),
                                   elec::gammq(10.0, 0.5),  elec::gammq(10.0, 5.0),  elec::gammq(10.0, 200.0),
                                   elec::gammq(100.0, 0.5), elec::gammq(100.0, 5.0), elec::gammq(100.0, 200.0)};
    std::vector<double> gammq_vref = {0.4720628901653282,
                                      0.00352609578734717,
                                      2.999317744047559e-88,
                                      0.999999999829033,
                                      0.9681719426937953,
                                      2.044095593580745e-72,
                                      1.0,
                                      1.0,
                                      0.000000000000001843893649711603};

    // Logarithm of gamma
    REQUIRE(VectorsAreEqual(gammln_v, gammln_vref));

    // Gammq
    REQUIRE(VectorsAreEqual(gammq_v, gammq_vref));
}

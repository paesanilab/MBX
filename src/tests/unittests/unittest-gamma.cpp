#include "testutils.h"

#include "gammq.h"

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

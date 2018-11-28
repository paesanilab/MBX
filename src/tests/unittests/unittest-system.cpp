#include "testutils.h"

#include "bblock/system.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("Test the system class.") {



    SECTION("One body (polynomials)") {
        REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
    }
}

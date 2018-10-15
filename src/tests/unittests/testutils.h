#include "catch.hpp"

#include <algorithm>
#include <vector>

bool VectorsAreEqual(const std::vector<double>& computed, const std::vector<double>& expected,
                     double tolerance = 1e-6) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](double x, double y) -> bool { return x == Approx(y).margin(tolerance); });
}

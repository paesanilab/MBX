#include "catch.hpp"

#include <algorithm>
#include <vector>
#include <string>

bool VectorsAreEqual(const std::vector<double>& computed, const std::vector<double>& expected,
                     double tolerance = 1e-6) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](double x, double y) -> bool { return x == Approx(y).margin(tolerance); });
}

bool VectorsAreEqual(const std::vector<std::string>& computed, const std::vector<std::string>& expected) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](std::string x, std::string y) -> bool { return x == y; });
}


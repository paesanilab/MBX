#include "catch.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

bool VectorsAreEqual(const std::vector<double>& computed, const std::vector<double>& expected,
                     double tolerance = 1e-6) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](double x, double y) -> bool { return x == Approx(y).margin(tolerance); });
}

template <typename T, typename U>
bool VectorsAreEqual(const std::vector<std::pair<T,U>>& computed, const std::vector<std::pair<T,U>>& expected) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](std::pair<T,U> x, std::pair<T,U> y) -> bool { std::cerr << "{" << x.first << " , " << x.second << "} <==> {" << y.first << " , " << y.second << "}" << std::endl; return x == y; });
}

template <typename T> 
bool VectorsAreEqual(const std::vector<T>& computed, const std::vector<T>& expected) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](T x, T y) -> bool { std::cerr << x << " <==> " << y << std::endl; return x == y; });
}


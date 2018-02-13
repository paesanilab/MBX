#ifndef ENERGY3B_H
#define ENERGY3B_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>

// 3B
#include "potential/3b/x3b-v2x.h"
#include "potential/3b/poly-3b-v2x.h"

namespace e3b {

double get_3b_energy(std::string m1, 
                     std::string m2, 
                     std::string m3, 
                     size_t nm,
                     std::vector<double> xyz1, 
                     std::vector<double> xyz2, 
                     std::vector<double> xyz3);

double get_3b_energy(std::string m1, 
                     std::string m2, 
                     std::string m3, 
                     size_t nm,
                     std::vector<double> xyz1, 
                     std::vector<double> xyz2, 
                     std::vector<double> xyz3,
                     std::vector<double> &grd1,
                     std::vector<double> &grd2,
                     std::vector<double> &grd3);

} // namespace e3b
#endif

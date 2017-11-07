#ifndef ENERGY2B_H
#define ENERGY2B_H

#include <string>
#include <vector>
#include <iostream>

// 2B
#include "potential/2b/x2b-v9x.h"
#include "potential/2b/x2b-A1B2Z2_C-v1x.h"

namespace e2b {

double get_2b_energy(std::string m1, std::string m2, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2);
double get_2b_energy(std::string m1, std::string m2,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2);

} // namespace e2b
#endif

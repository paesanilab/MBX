#ifndef ENERGY1B_H
#define ENERGY1B_H

#include <string>
#include <vector>
#include <iostream>

// 1B
#include "potential/1b/ps.h"

namespace e1b {

double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz);
double get_1b_energy(std::string mon,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> &grad);

} // namespace e1b
#endif

#ifndef ENERGY1B_H
#define ENERGY1B_H

#include <string>
#include <vector>
#include <iostream>

// 1B
#include "potential/1b/ps.h"

#define EMAX1B 60.0

namespace e1b {

double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz,
                     bool &good);
double get_1b_energy(std::string mon,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> &grad,
                     bool &good);

} // namespace e1b
#endif

#ifndef DISPERSION2B_H
#define DISPERSION2B_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "potential/dispersion/disptools.h"

namespace disp {

double GetDispersion(std::string m1, std::string m2,size_t nm, bool do_grads, 
                     std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2);

} // namespace disp
#endif

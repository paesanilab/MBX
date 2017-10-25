#ifndef ELECTROSTATICS_H
#define ELECTROSTATICS_H

#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>
#include <iostream>

#include "bblock/sys_tools.h"
#include "tools/definitions.h"
#include "tools/constants.h"
#include "potential/electrostatics/gammq.h"

namespace {
  size_t maxit = 1000;
}

namespace elec {
  double electrostatics(const std::vector<double> chg,
    const std::vector<double> polfac,
    const std::vector<double> pol,
    const std::vector<double> orig_xyz,
    const std::vector<std::string> mon_id,
    const std::vector<size_t> sites,
    const std::vector<size_t> first_ind,
    const std::vector<std::pair<std::string,size_t>> mon_type_count,
    const double tolerance, const bool do_grads,
    std::vector<double> &grad);
} //namespace elec

#endif

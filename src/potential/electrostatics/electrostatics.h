#ifndef ELECTROSTATICS_H
#define ELECTROSTATICS_H

#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>
#include <memory>

#ifdef _OPENMP
# include <omp.h>
#endif

#include "bblock/sys_tools.h"
#include "tools/definitions.h"
#include "tools/constants.h"
#include "potential/electrostatics/gammq.h"
#include "potential/electrostatics/fields.h"

////////////////////////////////////////////////////////////////////////////////

namespace elec {
  double Electrostatics(
    // Charges of each site. Order has to follow mon_type_count.
    const std::vector<double> chg,          
    // Gradients due to site dependent charges
    const std::vector<double> chg_grd,      
    // Polfacs of each site. For now assuming not site dependent.
    const std::vector<double> polfac,       
    // Polarizabilities of each site. For now assuming not site dependent.
    const std::vector<double> pol,
    // Original, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    const std::vector<double> orig_xyz,
    // Name of the monomers (h2o, f...)
    const std::vector<std::string> mon_id,
    // Number of sites of each mon
    const std::vector<size_t> sites,
    // First index of each atom in system
    const std::vector<size_t> first_ind,
    // Vector that contains all different monomer types and the number of
    // monomers of each type.
    const std::vector<std::pair<std::string,size_t>> mon_type_count,
    // Tolerance in the iterative calculation of the dipoles
    // Tolerance refers to the maximum squared difference overall the dipoles
    const double tolerance,
    // Maximum number of iterations allowed in the iterative dipole calculation
    const size_t maxit, 
    // Bool that if true will perform the gradients calculation.
    const bool do_grads,
    // Gradients in the original order (same as xyz). This is the vector in
    // which the electrostatics gradients will be added.
    std::vector<double> &orig_grd);

////////////////////////////////////////////////////////////////////////////////

} //namespace elec

////////////////////////////////////////////////////////////////////////////////

#endif

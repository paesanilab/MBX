#include "mon_h2o.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

h2o::h2o() {
}

h2o::h2o(double * coords, std::vector<std::string> names) {
  n_real_sites = 3;
  n_virt_sites = 1;

  n_sites = n_real_sites + n_virt_sites;

  xyz = new double[n_sites * 3];
  
  at_names.clear();
  
  for (int i = 0; i < n_real_sites; i++) {
    at_names.push_back(names[i]);
  }

  for (int i = n_real_sites; i < n_sites; i++) {
    at_names.push_back("virtual");
  }

  std::copy(coords, coords + n_real_sites * 3, xyz );
  std::fill(coords + n_real_sites * 3 , coords + n_sites * 3 , 0.0);
}

h2o::~h2o() {
  delete[] xyz;
}


void h2o::set_xyz(double * coords) {
}


////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

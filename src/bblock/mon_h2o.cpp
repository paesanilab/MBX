#include "mon_h2o.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Monomer :: H2O

////////////////////////////////////////////////////////////////////////////////

H2O::H2O() {
}

H2O::H2O(double * coords, std::vector<std::string> names) {
  //////////////////////////////////////////////////////////////////////////////
  ////////// USER DEFINED. ADD MANUALLY ////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  // Specify number of real and virtual sites
  n_real_sites = 3;
  n_virt_sites = 1;
  n_sites = n_real_sites + n_virt_sites;

  // Specify Monomer id
  mon_id = "h2o";

  // Specify the shift to be applied to the 1b energy
  shift_1b_energy = 0.0;

  //////////////////////////////////////////////////////////////////////////////
  ////////// DO NOT TOUCH. AUTOMATIC.   ////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  // Initialize in the heap the coordinates
  xyz = std::shared_ptr<double> (new double[n_sites * 3],
           []( double *p ) { delete[] p; });
  // Initialize in the heap the gradients
  grd = std::shared_ptr<double> (new double[n_sites * 3],
           []( double *p ) { delete[] p; });

  // Get the atom names and save them in the class
  // TODO: Maybe check that the names are in the proper order
  at_names.clear();
  for (size_t i = 0; i < n_real_sites; i++)
    at_names.push_back(names[i]);
  for (size_t i = n_real_sites; i < n_sites; i++)
    at_names.push_back("virt");

  // Copy the coordinates of real sites and set to 0 the virtual site ones
  std::copy(coords, coords + n_real_sites * 3, xyz.get() );
  if (n_virt_sites > 0) {
    std::fill(coords + n_real_sites * 3 , coords + n_sites * 3 , 0.0);
  }
  // Initialize gradients to 0
  std::fill(grd.get() , grd.get() + n_sites * 3 , 0.0);
}

H2O::~H2O() {
}

double H2O::Calc1BEnergy(double * syscoords) {
  // TODO: Update not necessary. We can check it later
  std::copy(syscoords + 3 * first_index,
            syscoords + 3 * (first_index + n_sites),  xyz.get()); 
  return ps::pot_nasa(xyz.get(),0);
}
double H2O::Calc1BEnergy(double * syscoords, double * grad) {
  // TODO: Update not necessary. We can check it later
  std::copy(syscoords + 3 * first_index, 
            syscoords + 3 * (first_index + n_sites),  xyz.get());
  double energy = ps::pot_nasa(xyz.get(),grd.get());
  for (size_t i = 0; i < 3*n_real_sites; i++)
    grad[i] +=  grd.get()[i];
  return energy;
}


////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Monomer :: H2O

////////////////////////////////////////////////////////////////////////////////

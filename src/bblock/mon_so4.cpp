#include "mon_so4.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

SO4::SO4() {
}

SO4::SO4(double * coords, std::vector<std::string> names) {

  //////////////////////////////////////////////////////////////////////////////
  ////////// USER DEFINED. ADD MANUALLY ////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  // Specify number of real and virtual sites
  n_real_sites = 5;
  n_virt_sites = 0;
  n_sites = n_real_sites + n_virt_sites;   

  // Specify Monomer id
  mon_id = "so4";

  // Specify the shift to be applied to the 1b energy
  shift_1b_energy = 0.0;

  // Specify the relative path from the Clusters Ultimate home directory
  // Just imagine you have a $CU_HOME in front of your path
  char rel_path[80] = "/src/potential/1b/params/mon_so4.dat";

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
  at_names.clear();
  for (size_t i = 0; i < n_real_sites; i++)
    at_names.push_back(names[i]);
  for (size_t i = n_real_sites; i < n_sites; i++) 
    at_names.push_back("virt");

  // Copy the coordinates of real sites and set to 0 the virtual site ones
  std::copy(coords, coords + n_real_sites * 3, xyz.get() );
  if (n_virt_sites > 0) {
    std::fill(xyz.get() + n_real_sites * 3 , xyz.get() + n_sites * 3 , 0.0);
  }
  // Initialize gradients to 0
  std::fill(grd.get() , grd.get() + n_sites * 3 , 0.0);

  char * env = std::getenv("CU_HOME");
  char path_to_dat[1000];
  if (env == NULL) {
    std::cerr << "Please set the environment variable CU_HOME" 
              << " to where the home directory of Clusters Ultimate is." 
              << std::endl;
    exit(EXIT_FAILURE);
  } else {
    strcpy(path_to_dat,env);
    strcat(path_to_dat,rel_path);
  }
  // Load the polynomial coefficients
  pot_1b.load_poly_dat(path_to_dat);

}

SO4::~SO4() {
}

double SO4::Calc1BEnergy(double * syscoords) {
  // TODO: Update not necessary. We can check it later
  std::copy(syscoords + 3 * first_index, 
            syscoords + 3 * (first_index + n_sites),  xyz.get());
  double energy = pot_1b(xyz.get());
  return energy;
}

double SO4::Calc1BEnergy(double * syscoords, double * grad) {
  // TODO: Update not necessary. We can check it later
  std::copy(syscoords + 3 * first_index, 
            syscoords + 3 * (first_index + n_sites),  xyz.get());
  double energy = pot_1b(xyz.get(),grd.get());
  for (size_t i = 0; i < 3*n_sites; i++)
    grad[i] +=  grd.get()[i];
  return energy;
}


////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

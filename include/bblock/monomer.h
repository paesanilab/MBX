#ifndef CU_INCLUDE_BBLOCK_MONOMER_H
#define CU_INCLUDE_BBLOCK_MONOMER_H

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>

#include <string.h>

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: Monomer
////////////////////////////////////////////////////////////////////////////////

// Monomer Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class describes an individual monomer. This is the general class.
// Depending on the identity of the monomer, it will be initialized with the 
// appropriate identity.
class Monomer {
 public:
  Monomer();
  // Virtual destructor. Since the monomer identity is in the derived
  // classes of monomer, the destructor must be called from there first.
  virtual ~Monomer();
  // Getters
  size_t GetNumSites();
  size_t GetNumRealSites();
  size_t GetNumVirtSites();
  // Copies in coords the coordinates of the real sites
  void GetRealSitesXyz(double * coords);
  void GetXyz(double * coords);
  void GetGrads(double * grads);
  // Returns a shared_ptr to the coordinates of the monomer
  std::shared_ptr<double> GetXyz();
  std::vector<std::string> GetAtomNames();
  std::string GetMonomerId();
  // Setters
  void SetXyz(double * coords);
  // Other 
  virtual double Calc1BEnergy() = 0; 
  virtual double Calc1BEnergy(double * grad) = 0; 
 protected:
  size_t n_real_sites;                            // Number of real sites
  size_t n_virt_sites;                            // Number of virtual sites
  size_t n_sites;                                 // Total number of sites
  double shift_1b_energy;
  std::shared_ptr<double>  xyz;                   // Coordinates of all sites
  std::shared_ptr<double>  grd;                   // Gradients of all sites
  std::string mon_id;                             // Identity of the monomer
  std::vector<std::string> at_names;              // Vector with the atom names
  
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONOMER_H

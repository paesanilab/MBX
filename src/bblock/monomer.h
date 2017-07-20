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
  size_t GetFirstIndex();
  // Copies in coords the coordinates of the real sites
  double * GetRealSitesXyz();
  void GetCrd(double * coords);
  void GetGrads(double * grads);
  // Returns a shared_ptr to the coordinates of the monomer
  double * GetXyz();
  std::vector<std::string> GetAtomNames();
  std::string GetMonomerId();
  // Setters
  void SetXyz(double * coords);
  void SetCrd(double * coords);
  void SetFirstIndex(size_t n);
  // Other 
  virtual double Calc1BEnergy() = 0; 
  virtual double Calc1BEnergy(double * grad) = 0; 
 protected:
  size_t n_real_sites;                            // Number of real sites
  size_t n_virt_sites;                            // Number of virtual sites
  size_t n_sites;                                 // Total number of sites
  size_t first_index;                             // Position of 1st atom in sys
  double shift_1b_energy;
  double * xyz;                                   // Pointer to coord in sys
  std::shared_ptr<double>  crd;                   // Coordinates of all sites
  std::shared_ptr<double>  grd;                   // Gradients of all sites
  std::string mon_id;                             // Identity of the monomer
  std::vector<std::string> at_names;              // Vector with the atom names
  std::vector<excluded_set_type> excluded12;      // 1-2 distance excluded pairs
  std::vector<excluded_set_type> excluded13;      // 1-3 distance excluded pairs
  std::vector<excluded_set_type> excluded14;      // 1-4 distance excluded pairs
 
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONOMER_H

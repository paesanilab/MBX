#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>

#include "molecule.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: System
////////////////////////////////////////////////////////////////////////////////

// System Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class contains a whole chemical system, which is made of molecules,
// which are made of monomers.
class System {
 public:
  System();
  ~System();
  // Getters
  size_t GetNumMol();
  size_t GetNumSites();
  std::vector<std::string> GetAtomNames();
  void GetXyz(double * coords);
  // Returns a pointer to the nth molecule in the system
  std::shared_ptr<Molecule> GetMolecule(size_t n);
  // Modifiers
  void Initialize();
  void SetNumMol(size_t n);
  void AddMolecule(std::shared_ptr<bblock::Molecule> molec);
  void SetXyz(double * coords);
  // Energy Functions
  double Energy();
  double Energy(double * grd);
 private:
  size_t n_mol;                                       // Number of molecules
  size_t n_sites;                                     // Number of sites in sys
  bool initialized;                                   // Systes is initialized?
  std::shared_ptr<double> grd;                        // Gradients of all sites
  std::shared_ptr<double> xyz;                        // Coords of all sites
  std::vector<std::shared_ptr<Molecule>> molecules;   // Vector with all molecs
  std::vector<std::string> at_names;
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

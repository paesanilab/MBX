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
  // Returns a pointer to the nth molecule in the system
  std::shared_ptr<Molecule> GetMolecule(size_t n);
  // Modifiers
  void SetNumMol(size_t n);
  void AddMolecule(std::shared_ptr<bblock::Molecule> molec);
 private:
  size_t n_mol;                                       // Number of molecules
  std::vector<std::shared_ptr<Molecule>> molecules;   // Vector with all molecs
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

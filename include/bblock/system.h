#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>

#include "molecule.h"

//Debugging
#include <iostream>


////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

class System {
 public:
  System();
  ~System();
  void clear();
  size_t GetNumMol();
  void SetNumMol(size_t n);
  std::shared_ptr<Molecule> GetMolecule(size_t n);
  void AddMolecule(std::shared_ptr<bblock::Molecule> molec);
 private:
  size_t n_mol;
  std::vector<std::shared_ptr<Molecule>> molecules;
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

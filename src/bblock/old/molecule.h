#ifndef CU_INCLUDE_BBLOCK_MOLECULE_H
#define CU_INCLUDE_BBLOCK_MOLECULE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "monomer.h"
#include "monomers.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: Molecule
////////////////////////////////////////////////////////////////////////////////

// Molecule Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class describes an individual molecule. This molecule is made of
// monomers, contained in the monomers vector.
class Molecule {
 public:
  Molecule();
  ~Molecule();
  // Accessing Functions:
  size_t GetNumMon();
  std::shared_ptr<Monomer> GetMonomer(size_t n);
  // Modifier Functions
  void SetNumMon(size_t n);
  void AddMonomer(std::string mon_name, double * xyz,
        std::vector<std::string> names);
 protected:
  size_t n_mon;                                     // Number of Monomers
  std::vector<std::shared_ptr<Monomer>> monomers;  // Vector with all monomers
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MOLECULE_H

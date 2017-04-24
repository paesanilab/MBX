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

namespace bblock { // Building Block :: molecule

////////////////////////////////////////////////////////////////////////////////

// Molecule class
class Molecule {
 public:
  Molecule();
  ~Molecule();
  void clear();
  size_t GetNumMon();
  void SetNumMon(size_t n);
  void AddMonomer(std::string mon_name, double * xyz,
        std::vector<std::string> names);
  std::shared_ptr<Monomer> GetMonomer(size_t n);
 private:
  size_t n_mon;
  std::vector<std::shared_ptr<Monomer> > monomers;
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MOLECULE_H

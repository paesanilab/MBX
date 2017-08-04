#ifndef CU_INCLUDE_TOOLS_READNRG_H
#define CU_INCLUDE_TOOLS_READNRG_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "system.h"
#include "molecule.h"
#include "monomer.h"

////////////////////////////////////////////////////////////////////////////////
namespace tools {
////////////////////////////////////////////////////////////////////////////////
void ReadNrg(char* filename, std::vector<bblock::System> & systems);
void ReadSystem(size_t& lineno, std::istream& ifs, bblock::System& sys);
void ReadMolecule(size_t& lineno, std::istream& ifs, 
                   std::shared_ptr<bblock::Molecule> molec);
void ReadMonomers(size_t& lineno, std::istream& ifs, 
                   std::shared_ptr<bblock::Molecule> molec);
////////////////////////////////////////////////////////////////////////////////
} // namespace tools
////////////////////////////////////////////////////////////////////////////////
#endif // CU_INCLUDE_TOOLS_READNRG_H

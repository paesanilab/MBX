#ifndef READ_NRG_H
#define READ_NRG_H

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

namespace tools {

// returns number of systems loaded
void read_nrg(char*, std::vector<bblock::system> );

void read_system(size_t& lineno, std::istream& ifs, bblock::system& sys);

void read_molecule(size_t& lineno, std::istream& ifs, bblock::molecule& molec);

void read_monomers(size_t& lineno, std::istream& ifs, bblock::molecule& molec);


} // namespace tools

#endif // READ_NRG_H

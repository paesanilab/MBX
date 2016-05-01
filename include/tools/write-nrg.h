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

void write_nrg(char * filename, std::vector<bblock::system> systems );
void save_nrg(std::ostream & os, std::vector<bblock::system> systems );


} // namespace tools

#endif // READ_NRG_H

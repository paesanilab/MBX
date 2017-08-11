#ifndef CU_INCLUDE_TOOLS_WRITENRG_H
#define CU_INCLUDE_TOOLS_WRITENRG_H

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

namespace tools {

void WriteNrg(const char * filename, std::vector<bblock::System> systems );
void SaveNrg(std::ostream & os, std::vector<bblock::System> systems );


} // namespace tools

#endif // CU_INCLUDE_TOOLS_WRITENRG_H

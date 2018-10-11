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

#include "bblock/system.h"

/**
 * @file write_nrg.h
 * @brief Contains the function declarations that allow to write an NRG file
 */

namespace tools {

/**
 * @brief Puts the information of a vector of systems in a file
 *
 * This function will save all the information of a set of systems
 * (molecules, monomers and atoms) in a file.
 * @param[in] filename Name of the file where the information must be saved
 * @param[in] systems Vector of systems that needs to be saved in the file
 */
void WriteNrg(const char* filename, std::vector<bblock::System> systems);

/**
 * @brief Helper function that saves the information in an output stream
 *
 * This function will save the information of a set of systems
 * (molecules, monomers and atoms) in an output stream
 * @param[in,out] os Output stream where the information must be saved
 * @param[in] systems Vector of systems that needs to be saved in the file
 */
void SaveNrg(std::ostream& os, std::vector<bblock::System> systems);

}  // namespace tools

#endif  // CU_INCLUDE_TOOLS_WRITENRG_H

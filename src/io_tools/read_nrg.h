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

#include "bblock/system.h"
#include "tools/custom_exceptions.h"

/**
 * @file read_nrg.h
 * @brief Contains the function declarations that allow to read an NRG file
 */

/**
 * @namespace tools
 * @brief Sets the namespace for useful tools in this software
 */
namespace tools {

/**
 * @brief Reads an NRG file
 *
 * Given the filename and a system vector to put the systems in,
 * reads all the systems, saving them in the vector in the order in
 * wich they are in the input
 * @param[in] filename Name of the file to read
 * @param[out] systems Vector of systems that will be filled with
 * the information in the file
 */
void ReadNrg(char* filename, std::vector<bblock::System> & systems);

/** 
 * @brief Reads a system inside the NRG file
 *
 * This function is called by ReadNrg, and reads one system at a time
 * @param[in] lineno Current line number of the file we are reading
 * @param[in,out] ifs Input file stream of the open file that we 
 * are reading
 * @param[in,out] sys System that will be filled with
 * the information in the file
 */
void ReadSystem(size_t& lineno, std::istream& ifs, bblock::System& sys);

/**
 * @brief Reads a molecule of a system
 * 
 * This function reads a molecule inside a system, and adds it to the system.
 * @param[in] lineno Current line number in the file we are reading
 * @param[in,out] ifs Input file stream of the open file that we 
 * are reading
 * @param[in,out] sys System that will be filled with
 * the information in the file
 * @param[out] mon_count Number of monomers in the molecule
 */
void ReadMolecule(size_t& lineno, std::istream& ifs, 
                  bblock::System& sys, size_t& mon_count);

/**
 * @brief Reads a monomer inside a molecule
 *
 * This function reads a monomer inside a molecule, and adds it to the system.
 * @param[in] lineno Current line number in the file we are reading
 * @param[in,out] ifs Input file stream of the open file that we 
 * are reading
 * @param[in,out] sys System that will be filled with
 * the information in the file
 * @param[out] mon_count Number of monomers in the molecule
 */
void ReadMonomers(size_t& lineno, std::istream& ifs, 
                  bblock::System& sys, size_t& mon_count);

} // namespace tools

#endif // CU_INCLUDE_TOOLS_READNRG_H

/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

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

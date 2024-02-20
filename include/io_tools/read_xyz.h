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

#ifndef MBX_INCLUDE_TOOLS_READXYZ_H
#define MBX_INCLUDE_TOOLS_READXYZ_H

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
 * @file read_xyz.h
 * @brief Contains the function declarations that allow to read an XYZ file
 */

/**
 * @namespace tools
 * @brief Sets the namespace for useful tools in this software
 */
namespace tools {

/**
 * @brief Reads an XYZ file
 *
 * Given the filename and vectors to put the coordinates and atom names,
 * reads all the frames
 * @param[in] filename Name of the xyz file
 * @param[out] atom_names Vector of the atom names for each frame
 * @param[out] coords Vector of the coordinates for each frame
 * @param[out] boxes Vector with boxes of each frame
 * @param[in] read_box Bool that states if the box will be read (i-pi format)
 * @param[in] to_angstrom Conversion factor to convert from the units to angstrom
 */
void ReadXYZ(const char *filename, std::vector<std::vector<std::string> > &atom_names,
             std::vector<std::vector<double> > &coords, std::vector<std::vector<double> > &boxes, bool read_box = false,
             double to_angstrom = 1.0);

/**
 * @brief Reads a frame inside the XYZ file
 *
 * This function is called by ReadXYZ, and reads one frame at a time
 * @param[in] lineno Current line number of the file we are reading
 * @param[in,out] ifs Input file stream of the open file that we
 * are reading
 * @param[out] atom_names Vector of the atom names
 * @param[out] coords Vector of the coordinates
 * @param[out] box Vector with the box
 * @param[in] read_box Bool that states if the box will be read (i-pi format)
 * @param[in] to_angstrom Conversion factor to convert from the units to angstrom

 * the information in the file
 */
void ReadFrame(size_t &lineno, std::istream &ifs, std::vector<std::string> &atom_names, std::vector<double> &coords,
               std::vector<double> &box, bool read_box = false, double to_angstrom = 1.0);

}  // namespace tools

#endif  // MBX_INCLUDE_TOOLS_READXYZ_H

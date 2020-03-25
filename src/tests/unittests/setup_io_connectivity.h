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

#ifndef UNITTESTS_SETUP_IO_CONNECTIVITY
#define UNITTESTS_SETUP_IO_CONNECTIVITY
#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/topology.h"
#include "potential/force_field/connectivity.h"
#include <vector>
#include "tools/custom_exceptions.h"
#include "setup_connectivity.h"
#include <unordered_map>
#include "io_tools/read_connectivity.h"
#include "io_tools/write_connectivity.h"

/**
 * @file setup_io_connectivity.h
 * @brief This file is used to test the io functions for the
 * connectivity. Multiple types of connectivity are defined, written to an
 * unordered set, and then written out to a file before being read back and
 * compared
 */

// Import first connectivity
SETUP_CONNECTIVITY

#define SETUP_IO_CONNECTIVITY                                                         \
    eff::Bond mon2_bond("bond", std::vector<size_t>{8, 3}, "harm");                   \
    eff::Angles mon2_angle1("angle", std::vector<size_t>{5, 3, 4}, "quartic");        \
    eff::Angles mon2_angle2("angle", std::vector<size_t>{2, 3, 6}, "harm");           \
    eff::Dihedral mon2_dihedral("dihedral", std::vector<size_t>{2, 1, 6, 4}, "hcos"); \
    eff::Inversion mon2_inversion("inversion", std::vector<size_t>{4, 1, 2, 3}, "harm");
#endif

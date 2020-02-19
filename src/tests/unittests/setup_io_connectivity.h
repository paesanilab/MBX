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
#include "bblock/connectivity.h"
#include <vector>
#include "tools/custom_exceptions.h"
#include "setup_connectivity.h"
#include <unordered_map>
#include "io_tools/read_connectivity.h"
#include "io_tools/write_connectivity.h"

// First connectivity
SETUP_CONNECTIVITY

#define SETUP_IO_CONNECTIVITY                                                         \
    Bond tmp5 = Bond("bond", 1, std::vector<size_t>{8, 3}, "harm");                   \
    Angles tmp6 = Angles("angle", 1, std::vector<size_t>{5, 3, 4}, "quartic");        \
    Angles tmp7 = Angles("angle", 2, std::vector<size_t>{2, 3, 6}, "harm");           \
    Dihedral tmp8 = Dihedral("dihedral", 1, std::vector<size_t>{2, 1, 6, 4}, "hcos"); \
    Inversion tmp9 = Inversion("inversion", 3, std::vector<size_t>{4, 1, 2, 3}, "harm");
#endif

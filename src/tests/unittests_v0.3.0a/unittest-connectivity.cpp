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

#include "tools/testutils.h"

#include "potential/force_field/connectivity.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/angles.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/inversion.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("connectivity::constructor") {
    // Create a couple bonds
    std::string topo;
    std::vector<size_t> idxs;
    std::string funcform;

    topo = "BoNd";
    idxs = {0, 1};
    funcform = "harm";
    eff::Bond bond1(topo, idxs, funcform);

    idxs = {0, 2};
    eff::Bond bond2(topo, idxs, funcform);

    // Create angles
    topo = "angle";
    idxs = {0, 1, 2};
    funcform = "harm";
    eff::Angles angle1(topo, idxs, funcform);

    idxs = {0, 2, 3};
    eff::Angles angle2(topo, idxs, funcform);

    idxs = {1, 2, 3};
    eff::Angles angle3(topo, idxs, funcform);

    // Create dihedrals
    topo = "dihedral";
    idxs = {0, 1, 2, 3};
    funcform = "cos";
    eff::Dihedral dh1(topo, idxs, funcform);

    idxs = {0, 1, 2, 4};
    eff::Dihedral dh2(topo, idxs, funcform);

    idxs = {0, 1, 3, 4};
    eff::Dihedral dh3(topo, idxs, funcform);

    // Create inversions
    topo = "inversion";
    idxs = {0, 1, 2, 3};
    funcform = "harm";
    eff::Inversion inv1(topo, idxs, funcform);

    idxs = {2, 3, 5, 1};
    eff::Inversion inv2(topo, idxs, funcform);

    idxs = {2, 3, 4, 5};
    eff::Inversion inv3(topo, idxs, funcform);

    // Make the vectors
    std::vector<eff::Bond> bv = {bond1, bond2};
    std::vector<eff::Angles> av = {angle1, angle2, angle3};
    std::vector<eff::Dihedral> dv = {dh1, dh2, dh3};
    std::vector<eff::Inversion> iv = {inv1, inv2, inv3};

    std::string monid = "ch4";

    // Create connectivity object
    eff::Conn con(monid, bv, av, dv, iv);
}

TEST_CASE("connectivity::getters") {
    // Create a couple bonds
    std::string topo;
    std::vector<size_t> idxs;
    std::string funcform;

    topo = "BoNd";
    idxs = {0, 1};
    funcform = "harm";
    eff::Bond bond1(topo, idxs, funcform);

    idxs = {0, 2};
    eff::Bond bond2(topo, idxs, funcform);

    // Create angles
    topo = "angle";
    idxs = {0, 1, 2};
    funcform = "harm";
    eff::Angles angle1(topo, idxs, funcform);

    idxs = {0, 2, 3};
    eff::Angles angle2(topo, idxs, funcform);

    idxs = {1, 2, 3};
    eff::Angles angle3(topo, idxs, funcform);

    // Create dihedrals
    topo = "dihedral";
    idxs = {0, 1, 2, 3};
    funcform = "cos";
    eff::Dihedral dh1(topo, idxs, funcform);

    idxs = {0, 1, 2, 4};
    eff::Dihedral dh2(topo, idxs, funcform);

    idxs = {0, 1, 3, 4};
    eff::Dihedral dh3(topo, idxs, funcform);

    // Create inversions
    topo = "inversion";
    idxs = {0, 1, 2, 3};
    funcform = "harm";
    eff::Inversion inv1(topo, idxs, funcform);

    idxs = {2, 3, 5, 1};
    eff::Inversion inv2(topo, idxs, funcform);

    idxs = {2, 3, 4, 5};
    eff::Inversion inv3(topo, idxs, funcform);

    // Make the vectors
    std::vector<eff::Bond> bv = {bond1, bond2};
    std::vector<eff::Angles> av = {angle1, angle2, angle3};
    std::vector<eff::Dihedral> dv = {dh1, dh2, dh3};
    std::vector<eff::Inversion> iv = {inv1, inv2, inv3};

    std::string monid = "ch4";

    // Create connectivity object
    eff::Conn con(monid, bv, av, dv, iv);

    REQUIRE(con.GetMonId() == monid);
    REQUIRE(con.GetBondVec() == bv);
    REQUIRE(con.GetAnglesVec() == av);
    REQUIRE(con.GetDihedralVec() == dv);
    REQUIRE(con.GetInversionVec() == iv);
}

TEST_CASE("connectivity::Operator==") {
    // Create a couple bonds
    std::string topo;
    std::vector<size_t> idxs;
    std::string funcform;

    topo = "BoNd";
    idxs = {0, 1};
    funcform = "harm";
    eff::Bond bond1(topo, idxs, funcform);

    idxs = {0, 2};
    eff::Bond bond2(topo, idxs, funcform);

    // Create angles
    topo = "angle";
    idxs = {0, 1, 2};
    funcform = "harm";
    eff::Angles angle1(topo, idxs, funcform);

    idxs = {0, 2, 3};
    eff::Angles angle2(topo, idxs, funcform);

    idxs = {1, 2, 3};
    eff::Angles angle3(topo, idxs, funcform);

    // Create dihedrals
    topo = "dihedral";
    idxs = {0, 1, 2, 3};
    funcform = "cos";
    eff::Dihedral dh1(topo, idxs, funcform);

    idxs = {0, 1, 2, 4};
    eff::Dihedral dh2(topo, idxs, funcform);

    idxs = {0, 1, 3, 4};
    eff::Dihedral dh3(topo, idxs, funcform);

    // Create inversions
    topo = "inversion";
    idxs = {0, 1, 2, 3};
    funcform = "harm";
    eff::Inversion inv1(topo, idxs, funcform);

    idxs = {2, 3, 5, 1};
    eff::Inversion inv2(topo, idxs, funcform);

    idxs = {2, 3, 4, 5};
    eff::Inversion inv3(topo, idxs, funcform);

    // Make the vectors
    std::vector<eff::Bond> bv = {bond1, bond2};
    std::vector<eff::Angles> av = {angle1, angle2, angle3};
    std::vector<eff::Dihedral> dv = {dh1, dh2, dh3};
    std::vector<eff::Inversion> iv = {inv1, inv2, inv3};

    std::string monid = "ch4";

    // Create connectivity object
    eff::Conn con1(monid, bv, av, dv, iv);

    // Create same connectivity
    eff::Conn con2(monid, bv, av, dv, iv);

    // Add an extra bond
    idxs = {4, 5};
    eff::Bond bextra("bond", idxs, "harm");
    bv.push_back(bextra);
    eff::Conn con3(monid, bv, av, dv, iv);

    REQUIRE(con1 == con2);
    REQUIRE(!(con1 == con3));
}

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2014 Stanford University and the Authors.           *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "MBnrgForce.h"
#include "internal/MBnrgForceImpl.h"
#include "openmm/OpenMMException.h"
#include "openmm/internal/AssertionUtilities.h"

using namespace MBnrgPlugin;
using namespace OpenMM;
using namespace std;

MBnrgForce::MBnrgForce() {
}

//int MBnrgForce::addBond(int particle1, int particle2, double length, double k) {
//    bonds.push_back(BondInfo(particle1, particle2, length, k));
//    return bonds.size()-1;
//}
//

int MBnrgForce::addMonomerList(std::vector<std::string> openmmMonomers) {
    mbnrg_monomer_names.clear();
    atoms.clear();
    sites.clear();
    for (size_t i = 0; i < openmmMonomers.size(); i++) {
        std::string mon = openmmMonomers[i];
        std::transform(mon.begin(), mon.end(), mon.begin(), ::tolower);
        if (mon == "hoh") {
            mbnrg_monomer_names.push_back("h2o");
            atoms.push_back(3);
            sites.push_back(4);
            std::vector<std::string> names(atoms.back());
            names[0] = "O";
            names[1] = "H";
            names[2] = "H";
            at_names.push_back(names);
        } else if (mon == "cl") {
            mbnrg_monomer_names.push_back("cl");
            atoms.push_back(1);
            sites.push_back(1);
            std::vector<std::string> names(atoms.back());
            names[0] = "Cl";
            at_names.push_back(names);
        }
    }
    return 0;
}
//void MBnrgForce::getBondParameters(int index, int& particle1, int& particle2, double& length, double& k) const {
//    ASSERT_VALID_INDEX(index, bonds);
//    particle1 = bonds[index].particle1;
//    particle2 = bonds[index].particle2;
//    length = bonds[index].length;
//    k = bonds[index].k;
//}
//
//void MBnrgForce::setBondParameters(int index, int particle1, int particle2, double length, double k) {
//    ASSERT_VALID_INDEX(index, bonds);
//    bonds[index].particle1 = particle1;
//    bonds[index].particle2 = particle2;
//    bonds[index].length = length;
//    bonds[index].k = k;
//}

ForceImpl* MBnrgForce::createImpl() const {
    return new MBnrgForceImpl(*this);
}

void MBnrgForce::updateParametersInContext(Context& context) {
    dynamic_cast<MBnrgForceImpl&>(getImplInContext(context)).updateParametersInContext(getContextImpl(context));
}

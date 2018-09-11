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

#include "ReferenceMBnrgKernels.h"
#include "MBnrgForce.h"
#include "openmm/OpenMMException.h"
#include "openmm/internal/ContextImpl.h"
#include "openmm/reference/RealVec.h"
#include "openmm/reference/ReferencePlatform.h"

using namespace MBnrgPlugin;
using namespace OpenMM;
using namespace std;

static vector<RealVec>& extractPositions(ContextImpl& context) {
    ReferencePlatform::PlatformData* data = reinterpret_cast<ReferencePlatform::PlatformData*>(context.getPlatformData());
    return *((vector<RealVec>*) data->positions);
}

static vector<RealVec>& extractForces(ContextImpl& context) {
    ReferencePlatform::PlatformData* data = reinterpret_cast<ReferencePlatform::PlatformData*>(context.getPlatformData());
    return *((vector<RealVec>*) data->forces);
}

void ReferenceCalcMBnrgForceKernel::initialize(const System& system, const MBnrgForce& force) {

    // Can do nothing here (MB-nrg)

    mbnrg_initialize(force);
    mbsys_initialized = true;

}

double ReferenceCalcMBnrgForceKernel::execute(ContextImpl& context, bool includeForces, bool includeEnergy) {
    vector<RealVec>& pos = extractPositions(context);
    vector<RealVec>& force = extractForces(context);

    double nmtoang = 10.0;
    std::vector<double> xyz_context(3*pos.size());
    for (size_t i = 0; i < pos.size(); i++) {
      xyz_context[3*i + 0] = pos[i][0] * nmtoang;
      xyz_context[3*i + 1] = pos[i][1] * nmtoang;
      xyz_context[3*i + 2] = pos[i][2] * nmtoang;
    }


    mbnrg_system.SetOriginalOrderSysXyz(xyz_context);
    
    double kcaltokj = 4.184;
    double kcalperAngtokjpernm = kcaltokj*10;
    std::vector<double> grad(3*force.size(),0.0);
    double energy = mbnrg_system.Energy(grad,true) * kcaltokj;
    
    for (size_t i = 0; i < force.size(); i++) {
      force[i][0] = -grad[3*i + 0]*kcalperAngtokjpernm;
      force[i][1] = -grad[3*i + 1]*kcalperAngtokjpernm;
      force[i][2] = -grad[3*i + 2]*kcalperAngtokjpernm;
    }

    return energy;
}

void ReferenceCalcMBnrgForceKernel::copyParametersToContext(ContextImpl& context, const MBnrgForce& force) { }

void ReferenceCalcMBnrgForceKernel::mbnrg_initialize(const MBnrgForce& force) {

    size_t pos = 0;
    for (size_t i = 0; i < force.mbnrg_monomer_names.size(); i++) {
      size_t num_coords = force.atoms[i]*3;
      std::vector<double> coords(num_coords,0.0);
      mbnrg_system.AddMonomer(coords, force.at_names[i], force.mbnrg_monomer_names[i]);
      pos += force.sites[i]*3;
    }
    
    mbnrg_system.Initialize();
}

#ifndef REFERENCE_MBNRG_KERNELS_H_
#define REFERENCE_MBNRG_KERNELS_H_

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

#include "MBnrgKernels.h"
#include "openmm/Platform.h"
#include "bblock/system.h"
#include <vector>

namespace MBnrgPlugin {

/**
 * This kernel is invoked by MBnrgForce to calculate the forces acting on the system and the energy of the system.
 */
class ReferenceCalcMBnrgForceKernel : public CalcMBnrgForceKernel {
public:
    ReferenceCalcMBnrgForceKernel(std::string name, const OpenMM::Platform& platform) : CalcMBnrgForceKernel(name, platform) {
    }
    /**
     * Initialize the kernel.
     * 
     * @param system     the System this kernel will be applied to
     * @param force      the MBnrgForce this kernel will be used for
     */
    void initialize(const OpenMM::System& system, const MBnrgForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy);
    /**
     * Copy changed parameters over to a context.
     *
     * @param context    the context to copy parameters to
     * @param force      the MBnrgForce to copy the parameters from
     */
    void copyParametersToContext(OpenMM::ContextImpl& context, const MBnrgForce& force);
private:

    void mbnrg_initialize(const MBnrgForce& force);
    bblock::System mbnrg_system;
    bool mbsys_initialized;

//    int numBonds;
//    std::vector<int> particle1, particle2;
//    std::vector<double> length, k;
};

} // namespace MBnrgPlugin

#endif /*REFERENCE_MBNRG_KERNELS_H_*/

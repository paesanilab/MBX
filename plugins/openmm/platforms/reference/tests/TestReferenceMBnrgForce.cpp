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

/**
 * This tests the Reference implementation of MBnrgForce.
 */

#include "MBnrgForce.h"
#include "openmm/internal/AssertionUtilities.h"
#include "openmm/Context.h"
#include "openmm/Platform.h"
#include "openmm/System.h"
#include "openmm/VerletIntegrator.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace MBnrgPlugin;
using namespace OpenMM;
using namespace std;

extern "C" OPENMM_EXPORT void registerMBnrgReferenceKernelFactories();

void testForce() {
    // MBnrg test
    // Create a water tetramer vector with the positions
    const int numParticles = 16;
    vector<Vec3> positions(16);
    positions[0 ] = Vec3(-5.271868960e+00, -5.118438792e+00, -6.444759217e-02); 
    positions[1 ] = Vec3(-5.042973646e+00, -5.970187860e+00,  3.087563258e-01);
    positions[2 ] = Vec3(-6.213650520e+00, -5.178381524e+00, -2.319150410e-01);
    positions[3 ] = Vec3(0.0,0.0,0.0);
    positions[4 ] = Vec3( 8.724746036e+00, -4.070724332e+00, -4.940333813e-01);
    positions[5 ] = Vec3( 8.584254966e+00, -5.018093793e+00, -5.260720525e-01);
    positions[6 ] = Vec3( 9.675007155e+00, -3.976482166e+00, -4.152170230e-01);
    positions[7 ] = Vec3(0.0,0.0,0.0);
    positions[8 ] = Vec3( 4.642386244e+00,  4.884658192e+00, -7.931968681e-01);
    positions[9 ] = Vec3( 5.588814045e+00,  5.043530554e+00, -8.123070367e-01);
    positions[10] = Vec3( 4.256517072e+00,  5.749582788e+00, -9.490054795e-01);
    positions[11] = Vec3(0.0,0.0,0.0);
    positions[12] = Vec3( 1.957774296e+00,  1.649823825e+00, -2.680393863e+00);
    positions[13] = Vec3( 2.279807956e+00,  1.053814129e+00, -3.360605419e+00);
    positions[14] = Vec3( 1.074407823e+00,  1.883791633e+00, -2.974171237e+00);    
    positions[15] = Vec3(0.0,0.0,0.0);

    // Fill the system
    System system;
    for (int i = 0; i < numParticles; i++) {
        system.addParticle(1.0);
    }
    MBnrgForce* force = new MBnrgForce();
    system.addForce(force);
    
    // Compute the forces and energy.

    VerletIntegrator integ(1.0);
    Platform& platform = Platform::getPlatformByName("Reference");
    Context context(system, integ, platform);
    context.setPositions(positions);
    State state = context.getState(State::Energy | State::Forces);
    
    // See if the energy is correct.
    
    double expectedEnergy = 0.931833;
    std::cout << "Asserting: " << expectedEnergy << " vs " << state.getPotentialEnergy() << std::endl;
    ASSERT_EQUAL_TOL(expectedEnergy, state.getPotentialEnergy(), 1e-5);

    // Validate the forces by moving each particle along each axis, and see if the energy changes by the correct amount.
    
    double offset = 1e-5;
    for (int i = 0; i < numParticles; i++)
        for (int j = 0; j < 3; j++) {
            vector<Vec3> offsetPos = positions;
            offsetPos[i][j] = positions[i][j]-offset;
            context.setPositions(offsetPos);
            double e1 = context.getState(State::Energy).getPotentialEnergy();
            offsetPos[i][j] = positions[i][j]+offset;
            context.setPositions(offsetPos);
            double e2 = context.getState(State::Energy).getPotentialEnergy();
            std::cout << "Asserting: " << state.getForces()[i][j] << " vs " << (e1-e2)/(2*offset) << std::endl;
            ASSERT_EQUAL_TOL(state.getForces()[i][j], (e1-e2)/(2*offset), 1e-3);
        }
}

//void testChangingParameters() {
//    const double k = 1.5;
//    const double length = 0.5;
//    Platform& platform = Platform::getPlatformByName("Reference");
//    
//    // Create a system with one bond.
//    
//    System system;
//    system.addParticle(1.0);
//    system.addParticle(1.0);
//    MBnrgForce* force = new MBnrgForce();
//    force->addBond(0, 1, length, k);
//    system.addForce(force);
//    vector<Vec3> positions(2);
//    positions[0] = Vec3(1, 0, 0);
//    positions[1] = Vec3(2, 0, 0);
//    
//    // Check the energy.
//    
//    VerletIntegrator integ(1.0);
//    Context context(system, integ, platform);
//    context.setPositions(positions);
//    State state = context.getState(State::Energy);
//    ASSERT_EQUAL_TOL(k*pow(1.0-length, 4), state.getPotentialEnergy(), 1e-5);
//    
//    // Modify the parameters.
//    
//    const double k2 = 2.2;
//    const double length2 = 0.9;
//    force->setBondParameters(0, 0, 1, length2, k2);
//    force->updateParametersInContext(context);
//    state = context.getState(State::Energy);
//    ASSERT_EQUAL_TOL(k2*pow(1.0-length2, 4), state.getPotentialEnergy(), 1e-5);
//}

int main() {
    try {
        registerMBnrgReferenceKernelFactories();
        testForce();
//        testChangingParameters();
    }
    catch(const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Done" << std::endl;
    return 0;
}

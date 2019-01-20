#include "testutils.h"

#include "potential/dispersion/dispersion.h"
#include "setupfluoridewatermbpol.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;
constexpr double CUTOFF = 10.0;

TEST_CASE("Test the dispersion class.") {
    // Create the bromide -- water system
    SETUP_WATER_FLUORIDE_MBPOL

    std::vector<double> box = {1000.0,0.0,0.0,0.0,1000.0,0.0,0.0,0.0, 1000.0};

    // create dispersion class
    disp::Dispersion my_disp;
    my_disp.Initialize(C6_long_range, real_coords,monomer_names, n_atoms_vector, internal_mon_type_count, false,  box);

    SECTION("Switch is 1") {
        // Get dispersion with no grads
        std::vector<double> dummy(3*n_atoms,0.0);
        my_disp.SetNewParameters(real_coords,false, CUTOFF, box);
        double energy_nograd = my_disp.GetDispersion(dummy);

        // Set do_grads to true
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3*n_atoms,0.0);
        double energy_grad = my_disp.GetDispersion(grad);
        
        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Switch is 0") {
        // Get dispersion with no grads
        std::vector<double> dummy(3*n_atoms,0.0);
        real_coords[11] = CUTOFF + 1.0;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3*n_atoms,0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Switch is between 0 and 1") {
        // Get dispersion with no grads
        std::vector<double> dummy(3*n_atoms,0.0);
        real_coords[11] = CUTOFF - 0.5;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3*n_atoms,0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}

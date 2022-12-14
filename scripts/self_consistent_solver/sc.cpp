#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

//#define PRINT_GRADS
namespace {

static std::vector<bblock::System> systems;

}  // namespace

int main(int argc, char** argv) {
    // Check number of arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.nrg> [mbx.json]" << std::endl;
        return 0;
    }

    // Load the nrg file
    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // Load JSON file
    std::vector<double> box;

    for (size_t i = 0; i < systems.size(); i++) {
        if (argc > 2) {
            systems[i].SetUpFromJson(argv[2]);
        } else {
            systems[i].SetUpFromJson();
        }
    }

    // Start
    // Calculate energy of system 0
    double e_ref = systems[0].Energy(true);
    double e_perm_ref = systems[0].GetPermanentElectrostaticEnergy();
    double e_ind_ref = systems[0].GetInducedElectrostaticEnergy();

    // Calculate energy of system 1
    double e1 = systems[1].Energy(true);
    double e1_perm = systems[1].GetPermanentElectrostaticEnergy();
    double e1_ind = systems[1].GetInducedElectrostaticEnergy();

    // Calculate fields in sites of 2
    std::vector<double> xyz2 = systems[2].GetXyz();
    systems[1].Hack3GetPotentialAtPoints(xyz2);

    std::vector<double> electric_field_1to2;
    std::vector<double> electric_potential_1to2;
    std::vector<double> electric_field_dip_1to2;
    std::vector<double> electric_potential_dip_1to2;

    systems[1].GetPhiXAndEfX(electric_potential_1to2, electric_field_1to2, electric_potential_dip_1to2,
                             electric_field_dip_1to2);

    std::vector<double> new_electric_field;
    std::vector<double> new_electric_potential;

    for (size_t i = 0; i < electric_field_1to2.size(); i++) {
        new_electric_field.push_back(electric_field_1to2[i] + electric_field_dip_1to2[i]);
    }

    for (size_t i = 0; i < electric_potential_1to2.size(); i++) {
        new_electric_potential.push_back(electric_potential_1to2[i] + electric_potential_dip_1to2[i]);
    }
    // Set fields in S2
    systems[2].SetExternalElectrostaticPotentialAndFieldInSites(new_electric_potential, new_electric_field);

    // Calculate E for s2
    double e2 = systems[2].Energy(true);
    double e2_perm = systems[2].GetPermanentElectrostaticEnergy();
    double e2_ind = systems[2].GetInducedElectrostaticEnergy();

    // Calculate fields in sites of 2
    std::vector<double> xyz1 = systems[1].GetXyz();
    systems[2].Hack3GetPotentialAtPoints(xyz1);

    std::vector<double> electric_field_2to1;
    std::vector<double> electric_potential_2to1;
    std::vector<double> electric_field_dip_2to1;
    std::vector<double> electric_potential_dip_2to1;

    systems[2].GetPhiXAndEfX(electric_potential_2to1, electric_field_2to1, electric_potential_dip_2to1,
                             electric_field_dip_2to1);

    std::vector<double> new_electric_field2;
    std::vector<double> new_electric_potential2;

    for (size_t i = 0; i < electric_field_2to1.size(); i++) {
        new_electric_field2.push_back(electric_field_2to1[i] + electric_field_dip_2to1[i]);
    }

    for (size_t i = 0; i < electric_potential_2to1.size(); i++) {
        new_electric_potential2.push_back(electric_potential_2to1[i] + electric_potential_dip_2to1[i]);
    }
    // Set fields in S2
    systems[1].SetExternalElectrostaticPotentialAndFieldInSites(new_electric_potential2, new_electric_field2);

    // Calculate energy of system 1
    e1 = systems[1].Energy(true);
    e1_perm = systems[1].GetPermanentElectrostaticEnergy();
    e1_ind = systems[1].GetInducedElectrostaticEnergy();

    return 0;
}

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

int main(int argc, char** argv) {

    /////////////////////////////
    // OPTION 1: Read NRG file //
    /////////////////////////////

    // Since C++ is being used, the built-in function ReadNrg can be used
    // Even if only one system is in the NRG file, declare a vector of systems
    std::vector<bblock::System> systems;

    // The json (settings) and nrg (monomers and coordinates) files
    std::string nrg = "input.nrg";
    std::string json = "mbx_pbc.json";

    // Convert the strings to char
    char nrg_c[nrg.size() + 1];
    std::strcpy(nrg_c,nrg.c_str());

    char json_c[json.size() + 1];
    std::strcpy(json_c,json.c_str());

    // Read the file and setup the system
    // Box and PBC options will be read from JSON
    tools::ReadNrg(nrg_c, systems);
    systems[0].SetUpFromJson(json_c);

    // Get the energy
    double en = systems[0].Energy(true);

    // Print the energy
    std::cout << std::endl << "OPTION 1" << std::endl;
    std::cout << "Energy [kcal/mol] = " << std::setprecision(10) << std::scientific << en << std::endl;

    // Get the gradients
    std::vector<double> grads = systems[0].GetRealGrads();

    // Print the gradients
    std::vector<std::string> atn = systems[0].GetRealAtomNames();
    size_t n_atoms = systems[0].GetNumRealSites();
    std::cout << std::endl << "Gradients" << std::endl;
    std::cout << std::setw(5) << std::left << "Atom" << std::right << std::setw(20) << "X" << std::setw(20) << "Y" << std::setw(20) << "Z" << std::endl;
    for (size_t i = 0; i < n_atoms; i++) {
        std::cout << std::setw(5) << std::left << atn[i] << std::right <<  std::setprecision(8) << std::scientific << std::setw(20) << grads[3 * i] << std::setw(20)
                  << grads[3 * i + 1] << std::setw(20) << grads[3 * i + 2] << std::endl;
        }

    //////////////////////////////////////////
    // OPTION 2: Set up manually the system //
    //////////////////////////////////////////

    /* The setup of the system must be done, but it can either be done automatically 
     * (with the ReadNrg function) or manually by adding the monomers one by one and setting up the system
     * after all of them are in the class
     */
    
    // Get the vector of the coordinates, monomers, and number of atoms
    // If the system is set up in this way, it is assumed that the user already has them defined
    // User needs coordinates,
    std::vector<double> coordinates = systems[0].GetRealXyz();
    // a vector with the atom names
    std::vector<std::string> atom_names = systems[0].GetRealAtomNames();
    // a vector with the monomer ids
    std::vector<std::string> monomer_ids = systems[0].GetMonId();
    // and the number of atoms in each monomer
    std::vector<size_t> monomer_number_of_atoms = systems[0].GetMonNumAt();

    // The initialization step just needs to be done once
    bblock::System system_user_defined;
    int count = 0;
    for (int i = 0; i < monomer_ids.size(); i++) {
        std::vector<double> xyz(3 * monomer_number_of_atoms[i]);
        std::vector<std::string> vAtNames(monomer_number_of_atoms[i]);

        std::copy(coordinates.begin() + 3 * count, coordinates.begin() + 3 * (count + monomer_number_of_atoms[i]), xyz.begin());
        std::copy(atom_names.begin() + count, atom_names.begin() + count + monomer_number_of_atoms[i], vAtNames.begin());
        std::string id = monomer_ids[i];
        system_user_defined.AddMonomer(xyz, vAtNames, id);
        count += monomer_number_of_atoms[i];
    }
    
    system_user_defined.Initialize();
    system_user_defined.SetUpFromJson(json_c);
    
    // And afterwards it is enough to just update box (if any) and coordinates and recalculate energy
    system_user_defined.SetRealXyz(coordinates);
    // Box can be set to anything.
    std::vector<double> box = systems[0].GetBox();
    system_user_defined.SetPBC(box);

    // Get the energy
    double en2 = system_user_defined.Energy(true);
    // Print the energy
    std::cout << std::endl << "OPTION 2" << std::endl;
    std::cout << "Energy [kcal/mol] = " << std::setprecision(10) << std::scientific << en2 << std::endl;
    
    // Get the gradients
    grads = system_user_defined.GetRealGrads();
    
    // Print the gradients
    atn = system_user_defined.GetRealAtomNames();
    n_atoms = system_user_defined.GetNumRealSites();
    std::cout << std::endl << "Gradients" << std::endl; 
    std::cout << std::setw(5) << std::left << "Atom" << std::right << std::setw(20) << "X" << std::setw(20) << "Y" << std::setw(20) << "Z" << std::endl;
    for (size_t i = 0; i < n_atoms; i++) {
        std::cout << std::setw(5) << std::left << atn[i] << std::right <<  std::setprecision(8) << std::scientific << std::setw(20) << grads[3 * i] << std::setw(20)
                  << grads[3 * i + 1] << std::setw(20) << grads[3 * i + 2] << std::endl;
        }

    return 0;
}

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

/**
 * @file read_connectivity.cpp
 * @brief Contains the implementation of reading a connectivity file
 */

#include "read_connectivity.h"

namespace tools {

void ReadConnectivity(const char* filename, std::unordered_map<std::string, eff::Conn>& connectivity_map) {
    // Check filename is not empty
    assert(filename);
    std::ifstream ifs(filename);

    // Check if file name is empty string.
    if (strcmp(filename, "") == 0) {
        std::unordered_map<std::string, eff::Conn> empty_connectivity_map;
        connectivity_map = empty_connectivity_map;
        return;
    }

    // Check that filename exists
    if (!ifs) {
        std::string text =
            std::string("Could not open connectivity file ") + std::string(filename) + std::string(" for reading.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    connectivity_map = GetConnectivity(ifs);
}

std::unordered_map<std::string, eff::Conn> GetConnectivity(std::ifstream& ifs) {
    std::unordered_map<std::string, eff::Conn> connectivity_map;

    std::string str_holder;  // Used to temporarily hold variable from stringstream or getline

    // While there are still lines to be read in the connectivity file
    while (std::getline(ifs, str_holder)) {
        // Connectivity field declaration
        std::string mon_id;
        std::vector<eff::Bond> bond_vec;
        std::vector<eff::Angles> angles_vec;
        std::vector<eff::Dihedral> dihedral_vec;
        std::vector<eff::Inversion> inversion_vec;
        std::stringstream mon_id_ss(str_holder);
        mon_id_ss >> mon_id;

        // While there are still lines to be read for the monomer
        while (std::getline(ifs, str_holder)) {
            std::stringstream ss(str_holder);
            ss >> str_holder;

            // If end of monomer, exit loop
            if (str_holder == "ENDMON") {
                break;
            }

            // Variable declaration
            std::string functional_form;
            std::vector<size_t> indexes;
            std::string topology;
            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;

            // Read connectivity
            topology = str_holder;

            // Read the remaining information
            if (topology == "bond") {
                // read the first two indicies
                for (int i = 0; i < 2; i++) {
                    ss >> str_holder;
                    indexes.push_back(stoi(str_holder));
                }

                // read the functional form that we want
                ss >> functional_form;

                eff::Bond bond_obj(topology, indexes, functional_form);

                // Set the parameters
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                bond_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // clear vectors for next topology
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // Store bond object to bond vector
                bond_vec.push_back(bond_obj);
            }

            // check if the topology is an angle
            else if (topology == "angle") {
                // read the first three indicies
                for (int i = 0; i < 3; i++) {
                    ss >> str_holder;
                    indexes.push_back(stoi(str_holder));
                }

                // read the functional form that we want
                ss >> functional_form;

                eff::Angles angle_obj(topology, indexes, functional_form);

                // Set the parameters
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                angle_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next topology
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the angle object and store it in the vector of angles
                angles_vec.push_back(angle_obj);
            }

            // check if the topology is a dihedral
            else if (topology == "dihedral") {
                // read the first four indicies
                for (int i = 0; i < 4; i++) {
                    ss >> str_holder;
                    indexes.push_back(stoi(str_holder));
                }

                // read the functional form that we want
                ss >> functional_form;

                eff::Dihedral dihedral_obj(topology, indexes, functional_form);

                // Set the parameters
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                dihedral_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next topology
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the dihedral object and store it in the vector of dihedral
                dihedral_vec.push_back(dihedral_obj);
            }

            else if (topology == "inversion") {
                // read the first four indicies
                for (int i = 0; i < 4; i++) {
                    ss >> str_holder;
                    indexes.push_back(stoi(str_holder));
                }

                // read the functional form that we want
                ss >> functional_form;

                eff::Inversion inversion_obj(topology, indexes, functional_form);

                // Set the parameters
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                inversion_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next topology
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the inversion object and store it in the vector of inversions
                inversion_vec.push_back(inversion_obj);
            }

            else {
                std::string text = std::string("Unknown topology detected. " + topology +
                                               " is not a bond, angle, dihedral, or inversion");
                throw CUException(__func__, __FILE__, __LINE__, text);
            }
        }

        // Create connectivity object and add to the unordered map
        eff::Conn conn(mon_id, bond_vec, angles_vec, dihedral_vec, inversion_vec);
        connectivity_map.insert(std::make_pair(mon_id, conn));

        // Clear vectors for next monomer
        bond_vec.clear();
        angles_vec.clear();
        dihedral_vec.clear();
        inversion_vec.clear();
    }

    return connectivity_map;
}

void ReadParameters(std::vector<double>& linear_parameters, std::vector<double>& nonlinear_parameters,
                    std::string functional_form, std::stringstream& ss) {
    try {
        std::string tmp;
        if (functional_form == "harm" || functional_form == "hcos") {
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            nonlinear_parameters.push_back(stod(tmp));
        } else if (functional_form == "morse" || functional_form == "cos") {
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            nonlinear_parameters.push_back(stod(tmp));
            ss >> tmp;
            nonlinear_parameters.push_back(stod(tmp));
        } else if (functional_form == "cos3") {
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
        } else if (functional_form == "quartic") {
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            nonlinear_parameters.push_back(stod(tmp));
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
            ss >> tmp;
            linear_parameters.push_back(stod(tmp));
        } else if (functional_form == "none") {
        } else {
            std::string text = std::string("Unknown functional form in connnectivity file");
            throw CUException(__func__, __FILE__, __LINE__, text);
        }
    } catch (CUException& e) {
        std::string text = std::string(
            "Tried to convert invalid parameter value while reading connectivity. Parameters can not be too small "
            "(e-316) otherwise they can't be converted to double");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }
}

}  // namespace tools

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
void ReadConnectivity(char* filename) {
    // std::unordered_map<std::string, connectivity::Conn> connectivity_map;

    // Check filename is not empty
    assert(filename);
    std::ifstream ifs(filename);

    // Check that filename exists
    if (!ifs) {
        std::string text =
            std::string("Could not open connectivity file ") + std::string(filename) + std::string(" for reading.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    std::unordered_map<std::string, connectivity::Conn> connectivity_map = GetConnectivity(ifs);

    // std::string tmp;  // Used to temporarily hold variable from stringstream or getline
    // std::getline(ifs, tmp);

    // // Loop over each monomer
    // do {
    //     // Connectivity field declaration
    //     std::string mon_id;
    //     std::vector<Bond> bond_vec;
    //     std::vector<Angles> angles_vec;
    //     std::vector<Dihedral> dihedral_vec;
    //     std::vector<Inversion> inversion_vec;
    //     std::stringstream mon_id_ss(tmp);
    //     mon_id_ss >> mon_id;

    //     std::getline(ifs, tmp);

    //     // Loop over all of the connectivity within a monomer
    //     do {
    //         std::stringstream ss(tmp); /* = std::stringstream(tmp);*/
    //         ss >> tmp;

    //         // If end of monomer, exit loop
    //         if (tmp == "ENDMON") {
    //             break;
    //         }

    //         // Variable declaration
    //         std::string functional_form;
    //         std::vector<size_t> indexes;
    //         size_t connectivity_type;
    //         std::string connectivity;
    //         bool linear;
    //         std::vector<double> linear_parameters;
    //         std::vector<double> nonlinear_parameters;

    //         // Read connectivity
    //         connectivity = tmp;

    //         // Read the remaining information
    //         if (connectivity == "bond") {
    //             // read the first two indicies
    //             for (int i = 0; i < 2; i++) {
    //                 ss >> tmp;
    //                 indexes.push_back(stoi(tmp));
    //             }

    //             // read the bond type
    //             ss >> connectivity_type;

    //             // read the functional form that we want
    //             ss >> functional_form;

    //             // Read if linear fit
    //             ss >> tmp;
    //             linear = (tmp == "1") ? true : false;

    //             Bond bond_obj(connectivity, connectivity_type, indexes, functional_form);

    //             // Set the parameters and linear flag
    //             bond_obj.SetLinearFlag(linear);
    //             ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
    //             bond_obj.SetParameters(linear_parameters, nonlinear_parameters);

    //             // clear vectors for next connectivity
    //             linear_parameters.clear();
    //             nonlinear_parameters.clear();
    //             indexes.clear();

    //             // Store bond object to bond vector
    //             bond_vec.push_back(bond_obj);
    //         }

    //         // check if the topology is an angle
    //         else if (connectivity == "angle") {
    //             // read the first three indicies
    //             for (int i = 0; i < 3; i++) {
    //                 ss >> tmp;
    //                 indexes.push_back(stoi(tmp));
    //             }

    //             // read the angle type
    //             ss >> connectivity_type;

    //             // read the functional form that we want
    //             ss >> functional_form;

    //             // Read if linear fit
    //             ss >> tmp;
    //             linear = (tmp == "1") ? true : false;

    //             Angles angle_obj(connectivity, connectivity_type, indexes, functional_form);

    //             // Set the linear flag.
    //             angle_obj.SetLinearFlag(linear);

    //             ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
    //             angle_obj.SetParameters(linear_parameters, nonlinear_parameters);

    //             // Clear vectors for next connectivity
    //             linear_parameters.clear();
    //             nonlinear_parameters.clear();
    //             indexes.clear();

    //             // create the angle object and store it in the vector of angles
    //             angles_vec.push_back(angle_obj);
    //         }

    //         // check if the topology is a dihedral
    //         else if (connectivity == "dihedral") {
    //             // read the first four indicies
    //             for (int i = 0; i < 4; i++) {
    //                 ss >> tmp;
    //                 indexes.push_back(stoi(tmp));
    //             }

    //             // read the dihedral type
    //             ss >> connectivity_type;

    //             // read the functional form that we want
    //             ss >> functional_form;

    //             // read linear flag
    //             ss >> tmp;
    //             linear = (tmp == "1") ? true : false;

    //             // Read include or exclude dihedral
    //             ss >> tmp;

    //             Dihedral dihedral_obj(connectivity, connectivity_type, indexes, functional_form);

    //             // Set the parameters and flag
    //             dihedral_obj.SetLinearFlag(linear);
    //             ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
    //             dihedral_obj.SetParameters(linear_parameters, nonlinear_parameters);

    //             // Clear vectors for next connectivity
    //             linear_parameters.clear();
    //             nonlinear_parameters.clear();
    //             indexes.clear();

    //             // create the dihedral object and store it in the vector of dihedral
    //             dihedral_vec.push_back(dihedral_obj);
    //         }

    //         else if (connectivity == "inversion") {
    //             // read the first four indicies
    //             for (int i = 0; i < 4; i++) {
    //                 ss >> tmp;
    //                 indexes.push_back(stoi(tmp));
    //             }

    //             // read the inversion type
    //             ss >> connectivity_type;

    //             // read the functional form that we want
    //             ss >> functional_form;

    //             // read linear flag
    //             ss >> tmp;
    //             linear = (tmp == "1") ? true : false;

    //             Inversion inversion_obj(connectivity, connectivity_type, indexes, functional_form);

    //             // Set the parameters and flag
    //             inversion_obj.SetLinearFlag(linear);
    //             ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
    //             inversion_obj.SetParameters(linear_parameters, nonlinear_parameters);

    //             // Clear vectors for next connectivity
    //             linear_parameters.clear();
    //             nonlinear_parameters.clear();
    //             indexes.clear();

    //             // create the inversion object and store it in the vector of inversions
    //             inversion_vec.push_back(inversion_obj);
    //         }

    //         else {
    //             std::string text = std::string("Unknown connectivity detected. " + connectivity +
    //                                            " is not a bond, angle, dihedral, or inversion");
    //             throw CUException(__func__, __FILE__, __LINE__, text);
    //         }
    //     } while (std::getline(ifs, tmp));

    //     // Create connectivity object and add to the unordered map
    //     connectivity::Conn conn(mon_id, bond_vec, angles_vec, dihedral_vec, inversion_vec);
    //     connectivity_map.insert(std::make_pair(mon_id, conn));

    //     // Clear vectors for next monomer
    //     bond_vec.clear();
    //     angles_vec.clear();
    //     dihedral_vec.clear();
    //     inversion_vec.clear();

    // } while (std::getline(ifs, tmp));

    bblock::System::SetConnectivity(connectivity_map);
}

void ReadConnectivity(char* filename, std::unordered_map<std::string, connectivity::Conn>& connectivity_map) {
    // Check filename is not empty
    assert(filename);
    std::ifstream ifs(filename);

    // Check that filename exists
    if (!ifs) {
        std::string text =
            std::string("Could not open connectivity file ") + std::string(filename) + std::string(" for reading.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    connectivity_map = GetConnectivity(ifs);
}

std::unordered_map<std::string, connectivity::Conn> GetConnectivity(std::ifstream& ifs) {
    std::unordered_map<std::string, connectivity::Conn> connectivity_map;

    std::string tmp;  // Used to temporarily hold variable from stringstream or getline
    std::getline(ifs, tmp);

    // Loop over each monomer
    do {
        // Connectivity field declaration
        std::string mon_id;
        std::vector<Bond> bond_vec;
        std::vector<Angles> angles_vec;
        std::vector<Dihedral> dihedral_vec;
        std::vector<Inversion> inversion_vec;
        std::stringstream mon_id_ss(tmp);
        mon_id_ss >> mon_id;

        std::getline(ifs, tmp);

        // Loop over all of the connectivity within a monomer
        do {
            std::stringstream ss(tmp); /* = std::stringstream(tmp);*/
            ss >> tmp;

            // If end of monomer, exit loop
            if (tmp == "ENDMON") {
                break;
            }

            // Variable declaration
            std::string functional_form;
            std::vector<size_t> indexes;
            size_t connectivity_type;
            std::string connectivity;
            bool linear;
            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;

            // Read connectivity
            connectivity = tmp;

            // Read the remaining information
            if (connectivity == "bond") {
                // read the first two indicies
                for (int i = 0; i < 2; i++) {
                    ss >> tmp;
                    indexes.push_back(stoi(tmp));
                }

                // read the bond type
                ss >> connectivity_type;

                // read the functional form that we want
                ss >> functional_form;

                // Read if linear fit
                ss >> tmp;
                linear = (tmp == "1") ? true : false;

                Bond bond_obj(connectivity, connectivity_type, indexes, functional_form);

                // Set the parameters and linear flag
                bond_obj.SetLinearFlag(linear);
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                bond_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // clear vectors for next connectivity
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // Store bond object to bond vector
                bond_vec.push_back(bond_obj);
            }

            // check if the topology is an angle
            else if (connectivity == "angle") {
                // read the first three indicies
                for (int i = 0; i < 3; i++) {
                    ss >> tmp;
                    indexes.push_back(stoi(tmp));
                }

                // read the angle type
                ss >> connectivity_type;

                // read the functional form that we want
                ss >> functional_form;

                // Read if linear fit
                ss >> tmp;
                linear = (tmp == "1") ? true : false;

                Angles angle_obj(connectivity, connectivity_type, indexes, functional_form);

                // Set the linear flag.
                angle_obj.SetLinearFlag(linear);

                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                angle_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next connectivity
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the angle object and store it in the vector of angles
                angles_vec.push_back(angle_obj);
            }

            // check if the topology is a dihedral
            else if (connectivity == "dihedral") {
                // read the first four indicies
                for (int i = 0; i < 4; i++) {
                    ss >> tmp;
                    indexes.push_back(stoi(tmp));
                }

                // read the dihedral type
                ss >> connectivity_type;

                // read the functional form that we want
                ss >> functional_form;

                // read linear flag
                ss >> tmp;
                linear = (tmp == "1") ? true : false;

                // Read include or exclude dihedral
                ss >> tmp;

                Dihedral dihedral_obj(connectivity, connectivity_type, indexes, functional_form);

                // Set the parameters and flag
                dihedral_obj.SetLinearFlag(linear);
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                dihedral_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next connectivity
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the dihedral object and store it in the vector of dihedral
                dihedral_vec.push_back(dihedral_obj);
            }

            else if (connectivity == "inversion") {
                // read the first four indicies
                for (int i = 0; i < 4; i++) {
                    ss >> tmp;
                    indexes.push_back(stoi(tmp));
                }

                // read the inversion type
                ss >> connectivity_type;

                // read the functional form that we want
                ss >> functional_form;

                // read linear flag
                ss >> tmp;
                linear = (tmp == "1") ? true : false;

                Inversion inversion_obj(connectivity, connectivity_type, indexes, functional_form);

                // Set the parameters and flag
                inversion_obj.SetLinearFlag(linear);
                ReadParameters(linear_parameters, nonlinear_parameters, functional_form, ss);
                inversion_obj.SetParameters(linear_parameters, nonlinear_parameters);

                // Clear vectors for next connectivity
                linear_parameters.clear();
                nonlinear_parameters.clear();
                indexes.clear();

                // create the inversion object and store it in the vector of inversions
                inversion_vec.push_back(inversion_obj);
            }

            else {
                std::string text = std::string("Unknown connectivity detected. " + connectivity +
                                               " is not a bond, angle, dihedral, or inversion");
                throw CUException(__func__, __FILE__, __LINE__, text);
            }
        } while (std::getline(ifs, tmp));

        // Create connectivity object and add to the unordered map
        connectivity::Conn conn(mon_id, bond_vec, angles_vec, dihedral_vec, inversion_vec);
        connectivity_map.insert(std::make_pair(mon_id, conn));

        // Clear vectors for next monomer
        bond_vec.clear();
        angles_vec.clear();
        dihedral_vec.clear();
        inversion_vec.clear();

    } while (std::getline(ifs, tmp));

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
        } else if (functional_form == "plan") {
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

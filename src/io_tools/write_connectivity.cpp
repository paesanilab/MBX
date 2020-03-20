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

#include "write_connectivity.h"

/**
 * @file write_connectivity.cpp
 * @brief Contains the function implementations of the write connectivity header
 * file
 */

namespace tools {

void WriteConnectivity(char* filename, std::unordered_map<std::string, eff::Conn> connectivity_map) {
    // Ensure that filename is not empty
    assert(filename);

    // Ensure that output file can be opened
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("could not open connectivity file for writting");

    // Save system information in energy file
    SaveConnectivity(ofs, connectivity_map);
    return;
}

void SaveConnectivity(std::ostream& os, std::unordered_map<std::string, eff::Conn> connectivity_map) {
    // Loop over all of the keys and values in the map
    for (auto conn_iter = connectivity_map.begin(); conn_iter != connectivity_map.end(); conn_iter++) {
        // Write the mon_id
        os << conn_iter->first << std::endl;
        eff::Conn curr_conn = conn_iter->second;
        // Write the bonds, angles, dihedrals, and inversions
        std::vector<eff::Bond> bonds = curr_conn.GetBondVec();
        // Loop over all the bond objects
        for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
            os << bond->GetTopology() << " ";
            std::vector<size_t> indexes = bond->GetIndexes();

            // Loop over indexes
            for (size_t idx : indexes) {
                os << idx << " ";
            }

            std::string functional_form = bond->GetFunctionalForm();
            os << functional_form << " ";

            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;
            bond->GetParameters(linear_parameters, nonlinear_parameters);
            WriteParameters(os, linear_parameters, nonlinear_parameters, functional_form);
            os << std::endl;
        }

        std::vector<eff::Angles> angles = curr_conn.GetAnglesVec();
        // Loop over all the angle objects
        for (auto angle = angles.begin(); angle != angles.end(); angle++) {
            os << angle->GetTopology() << " ";
            std::vector<size_t> indexes = angle->GetIndexes();

            // Loop over indexes
            for (size_t idx : indexes) {
                os << idx << " ";
            }

            std::string functional_form = angle->GetFunctionalForm();
            os << functional_form << " ";

            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;
            angle->GetParameters(linear_parameters, nonlinear_parameters);
            WriteParameters(os, linear_parameters, nonlinear_parameters, functional_form);
            os << std::endl;
        }

        std::vector<eff::Dihedral> dihedrals = curr_conn.GetDihedralVec();
        // Loop over all the angle objects
        for (auto dihedral = dihedrals.begin(); dihedral != dihedrals.end(); dihedral++) {
            os << dihedral->GetTopology() << " ";
            std::vector<size_t> indexes = dihedral->GetIndexes();

            // Loop over indexes
            for (size_t idx : indexes) {
                os << idx << " ";
            }

            std::string functional_form = dihedral->GetFunctionalForm();
            os << functional_form << " ";
            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;
            dihedral->GetParameters(linear_parameters, nonlinear_parameters);
            WriteParameters(os, linear_parameters, nonlinear_parameters, functional_form);
            os << std::endl;
        }

        std::vector<eff::Inversion> inversions = curr_conn.GetInversionVec();
        for (auto inversion = inversions.begin(); inversion != inversions.end(); inversion++) {
            os << inversion->GetTopology() << " ";
            std::vector<size_t> indexes = inversion->GetIndexes();

            // Loop over indexes
            for (size_t idx : indexes) {
                os << idx << " ";
            }

            std::string functional_form = inversion->GetFunctionalForm();
            os << functional_form << " ";
            std::vector<double> linear_parameters;
            std::vector<double> nonlinear_parameters;
            inversion->GetParameters(linear_parameters, nonlinear_parameters);
            WriteParameters(os, linear_parameters, nonlinear_parameters, functional_form);
            os << std::endl;
        }

        os << "ENDMON" << std::endl;
    }
}

void WriteParameters(std::ostream& os, std::vector<double>& linear_parameters,
                     std::vector<double>& nonlinear_parameters, std::string functional_form) {
    os << std::scientific;
    os << std::setprecision(3);

    if (functional_form == "harm" || functional_form == "hcos") {
        os << linear_parameters[0] << " ";
        os << nonlinear_parameters[0];
    } else if (functional_form == "morse" || functional_form == "cos") {
        os << linear_parameters[0] << " ";
        os << nonlinear_parameters[0] << " ";
        os << nonlinear_parameters[1];
    } else if (functional_form == "cos3") {
        os << linear_parameters[0] << " ";
        os << linear_parameters[1] << " ";
        os << linear_parameters[2];
    } else if (functional_form == "quartic") {
        os << linear_parameters[0] << " ";
        os << nonlinear_parameters[0] << " ";
        os << linear_parameters[1] << " ";
        os << linear_parameters[2];
    } else if (functional_form == "none") {
        os << " ";
    } else {
        std::string text = std::string("Unknown functional form in connnectivity file");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }
}

}  // namespace tools

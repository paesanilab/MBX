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

#include "energy4b.h"

namespace e4b {

double get_4b_energy(std::string mon1, std::string mon2, std::string mon3, std::string mon4, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2, std::vector<double> xyz3,
                     std::vector<double> xyz4) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Tetramer " << mon1 << " -- " << mon2 << " -- " << mon3 << " -- " << mon4 << ":\n";
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon2 << ":\n";
    for (size_t i = 0; i < xyz2.size(); i++) {
        std::cerr << xyz2[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon3 << ":\n";
    for (size_t i = 0; i < xyz3.size(); i++) {
        std::cerr << xyz3[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon4 << ":\n";
    for (size_t i = 0; i < xyz4.size(); i++) {
        std::cerr << xyz4[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    // Order the four monomer names and corresponding xyz
    // bubble sort
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
    }
    if (mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
    }
    if (mon3 > mon4) {
        std::swap(mon3, mon4);
        std::swap(xyz3, xyz4);
    }
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
    }
    if (mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
    }
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
    }

    double energy = 0.0;

    if (false) {
        energy = 0.0;
        // =====>> BEGIN SECTION 4B_NO_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o" and mon4 == "h2o") {
        mbnrg_A1B2_A1B2_A1B2_A1B2_deg4::mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1 pot(mon1, mon2, mon3, mon4);
        energy = pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), xyz4.data(), nm);

        // =====>> END SECTION 4B_NO_GRADIENT <<=====
    } else {
        energy = 0.0;
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Tetramer " << mon1 << " -- " << mon2 << " -- " << mon3 << " -- " << mon4 << ":\n";
    std::cerr << "Output energy: " << energy << std::endl;
#endif

    return energy;
}

double get_4b_energy(std::string mon1, std::string mon2, std::string mon3, std::string mon4, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2, std::vector<double> xyz3,
                     std::vector<double> xyz4, std::vector<double> &grad1, std::vector<double> &grad2,
                     std::vector<double> &grad3, std::vector<double> &grad4, std::vector<double> *virial) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Tetramer " << mon1 << " -- " << mon2 << " -- " << mon3 << " -- " << mon4 << ":\n";
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon2 << ":\n";
    for (size_t i = 0; i < xyz2.size(); i++) {
        std::cerr << xyz2[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon3 << ":\n";
    for (size_t i = 0; i < xyz3.size(); i++) {
        std::cerr << xyz3[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon4 << ":\n";
    for (size_t i = 0; i < xyz4.size(); i++) {
        std::cerr << xyz4[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input gradients for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input gradients for " << nm << " monomers of type " << mon2 << ":\n";
    for (size_t i = 0; i < grad2.size(); i++) {
        std::cerr << grad2[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input gradients for " << nm << " monomers of type " << mon3 << ":\n";
    for (size_t i = 0; i < grad3.size(); i++) {
        std::cerr << grad3[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input gradients for " << nm << " monomers of type " << mon4 << ":\n";
    for (size_t i = 0; i < grad4.size(); i++) {
        std::cerr << grad4[i] << " , ";
    }
    std::cerr << std::endl;
    if (virial != 0) {
        std::cerr << "Input virial:\n";
        for (size_t i = 0; i < 9; i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }
#endif

    size_t index1(1), index2(2), index3(3), index4(4);

    // Order the three monomer names and corresponding xyz
    // bubble sort
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
        std::swap(grad1, grad2);
        std::swap(index1, index2);
    }
    if (mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
        std::swap(grad2, grad3);
        std::swap(index2, index3);
    }
    if (mon3 > mon4) {
        std::swap(mon3, mon4);
        std::swap(xyz3, xyz4);
        std::swap(grad3, grad4);
        std::swap(index3, index4);
    }
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
        std::swap(grad1, grad2);
        std::swap(index1, index2);
    }
    if (mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
        std::swap(grad2, grad3);
        std::swap(index2, index3);
    }
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
        std::swap(grad1, grad2);
        std::swap(index1, index2);
    }

    double energy = 0.0;
    // Note: in the conditional, mon2 >= mon1 ALWAYS
    if (false) {
        energy = 0.0;
        // =====>> BEGIN SECTION 4B_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o" and mon4 == "h2o") {
        mbnrg_A1B2_A1B2_A1B2_A1B2_deg4::mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1 pot(mon1, mon2, mon3, mon4);
        energy = pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), xyz4.data(), grad1.data(), grad2.data(), grad3.data(),
                          grad4.data(), nm, virial);
        // =====>> END SECTION 4B_GRADIENT <<=====
    } else {
        energy = 0.0;
    }

    if (index1 != 1) {
        if (index2 == 1) {
            std::swap(index1, index2);
            std::swap(grad1, grad2);
            std::swap(mon1, mon2);
        } else if (index3 == 1) {
            std::swap(index1, index3);
            std::swap(grad1, grad3);
            std::swap(mon1, mon3);
        } else if (index4 == 1) {
            std::swap(index1, index4);
            std::swap(grad1, grad4);
            std::swap(mon1, mon4);
        }  // else { error
    }

    if (index2 != 2) {
        if (index3 == 2) {
            std::swap(index2, index3);
            std::swap(grad2, grad3);
            std::swap(mon2, mon3);
        } else if (index4 == 2) {
            std::swap(index2, index4);
            std::swap(grad2, grad4);
            std::swap(mon2, mon4);
        }  // else { error
    }

    if (index3 != 3) {
        std::swap(index3, index4);
        std::swap(grad3, grad4);
        std::swap(mon3, mon4);
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Tetramer " << mon1 << " -- " << mon2 << " -- " << mon3 << " -- " << mon4 << ":\n";
    std::cerr << "Output gradients for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output gradients for " << nm << " monomers of type " << mon2 << ":\n";
    for (size_t i = 0; i < grad2.size(); i++) {
        std::cerr << grad2[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output gradients for " << nm << " monomers of type " << mon3 << ":\n";
    for (size_t i = 0; i < grad3.size(); i++) {
        std::cerr << grad3[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output gradients for " << nm << " monomers of type " << mon4 << ":\n";
    for (size_t i = 0; i < grad4.size(); i++) {
        std::cerr << grad4[i] << " , ";
    }
    std::cerr << std::endl;
    if (virial != 0) {
        std::cerr << "Output virial:\n";
        for (size_t i = 0; i < 9; i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output energy: " << energy << std::endl;
#endif

    return energy;
}

}  // namespace e4b

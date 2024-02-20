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

#include "Catch2/single_include/catch.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#ifndef TESTUTILS_H
#define TESTUTILS_H
bool VectorsAreEqual(const std::vector<double>& computed, const std::vector<double>& expected,
                     double tolerance = 1e-6) {
    return std::equal(computed.begin(), computed.end(), expected.begin(), [&](double x, double y) -> bool {
        std::cout << std::scientific << std::setprecision(8) << std::setw(20) << x << " <==> " << std::setw(20) << y
                  << std::endl;
        return x == Approx(y).margin(tolerance);
    });
}

template <typename T, typename U>
bool VectorsAreEqual(const std::vector<std::pair<T, U>>& computed, const std::vector<std::pair<T, U>>& expected) {
    return std::equal(computed.begin(), computed.end(), expected.begin(),
                      [&](std::pair<T, U> x, std::pair<T, U> y) -> bool {
                          std::cout << "{" << x.first << " , " << x.second << "} <==> {" << y.first << " , " << y.second
                                    << "}" << std::endl;
                          return x == y;
                      });
}

template <typename T>
bool VectorsAreEqual(const std::vector<T>& computed, const std::vector<T>& expected) {
    return std::equal(computed.begin(), computed.end(), expected.begin(), [&](T x, T y) -> bool {
        std::cout << x << " <==> " << y << std::endl;
        return x == y;
    });
}

class TestEnergyData {
   public:
    TestEnergyData() {}
    ~TestEnergyData() {}
    double total_energy;
    std::vector<double> real_grad;
    std::vector<double> grad;
    std::vector<double> virial;
    double energy_1b;
    std::vector<double> real_grad_1b;
    std::vector<double> grad_1b;
    std::vector<double> virial_1b;
    double energy_2b;
    std::vector<double> real_grad_2b;
    std::vector<double> grad_2b;
    std::vector<double> virial_2b;
    double energy_3b;
    std::vector<double> real_grad_3b;
    std::vector<double> grad_3b;
    std::vector<double> virial_3b;
    double energy_disp;
    std::vector<double> real_grad_disp;
    std::vector<double> grad_disp;
    std::vector<double> virial_disp;
    double energy_buck;
    std::vector<double> real_grad_buck;
    std::vector<double> grad_buck;
    std::vector<double> virial_buck;
    double energy_lj;
    std::vector<double> real_grad_lj;
    std::vector<double> grad_lj;
    std::vector<double> virial_lj;
    double energy_ff;
    std::vector<double> real_grad_ff;
    std::vector<double> grad_ff;
    std::vector<double> virial_ff;
    std::string json_mbx;
    std::string json_mon;
    std::string json_repdisp;
    std::vector<double> coords;
    std::vector<double> real_coords;
    std::vector<std::string> atom_names;
    std::vector<std::string> real_atom_names;
    std::vector<std::string> monomer_names;
    std::vector<size_t> first_index;
    std::vector<size_t> real_first_index;
    std::vector<size_t> natoms;
    std::vector<size_t> nsites;
    size_t tot_sites;
    size_t tot_atoms;
};
#endif

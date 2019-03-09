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

#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

#define REL_TOL 1E-06
namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

void CompareEnergies(std::vector<double> en_ref, std::vector<double> en, std::string test, int& exitcode) {
    if (en_ref.size() != en.size()) {
        std::cerr << " ** Error ** : "
                  << "Energy vectors do not have the same size.\n";
        exitcode = 1;
        return;
    }

    for (size_t i = 0; i < en.size(); i++) {
        double diff = std::abs(en_ref[i] - en[i]);
        if (diff > REL_TOL) {
            std::cerr << " ** Error ** : "
                      << " Energies in test: \" " << test << " \" do not match for system[" << i << "]\n";
            std::cerr << en_ref[i] << " vs. " << en[i] << std::endl;
            exitcode = 1;
        }
    }
}

void CompareGrads(std::vector<double> grad_ref, std::vector<double> grad, std::string test, size_t sys_index,
                  int& exitcode) {
    if (grad_ref.size() != grad.size()) {
        std::cerr << " ** Error ** : "
                  << "Gradient vectors do not have the same size.\n";
        exitcode = 1;
        return;
    }

    for (size_t i = 0; i < grad.size(); i++) {
        double diff = std::abs(grad_ref[i] - grad[i]);
        if (diff > REL_TOL) {
            std::cerr << " ** Error ** : "
                      << " Energies in test: \" " << test << " \" do not match for system[" << sys_index << "]\n";
            std::cerr << grad_ref[i] << " vs. " << grad[i] << std::endl;
            exitcode = 1;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: energy input.nrg" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // Declare vectors for all systems
    std::vector<std::vector<double>> grads(systems.size(), std::vector<double>(1, 0.0));
    std::vector<double> energy_nograd(systems.size(), 0.0);
    std::vector<double> energy_grad(systems.size(), 0.0);
    std::vector<double> grad;

    std::vector<double> e_nograd_test(systems.size(), 0.0);
    std::vector<double> e_grad_test(systems.size(), 0.0);

    // Declare return code
    int exit_code = 0;

    // No gradient energy. Save energies in vector
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetDipoleMethod("cg");
        energy_nograd[i] = systems[i].Energy(false);
    }

    // Gradient energy. Save energies and gradients in vector
    for (size_t i = 0; i < systems.size(); i++) {
        energy_grad[i] = systems[i].Energy(true);
        grads[i] = systems[i].GetGrads();
    }

    // CompareEnergies energies with and without grads
    std::string testcase = "Gradient energies vs. no gradient energies";
    CompareEnergies(energy_nograd, energy_grad, testcase, exit_code);

    // Try different max number of evaluation of monomers, dimers, and trimers
    // First set all to 1
    testcase = "All NMaxEval and steps set to 1";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetNMaxEval1b(1);
        systems[i].SetNMaxEval2b(1);
        systems[i].SetNMaxEval3b(1);
        e_nograd_test[i] = systems[i].Energy(false);
    }
    CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);

    // Step > NMaxEval
    testcase = "All NMaxEval set to 1 and Steps set to 3";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetNMaxEval1b(1);
        systems[i].SetNMaxEval2b(1);
        systems[i].SetNMaxEval3b(1);
        e_nograd_test[i] = systems[i].Energy(false);
    }
    CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);

    // NmaxEval > step
    testcase = "All NMaxEval set to 3 and Steps set to 2";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetNMaxEval1b(3);
        systems[i].SetNMaxEval2b(3);
        systems[i].SetNMaxEval3b(3);
        e_nograd_test[i] = systems[i].Energy(false);
    }
    CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);

    // Gradients with different values for steps
    testcase = "Gradients with different values for steps and NMaxEval";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetNMaxEval1b(4);
        systems[i].SetNMaxEval2b(3);
        systems[i].SetNMaxEval3b(4);
        e_grad_test[i] = systems[i].Energy(true);
        grad = systems[i].GetGrads();
        CompareGrads(grads[i], grad, testcase, i, exit_code);
    }
    testcase = "Energies (wgrad) with different values for steps and NMaxEval";
    CompareEnergies(energy_grad, e_grad_test, testcase, exit_code);

    // Try a smaller dipole tolerance, increasing the number of iterations
    testcase = "Gradients with smaller dipole tolerance";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetDipoleTol(1E-18);
        systems[i].SetDipoleMaxIt(200);
        e_grad_test[i] = systems[i].Energy(true);
        grad = systems[i].GetGrads();
        CompareGrads(grads[i], grad, testcase, i, exit_code);
    }
    testcase = "Energies (wgrad) with maller dipole tolerance";
    CompareEnergies(energy_grad, e_grad_test, testcase, exit_code);

    testcase = "ASPC energy for 10 iterations";
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetDipoleMethod("aspc");
        double energy_aspc = systems[i].Energy(false);
        std::vector<double> evec(10, energy_aspc);
        std::vector<double> evec_test(10);
        for (size_t j = 0; j < 10; j++) {
            evec_test[j] = systems[i].Energy(false);
        }
        CompareEnergies(evec, evec_test, testcase, exit_code);
    }

    if (exit_code == 0) {
        std::cout << "All tests passed!\n";
    }

    return exit_code;
}

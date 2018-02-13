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

#define REL_TOL 1E-08
//#define PRINT_GRADS
//#define NUM_GRADS
namespace {

static std::vector<bblock::System> systems;

} // namespace

////////////////////////////////////////////////////////////////////////////////

void CompareEnergies(std::vector<double> en_ref, std::vector<double> en,
             std::string test, int &exitcode) {
  if (en_ref.size() != en.size()) {
    std::cerr << " ** Error ** : " << "Energy vectors do not have the same size.\n";
    exitcode = 1;
    return;
  }

  for (size_t i = 0; i < en.size(); i++) {
    double diff = std::abs(en_ref[i] - en[i]);
    if (diff > REL_TOL) {
      std::cerr << " ** Error ** : " << " Energies in test: \" " << test
                << " \" do not match for system[" << i << "]\n";
      std::cerr << en_ref[i] << " vs. " << en[i] << std::endl;
      exitcode = 1;
    }
  }
}

void CompareGrads(std::vector<double> grad_ref, std::vector<double> grad,
                  std::string test, size_t sys_index, int &exitcode) {
  if (grad_ref.size() != grad.size()) {
    std::cerr << " ** Error ** : " << "Gradient vectors do not have the same size.\n";
    exitcode = 1;
    return;
  }

  for (size_t i = 0; i < grad.size(); i++) {
    double diff = std::abs(grad_ref[i] - grad[i]);
    if (diff > REL_TOL) {
      std::cerr << " ** Error ** : " << " Energies in test: \" " << test
                << " \" do not match for system[" << sys_index << "]\n";
      std::cerr << grad_ref[i] << " vs. " << grad[i] << std::endl;
      exitcode = 1;
    }
  }
}



int main(int argc, char** argv)
{

  if (argc != 2) {
    std::cerr << "usage: energy input.nrg"
              << std::endl;
    return 0;
  }

  try {
    std::ifstream ifs(argv[1]);

    if (!ifs){
      throw std::runtime_error("could not open the NRG file");
    }

    tools::ReadNrg(argv[1], systems);
  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }

  // Declare vectors for all systems
  std::vector<std::vector<double>> grads(systems.size(),std::vector<double>(1,0.0));
  std::vector<double> energy_nograd(systems.size(),0.0);
  std::vector<double> energy_grad(systems.size(),0.0);
  std::vector<double> grad;

  std::vector<double> e_nograd_test(systems.size(),0.0);
  std::vector<double> e_grad_test(systems.size(),0.0);

  // Declare return code
  int exit_code = 0;
  
  // No gradient energy. Save energies in vector
  for (size_t i = 0; i < systems.size(); i++) {
    energy_nograd[i] = systems[i].Energy(grad, false);
  }

  // Gradient energy. Save energies and gradients in vector
  for (size_t i = 0; i < systems.size(); i++) {
    energy_grad[i] = systems[i].Energy(grad, true);
    grads[i] = grad;
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
    systems[i].SetStepEval2b(1);
    systems[i].SetStepEval3b(1);
    e_nograd_test[i] = systems[i].Energy(grad, false);
  } 
  CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);
  
  // Step > NMaxEval
  testcase = "All NMaxEval set to 1 and Steps set to 3";
  for (size_t i = 0; i < systems.size(); i++) {
    systems[i].SetNMaxEval1b(1);
    systems[i].SetNMaxEval2b(1);
    systems[i].SetNMaxEval3b(1);
    systems[i].SetStepEval2b(3);
    systems[i].SetStepEval3b(3);
    e_nograd_test[i] = systems[i].Energy(grad, false);
  } 
  CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);

  // NmaxEval > step
  testcase = "All NMaxEval set to 3 and Steps set to 2";
  for (size_t i = 0; i < systems.size(); i++) {
    systems[i].SetNMaxEval1b(3);
    systems[i].SetNMaxEval2b(3);
    systems[i].SetNMaxEval3b(3);
    systems[i].SetStepEval2b(2);
    systems[i].SetStepEval3b(2);
    e_nograd_test[i] = systems[i].Energy(grad, false);
  } 
  CompareEnergies(energy_nograd, e_nograd_test, testcase, exit_code);

  // Gradients with different values for steps
  testcase = "Gradients with different values for steps and NMaxEval";
  for (size_t i = 0; i < systems.size(); i++) {
    systems[i].SetNMaxEval1b(4);
    systems[i].SetNMaxEval2b(3);
    systems[i].SetNMaxEval3b(4);
    systems[i].SetStepEval2b(2);
    systems[i].SetStepEval3b(5);
    e_grad_test[i] = systems[i].Energy(grad, true);
    CompareGrads(grads[i], grad, testcase, i, exit_code);
  } 
  testcase = "Energies (wgrad) with different values for steps and NMaxEval";
  CompareEnergies(energy_grad, e_grad_test, testcase, exit_code);
  
  if (exit_code == 0) {
    std::cout << "All tests passed!\n";
  }

  return exit_code;
}

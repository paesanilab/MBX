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

//#define PRINT_XYZ_PBC_TEST

namespace {

static std::vector<bblock::System> systems;
//static double tol = 1E-06;

} // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

  if (argc != 2) {
    std::cerr << "usage: energy h2o_ion.nrg"
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

  std::vector<double> g;
  std::vector<double> box(9,0.0);
  box[0] = 100.0;
  box[4] = 100.0;
  box[8] = 100.0;
  std::vector<bblock::System> systems_copy = systems;

  // Compare energies
  for (size_t i = 0; i < systems.size(); i++) {
    // Get NB energy for no_pbc and for pbc
    // First no PBC
    std::vector<double> xyz = systems[i].GetRealXyz();
    std::vector<std::string> ats = systems[i].GetAtomNames();
    double e1b = systems[i].OneBodyEnergy(false);
    double e2b = systems[i].TwoBodyEnergy(false);
    double e3b = systems[i].ThreeBodyEnergy(false);

    // Then PBC
    systems[i].SetPBC(true,box);
    std::vector<double> xyz_pbc = systems[i].GetRealXyz();
    double e1b_pbc = systems[i].OneBodyEnergy(false);
    double e2b_pbc = systems[i].TwoBodyEnergy(false);
    double e3b_pbc = systems[i].ThreeBodyEnergy(false);

    // Print output and differences
    std::cout << std::setprecision(6) << std::scientific
              << "system["  << std::setfill('.')
              << std::setw(10) << i 
              << std::setw(12) << "] e1b(nopbc)= " << std::setfill(' ')
              << std::setw(15) << std::right <<  e1b << std::setfill(' ')
              << std::setw(12) << " e1b(pbc)= " << std::setfill(' ')
              << std::setw(15) << std::right << e1b_pbc << std::setfill(' ')
              << std::setw(12) << std::right << " kcal/mol" << std::endl;          

    std::cout << std::setprecision(6) << std::scientific
              << "system["  << std::setfill('.')
              << std::setw(10) << i 
              << std::setw(12) << "] e2b(nopbc)= " << std::setfill(' ')
              << std::setw(15) << std::right <<  e2b << std::setfill(' ')
              << std::setw(12) << " e2b(pbc)= " << std::setfill(' ')
              << std::setw(15) << std::right << e2b_pbc << std::setfill(' ')
              << std::setw(12) << std::right << " kcal/mol" << std::endl;          

    std::cout << std::setprecision(6) << std::scientific
              << "system["  << std::setfill('.')
              << std::setw(10) << i
              << std::setw(12) << "] e3b(nopbc)= " << std::setfill(' ')
              << std::setw(15) << std::right <<  e3b << std::setfill(' ')
              << std::setw(12) << " e3b(pbc)= " << std::setfill(' ')
              << std::setw(15) << std::right << e3b_pbc << std::setfill(' ')
              << std::setw(12) << std::right << " kcal/mol" << std::endl;

#   ifdef PRINT_XYZ_PBC_TEST
    std::cout << "\nXYZ no PBC\n " << ats.size() << "\n 0.0 \n"; 
    for (size_t j = 0; j < ats.size(); j++) {
      std::cout << std::setprecision(6) << std::scientific
                << std::setw(5) << std::right << ats[j]
                << std::setw(15) << std::right << xyz[3*j + 0]
                << std::setw(15) << std::right << xyz[3*j + 1]
                << std::setw(15) << std::right << xyz[3*j + 2] << std::endl;
    }

    std::cout << "\nXYZ PBC\n " << ats.size() << "\n 0.0 \n"; 
    for (size_t j = 0; j < ats.size(); j++) {
      std::cout << std::setprecision(6) << std::scientific
                << std::setw(5) << std::right << ats[j]
                << std::setw(15) << std::right << xyz_pbc[3*j + 0]
                << std::setw(15) << std::right << xyz_pbc[3*j + 1]
                << std::setw(15) << std::right << xyz_pbc[3*j + 2] << std::endl;
    }
#   endif    

    std::cout << std::endl;
  }

  // Test for dispersion in pbc and no pbc for a I -- H2O dimer
  double cutoff = 15.0;

  // Create water coordinates
  std::vector<double> xyz_w = { 0.0, 0.0, 0.0,
                                0.3233977994, -0.399214478, -0.8241726072,
                                0.5584555607, -0.454978538, 0.6418369734};
  // Create iodide coordinates
  std::vector<double> xyz_i = {0.0, 0.0, 0.0};

  // Create gradient vectors
  std::vector<double> grad_w(9,0.0);
  std::vector<double> grad_i(3,0.0);
  
  double step = 0.05;
  double d = cutoff - 2.0;
  std::vector<double> disp_nocutoff;
  std::vector<double> disp_cutoff;
  while (d < cutoff + 1.0) {
    xyz_i[2] = d;
    double disp = disp::GetDispersion("h2o", "i", 1, false, xyz_w, xyz_i,
                               grad_w, grad_i, cutoff, false);
    disp_nocutoff.push_back(disp);
   
    disp = disp::GetDispersion("h2o", "i", 1, false, xyz_w, xyz_i,
                               grad_w, grad_i, cutoff, true);
    disp_cutoff.push_back(disp);
    
    d += step;
  }

  // Print results to compare
  std::cout << "\n  No cutoff dispersion energy:\n";
  for (size_t i = 0; i < disp_nocutoff.size(); i++) {
    if (i%5 == 0)
      std::cout << std::endl;

    std::cout << std::setprecision(6) << std::scientific
              << std::setw(15) << std::right << disp_nocutoff[i];
  }
    
  std::cout << "\n\n  Cutoff dispersion energy:\n";
  for (size_t i = 0; i < disp_cutoff.size(); i++) {
    if (i%5 == 0)
      std::cout << std::endl;

    std::cout << std::setprecision(6) << std::scientific
              << std::setw(15) << std::right << disp_cutoff[i];
  }
  std::cout << std::endl;

  std::cout << "\n\n  Difference between cutoff and no cutoff dispersion energy:\n";
  for (size_t i = 0; i < disp_cutoff.size(); i++) {
    if (i%5 == 0)
      std::cout << std::endl;

    std::cout << std::setprecision(6) << std::scientific
              << std::setw(15) << std::right << disp_cutoff[i] - disp_nocutoff[i];
  }
  std::cout << std::endl;

  return 0;
}











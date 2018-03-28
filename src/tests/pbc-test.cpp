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
static double tol = 1E-06;

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
    std::vector<double> xyz = systems[i].GetOriginalOrderSysXyz();
    std::vector<std::string> ats = systems[i].GetOriginalOrderSysAtNames();
    double e1b = systems[i].Get1B(false);
    double e2b = systems[i].Get2B(false);
    double e3b = systems[i].Get3B(false);

    // Then PBC
    systems[i].SetPBC(true,box);
    std::vector<double> xyz_pbc = systems[i].GetOriginalOrderSysXyz();
    double e1b_pbc = systems[i].Get1B(false);
    double e2b_pbc = systems[i].Get2B(false);
    double e3b_pbc = systems[i].Get3B(false);

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
  return 0;
}

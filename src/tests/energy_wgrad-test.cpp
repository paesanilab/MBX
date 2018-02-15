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

#define PRINT_GRADS
//#define NUM_GRADS
namespace {

static std::vector<bblock::System> systems;

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
  std::cout << "Energies without gradients:" << std::endl;
  for (size_t i = 0; i < systems.size(); i++) {
    double energy = systems[i].Energy(g, false);
    std::cout << std::setprecision(5) << std::scientific
              << "system["  << std::setfill('.')
              << std::setw(5) << i << "]= " << std::setfill(' ')
              << std::setw(20) << std::right << energy 
              << std::setw(12) << std::right << " kcal/mol" << std::endl;          
  }

  std::cout << "Energies with gradients:" << std::endl;
  for (size_t i = 0; i < systems.size(); i++) {
    std::vector<double> grd;
    
    double energy = systems[i].Energy(grd, true);
    std::cout << std::setprecision(5) << std::scientific
              << "system["  << std::setfill('.')
              << std::setw(5) << i << "]= " << std::setfill(' ')
              << std::setw(20) << std::right << energy
              << std::setw(12) << std::right << " kcal/mol" 
              << std::endl << std::endl;
# ifdef PRINT_GRADS

    std::vector<std::string> atn = systems[i].GetSysAtNames();

    size_t n_sites = systems[i].GetNumSites();

    std::cout << std::setw(6)  << std::left << "Atom"
              << std::setw(20) << std::right << "GradientX"
              << std::setw(20) << std::right << "GradientY"
              << std::setw(20) << std::right << "GradientZ"
              << std::endl;
    for (size_t j = 0; j < n_sites; j++) {
      if (atn[j] == "virt") continue;
      std::cout << std::setprecision(3) << std::scientific
                << std::setw(6) << std::left << atn[j]
                << std::setw(20) << std::right << grd[3*j]
                << std::setw(20) << std::right << grd[3*j + 1]
                << std::setw(20) << std::right << grd[3*j + 2]
                << std::endl;
    }
#ifdef NUM_GRADS
    std::cout << std::endl << std::setw(6)  << std::left << "Atom"
              << std::setw(20) << std::right << "Analytical"
              << std::setw(20) << std::right << "Numerical"
              << std::setw(20) << std::right << "Difference"
              << std::endl;
    // Comparing analytical and numerical
    std::vector<double> xyz;
    xyz = systems[i].GetSysXyz();
    const double eps = 1.0e-6;
    for (size_t j = 0; j < n_sites * 3; j++) {
      if (atn[j/3] == "virt") continue;
      const double x_orig = xyz[j];
      xyz[j] = x_orig + eps;
      systems[i].SetSysXyz(xyz);
      const double Ep = systems[i].Energy(g,false);
      xyz[j] = x_orig + 2 * eps;
      systems[i].SetSysXyz(xyz);
      const double Epp = systems[i].Energy(g,false);
      xyz[j] = x_orig - eps;
      systems[i].SetSysXyz(xyz);
      const double Em = systems[i].Energy(g,false);
      xyz[j] = x_orig - 2 * eps;
      systems[i].SetSysXyz(xyz);
      const double Emm = systems[i].Energy(g,false);
      const double gfd = (8*(Ep - Em) - (Epp - Emm))/(12*eps);
      xyz[j] = x_orig;
      systems[i].SetSysXyz(xyz);
      std::cout << std::setprecision(5) << std::scientific
                << std::setw(6) << std::left << atn[j/3]
                << std::setw(20) << std::right << grd[j]
                << std::setw(20) << std::right << gfd
                << std::setw(20) << std::right << std::fabs(grd[j] - gfd)
                << std::endl;
    }  
# endif // NUM_GRADS
# endif // PRINT_GRADS
  }
  return 0;
}

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

namespace {

static std::vector<bblock::System> systems;

} // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " input.nrg"
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


  std::cout << "\nCharges:" << std::endl;
  for (size_t i = 0; i < systems.size(); i++) {
    std::vector<double> chg = systems[i].GetCharges();
    std::vector<std::string> at = systems[i].GetSysAtNames();
    for (size_t j = 0; j < systems[i].GetNumSites(); j++) {
      std::cout << std::setprecision(5) << std::scientific
                << std::setw(8)  << at[j] 
                << std::setw(12) << chg[j] << std::endl;      
    }    
  }
  
  std::cout << "\nPolfac: " << std::endl;
  for (size_t i = 0; i < systems.size(); i++) {
    std::vector<double> plfcs = systems[i].GetPolfacs();
    std::vector<std::string> at = systems[i].GetSysAtNames();
    for (size_t j = 0; j < systems[i].GetNumSites(); j++) {
      std::cout << std::setprecision(5) << std::scientific
                << std::setw(8)  << at[j]
                << std::setw(12) << plfcs[j] << std::endl;
    }
  }

  std::cout << "\nPols: " << std::endl;
  for (size_t i = 0; i < systems.size(); i++) {
    std::vector<double> pols = systems[i].GetPols();
    std::vector<std::string> at = systems[i].GetSysAtNames();
    for (size_t j = 0; j < systems[i].GetNumSites(); j++) {
      std::cout << std::setprecision(5) << std::scientific
                << std::setw(8)  << at[j]
                << std::setw(12) << pols[j] << std::endl;
    }
  }
  

  return 0;
}

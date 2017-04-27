#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "read_nrg.h"
#include "write_nrg.h"

#include "system.h"


#define PRINT_GRADS
//#define CHECK_GRADS
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

  for (size_t i = 0; i < systems.size(); i++) {
    double energy = systems[i].Energy();
    std::cout << std::setprecision(8)
              << "system["  
              << std::setw(5) << i << "]= "
              << std::setw(20) << std::right << energy << std::endl;          
  }

  return 0;
}

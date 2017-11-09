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

// For testing
//#define PRIVATE PUBLIC


namespace {

static std::vector<bblock::System> systems;

} // namespace

////////////////////////////////////////////////////////////////////////////////

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

  for (size_t i = 0; i < systems.size(); i++) {
    // Retrieve dimers
    systems[i].AddClusters(2,15.0,0,systems[i].GetNumMon());
    std::vector<size_t> dimers = systems[i].GetDimers();
    size_t ndimers = dimers.size() / 2;
    // Print all dimers
    for (size_t j = 0; j < ndimers; j++) {
      std::cout << "Dimer[" << std::setw(10) << std::right 
                << j << "] -> " << std::setw(8) << std::right
                << dimers[2*j] << std::setw(8) << std::right
                << dimers[2*j + 1] << std::endl;
    }
    
    // retrieve Trimers
    systems[i].AddClusters(3,4.5,0,systems[i].GetNumMon());
    std::vector<size_t> trimers = systems[i].GetTrimers();
    size_t ntrimers = trimers.size() / 3;
    //Print all trimers
    for (size_t j = 0; j < ntrimers; j++) {
      std::cout << "Trimer[" << std::setw(10) << std::right
                << j << "] -> " << std::setw(8) << std::right
                << trimers[3*j] << std::setw(8) << std::right
                << trimers[3*j + 1] << std::setw(8) << std::right
                << trimers[3*j + 2] << std::endl;
    }
    
  }
  
  return 0;
}

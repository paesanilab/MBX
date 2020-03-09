#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

//#define PRINT_GRADS
namespace {

static std::vector<bblock::System> systems;

} // namespace

int main(int argc, char** argv)
{

    if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <input.nrg> [mbx.json]"
                << std::endl;
      return 0;
    }


    try {
        std::ifstream ifs(argv[1]);

        if (!ifs){
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();
      
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> box;
    
    if (argc > 2) {
        systems[0].SetUpFromJson(argv[2]);
    } else {
        systems[0].SetUpFromJson();
    }


    double en = systems[0].Energy(true);

    std::cout << "Energy= " << en << std::endl;
    
    tools::WriteNrg("input_out.nrg", systems);

#ifdef PRINT_GRADS
    {
        std::vector<std::string> atn = systems[0].GetAtomNames();

        size_t n_sites = systems[0].GetNumSites();
        size_t n_atoms = systems[0].GetNumRealSites();

        std::vector<double> grads = systems[0].GetRealGrads();

        for (size_t i = 0; i < n_atoms; i++) {
            std::cout << std::setprecision(8) << std::scientific
                      << std::setw(20) << grads[3*i]  
                      << std::setw(20) << grads[3*i + 1]  
                      << std::setw(20) << grads[3*i + 2] << std::endl;
        }  
    }
#endif

    return 0;
}

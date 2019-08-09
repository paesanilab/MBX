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

//#define NUMGRADS
//#define PRINT_GRADS

namespace {

static std::vector<bblock::System> systems;

} // namespace

int main(int argc, char** argv)
{

    if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <input.nrg> <n_evals> [<box_side>]"
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
    size_t neval = atoi(argv[2]);

    if (argc > 5) {
        box = std::vector<double>(9,0.0);
        box[0] = atof(argv[3]);
        box[4] = atof(argv[4]);
        box[8] = atof(argv[5]);
    } else if (argc > 3) {
        double box_l = atof(argv[3]);
        box = std::vector<double>(9,0.0);
        box[0] = box[4] = box[8] = box_l;
    }


    systems[0].SetPBC(box);
    systems[0].SetDipoleMethod("cg");
    if (box.size()) {
        systems[0].Set2bCutoff(9.0);
        systems[0].SetEwaldElectrostatics(0.6, 2.5, 6);
        systems[0].SetEwaldDispersion(0.5, 2.5, 6);
    } else {
        systems[0].Set2bCutoff(100.0);
    }


    for (size_t i = 0; i < neval; i++) {
        double en = systems[0].Energy(true);
        std::cout << "Energy[" << i << "]= " << en << std::endl;
    }

    return 0;
}

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

}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.nrg> <neval> [mbx.json]" << std::endl;
        return 0;
    }

    size_t neval = 1;
    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();

        neval = atoi(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> box;

    if (argc > 3) {
        systems[0].SetUpFromJson(argv[3]);
    } else {
        systems[0].SetUpFromJson();
    }

    for (size_t i = 0; i < neval; i++) {
        double en = systems[0].Energy(true);
        std::cout << "Energy[" << i << "]= " << en << std::endl;
    }

    return 0;
}

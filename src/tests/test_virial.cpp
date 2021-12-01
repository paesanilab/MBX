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
//#define PRINT_VIRIAL
namespace {

static std::vector<bblock::System> systems;

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.nrg> [mbx.json]" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
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

    size_t n_sites = systems[0].GetNumSites();
    size_t n_atoms = systems[0].GetNumRealSites();

    std::vector<double> grads = systems[0].GetRealGrads();
    std::vector<double> xyz = systems[0].GetRealXyz();

    std::vector<double> virial = systems[0].GetVirial();

    std::vector<double> virial_forces(3, 0.0);

    // Calculate diagonal terms of the virial xx, vyy, vzz
    for (size_t i = 0; i < n_atoms; i++) {
        virial_forces[0] -= grads[3 * i] * xyz[3 * i];
        virial_forces[1] -= grads[3 * i + 1] * xyz[3 * i + 1];
        virial_forces[2] -= grads[3 * i + 2] * xyz[3 * i + 2];
    }

    for (size_t i = 0; i < 3; i++) {
        std::cout << std::scientific << std::setprecision(6) << "virial 1 system = " << virial[3 * i + i]
                  << "  ; virial 1 forces = " << virial_forces[i]
                  << "  ; diff = " << virial[3 * i + i] - virial_forces[i] << std::endl;
    }

    return 0;
}

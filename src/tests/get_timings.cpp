#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>
#include <chrono>

#include "io_tools/read_nrg.h"
#include "bblock/system.h"

const size_t NCALC = 10;

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

    double e_tot = systems[0].Energy(true);

    size_t t_tot = 0;
    size_t t_1b = 0;
    size_t t_2b = 0;
    size_t t_3b = 0;
    size_t t_disp = 0;
    size_t t_elec = 0;
    size_t t_buck = 0;
    size_t t_ff = 0;
    size_t t_lj = 0;

    for (size_t i = 0; i < NCALC; i++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        double e_tot = systems[0].Energy(true);
        auto t2 = std::chrono::high_resolution_clock::now();
        double e_1b = systems[0].OneBodyEnergy(true);
        auto t3 = std::chrono::high_resolution_clock::now();
        double e_2b = systems[0].TwoBodyEnergy(true);
        auto t4 = std::chrono::high_resolution_clock::now();
        double e_3b = systems[0].ThreeBodyEnergy(true);
        auto t5 = std::chrono::high_resolution_clock::now();
        double e_disp = systems[0].Dispersion(true);
        auto t6 = std::chrono::high_resolution_clock::now();
        double e_buck = systems[0].Buckingham(true);
        auto t7 = std::chrono::high_resolution_clock::now();
        double e_elec = systems[0].Electrostatics(true);
        auto t8 = std::chrono::high_resolution_clock::now();
        double e_ff = systems[0].ClassicPotential(true);
        auto t9 = std::chrono::high_resolution_clock::now();
        double e_lj = systems[0].LennardJones(true);
        auto t10 = std::chrono::high_resolution_clock::now();

        t_tot += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        t_1b += std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
        t_2b += std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        t_3b += std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count();
        t_disp += std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
        t_buck += std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count();
        t_elec += std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
        t_ff += std::chrono::duration_cast<std::chrono::milliseconds>(t9 - t8).count();
        t_lj += std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
    }

    double txs_tot = double(t_tot) / double(NCALC);
    double txs_1b = double(t_1b) / double(NCALC);
    double txs_2b = double(t_2b) / double(NCALC);
    double txs_3b = double(t_3b) / double(NCALC);
    double txs_disp = double(t_disp) / double(NCALC);
    double txs_buck = double(t_buck) / double(NCALC);
    double txs_elec = double(t_elec) / double(NCALC);
    double txs_lj = double(t_lj) / double(NCALC);
    double txs_ff = double(t_ff) / double(NCALC);

    std::cout << std::fixed;
    std::cout << std::setprecision(1) << std::setw(20) << "TOTAL" << std::setw(20) << txs_tot << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "1B" << std::setw(20) << txs_1b << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "2B" << std::setw(20) << txs_2b << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "3B" << std::setw(20) << txs_3b << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "ELECTROSTATICS" << std::setw(20) << txs_elec << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "DISPERSION" << std::setw(20) << txs_disp << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "BUCKINGHAM" << std::setw(20) << txs_buck << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "CLASSICAL1B" << std::setw(20) << txs_ff << std::endl;

    std::cout << std::setprecision(1) << std::setw(20) << "LENNARDJONES" << std::setw(20) << txs_lj << std::endl;

    return 0;
}

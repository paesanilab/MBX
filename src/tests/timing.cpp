#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <chrono>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: energy h2o_ion.nrg" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> g;
    double dummy;
    for (size_t i = 0; i < systems.size(); i++) {
        std::cout << "Energies without gradients:" << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        double energy = systems[i].Energy(g, false);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << std::setprecision(10) << std::scientific << "system[" << std::setfill('.') << std::setw(5) << i
                  << "]= " << std::setfill(' ') << std::setw(20) << std::right << energy << std::setw(12) << std::right
                  << " kcal/mol" << std::endl;

        std::cout << "Energies with gradients:" << std::endl;
        auto t3 = std::chrono::high_resolution_clock::now();
        energy = systems[i].Energy(g, true);
        auto t4 = std::chrono::high_resolution_clock::now();
        std::cout << std::setprecision(10) << std::scientific << "system[" << std::setfill('.') << std::setw(5) << i
                  << "]= " << std::setfill(' ') << std::setw(20) << std::right << energy << std::setw(12) << std::right
                  << " kcal/mol" << std::endl;

        auto t5 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get1B(false);

        auto t6 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get1B(true);

        auto t7 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get2B(false);

        auto t8 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get2B(true);

        auto t9 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get3B(false);

        auto t10 = std::chrono::high_resolution_clock::now();
        dummy = systems[i].Get3B(true);

        auto t11 = std::chrono::high_resolution_clock::now();
        systems[i].GetPairList(2, 15.0, 0, systems[i].GetNumMon());

        auto t12 = std::chrono::high_resolution_clock::now();
        systems[i].GetPairList(3, 5.0, 0, systems[i].GetNumMon());

        auto t13 = std::chrono::high_resolution_clock::now();
        systems[i].GetElectrostatics(false);

        auto t14 = std::chrono::high_resolution_clock::now();
        systems[i].GetElectrostatics(true);

        auto t15 = std::chrono::high_resolution_clock::now();

        std::cout << "Energy:(nograd) " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " milliseconds\n";
        std::cout << "Energy:(grad) " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()
                  << " milliseconds\n";
        std::cout << "1B:(nograd) " << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count()
                  << " milliseconds\n";
        std::cout << "1B:(grad) " << std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count()
                  << " milliseconds\n";
        std::cout << "2B:(nograd) " << std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count()
                  << " milliseconds\n";
        std::cout << "2B:(grad) " << std::chrono::duration_cast<std::chrono::milliseconds>(t9 - t8).count()
                  << " milliseconds\n";
        std::cout << "3B:(nograd) " << std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count()
                  << " milliseconds\n";
        std::cout << "3B:(grad) " << std::chrono::duration_cast<std::chrono::milliseconds>(t11 - t10).count()
                  << " milliseconds\n";
        std::cout << "Get and set 2b clusters: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count() << " milliseconds\n";
        std::cout << "Get and set 3b clusters: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t13 - t12).count() << " milliseconds\n";
        std::cout << "Electrostatics:(nograd) "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t14 - t13).count() << " milliseconds\n";
        std::cout << "Electrostatics:(grad) "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t15 - t14).count() << " milliseconds\n";
    }
    return 0;
}

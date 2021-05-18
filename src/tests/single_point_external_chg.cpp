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

#define NUMGRADS
//#define PRINT_GRADS
//#define PRINT_VIRIAL
namespace {

static std::vector<bblock::System> systems;

}  // namespace

int main(int argc, char** argv) {

    //std::vector<double> extern_chg = {1.0};
    //std::vector<double> extern_xyz = {2.0,0.0,0.0};
    std::vector<double> extern_chg = {1.0,1.0};
    std::vector<double> extern_xyz = {2.0,0.0,0.0,0.0,1.0,3.0};


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

    systems[0].SetExternalChargesAndPositions(extern_chg,extern_xyz);

    double en = systems[0].Energy(true);

    std::cout << "Energy= " << en << std::endl;

#ifdef PRINT_GRADS
    {
        std::vector<std::string> atn = systems[0].GetAtomNames();

        size_t n_sites = systems[0].GetNumSites();
        size_t n_atoms = systems[0].GetNumRealSites();

        std::vector<double> grads = systems[0].GetRealGrads();

        for (size_t i = 0; i < n_atoms; i++) {
            std::cout << std::setprecision(8) << std::scientific << std::setw(20) << grads[3 * i] << std::setw(20)
                      << grads[3 * i + 1] << std::setw(20) << grads[3 * i + 2] << std::endl;
        }
    }
#endif

#ifdef PRINT_VIRIAL
    std::vector<double> virial = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double E1b, E2b, E3b, Eelec, Edisp, Ebuck, Etotal;
    E1b = systems[0].OneBodyEnergy(true);
    std::cout << " 1B Energy [kcal/mol] " << std::setprecision(15) << E1b << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " 1B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , " << virial[3]
              << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7] << " , "
              << virial[8] << std::endl;
    E2b = systems[0].TwoBodyEnergy(true);
    std::cout << " 2B Energy [kcal/mol] " << std::setprecision(15) << E2b << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " 2B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , " << virial[3]
              << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7] << " , "
              << virial[8] << std::endl;
    E3b = systems[0].ThreeBodyEnergy(true);
    std::cout << " 3B Energy [kcal/mol] " << std::setprecision(15) << E3b << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " 3B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , " << virial[3]
              << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7] << " , "
              << virial[8] << std::endl;
    Eelec = systems[0].Electrostatics(true);
    std::cout << " Elec Energy [kcal/mol] " << std::setprecision(15) << Eelec << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " Elec Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
              << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
              << " , " << virial[8] << std::endl;
    Edisp = systems[0].Dispersion(true);
    std::cout << " Disp Energy [kcal/mol] " << std::setprecision(15) << Edisp << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " Disp Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
              << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
              << " , " << virial[8] << std::endl;
    Ebuck = systems[0].Buckingham(true);
    std::cout << " Buck Energy [kcal/mol] " << std::setprecision(15) << Ebuck << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " Buck Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
              << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
              << " , " << virial[8] << std::endl;
    Etotal = systems[0].Energy(true);
    std::cout << " Total Energy [kcal/mol] " << std::setprecision(15) << Etotal << std::endl;
    virial = systems[0].GetVirial();
    std::cout << " Total Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
              << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
              << " , " << virial[8] << std::endl;

#endif

#ifdef NUMGRADS

    std::vector<std::string> atn = systems[0].GetAtomNames();

    size_t n_sites = systems[0].GetNumSites();
    size_t n_atoms = systems[0].GetNumRealSites();

    std::cout << std::endl
              << std::setw(6) << std::left << "Atom" << std::setw(20) << std::right << "Analytical" << std::setw(20)
              << std::right << "Numerical" << std::setw(20) << std::right << "Difference" << std::endl;
    // Comparing analytical and numerical
    std::vector<double> xyz, real_grd;
    xyz = systems[0].GetRealXyz();
    real_grd = systems[0].GetRealGrads();
    std::vector<std::string> atms = systems[0].GetRealAtomNames();
    const double eps = 1.0e-3;

    for (size_t j = 0; j < n_atoms * 3; j++) {
        const double x_orig = xyz[j];
        xyz[j] = x_orig + eps;
        systems[0].SetRealXyz(xyz);
        const double Ep = systems[0].Energy(false);
        xyz[j] = x_orig + 2 * eps;
        systems[0].SetRealXyz(xyz);
        const double Epp = systems[0].Energy(false);
        xyz[j] = x_orig - eps;
        systems[0].SetRealXyz(xyz);
        const double Em = systems[0].Energy(false);
        xyz[j] = x_orig - 2 * eps;
        systems[0].SetRealXyz(xyz);
        const double Emm = systems[0].Energy(false);
        const double gfd = (8 * (Ep - Em) - (Epp - Emm)) / (12 * eps);
        xyz[j] = x_orig;
        systems[0].SetRealXyz(xyz);
        std::cout << std::setprecision(5) << std::scientific << std::setw(6) << std::left << atms[j / 3]
                  << std::setw(20) << std::right << real_grd[j] << std::setw(20) << std::right << gfd << std::setw(20)
                  << std::right << std::fabs(real_grd[j] - gfd) << std::endl;
    }

#endif

    return 0;
}

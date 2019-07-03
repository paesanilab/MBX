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

namespace {

static std::vector<bblock::System> systems;

} // namespace

int main(int argc, char** argv)
{

    if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <input.nrg> [<box_side>]"
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
        double box_l = atof(argv[2]);
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


    double en = systems[0].Energy(true);

    std::cout << "Energy= " << en << std::endl;
    
    tools::WriteNrg("input_out.nrg", systems);

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
            systems[0].SetPBC(box);
            const double Ep = systems[0].Energy(false);
            xyz[j] = x_orig + 2 * eps;
            systems[0].SetRealXyz(xyz);
            systems[0].SetPBC(box);
            const double Epp = systems[0].Energy(false);
            xyz[j] = x_orig - eps;
            systems[0].SetRealXyz(xyz);
            systems[0].SetPBC(box);
            const double Em = systems[0].Energy(false);
            xyz[j] = x_orig - 2 * eps;
            systems[0].SetRealXyz(xyz);
            systems[0].SetPBC(box);
            const double Emm = systems[0].Energy(false);
            const double gfd = (8 * (Ep - Em) - (Epp - Emm)) / (12 * eps);
            xyz[j] = x_orig;
            systems[0].SetRealXyz(xyz);
            systems[0].SetPBC(box);
            std::cout << std::setprecision(5) << std::scientific << std::setw(6) << std::left << atms[j / 3]
                      << std::setw(20) << std::right << real_grd[j] << std::setw(20) << std::right << gfd
                      << std::setw(20) << std::right << std::fabs(real_grd[j] - gfd) << std::endl;
        }


        

#endif    
    

    return 0;
}

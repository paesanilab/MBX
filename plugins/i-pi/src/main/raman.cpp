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

#include "perturb.h"
#include "read_xyz.h"

namespace {

static std::vector<bblock::System> systems;

} // namespace

int main(int argc, char** argv)
{

    if (argc < 3) {
      std::cerr << "Usage: " << argv[0] << " <input.nrg> <trajectory.xyz> [<box_side>]"
                << std::endl;
      return 0;
    }

    std::vector<std::vector<double> > xyz;
    std::vector<std::vector<std::string> > atoms;

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs){
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();

        ReadXyzTrajectory(argv[2],xyz,atoms);

      
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    size_t nframes = xyz.size();
    std::vector<double> box;
    
    if (argc > 3) {
        double box_l = atof(argv[3]);
        box = std::vector<double>(9,0.0);
        box[0] = box[4] = box[8] = box_l;
    }

    std::vector<std::vector<double> > pi(nframes);
    std::vector<double> my_pol = systems[0].GetPolarizabilities();
    std::vector<double> my_polfac = systems[0].GetPolarizabilityFactors();
    std::vector<double> my_chg = systems[0].GetCharges();
    

    for (size_t i = 0; i < nframes; i++) {
        systems[0].SetRealXyz(xyz[i]);
        systems[0].SetPBC(box);
        std::vector<double> my_xyz = systems[0].GetXyz();
        // Declare perturbation
        noneq::Perturbation my_perturb;
        // initiallize perturbation
        my_perturb.Initialize(my_xyz, my_chg, my_pol, my_polfac, box);
        my_perturb.GetPolarizability(pi[i]);
    }

    // Calculate correlation
    for (size_t i = 0; i < nframes; i++) {
        std::cout << "Frame " << i << std::endl;
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                std::cout << std::setw(20) << std::scientific << std::setprecision(10) << pi[i][3*j + k];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}

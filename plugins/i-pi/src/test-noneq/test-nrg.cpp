#include <iostream>
#include <iomanip>
#include <vector>

#include "perturb.h"
#include "bblock/system.h"
#include "io_tools/read_nrg.h"

namespace {

static std::vector<bblock::System> systems;

}  // namespace

int main() {

    try {
        std::ifstream ifs("input.nrg");

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg("input.nrg", systems);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    //std::vector<double> coords = systems[0].GetRealXyz();
    //std::vector<double> charges = systems[0].GetRealCharges();
    //std::vector<double> pol = systems[0].GetRealPolarizabilities();
    //std::vector<double> polfac = systems[0].GetRealPolarizabilityFactors();
    std::vector<double> coords = systems[0].GetXyz();
    std::vector<double> charges = systems[0].GetCharges();
    std::vector<double> pol = systems[0].GetPolarizabilities();
    std::vector<double> polfac = systems[0].GetPolarizabilityFactors();

    noneq::Perturbation my_pert;
    my_pert.Initialize(coords,charges,pol,polfac);
    std::vector<double> pi;
    my_pert.GetPolarizability(pi);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++)
            std::cout << std::setw(20) << std::setprecision(10) <<  pi[3*i + j];
        std::cout << std::endl;
    }
}

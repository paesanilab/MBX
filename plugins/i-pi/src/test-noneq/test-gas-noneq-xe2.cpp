#include <iostream>
#include <iomanip>
#include <vector>

#include "perturb.h"
#include "setupxe2.h"

int main() {
    SETUP_XE_2
    noneq::Perturbation my_pert;
    my_pert.Initialize(coords,charges,pol,polfac);
    std::vector<double> Tij_a;
    std::vector<double> Tij_ab;
    std::vector<double> Tij_abc;
    std::vector<double> pi_all;
    std::vector<double> dk_pi;
    my_pert.GetElectrostaticTensors(Tij_a,Tij_ab,Tij_abc);
    my_pert.GetPolarizabilityAll(pi_all);
    my_pert.GetDkPi(dk_pi);
    //for (size_t i = 0; i < Tij_abc.size(); i++) std::cout << Tij_abc[i] << std::endl;
    //for (size_t i = 0; i < pi_all.size(); i++) std::cout << pi_all[i] << std::endl;
    //for (size_t i = 0; i < dk_pi.size(); i++) std::cout << dk_pi[i] << std::endl;

    for (size_t i = 0; i < charges.size(); i++) {
        for (size_t a = 0; a < 3; a++)
            for (size_t b = 0; b < 3; b++)
                for (size_t u = 0; u < 3; u++)
                    std::cout << std::setprecision(10) <<  dk_pi[27*i + 9*u + 3*a + b] << ", ";
        std::cout << std::endl;
    }
}

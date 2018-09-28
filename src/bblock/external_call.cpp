#include "bblock/system.h"

extern "C" {
void energyf90_(double* coords, int* nat_monomers, char at_names[][5], char monomers[][5], int* nmon, double* pot) {

    bblock::System s;
    int count = 0;
    for (int i = 0; i < *nmon; i++) {
        std::vector<double> xyz(3 * nat_monomers[i]);
        std::vector<std::string> vAtNames(nat_monomers[i]);

        std::copy(coords + 3 * count, coords + 3 * (count + nat_monomers[i]), xyz.begin());
        std::copy(at_names + count, at_names + count + nat_monomers[i], vAtNames.begin());
        std::string id = monomers[i];
        s.AddMonomer(xyz, vAtNames, id);
        count += nat_monomers[i];
    }

    s.Initialize();

    *pot = s.Energy(false);
}

void energyf90g_(double* coords, int* nat_monomers, char at_names[][5], char monomers[][5], int* nmon, double* grad,
                 double* pot) {

    bblock::System s;
    int count = 0;
    for (int i = 0; i < *nmon; i++) {
        std::vector<double> xyz(3 * nat_monomers[i]);
        std::vector<std::string> vAtNames(nat_monomers[i]);

        std::copy(coords + 3 * count, coords + 3 * (count + nat_monomers[i]), xyz.begin());
        std::copy(at_names + count, at_names + count + nat_monomers[i], vAtNames.begin());
        std::string id = monomers[i];
        s.AddMonomer(xyz, vAtNames, id);
        count += nat_monomers[i];
    }

    s.Initialize();

    double energy = s.Energy(true);

    std::vector<double> gradv = s.GetGrads();

    std::vector<std::string> newNames = s.GetAtomNames();
    int count2 = 0;
    for (size_t i = 0; i < s.GetNumSites(); i++) {
        if (newNames[i] != "virt") {
            for (size_t j = 0; j < 3; j++) {
                grad[count2 + j] = gradv[3 * i + j];
            }
            count2 += 3;
        }
    }

    *pot = energy;
}

}  // extern C

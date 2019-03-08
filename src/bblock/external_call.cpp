/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:

Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910

Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

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

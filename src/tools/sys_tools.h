#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#include <vector>
#include <string>

namespace systools {

// Function that given the monomers, modifies the ordered monomer list
// in mon, and the original order in the vector original_order
void OrderMonomers(std::vector<std::string> &mon, 
                   std::vector<size_t> &original_order);

// Function that sets up initial charges, pols, polfacs, number of sites
// number of atoms and first index. Returns the total number of sites
size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites
                     std::vector<size_t> &nat, std::vector<size_t> &fstind,
                     std::vector<double> &chg, std::vector<double> &pol,
                     std::vector<double> &polfac);

} // systools
#endif // SYS_TOOLS_H

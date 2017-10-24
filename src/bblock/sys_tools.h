#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <utility>

#include "nanoflann.hpp"
#include "kdtree_utils.h"

namespace systools {

// Function that given the monomers, modifies the ordered monomer list
// in mon, and the original order in the vector original_order
std::vector<std::pair<std::string,size_t>> OrderMonomers
                   (std::vector<std::string> &mon, 
                    std::vector<size_t> &original_order);

// Function that sets up initial charges, pols, polfacs, number of sites
// number of atoms and first index. Returns the total number of sites
size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites,
                     std::vector<size_t> &nat, std::vector<size_t> &fstind,
                     std::vector<double> &chg, std::vector<double> &pol,
                     std::vector<double> &polfac);

void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend,
                 std::vector<std::string> mons,
                 std::vector<double> xyz, 
                 std::vector<size_t> first_index,
                 std::vector<size_t> &dimers, 
                 std::vector<size_t> &trimers); 

} // systools
#endif // SYS_TOOLS_H

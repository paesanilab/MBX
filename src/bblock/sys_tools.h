#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <utility>

#include "nanoflann.hpp"
#include "kdtree_utils.h"
#include "tools/definitions.h"

#include "potential/1b/ps.h"
#include "tools/constants.h"

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

// Given ifnormation of the system, this subroutine fills up the dimers and 
// trimers of the system.
void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend,
                 size_t nmon,
                 std::vector<double> xyz_orig, 
                 std::vector<size_t> first_index,
                 std::vector<size_t> &dimers, 
                 std::vector<size_t> &trimers); 

// Fills up the excluded set pairs for a given monomer
void GetExcluded(std::string mon, 
                 excluded_set_type &exc12,
                 excluded_set_type &exc13,
                 excluded_set_type &exc14);

// Returns if a pair a,b (or b,a) is in the excluded set exc
bool IsExcluded(excluded_set_type exc, size_t a, size_t b);

// Returns the proper aDD value for the electrostatics
double GetAdd(bool is12, bool is13, bool is14, std::string mon);

// Calculates the coordinates of the virtual site of a monomer when
// given the coordinates of the other sites
void SetVSites(std::vector<double> &xyz, std::string mon_id,
               size_t n_mon, size_t nsites, size_t fst_ind);

// Calculates the charges of all the sites in a monomer using its xyz
// coordinates
void SetCharges(std::vector<double> xyz, std::vector<double> &charges,
                std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);
void SetPolfac (std::vector<double> &polfac, std::string mon_id,
                size_t n_mon, size_t nsites, size_t fst_ind);
// FIXME polfac here should not be necessary (we are only setting pols
void SetPol (std::vector<double> &pol, 
             std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);

} // systools
#endif // SYS_TOOLS_H

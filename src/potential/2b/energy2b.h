#ifndef ENERGY2B_H
#define ENERGY2B_H

#include <string>
#include <vector>
#include <iostream>

// 2B
#include "potential/2b/x2b-v9x.h"
#include "potential/2b/x2b-A1B2Z2_C-v1x.h"

/**
 * @file energy2b.h
 * @brief Contains the calls to the two-body energy with and without gradients
 */

/**
 * @namespace e2b
 * @brief Sets the namespace for the energy calls for the 2 body energy
 */
namespace e2b {

/**
 * @brief Gets the two body energy for a given set of dimers
 *
 * Given the monomer ids of the dimer
 * and the number of monomers, will return the total sum
 * of the 2b energy of those dimers. A dimer will be mon1-mon2.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] nm number of monomers (both m1 and m2 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @return Sum of the two-body energies of all the dimers passed as arguments
 */
double get_2b_energy(std::string m1, std::string m2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2);

/**
 * @brief Gets the two body energy for a given set of dimers
 *
 * Given the monomer ids of the dimer
 * and the number of monomers, will return the total sum
 * of the 2b energy of those dimers. A dimer will be mon1-mon2.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] nm number of monomers (both m1 and m2 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in,out] grad1 gradients of the monomer 1. Gradients will be updated
 * @param[in,out] grad2 gradients of the monomer 2. Gradients will be updated
 * @return Sum of the two-body energies of all the dimers passed as arguments
 */
double get_2b_energy(std::string m1, std::string m2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grad1, std::vector<double> &grad2);

}  // namespace e2b
#endif

#ifndef ENERGY3B_H
#define ENERGY3B_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>

// 3B
#include "potential/3b/x3b-v2x.h"
#include "potential/3b/poly-3b-v2x.h"

/**
 * @file energy3b.h
 * @brief Contains the calls to the three-body energy with and without gradients
 */

/**
 * @namespace e3b
 * @brief Sets the namespace for the energy calls for the 3 body energy
 */
namespace e3b {

/**
 * @brief Gets the two body energy for a given set of dimers
 *
 * Given the monomer ids of the trimer
 * and the number of monomers, will return the total sum
 * of the 3b energy of those dimers. A trimer will be mon1-mon2-mon3.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] nm number of monomers (m1, m2, and m3 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @return Sum of the three-body energies of all the trimers passed as arguments
 */
double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3);

/**
 * @brief Gets the three body energy for a given set of trimers
 *
 * Given the monomer ids of the trimer
 * and the number of monomers, will return the total sum
 * of the 3b energy of those dimers. A trimer will be mon1-mon2-mon3.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] nm number of monomers (both m1 and m2 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @param[in,out] grad1 gradients of the monomer 1. Gradients will be updated
 * @param[in,out] grad2 gradients of the monomer 2. Gradients will be updated
 * @param[in,out] grad3 gradients of the monomer 3. Gradients will be updated
 * @return Sum of the three-body energies of all the trimers passed as arguments
 */
double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3, std::vector<double> &grd1,
                     std::vector<double> &grd2, std::vector<double> &grd3);

}  // namespace e3b
#endif

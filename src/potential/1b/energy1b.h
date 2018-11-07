#ifndef ENERGY1B_H
#define ENERGY1B_H

#include <string>
#include <vector>
#include <iostream>

// 1B
#include "potential/1b/ps.h"
// ====>> SECTION INCLUDE1B <<====
// ===>> PASTE INCLUDE BELOW <<===

#include "tools/definitions.h"

/**
 * @file energy1b.h
 * @brief Contains the calls to the one-body energy with and without gradients
 */

/**
 * @namespace e1b
 * @brief Sets the namespace for the energy calls for the 1 body energy
 */
namespace e1b {

/**
 * @brief Gets the one body energy for a given set of monomers of the same
 * monomer type.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the 1b energy of those monomers.
 * @param[in] mon Monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] good Boolean that will be set to false if any of the monomers
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * @return Sum of the one-body energies of all the monomers passed as arguments
 */
double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz, bool &good);

/**
 * @brief Gets the one body energy for a given set of monomers of the same
 * monomer type, calculating the gradients.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the 1b energy of those monomers and their corresponding gradients.
 * @param[in] mon Monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] grad Gradients of the one-body energy
 * @param[in,out] good Boolean that will be set to false if any of the monomers
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * @return Sum of the one-body energies of all the monomers passed as arguments
 */
double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz, std::vector<double> &grad, bool &good);

}  // namespace e1b
#endif

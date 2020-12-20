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

#ifndef LJTOOLS_H
#define LJTOOLS_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iomanip>

#include "tools/math_tools.h"
#include "json/json.h"

/**
 * @file ljtools.h
 * @brief Contains the helper functions definition for the LennardJones class
 */

/**
 * @namespace lj
 * @brief Encloses the functions related to classical lennard-jones.
 */
namespace lj {

/**
 * @brief Calculates the Lennard-Jones contribution for a given pair of atoms for all pairs in the system.
 *
 * Given a pair of atoms, and the LJ constants, it calculates the Lennard-Jones contribution for all pairs
 * involving atom 1 and all the atoms of type 2. The coordinates of the second atom are in vectorized form.
 * @param[in] eps Epsilon coefficient for the expression 4*eps*( (sigma/r)^12 - (sigma/r)^6 )
 * @param[in] sigma Sigma coefficient for the expression 4*eps*( (sigma/r)^12 - (sigma/r)^6 )
 * @param[in] ljchgi Lennard-Jones "charge" for atom i, only for attractive part
 * @param[in] ljchgj Lennard-Jones "charge" for atom j, only for attractive part
 * @param[in] p1 Pointer to double array with the coordinates of atom1. Length is 3.
 * @param[in] xyz2 Pointer to double array with the coordinates of atom2. The origin of the pointer is the first
 * coordinate of the monomer type 2, and will be in xxxxx[at1]yyyyy[at1]zzzzzz[at1]xxxx[at2]... and so on. See the
 * reordering function for more details.
 * @param[in,out] grad1 Pointer to a double array that accumulates teh gradients in atom 1
 * @param[in,out] grad2 Pointer to a double array that will store the gradients for all atoms in monomer 2 that are
 * involved in the energy
 * @param[in,out] phi1 LJ field on atom type 1. LJ field can be understood as electric field, but
 * instead of decaying as 1/r, decays as 1/r^6, and the charge is sqrt((4*eps)^6*sigma)
 * @param[in,out] phi2 LJ field on atom type 2. LJ field can be understood as electric field, but
 * instead of decaying as 1/r, decays as 1/r^6, and the charge is sqrt((4*eps)^6*sigma)
 * @param[in] nmon1 Number of monomers of type 1. Should always be 1.
 * @param[in] nmon2 Number of monomers of type 2.
 * @param[in] start2 Index of the first monomer of type 2 that will have the LJ energy calculated. This is meant
 * for parallel calculations. See LennarJones class implementation.
 * @param[in] end2 Index of the last monomer of type 2 that will have the LJ energy calculated. This is meant
 * for parallel calculations. See LennarJones class implementation.
 * @param[in] atom_index1 Index of the atom of monomer type 1 involved.
 * @param[in] atom_index2 Index of the atom of monomer type 2 involved.
 * @param[in] lj_scale_factor 0 if i-j is excluded, 1 if it is not.
 * @param[in] do_grads Gradients will be calculated if set to true.
 * @param[in] cutoff Distance for which the pairs will be assumed to have interaction 0.
 * @param[in] ewald_alpha Alpha value for PME dispersion
 * @param[in] box Box of the system
 * @param[in] box_inverse Inverse of the box matrix
 * @param[in] use_ghost Will use ghost atoms if true
 * @param[in] islocal
 * @param[in] isl1_offset
 * @param[in] isl2_offset
 * @param[in,out] virial Virial tensor of the system
 * @return Sum of all the LJ energies for all the atoms involved in the pair i,j
 */
double lj(const double eps, const double sigma, double ljchgi, double ljchgj, const std::vector<double>& p1,
          const std::vector<double>& xyz2, std::vector<double>& grad1, std::vector<double>& grad2, double& phi1,
          std::vector<double>& phi2, const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2,
          const size_t atom_index1, const size_t atom_index2, const double lj_scale_factor, bool do_grads,
          const double cutoff, const double ewald_alpha, const std::vector<double>& box,
          const std::vector<double>& box_inverse, bool use_ghost, const std::vector<size_t>& islocal,
          const size_t isl1_offset, const size_t isl2_offset, std::vector<double>* virial = 0);

/**
 * @brief Retrieves the parameters for lj energy
 *
 * Obtains the parameters sigma and epsilon for the LJ potential, for atom i of monomer 1 and atom j of monomer 2
 * @param[in] mon_id1 Monomer id (h2o,co2...) of monomer 1
 * @param[in] mon_id2 Monomer id (h2o,co2...) of monomer 2
 * @param[in] index1 Atom index of the atom i of monomer 1. In case of water, O -> 0 and H -> 1 or 2
 * @param[in] index2 Atom index of the atom j of monomer 2. In case of water, O -> 0 and H -> 1 or 2
 * @param[in] lj_pairs Contains the pairs of monomers that require the LJ. If the pair {mon_id1,mon_id2}
 * is not there, sigma and epsilon will be set to 0.
 * @param[out] out_eps Contains the parameter epsilon corresponding to the atoms i,j of monomers 1 and 2.
 * @param[out] out_sigma Contains the parameter sigma corresponding to the atoms i,j of monomers 1 and 2.
 * @param[in] use_lj Vector of pairs with the pairs that will use the LJ potential
 * @param[in] lj_j JSON object witht the extra nonbonded pair information
 * @return True if pair found or used, false otherwise
 */
bool GetLjParams(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double& out_epsilon,
                 double& out_sigma, std::vector<std::pair<std::string, std::string> > use_lj, nlohmann::json lj_j = {});

}  // namespace lj

#endif  // LJTOOLS_H

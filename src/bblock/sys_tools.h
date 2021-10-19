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

#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <algorithm>

#include "kdtree/nanoflann.hpp"
#include "kdtree/kdtree_utils.h"
#include "tools/definitions.h"
#include "potential/lj/ljtools.h"
#include "json/json.h"

#include "potential/1b/ps.h"
#include "tools/constants.h"
#include "tools/custom_exceptions.h"

/**
 * @file sys_tools.h
 * @brief Function definitions for system class tools
 */

/**
 * @namespace systools
 * @brief Namespace that contains the helper functions for the system
 * class
 *
 * These functions are callable from anywhere, but are specifically designed
 * to assist the system class member functions.
 */
namespace systools {

/**
 * @brief TTM4-F M-site position parameter \f$\gamma _M\f$
 *
 * The position of the M-site is computed by the formula:
 * \f$r^{\alpha} = \gamma _1 O^{\alpha} + \gamma _2 (H_1^{\alpha} + H_2^{\alpha})\f$
 * with \f$\alpha = x,y,z \f$
 */
const double gammaM = 0.426706882;

/**
 * @brief Constant used in M-site calculation
 */
const double gamma1 = 1.0 - gammaM;

/**
 * @brief Constant used in M-site calculation
 */
const double gamma2 = gammaM / 2;

/**
 * @brief Constant used in M-site gradient distribution

 */
const double gamma21 = gamma2 / gamma1;

/**
 * @brief Orders the monomers into the internal order, putting
 * equivalent monomers one after the otehr one to maximize efficiency.
 *
 * This function will reorder the monomer list in a way that can maximize
 * the efficiency in the parallel loops. It will put all the equivalent
 * monomers together, one after the other one, from less frquent to
 * more frequent.
 * @param[in,out] mon List of monomers in the input order that will be
 * cleared and replaced by the list of monomers in the internal order.
 * @param[in,out] local/ghost descriptor of monomers in the input order
 * that will be cleared and replaced by the descriptros in the internal order
 * @param[in] sites Vector of size_t with the number of sites of each
 * monomer in the input order
 * @param[in] nats Vector of size_t with the number of real sites of each
 * monomer in the input order
 * @param[out] original2current_order This vector will contain the relation
 * between the input order and the interanl order. As an example,
 * @verbatim  original2current_order[4] @endverbatim
 * will contain the position in the internal order of the 5th monomer
 * in the input order
 * @param[out] original_order This vector of pairs contains the input order
 * and the first index in the input order of a given monomer index.
 * In this case, the virtual sites are taken into account. As an example,
 * @verbatim original_order[3].first @endverbatim
 * will contain the index in the input order of the 4th monomer in the
 * internal order, and
 * @verbatim original_order[3].second @endverbatim
 * contains the first index of the 4th monomer in the internal order
 * when it was in the input order
 * @param[out] original_order_realSites This vector of pairs contains the input
 * order and the first index in the input order of a given monomer index.
 * In this case, the virtual sites are not taken into account. As an example,
 * @verbatim original_order_realSites[3].first @endverbatim
 * will contain the index in the input order of the 4th monomer in the
 * internal order, and
 * @verbatim original_order_realSites[3].second @endverbatim
 * contains the first index of the 4th monomer in the internal order
 * when it was in the input order with virtual sites not taken into account
 * @return An ordered vector of pairs. Each pair has the monomer type id
 * in the first place, and the number of that monomer type in the second one.
 * The order inside this vector is from less number of monomers to more
 * number of monomers.
 */
std::vector<std::pair<std::string, size_t>> OrderMonomers(
    std::vector<std::string> &mon, std::vector<size_t> &islocal, std::vector<size_t> sites, std::vector<size_t> nats,
    std::vector<size_t> &original2current_order, std::vector<std::pair<size_t, size_t>> &original_order,
    std::vector<std::pair<size_t, size_t>> &original_order_realSites);

/**
 * @brief Function that fills the sites, atoms and first index of
 * the system given the list of monomer ids
 *
 * Given the monomers in an arbitrary order, this function creates the sites,
 * atoms, and first index vectors for that order.
 * @param[in] mon A vector of strings with the ids of the monomers of
 * the system
 * @param[out] sites Vector with same length as mon than contains the number
 * of sites of the monomers in the same order as the mon vector
 * @param[out] nat Vector with same length as mon than contains the number
 * of atoms of the monomers in the same order as the mon vector
 * @param[out] fi_at Vector with same length as mon than contains the first
 * index of the monomers in the same order as the mon vector
 * @param[out] fi_sites Vector with same length as mon than contains the first
 * index of the monomers in the same order as the mon vector, but also accounting for virtual sites
 * @param[in] mon_j Json object with extra monomer info
 * @return Total number of sites
 */
size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites, std::vector<size_t> &nat,
                     std::vector<size_t> &fi_at, std::vector<size_t> &fi_sites, nlohmann::json mon_j);

/**
 * @brief Makes sure that the coordinates of all atoms of the same monomer
 * are in the same region of the space
 *
 * This function is only useful when periodic boundary conditions are used.
 * It will put all the atoms of the monomer, no matter where they are,
 * close to the first atom of that monomer.
 * @param[in,out] xyz Vector of doubles with all the monomer coordinates
 * that will be replaced by the fixed monomer coordinates after applying
 * mirror images of the atoms. It will take
 * @param[in] box Vector of 9 component with the 3 vectors of the box
 * @param[in] nat Vector with the number of atoms of each monomer
 * @param[in] first_index Vector with the first index of each monomer
 */
void FixMonomerCoordinates(std::vector<double> &xyz, std::vector<double> box, std::vector<double> box_inv,
                           std::vector<size_t> nat, std::vector<size_t> first_index);

/**
 * @brief This function finds the monomer 2 mirror image that is closer to
 * the monomer 1 image.
 *
 * In periodic boundary conditions, it will look for the image of the monomer
 * in xyz2 that is closer to the monomer in xyz1. This function assumes that
 * all monomers in xyz1 are of the same type, and all monomers in xyz2 are
 * also of the same type.
 * Monomers in xyz1, xyz2, and xyz3 can be of a different type.
 * The monomers in xyz1 and xyz2 have to be paired,
 * meaning that monomer 1 in xyz1 and monomer 1 in xyz2 are the first dimer
 * @param[in] box Vector of 9 component with the 3 vectors of the box
 * @param[in] nat1 Number of atoms of monomer 1
 * @param[in] nat2 Number of atoms of monomer 2
 * @param[in] nd Number of dimers
 * @param[in,out] xyz1 coordinates of all the monomers 1, one after
 * the other one. At output, it will be untouched.
 * @param[in,out] xyz2 coordinates of all the monomers 2, one after
 * the other one. At output, it will be modified so the coordinates
 * of monomer 2 are the closer image to monomer 1.
 */
void GetCloseDimerImage(std::vector<double> box, std::vector<double> box_inv, size_t nat1, size_t nat2, size_t nd,
                        double *xyz1, double *xyz2);

/**
 * @brief This function finds the monomers 2 and 3 mirror image that is
 * closer to the monomer 1 image.
 *
 * In periodic boundary conditions, it will look for the image of the monomer
 * in xyz2 and xyz3 that is closer to the monomer in xyz1.
 * This function assumes that
 * all monomers in xyz1 are of the same type, and all monomers in xyz2 are
 * also of the same type, and all monomers in xyz3 are
 * also of the same type.
 * Monomers in xyz1, xyz2, and xyz3 can be of a different type.
 * The monomers in xyz1, xyz2, and xyz3 have to be paired,
 * meaning that monomer 1 in xyz1, monomer 1 in xyz2, and monomer 1
 * in xyz3 are the first trimer
 * @param[in] box Vector of 9 component with the 3 vectors of the box
 * @param[in] nat1 Number of atoms of monomer 1
 * @param[in] nat2 Number of atoms of monomer 2
 * @param[in] nat3 Number of atoms of monomer 3
 * @param[in] nt Number of dimers
 * @param[in,out] xyz1 coordinates of all the monomers 1, one after
 * the other one. At output, it will be untouched.
 * @param[in,out] xyz2 coordinates of all the monomers 2, one after
 * the other one. At output, it will be modified so the coordinates
 * @param[in,out] xyz3 coordinates of all the monomers 3, one after
 * the other one. At output, it will be modified so the coordinates
 * of monomer 3 are the closer image to monomer 1.
 */
void GetCloseTrimerImage(std::vector<double> box, std::vector<double> box_inv, size_t nat1, size_t nat2, size_t nat3,
                         size_t nt, std::vector<double> &xyz1, std::vector<double> &xyz2, std::vector<double> &xyz3);

/**
 * @brief Gets the dimers and/or trimers of a system in which the first
 * monomer index is between istart and iend (iend not included)
 *
 * Given a vector of doubles with the coordinates of all monomers,
 * this function calculates, using a kd-tree, the monomers that are
 * closer to the first one, always inside the cutoff range. This
 * function can also work in PBC, but won't be very efficient for
 * extremely large systems (>8000 monomers)
 * The dimers and trimers will be returned in order, <i,j> and
 * <i,j,k> with k > j > i
 * @param[in] n_max Maximum order of the cluster. Implemented for
 * n_max = 2,3. Otherwise will give an error.
 * @param[in] cutoff Maximum distance between monomers accepted. Larger
 * distances will not be considered a cluster
 * @param[in] istart Minimum value of index i
 * @param[in] iend Maximum value of index i (not included in the clusters)
 * @param[in] nmon Number of monomers
 * @param[in] use_pbc Boolean that states if we are in PBC or not
 * @param[in] box Vector of 9 components with the three main vectors
 * of the box
 * @param[in] xyz_orig Coordinates of the system
 * @param[in] first_index First index of the monomers in the system
 * @param[in] is_local is local/ghost descriptor for monomers in system
 * @param[out] dimers Vector of unsigned integers with the dimers
 * @param[out] trimers Vector of unsigned integers with the trimers
 * @param[in] use_ghost whether or not to include ghost monomers in clusters; this is optional
 * @warning The distance between monomers is computed as the distance
 * between the first atom of both monomers
 */
void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend, size_t nmon, bool use_pbc,
                 std::vector<double> box, std::vector<double> box_inverse, std::vector<double> xyz_orig,
                 std::vector<size_t> first_index, std::vector<size_t> is_local, std::vector<size_t> &dimers,
                 std::vector<size_t> &trimers, bool use_ghost = false);

/**
 * @brief Sets the excluded pairs for a given monomer
 *
 * Given the id of a monomer, it will return the excluded pairs at
 * distances 1-2, 1-3 and 1-4. The excluded pairs are given by a set
 * of pairs, in which each pair specifies the two atoms that are
 * excluded.
 * @param[in] mon Monomer id
 * @param[in] mon_j Json object with monomer information
 * @param[out] exc12 Set of pairs with the 1-2 excluded atoms
 * @param[out] exc13 Set of pairs with the 1-3 excluded atoms
 * @param[out] exc14 Set of pairs with the 1-4 excluded atoms
 */
void GetExcluded(std::string mon, nlohmann::json mon_j, excluded_set_type &exc12, excluded_set_type &exc13,
                 excluded_set_type &exc14);

/**
 * @brief Helper function to compare a pair of an unsigned integer and a
 * double.
 *
 * Function that compares the pairs used in AddCLusters
 * @param[in] a First pair
 * @param[in] b Second pair
 * @return True if a.first < b.first, False otherwise
 */
bool ComparePair(std::pair<size_t, double> a, std::pair<size_t, double> b);

/**
 * @brief Checks if the pair a,b or b,a is in the excluded set exc
 *
 * Returns if a pair a,b (or b,a) is in the excluded set.
 * @param[in] exc Excluder pairs set
 * @param[in] a Atom index 1
 * @param[in] b Atom index 2
 * @return True if the pair a,b or b,a is in exc
 */
bool IsExcluded(excluded_set_type exc, size_t a, size_t b);

/**
 * @brief Gets the thole damping for dipole dipole
 *
 * Depending on if the pair is 1-2, 1-3 or 1-4 distance, it returns the
 * thole damping for dipole-dipole for a given monomer
 * @param[in] is12 Boolean that states if we want the 1-2 damping
 * @param[in] is13 Boolean that states if we want the 1-3 damping
 * @param[in] is14 Boolean that states if we want the 1-4 damping
 * @param[in] mon Id of the monomer
 * @return The damping. If all the booleans are false, will return the
 * intermolecular damping (non-bonded damping).
 */
double GetAdd(bool is12, bool is13, bool is14, std::string mon);
double GetAcc(std::string mon);

/**
 * @brief Reorders a vector of 3N coordinates, where N is the number
 * of sites, from the system order to the input order
 *
 * This function is used to reorder coordinates, dipoles, or
 * gradients from the system order to the input order.
 * @param[in] coords Vector of doubles that can be any physical
 * or spatial property that has 3N elements
 * @param[in] original_order Vector of pairs with the input order
 * of the monomers in thesystem order
 * @param[in] first_index Contains the position of the first atom
 * of a monomer in the system atom list
 * @param[in] sites Vector with the number of sites of each monomer
 * @return The reordered vector that includes ALL sites
 */
std::vector<double> ResetOrder3N(std::vector<double> coords, std::vector<std::pair<size_t, size_t>> original_order,
                                 std::vector<size_t> first_index, std::vector<size_t> sites);

/**
 * @brief Reorders a vector of 3N coordinates, where N is the number
 * of atoms, from the system order to the input order
 *
 * This function is used to reorder coordinates, dipoles, or
 * gradients from the system order to the input order.
 * @param[in] coords Vector of doubles that can be any physical
 * or spatial property that has 3N elements
 * @param[in] original_order Vector of pairs with the input order
 * of the monomers in thesystem order
 * @param[in] numats Number of atoms (real atoms) in the system
 * @param[in] first_index Contains the position of the first atom
 * of a monomer in the system atom list
 * @param[in] nats Vector with the number of real atoms of each monomer
 * @return The reordered vector that includes only the real sites
 */
std::vector<double> ResetOrderReal3N(std::vector<double> coords, std::vector<std::pair<size_t, size_t>> original_order,
                                     size_t numats, std::vector<size_t> first_index, std::vector<size_t> nats);

/**
 * @brief Reorders a vector of N elements, where N is the number
 * of sites, from the system order to the input order
 *
 * This function is used to reorder atom names, charges, pols...
 * from the system order to the input order.
 * @param[in] vector_T Vector that can be any physical property
 * with N elements
 * @param[in] original_order Vector of pairs with the input order
 * of the monomers in the system order
 * @param[in] first_index Contains the position of the first atom
 * of a monomer in the system atom list
 * @param[in] sites Vector with the number of real atoms of each monomer
 * @return The reordered vector that includes ALL sites
 */
template <typename T>
std::vector<T> ResetOrderN(std::vector<T> vector_T, std::vector<std::pair<size_t, size_t>> original_order,
                           std::vector<size_t> first_index, std::vector<size_t> sites) {
    std::vector<T> new_vector_T(vector_T.size());
    for (size_t i = 0; i < sites.size(); i++) {
        size_t ini = first_index[i];
        size_t fin = ini + sites[i];
        size_t ini_orig = original_order[i].second;
        std::copy(vector_T.begin() + ini, vector_T.begin() + fin, new_vector_T.begin() + ini_orig);
    }

    return new_vector_T;
}

/**
 * @brief Reorders a vector of N elements, where N is the number
 * of atoms, from the system order to the input order
 *
 * This function is used to reorder atom names, charges, pols...
 * from the system order to the input order.
 * @param[in] vector_T Vector that can be any physical property
 * with N elements
 * @param[in] original_order Vector of pairs with the input order
 * of the monomers in the system order
 * @param[in] numats Number of atoms (real atoms) in the system
 * @param[in] first_index Contains the position of the first atom
 * of a monomer in the system atom list
 * @param[in] nats Vector with the number of real atoms of each monomer
 * @return The reordered vector that includes only the real sites
 */
template <typename T>
std::vector<T> ResetOrderRealN(std::vector<T> vector_T, std::vector<std::pair<size_t, size_t>> original_order,
                               size_t numats, std::vector<size_t> first_index, std::vector<size_t> nats) {
    std::vector<T> new_vector_T(numats);
    for (size_t i = 0; i < nats.size(); i++) {
        size_t ini = first_index[i];
        size_t fin = ini + nats[i];
        size_t ini_orig = original_order[i].second;
        std::copy(vector_T.begin() + ini, vector_T.begin() + fin, new_vector_T.begin() + ini_orig);
    }

    return new_vector_T;
}

/**
 * @brief Calculates the coordinates of the virtual site of a monomer when
 * given the coordinates of the other sites
 *
 * @param[in,out] xyz Vector of doubles with the positions of all sites.
 * It needs the real sites set up, and it will fill the virtual sites
 * (if any)
 * @param[in] mon_id Id of the monomer
 * @param[in] n_mon Number of monomers of the type mon_id
 * @param[in] nsites Number of sites of the type mon_id monomer
 * @param[in] fst_ind First index of the coordinates of the type monomer mon_id
 */
void SetVSites(std::vector<double> &xyz, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);

/**
 * @brief Sets the charges of a system. If there are osition dependent charges,
 * it also calculates them.
 *
 * Given the xyz of the system, and the first index of the monomer type we are
 * filling in teh charges, it will set the vector charges with the (position
 * dependent) charges.
 * @param[in] xyz Coordinates of the system
 * @param[out] charges Vector with the charges of the monomer type filled.
 * Can contain charges for other monomer types. They won't be overwritten
 * @param[in] mon_id Id of the monomer we are filling the charges for
 * @param[in] n_mon Number of monomers of type mon_id
 * @param[in] nsites Number of sites of monomer type mon_id
 * @param[in] fst_ind First index of first monomer of type mon_id
 * @param[out] chg_der Vector of doubles that will be filled with the charge
 * gradients of the position dependent charges
 * @param[in] mon_j Json object with extra monomer definitions
 */
void SetCharges(std::vector<double> xyz, std::vector<double> &charges, std::string mon_id, size_t n_mon, size_t nsites,
                size_t fst_ind, std::vector<double> &chg_der, nlohmann::json mon_j);

/**
 * @brief Sets the polarizability factors of a system.
 *
 * Given the first index of the monomer type we are
 * filling in, it will set the polarizability factors
 * @param[out] polfac Vector with the polarizability factor of the monomer type filled.
 * Can contain polarizability factors for other monomer types. They won't be overwritten
 * @param[in] mon_id Id of the monomer we are filling the charges for
 * @param[in] n_mon Number of monomers of type mon_id
 * @param[in] nsites Number of sites of monomer type mon_id
 * @param[in] fst_ind First index of first monomer of type mon_id
 * @param[in] mon_j Json object with extra monomer definitions
 */
void SetPolfac(std::vector<double> &polfac, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind,
               nlohmann::json mon_j);

/**
 * @brief Sets the polarizabilities of a system.
 *
 * Given the first index of the monomer type we are
 * filling in, it will set the polarizabilities
 * @param[out] pol Vector with the polarizabilities of the monomer type filled.
 * Can contain polarizabilities for other monomer types. They won't be overwritten
 * @param[in] mon_id Id of the monomer we are filling the charges for
 * @param[in] n_mon Number of monomers of type mon_id
 * @param[in] nsites Number of sites of monomer type mon_id
 * @param[in] fst_ind First index of first monomer of type mon_id
 * @param[in] mon_j Json object with extra monomer definitions
 */
void SetPol(std::vector<double> &pol, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind,
            nlohmann::json mon_j);

/**
 * @brief Sets the C6 "charge" for each atom of a system.
 *
 * Given the first index of the monomer type we are
 * filling in, it will set the long range C6. For an atom A, C6_lr = sqrt(C6_AA).
 * @param[out] c6_lr Vector with the C6 coefficients for long range of the monomer type filled.
 * Can contain C6s for other monomer types. They won't be overwritten
 * @param[in] mon_id Id of the monomer we are filling the charges for
 * @param[in] n_mon Number of monomers of type mon_id
 * @param[in] natoms Number of real atoms of monomer type mon_id
 * @param[in] fst_ind First index of first monomer of type mon_id
 * @param[in] mon_j Json object with extra monomer definitions
 */
void SetC6LongRange(std::vector<double> &c6_lr, std::string mon_id, size_t n_mon, size_t natoms, size_t fst_ind,
                    nlohmann::json mon_j);

/**
 * @brief Sets the LJ "charge" for each atom of a system.
 *
 * Given the first index of the monomer type we are
 * filling in, it will set the long range lennard jones charge. For an atom A, lj_lr = 2*sqrt(eps_AA)*sigma_AA^3.
 * @param[out] lj_lr Vector with the LJ charges for long range of the monomer type filled.
 * Can contain lj_chg for other monomer types. They won't be overwritten
 * @param[in] mon_id Id of the monomer we are filling the charges for
 * @param[in] n_mon Number of monomers of type mon_id
 * @param[in] natoms Number of real atoms of monomer type mon_id
 * @param[in] fst_ind First index of first monomer of type mon_id
 * @param[in] repdisp_j Json object with lennard jones coefficients
 */
void SetLJLongRange(std::vector<double> &lj_lr, std::string mon_id, size_t n_mon, size_t natoms, size_t fst_ind,
                    std::vector<std::pair<std::string, std::string>> use_lj, nlohmann::json repdisp_j);

/**
 * @brief Redistributes the virtual site gradients into the real atoms
 *
 * Given the gradients of the whole system, it will redistribute the gradients
 * of the virtual sites (if any) to the corresponding real atoms
 * @param[in] mon Monomer id
 * @param[in] nmon Number of monomers of that type
 * @param[in] fi_crd First index of the coordinates of the first monomer of this type
 * @param[in,out] grad Gradients of the whole system. At exit, will have 0.0 for
 * the virtual sites, and the real sites will be modified accordingly.
 */
void RedistributeVirtGrads2Real(const std::string mon, const size_t nmon, const size_t fi_crd,
                                std::vector<double> &grad);

/**
 * @brief Redistributes the charge gradient into the real gradients.
 *
 * Given the electric field of the system in each site, the charge derivatives
 * and the real gradients, it updates teh gradients in the real sites to
 * account for the charge derivatives
 * @param[in] mon Monomer id
 * @param[in] nmon Number of monomers of that type
 * @param[in] fi_crd First index of the coordinates of the first monomer of this type
 * @param[in] fi_sites First index of the sites of the first monomer of this type
 * @param[in] phi Electric field of the system in each site
 * @param[in,out] grad Gradients of the system
 * @param[in] chg_grad Charge derivatives
 */
void ChargeDerivativeForce(const std::string mon, const size_t nmon, const size_t fi_crd, const size_t fi_sites,
                           const std::vector<double> phi, std::vector<double> &grad, const std::vector<double> chg_grad,
                           double *crd = 0, std::vector<double> *qdvirial = 0);

}  // namespace systools
#endif  // SYS_TOOLS_H

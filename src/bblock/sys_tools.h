#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <algorithm>

#include "nanoflann.hpp"
#include "kdtree_utils.h"
#include "tools/definitions.h"

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
 * @brief Orders the monomers into the internal order, putting
 * equivalent monomers one after the otehr one to maximize efficiency.
 *
 * This function will reorder the monomer list in a way that can maximize
 * the efficiency in the parallel loops. It will put all the equivalent
 * monomers together, one after the other one, from less frquent to 
 * more frequent. 
 * @param[in,out] mon List of monomers in the input order that will be
 * cleared and replaced by the list of monomers in the internal order.
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
std::vector<std::pair<std::string,size_t>> OrderMonomers
         (std::vector<std::string> &mon,
          std::vector<size_t> sites,
          std::vector<size_t> nats,
          std::vector<size_t> &original2current_order,
          std::vector<std::pair<size_t,size_t> > &original_order,
          std::vector<std::pair<size_t,size_t> > &original_order_realSites);

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
 * @return Total number of sites
 */
size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites,
                     std::vector<size_t> &nat, std::vector<size_t> &fi_at);

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
void FixMonomerCoordinates(std::vector<double> &xyz,
                           std::vector<double> box,
                           std::vector<size_t> nat,
                           std::vector<size_t> first_index);                           

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
void GetCloseDimerImage(std::vector<double> box,
                        size_t nat1, size_t nat2, size_t nd,
                        double * xyz1, double * xyz2);

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
void GetCloseTrimerImage(std::vector<double> box,
                        size_t nat1, size_t nat2, size_t nat3, size_t nt,
                        double * xyz1, double * xyz2, double * xyz3);

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
 * @param[out] dimers Vector of unsigned integers with the dimers
 * @param[out] trimers Vector of unsigned integers with the trimers
 * @warning The distance between monomers is computed as the distance 
 * between the first atom of both monomers
 */
void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend,
                 size_t nmon, bool use_pbc, 
                 std::vector<double> box,
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

// Reorders the gradients or coordinates to the original order
std::vector<double> ResetOrder3N(std::vector<double> coords,
    std::vector<std::pair<size_t,size_t> > original_order, 
    std::vector<size_t> first_index,
    std::vector<size_t> sites);

// Reorders the gradients or coordinates to the original order
// only taking into account real sites. 
std::vector<double> ResetOrderReal3N(std::vector<double> coords,
    std::vector<std::pair<size_t,size_t> > original_order,
    size_t numats,
    std::vector<size_t> first_index,
    std::vector<size_t> nats);

// Reorders all the atom names to the original order
template <typename T>
std::vector<T> ResetOrderN(std::vector<T> vector_T,
    std::vector<std::pair<size_t,size_t> > original_order,
    std::vector<size_t> first_index,
    std::vector<size_t> sites) {

  std::vector<T> new_vector_T(vector_T.size());
  for (size_t i = 0; i < sites.size(); i++) {
    size_t ini = first_index[i];
    size_t fin = ini + sites[i];
    size_t ini_orig = original_order[i].second;
    std::copy(vector_T.begin() + ini, vector_T.begin() + fin,
              new_vector_T.begin() + ini_orig);
  }

  return new_vector_T;
}

// Reorders the real atom names to the original order
template <typename T>
std::vector<T> ResetOrderRealN(std::vector<T> vector_T,
    std::vector<std::pair<size_t,size_t> > original_order,
    size_t numats,
    std::vector<size_t> first_index,
    std::vector<size_t> nats) {

  std::vector<T> new_vector_T(numats);
  for (size_t i = 0; i < nats.size(); i++) {
    size_t ini = first_index[i];
    size_t fin = ini + nats[i];
    size_t ini_orig = original_order[i].second;
    std::copy(vector_T.begin() + ini, vector_T.begin() + fin,
              new_vector_T.begin() + ini_orig);
  }

  return new_vector_T;
}

// Calculates the coordinates of the virtual site of a monomer when
// given the coordinates of the other sites
void SetVSites(std::vector<double> &xyz, std::string mon_id,
               size_t n_mon, size_t nsites, size_t fst_ind);

// Calculates the charges of all the sites in a monomer using its xyz
// coordinates
void SetCharges(std::vector<double> xyz, std::vector<double> &charges,
                std::string mon_id, size_t n_mon, size_t nsites, 
                size_t fst_ind, std::vector<double> &chg_der);
void SetPolfac (std::vector<double> &polfac, std::string mon_id,
                size_t n_mon, size_t nsites, size_t fst_ind);
// FIXME polfac here should not be necessary (we are only setting pols
void SetPol (std::vector<double> &pol, 
             std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);

void RedistributeVirtGrads2Real(const std::string mon, const size_t nmon,
        const size_t fi_crd, std::vector<double> &grad);

void ChargeDerivativeForce(const std::string mon, const size_t nmon,
        const size_t fi_crd, const size_t fi_sites,
        const std::vector<double> phi, std::vector<double> &grad,
        const std::vector<double> chg_grad); 

} // systools
#endif // SYS_TOOLS_H

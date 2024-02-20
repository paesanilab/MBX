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

#ifndef BUCKINGHAM_H
#define BUCKINGHAM_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "potential/buckingham/bucktools.h"
#include "tools/definitions.h"
#include "bblock/sys_tools.h"
#include "tools/math_tools.h"
#include "json/json.h"

/**
 * @file buckingham.h
 * @brief Contains the class Buckingham, that calculates the classical repulsion.
 */

/**
 * @namespace buck
 * @brief Encloses the functions related to classical repulsion.
 */

namespace buck {

class Buckingham {
   public:
    // Default constructor
    Buckingham(){};

    // Default destructor
    ~Buckingham(){};

    /**
     * @brief Initializes the Buckingham class
     *
     * Initializes the class with some control parameters and the system itself. The documentation and the code refers
     * to system order and internal order. System order is the standard order for a normal system. This is
     * at1,at2,at3... and if xyz or gradients, x_at1,y_at1,z_at1,x_at2,y_at2..., The vectorized order resorts the data
     * to allow vectorization in the operations.
     * @param[in] sys_xyz Vector of doubles with the coordinates of the system in system order, of the full system.
     * @param[in] mon_id Vector with the monomer ids of the full system.
     * @param[in] num_atoms Vector with the number of atoms of each of the monomers
     * @param[in] mon_type_count Vector of pairs in which each pair contains the monomer id and the number of monomers
     * of that type.
     * @param[in] force_ttm_for_idx Vector with th monomer indexes for which TTM-nrg will be forced, typically because
     * that monomer is highly distorted.
     * @param[in] islocal
     * @param[in] do_grads Gradients will be calculated if set to true
     * @param[in] box Contains the box of the system
     */
    void Initialize(const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                    const std::vector<size_t> &num_atoms,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count,
                    const std::vector<size_t> force_ttm_for_idx, const std::vector<size_t> &islocal,
                    const bool do_grads, const std::vector<double> &box);

    /**
     * @brief Calculates the repulsion energy of the system
     *
     * With the information stored in the class from initialization or setting new parameters, this function will
     * calculate the buckingham energy for the system.
     * @param[in,out] grad Gradients of the system. they will be updated.
     * @param[in,out] virial Contains teh virial tensor
     * @param[in] use_ghost Will use ghost atoms if true
     * @return Total repulsion energy
     */
    double GetRepulsion(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);

    void SetJsonDispersionRepulsion(nlohmann::json repdisp_j);
    void SetJsonMonomers(nlohmann::json mon_j);

    /**
     * @brief Updates the information of the system in the class.
     *
     * This function is used to update coordinates and other information in the class, to be able to recalculate the
     * energy without having to initialize it all over again
     * @param[in] xyz Coordinates of the system in system order
     * @param[in] buck_pairs Pairs of monomers for which the buckingham must be calculated
     * @param[in] force_ttm_for_idx Vector with th monomer indexes for which TTM-nrg will be forced, typically because
     * that monomer is highly distorted.
     * @param[in] do_grads Gradients will be calculated if set to true
     * box Contains the box of the system
     */
    void SetNewParameters(const std::vector<double> &xyz,
                          const std::vector<std::pair<std::string, std::string> > &buck_pairs,
                          const std::vector<size_t> force_ttm_for_idx, bool do_grads, const double cutoff,
                          const std::vector<double> &box);

    /**
     * @brief Sets the cutoff for dispersion interactions
     *
     * @param[in] cutoff New cutoff value
     */
    void SetCutoff(double cutoff);

    // Getters
    /**
     * @brief Returns the IsLocal variable in the class
     */
    std::vector<size_t> GetIsLocal();
    /**
     * @brief Returns the vector with the indexes for which ttm will be enforced due to high distorsions
     */
    std::vector<size_t> GetEnforceTTMForIndex();
    /**
     * @brief Returns the vector with the pairs for which TTM must be computed.
     */
    std::vector<std::pair<std::string, std::string> > GetBuckPairs();
    /**
     * @brief Returns the monomers ids variable in the class
     */
    std::vector<std::string> GetMonIds();
    /**
     * @brief Returns the vector with number of atoms in the class
     */
    std::vector<size_t> GetNumAtomsVector();
    /**
     * @brief Returns the count of monomers of each type variable in the class
     */
    std::vector<std::pair<std::string, size_t> > GetMonTypeCount();
    /**
     * @brief Returns the do_grads variable in the class
     */
    bool GetDoGrads();
    /**
     * @brief Returns the box variable in the class
     */
    std::vector<double> GetBox();
    /**
     * @brief Returns the box inverse variable in the class
     */
    std::vector<double> GetBoxInverse();
    /**
     * @brief Returns the xyz variable in system order in the class
     */
    std::vector<double> GetSystemXyz();
    /**
     * @brief Returns the xyz variable in internal order in the class
     */
    std::vector<double> GetInternalXyz();
    /**
     * @brief Returns the gradients in internal order in the class
     */
    std::vector<double> GetInternalGrads();
    /**
     * @brief Returns the gradients in system order in the class
     */
    std::vector<double> GetSystemGrads();
    /**
     * @brief Returns the  variable in the class
     */
    std::vector<double> GetVirial();
    /**
     * @brief Gets the cutoff for real space interactions
     */
    double GetCutoff();
    /**
     * @brief Gets the current dispersion repulsion json
     */
    nlohmann::json GetJsonDispersionRepulsion();
    /**
     * @brief Gets the current monomer info json
     */
    nlohmann::json GetJsonMonomers();

   private:
    /**
     * @brief Reorganizes data from system order to vectorized order
     *
     * Organize xyz so we have
     * x1_1 x1_2 ... y1_1 y1_2... z1_1 z1_2 ... x2_1 x2_2 ...
     * where xN_M is read as coordinate x of site N of monomer M
     * for the first monomer type. Then follows the second, and so on.
     * As an example, for a water dimer (OHHM), the system xyz would be
     * sys_xyz = {Ox, Oy, Oz, Hx, Hy, Hz, Hx, Hy, ...}
     * And after reordering,
     * xyz = {Ox, Ox, Oy, Oy, Oz, Oz, Hx, Hx, Hy, Hy, Hz, Hz, Hx, Hx, ... ...}
     */
    void ReorderData();

    /**
     * @brief Calculates the repulsion for the system
     *
     * Once the system is initialized, this function calculates the repulsion energy
     * @param[in] use_ghost Will use ghost atoms if true
     */
    void CalculateRepulsion(bool use_ghost = 0);

    /**
     * @brief Calculates the repulsion for the system only for the enforced monomers
     *
     * Once the system is initialized, this function calculates the repulsion energy for the pairs involving monoemrs
     * for which TTM is enforced.
     * @param[in] use_ghost Will use ghost atoms if true
     */
    void CalculateEnforcedRepulsion(bool use_ghost = 0);

    // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    std::vector<double> sys_xyz_;
    // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
    std::vector<double> xyz_;
    // local/ghost descriptor for monomers
    std::vector<size_t> islocal_;
    // Name of the monomers (h2o, f...)
    std::vector<std::string> mon_id_;
    // Number of sites of each mon
    std::vector<size_t> num_atoms_;
    // monomers of each type.
    std::vector<std::pair<std::string, size_t> > mon_type_count_;

    // pairs that will use the buckingham
    std::vector<std::pair<std::string, std::string> > buck_pairs_;

    // Indexes of the monomers for which TTM is forced
    std::vector<size_t> force_ttm_for_idx_;

    // Bool that if true will perform the gradients calculation.
    bool do_grads_;

    // Gradients in the original order (same as xyz). This is the vector in
    // which the electrostatics gradients will be added.
    std::vector<double> sys_grad_;
    // Gradients
    std::vector<double> grad_;
    // Virial tensor
    std::vector<double> virial_;
    // virial calc variable
    bool calc_virial_;

    // Total number of atoms
    size_t natoms_;
    // Max number of monomers
    size_t maxnmon_;
    // Repulsion energy
    double rep_energy_;
    // box of the system
    std::vector<double> box_;
    // inverted box of the system
    std::vector<double> box_inverse_;
    // use pbc in the electrostatics calculation
    bool use_pbc_;
    // dispersion cutoff
    double cutoff_;

    // Json object with extra user-defined buckingham coefficients
    nlohmann::json repdisp_j_;

    // Json object with extra monomer properties
    nlohmann::json mon_j_;

    // Vector with c6 coefficients
    std::vector<std::vector<double> > a_all_;

    // Vector with d6 coefficients
    std::vector<std::vector<double> > b_all_;

    // Vector with the bool use ttm
    std::vector<bool> use_buck_all_;
};

}  // namespace buck

#endif

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

#ifndef ELECTROSTATICS_H
#define ELECTROSTATICS_H

#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>
#include <memory>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "bblock/sys_tools.h"
#include "tools/definitions.h"
#include "tools/constants.h"
#include "tools/math_tools.h"
#include "potential/electrostatics/gammq.h"
#include "potential/electrostatics/fields.h"

#include "kdtree/kdtree_utils.h"
#include "helpme.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @file electrostatics.h
 * @brief Contains the electrostatics class declaration, along
 * with all its member functions and variables.
 */

enum {
    ELE_PERMDIP_REAL = 0,
    ELE_PERMDIP_PME,

    ELE_DIPFIELD_REAL,
    ELE_DIPFIELD_PME,

    ELE_GRAD_REAL,
    ELE_GRAD_PME,
    ELE_GRAD_FIN,

    ELE_NUM_TIMERS
};

/**
 * @namespace elec
 * @brief Namespace that includes all the electrostatic related functions
 * and classes
 */
namespace elec {

class Electrostatics {
   public:
    /**
     * @brief Initializes the electrostatics class with the system information
     *
     * Given the information of the system, such as charges, coordinates,
     * polarizabilities, ... of the system, it initializes the class,
     * reordering in the proper way the variables that need to be reordered.
     * @param[in] chg Charges of the system
     * @param[in] chg_grad Charge gradients previously obtained when
     * the charges were set in the system
     * @param[in] polfac Vector of polarizability factors of all atoms the system
     * @param[in] pol Vector of polarizabilities of all atoms the system
     * @param[in] sys_xyz Vector of coordinates of all atoms of the system
     * @param[in] mon_id Vector with the id of all the monomers in the system
     * @param[in] sites Vector with the number of sites of all monomers int he system
     * @param[in] first_ind Vector with the index in the atom list of the
     * first atom of each monomer
     * @param[in] mon_type_count Vector of pairs. First is the id, second is
     * the number of monomers of that id.
     * @param[in] do_grads If true, grads will be computed
     * @param[in] tolerance Maximum error per dipole to assume convergence in
     * iterative methods
     * @param[in] maxit Maximum number of iterations of the dipole calculation
     * @param[in] dip_method Method to use to compute the dipoles
     * @param[in] box Vector of 9 components with the box. The elements are:
     * {v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z} where v1, v2, and v3 are the
     * trhee main vectors of the box
     */
    void Initialize(const std::vector<double> &chg, const std::vector<double> &chg_grad,
                    const std::vector<double> &polfac, const std::vector<double> &pol,
                    const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                    const std::vector<size_t> &sites, const std::vector<size_t> &first_ind,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count,
                    const std::vector<size_t> &islocal_, const bool do_grads = true, const double tolerance = 1E-16,
                    const size_t maxit = 100, const std::string dip_method = "iter",
                    const std::vector<double> &box = {});

    void SetMPI(MPI_Comm world_, size_t proc_grid_x, size_t proc_grid_y, size_t proc_grid_z);

    /**
     * @brief Gets the electrostatic energy
     *
     * Once the system has been initialized, computes the electrostatic energy
     * of the system.
     * @param[out] grad Gradients will be saved here
     * @return Total electrostatic energy
     */
    double GetElectrostatics(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);
    double GetElectrostaticsMPIlocal(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);

    /**
     * @brief Clears the ASPC history
     *
     * If ASPC is not being used, will reset the dipole history anyways.
     * It basically clears out the dipole history vector. Then it is forced to
     * recalculate the dipoles iteratively to get a new history.
     */
    void ResetAspcHistory();

    /**
     * @brief "Reinitializes" the electrostatics class.
     *
     * If the system changes (coordinates, charges, and so on) it is
     * needed to reenter the information to the system, and it is done
     * with this function.
     * @param[in] xyz Vector of coordinates of all atoms of the system
     * @param[in] chg Charges of the system
     * @param[in] chg_grad Charge gradients previously obtained when
     * the charges were set in the system
     * @param[in] pol Vector of polarizabilities of all atoms the system
     * @param[in] polfac Vector of polarizability factors of all atoms the system
     * @param[in] dip_method Method to use to compute the dipoles
     * @param[in] do_grads If true, grads will be computed
     * @param[in] box Vector of 9 components with the box. The elements are:
     * {v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z} where v1, v2, and v3 are the
     * trhee main vectors of the box
     * @param[in] use_pbc Boolean specifying if PBC are going to be used or not
     */
    void SetNewParameters(const std::vector<double> &xyz, const std::vector<double> &chg,
                          const std::vector<double> &chg_grad, const std::vector<double> &pol,
                          const std::vector<double> &polfac, const std::string dip_method, const bool do_grads,
                          const std::vector<double> &box = {}, const double cutoff = 100.0);

    /**
     * @brief Sets the cutoff for electrostatic interactions
     *
     * @param[in] cutoff New cutoff value
     */
    void SetCutoff(double cutoff);

    /**
     * @brief Sets the dipole convergence criteria
     *
     * @param[in] tol Dipole convergence criteria
     */
    void SetDipoleTolerance(double tol);

    /**
     * @brief Sets the maximum number of iterations in the dipole loop
     *
     * @param[in] maxit Maximum number of iterations
     */
    void SetDipoleMaxIt(size_t maxit);

    /**
     * @brief Sets the Ewald attenuation parameter (in units of 1/Angstrom)
     *
     * @param[in] cutoff New cutoff value
     */
    void SetEwaldAlpha(double cutoff);

    /**
     * @brief Sets the PME grid density.
     *
     * @param[in] density the number of grid points per Angstrom in each dimension of the PME grid.
     */
    void SetEwaldGridDensity(double density);

    /**
     * @brief Sets the PME B-Spline order.
     *
     * @param[in] order the order of the B-Spline used to spread charges
     */
    void SetEwaldSplineOrder(int order);

    /**
     * @brief Returns permanent electrostatic energy.
     *
     * @return Permanent electrostatic energy. Undefined if energy has not yet been calculated
     */
    double GetPermanentElectrostaticEnergy();

    std::vector<double> GetInducedDipoles();
    std::vector<double> GetPermanentDipoles();
    std::vector<double> GetMolecularInducedDipoles();
    std::vector<double> GetMolecularPermanentDipoles();

    /**
     * @brief Returns induced electrostatic energy.
     *
     * @return Induced electrostatic energy. Undefined if energy has not yet been calculated
     */
    double GetInducedElectrostaticEnergy();

    std::vector<size_t> GetInfoCounts();
    std::vector<double> GetInfoTimings();

    /**
     * Sets global box dimensions for PME solver; does not alter original PBC settings
     * @param[in] box is a 9 component vector of double with
     * the three main vectors of the cell: {v1x v1y v1z v2x v2y v2z v3x v3y v3z}
     */
    void SetBoxPMElocal(std::vector<double> box);

   private:
    void CalculatePermanentElecField(bool use_ghost = 0);
    void CalculatePermanentElecFieldMPIlocal(bool use_ghost = 0);
    void CalculateDipolesIterative();
    void ComputeDipoleField(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost = 0);
    void ComputeDipoleFieldMPIlocal(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost = 0);
    void CalculateDipolesCG();
    void CalculateDipolesCGMPIlocal(bool use_ghost = 0);
    void DipolesCGIteration(std::vector<double> &in_v, std::vector<double> &out_v);
    void DipolesCGIterationMPIlocal(std::vector<double> &in_v, std::vector<double> &out_v, bool use_ghost = 0);
    void CalculateDipolesAspc();
    void SetAspcParameters(size_t k);
    void CalculateDipoles();
    void CalculateDipolesMPIlocal(bool use_ghost = 0);
    void CalculateElecEnergy();
    void CalculateElecEnergyMPIlocal();
    void CalculateGradients(std::vector<double> &grad);
    void CalculateGradientsMPIlocal(std::vector<double> &grad, bool use_ghost = 0);

    void reverse_forward_comm(std::vector<double> &in_v);
    void reverse_comm(std::vector<double> &in_v);

    void ReorderData();

    // PME solver
    // helpme::PMEInstance<double> pme_solver_;
    // Charges of each site. Order has to follow mon_type_count.
    std::vector<double> chg_;
    // Charges of each site. Order has to follow mon_type_count.
    std::vector<double> sys_chg_;
    // Gradients due to site dependent charges
    std::vector<double> sys_chg_grad_;
    // Polfacs of each site. For now assuming not site dependent.
    std::vector<double> polfac_;
    // Square root of pol of each site. Size 3N (3 per atom, are the same)
    std::vector<double> pol_sqrt_;
    // Polarizabilities of each site. For now assuming not site dependent.
    std::vector<double> pol_;
    // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    std::vector<double> sys_xyz_;
    // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
    std::vector<double> xyz_;
    // local/ghost descriptor for monomers
    std::vector<size_t> islocal_;
    // local/ghost descriptor for atoms
    std::vector<size_t> islocal_atom_;
    // local/ghost descriptor for xyz of atoms
    std::vector<size_t> islocal_atom_xyz_;
    // Name of the monomers (h2o, f...)
    std::vector<std::string> mon_id_;
    // Number of sites of each mon
    std::vector<size_t> sites_;
    // First index of each atom in system
    std::vector<size_t> first_ind_;
    // Vector that contains all different monomer types and the number of
    // monomers of each type.
    std::vector<std::pair<std::string, size_t> > mon_type_count_;
    // Tolerance in the iterative calculation of the dipoles
    // Tolerance refers to the maximum squared difference overall the dipoles
    double tolerance_;
    // Maximum number of iterations allowed in the iterative dipole calculation
    size_t maxit_;
    // Bool that if true will perform the gradients calculation.
    bool do_grads_;
    // Gradients in the original order (same as xyz). This is the vector in
    // which the electrostatics gradients will be added.
    std::vector<double> sys_grad_;
    // Gradients
    std::vector<double> grad_;
    // Electric potential on each site with sys order
    std::vector<double> sys_phi_;
    // Electric potential on each site
    std::vector<double> phi_;
    // Reciprocal space electric potential and field, in sys order at each site
    std::vector<double> rec_phi_and_field_;
    // Permanent electric field with sys order
    std::vector<double> sys_Efq_;
    // Permanent electric field
    std::vector<double> Efq_;
    // Induced electric field with sys_order
    std::vector<double> sys_Efd_;
    // Induced electric field
    std::vector<double> Efd_;
    // Induced dipoles with sys order
    std::vector<double> sys_mu_;
    // Permanent dipoles with sys order
    std::vector<double> sys_perm_mu_;
    // Molecular induced dipoles with sys order
    std::vector<double> sys_mol_mu_;
    // Molecular permanent dipoles with sys order
    std::vector<double> sys_mol_perm_mu_;
    // Dipoles
    std::vector<double> mu_;
    // Dipole history for ASPC
    std::vector<double> mu_hist_;
    // Dipole predictor
    std::vector<double> mu_pred_;
    // B constants to use for ASPC
    std::vector<double> b_consts_aspc_;
    // omega used in ASPC
    double omega_aspc_;
    // Number of history steps stored
    size_t hist_num_aspc_;
    // Order of ASPC
    size_t k_aspc_;
    // Total number of electrostatic sites
    size_t nsites_;
    // Total number of monomers (sum of monomer type)
    size_t nmon_total_;
    // Thole dampings
    double aCC_, aCD_, aDD_;
    // Constants to be used later:
    // Gamma 3/4
    double g34_;
    // aCC ^ (1/4)
    double aCC1_4_;
    // Max number of monomers
    size_t maxnmon_;
    // Permanent electrostatics
    double Eperm_;
    // Induced electrostatics
    double Eind_;
    // Method for dipoles (ITERative, Conjugate Gradient, ASPC, INVersion)
    std::string dip_method_;
    // box of the system
    std::vector<double> box_;
    // inverse of the unit cell
    std::vector<double> box_inverse_;
    // box in ABCabc format
    std::vector<double> box_ABCabc_;
    // box of the domain-decomposed system
    std::vector<double> box_PMElocal_;
    std::vector<double> box_inverse_PMElocal_;
    // use pbc in the electrostatics calculation
    bool use_pbc_;
    // electrostatics cutoff
    double cutoff_;
    // ewald attenuation parameter in inverse angstroms
    double ewald_alpha_;
    // PME grid density
    double pme_grid_density_;
    // PME spline order
    int pme_spline_order_;
    // Has the energy been calculated?
    bool has_energy_;
    // virial tensor
    std::vector<double> virial_;
    // calculate the virial tensor ?
    bool calc_virial_;
    // MPI initialized
    bool mpi_initialized_ = false;
    // MPI Communicator
    MPI_Comm world_;
    // MPI Rank
    int mpi_rank_;
    // MPI Comm size
    size_t num_mpi_ranks_;
    // proc_grid
    size_t proc_grid_x_;
    size_t proc_grid_y_;
    size_t proc_grid_z_;

    bool first;

    std::vector<size_t> mbxt_ele_count_;
    std::vector<double> mbxt_ele_time_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace elec

////////////////////////////////////////////////////////////////////////////////

#endif

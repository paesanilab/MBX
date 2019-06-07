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

#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <memory>

// Tools
#include "nanoflann.hpp"
#include "kdtree_utils.h"
#include "bblock/sys_tools.h"
#include "tools/definitions.h"
#include "tools/custom_exceptions.h"

// Potential
// 1B
#include "potential/1b/energy1b.h"
// 2B
#include "potential/2b/energy2b.h"
// 3B
#include "potential/3b/energy3b.h"
// DISPERSION
#include "potential/dispersion/dispersion.h"
//#include "potential/dispersion/dispersion2b.h"
// ELECTROSTATICS
#include "potential/electrostatics/electrostatics.h"

/**
 * @file system.h
 * @brief System class definition, along with all its member functions
 * and variables
 */

/**
 * @namespace bblock
 * @brief Contains all the system building block functions and classes
 */
namespace bblock {

/**
 * The System class is the core class of the whole software.
 * It contains the calls to the enegy functions
 * More stuff to come.
 */

class System {
   public:
    /**
     * Default constructor. Creates a System class.
     */
    System();

    /**
     * Default Destructor. Destroys the System class.
     */
    ~System();

    /////////////////////////////////////////////////////////////////////////////
    // Getters //////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the total number of monomers in the system.
     * @return Number of monomers in the system
     */
    size_t GetNumMon();

    /**
     * Gets the total number of molecules in the system.
     * @return Number of molecules in the system
     * @warning This function should not be used for now
     */
    size_t GetNumMol();

    /**
     * Gets the total number of sites.
     * (real + electrostatic sites such as M-sites)
     * @return Total number of sites (real + electrostatic)
     */
    size_t GetNumSites();

    /**
     * Gets the total number of REAL sites, i.e., number of atoms.
     * @return Number of real sites
     */
    size_t GetNumRealSites();

    /**
     * Gets the numer of atoms the n-th monomer
     * @param[in] n Index of the monomer in the monomer list
     * @return Number of atoms of the n-th monomer
     */
    size_t GetMonNumAt(size_t n);

    void GetMolecularDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind);
    void GetDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind);
    void GetTotalDipole(std::vector<double> &mu_perm, std::vector<double> &mu_ind, std::vector<double> &mu_tot);
    
    /**
     * Returns the charge derivatives for the whole system
     */
    std::vector<double> GetChargeDerivativesOHH();

    /**
     * Returns the charge derivatives for the whole system
     */
    std::vector<double> GetChargeDerivatives();

    /**
     * Gets the position of the first site of monomer n in the atoms vector
     * @param[in] n Index of the monomer in the monomer list
     * @return First index of monomer n
     */
    size_t GetFirstInd(size_t n);

    /**
     * Gets the pairs involving, as the first index, the monomers specified.
     * It will return dimers or trimers depending on nmax.
     * @param[in] nmax Maximum order of the pairs. 2 will calculate dimers.
     * 3 will calculate trimers. nmax > 3 not implemented.
     * @param[in] cutoff Maximum distance conidered to pair monomers
     * @param[in] istart Index of the first monomer to take into account
     * @param[in] iend Index of the last monomer to take into account.
     * This one will NOT be included
     * @return size_t vector with dimension nmax * n-mers. If nmax = 2,
     * it will return a single vector where v[n], n = 0,2,4... is the index
     * first monomer of the dimer, and v[n + 1] is the second one.
     * @warning Order is not guaranteed. This means that the n-mers
     * can come in any order
     */
    std::vector<size_t> GetPairList(size_t nmax, double cutoff, size_t istart, size_t iend);

    /**
     * Gets a vector with the indexes that form a molecule.
     * @param[in] n Index of the molecule in the molecule list
     * @return Vecotr of size_t with the indexes of the monomer that
     * compoise the n-th molecule.
     * @warning Should not be used for now
     */
    std::vector<size_t> GetMolecule(size_t n);

    /**
     * Gets the coordinates of the system in the input order.
     * It will return the coordinates of all sites, including the
     * electrostatic virtual sites.
     * @return A vector of doubles with the xyz in the same order as the input
     */
    std::vector<double> GetXyz();

    /**
     * Gets the coordinates of the system in the input order.
     * It will return the coordinates of only real sites.
     * @return A vector of doubles with the xyz in the same order as the input
     */
    std::vector<double> GetRealXyz();

    /**
     * Gets the gradients of the system in the input order.
     * It will return the gradients of all sites, including the
     * electrostatic virtual sites.
     * @return A vector of doubles with the gradients in the same order as the input
     */
    std::vector<double> GetGrads();

    /**
     * Gets the gradients of the system in the input order.
     * It will return the gradients of only real sites.
     * @return A vector of doubles with the gradients in the same order as the input
     */
    std::vector<double> GetRealGrads();

    /**
     * Gets the charges of the system. It includes the charges of ALL sites,
     * including the virtual sites such as the M-sites
     * @return A vector of doubles with the charges of all sites
     */
    std::vector<double> GetCharges();

    /**
     * Gets the charges of the system. It includes the charges of only real
     * sites
     * @return A vector of doubles with the charges of all real sites
     */
    std::vector<double> GetRealCharges();

    /**
     * Gets the polarizabilities of the system.
     * It includes the charges of ALL sites,
     * including the virtual sites such as the M-sites
     * @return A vector of doubles with the polarizabilities of all sites
     */
    std::vector<double> GetPolarizabilities();

    /**
     * Gets the polarizabilities of the system.
     * It includes the charges of only real sites
     * @return A vector of doubles with the polarizabilities of all real sites
     */
    std::vector<double> GetRealPolarizabilities();

    /**
     * Gets the polarizability factors of the system.
     * It includes the charges of ALL sites,
     * including the virtual sites such as the M-sites
     * @return A vector of doubles with the polarizability factors of all sites
     */
    std::vector<double> GetPolarizabilityFactors();

    /**
     * Gets the polarizability factors of the system.
     * It includes the charges of only real sites
     * @return A vector of doubles with the polarizability factors of all real sites
     */
    std::vector<double> GetRealPolarizabilityFactors();

    /**
     * Get the atom names in the same order as inputed.
     * Will get an array of all the atoma names.
     * This includes also the virtual sites, which will come with
     * the name "virt"
     * @return A vector of strings with the names of all atoms
     */
    std::vector<std::string> GetAtomNames();

    /**
     * Get the atom names in the same order as inputed.
     * Will get an array of all the atoma names.
     * This includes also the virtual sites, which will come with
     * the name "virt"
     * @return A vector of strings with the names of all atoms
     */
    std::vector<std::string> GetRealAtomNames();

    /**
     * Gets the id string of the n-th monomer
     * @param[in] n The index of the monomer which ID is wanted
     * @return A string with the ID of the n-th monomer
     */
    std::string GetMonId(size_t n);

    /////////////////////////////////////////////////////////////////////////////
    // Modifiers ////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Sets the xyz of the system. It assumes that all the coordinates are in the
     * input vector, including the virtual sites.
     * @param[in] xyz Is a vector of doubles that contains the coordinates of
     * the whole system as x1y1z1x2y2z2x3y3z3..., including the virtual sites
     */
    void SetXyz(std::vector<double> xyz);

    /**
     * Sets the xyz of the system. It assumes that only the real coordinates
     * are in the input vector.
     * @param[in] xyz Is a vector of doubles that contains the coordinates of
     * the real atoms as x1y1z1x2y2z2x3y3z3...
     */
    void SetRealXyz(std::vector<double> xyz);

    // TODO Keep in mind that the order must be consistent with
    // the database!!
    /**
     * Adds a monomer to the system
     * @param[in] xyz Is a vector of doubles that contains the coordinates of
     * the real atoms of the monomer
     * @param[in] atoms Is a vector of strings with the atom names (atomic
     * symbols) of the monomer
     * @param[in] id Is a string that contains the identity of the monomer
     * @warning The monomer coordinates and atoms must be in the same order
     * as the database.
     * The id must also match with the database.
     * Please read the documentation carefully.
     */
    void AddMonomer(std::vector<double> xyz, std::vector<std::string> atoms, std::string id);

    /**
     * Adds a molecule to the system. A molecule, in the context of this
     * software, is a set of indexes of monomers that belong to the same
     * molecule.
     * @param[in] molec Is a vector of unsigned integers that contains
     * the indeces of the monomers that belong to this molecule
     * @warning This function should not be used. The code will handle
     * it but do nothing about it.
     */
    void AddMolecule(std::vector<size_t> molec);

    /**
     * Initializes the system once the monomer information is inputed. The
     * system, once created, cannot be modified in terms of monomer composition.
     * To see the default values of the initialization,
     * please read the documentation.
     */
    void Initialize();

    /**
     * Sets the two-body cutoff for dispersion and polynomials.
     * Molecules that are at a larger distance than the cutoff will not
     * be evaluated
     * @param[in] cutoff2b Is the distance, in angstrom, of the cutoff
     */
    void Set2bCutoff(double cutoff2b);

    /**
     * Sets the three-body cutoff for polynomials.
     * Molecules that are at a larger distance than the cutoff will not
     * be evaluated
     * @param[in] cutoff3b Is the distance, in angstrom, of the cutoff
     */
    void Set3bCutoff(double cutoff3b);

    /**
     * Sets the maximum number of monomers in the batch of the 1B evaluation
     * @param[in] nmax Is an unsigned int that will set the
     * maximum number of monomers in the batch of the 1B evaluation
     */
    void SetNMaxEval1b(size_t nmax);

    /**
     * Sets the maximum number of dimers in the batch of the 2B evaluation
     * @param[in] nmax Is an unsigned int that will set the
     * maximum number of dimers in the batch of the 2B evaluation
     */
    void SetNMaxEval2b(size_t nmax);

    /**
     * Sets the maximum number of trimers in the batch of the 3B evaluation
     * @param[in] nmax Is an unsigned int that will set the
     * maximum number of trimers in the batch of the 3B evaluation
     */
    void SetNMaxEval3b(size_t nmax);

    /**
     * Sets the maximum error per dipole in the iterative methods to
     * compute the induced dipoles
     * @param[in] tol Double value that sets the maximum error per dipole
     * in the iterative methods to compute the induced dipoles
     */
    void SetDipoleTol(double tol);

    /**
     * Sets the maximum number of iterations in the iterative methods
     * to converge the induced dipoles
     * @param[in] maxit Maximum number of iterations in the iterative methods
     * to converge the induced dipoles
     */
    void SetDipoleMaxIt(size_t maxit);

    /**
     * Sets the iterative dipole method. See documentation for available methods
     * @param[in] method String with the method abbreviation (iter, cg or aspc)
     */
    void SetDipoleMethod(std::string method);

    /**
     * Resets the dipole history when using ASPC. If other method is used,
     * this function does nothing.
     */
    void ResetDipoleHistory();

    /**
     * Tells the system if we are in Periodic Boundary Conditions (PBC)
     * or not. If the box is not passed as argument, it is set to
     * a cubic box of 1000 Angstrom of side length
     * @param[in] use_pbc Boolean setting if we use PBC (true) or not (false)
     * @param[in] box Optional argument. Is a 9 component vector of double with
     * the three main vectors of the cell: {v1x v1y v1z v2x v2y v2z v3x v3y v3z}
     */
    void SetPBC(std::vector<double> box = {});

    void SetEwald(double alpha, double grid_density, int spline_order);

    void SetEwaldElectrostatics(double alpha, double grid_density, int spline_order);

    void SetEwaldDispersion(double alpha, double grid_density, int spline_order);

    /////////////////////////////////////////////////////////////////////////////
    // Energy Functions /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Returns the energy of the system.
     * In order to retrieve the gradients for only the real sites, use
     * the function GetRealGrads(), and use GetGrads() otherwise.
     * @param[in] do_grads If true, the gradients will be computed. Otherwise,
     * the gradient calculation will not be performed
     * @return Energy of the system in kcal/mol
     */
    double Energy(bool do_grads);

    /**
     * Obtains the one-body energy. This is the sum of all the monomer
     * deformation energies.
     * Gradients will be ONLY for the one-body part.
     * @param[in] do_grads If true, the gradients will be computed. Otherwise,
     * the gradient calculation will not be performed
     * @return One-body energy of the system
     */
    double OneBodyEnergy(bool do_grads);

    /**
     * Obtains the two-body energy. This is the sum of all two-body
     * polynomials and the two-body dispersion.
     * Gradients will be ONLY for the two-body part.
     * @param[in] do_grads If true, the gradients will be computed. Otherwise,
     * the gradient calculation will not be performed
     * @return Two-body energy of the system
     */
    double TwoBodyEnergy(bool do_grads);

    /**
     * Obtains the three-body energy. This is the sum of all the 3B
     * polynomials.
     * Gradients will be ONLY for the three-body part.
     * @param[in] do_grads If true, the gradients will be computed. Otherwise,
     * the gradient calculation will not be performed
     * @return Three-body energy of the system
     */
    double ThreeBodyEnergy(bool do_grads);

    /**
     * Obtains the electrostatic energy. This is the sum of the permanent
     * and induced electrostatics
     * Gradients will be ONLY for the electrostatics part.
     * @param[in] do_grads If true, the gradients will be computed. Otherwise,
     * the gradient calculation will not be performed
     * @return Three-body energy of the system
     */
    double Electrostatics(bool do_grads);

    double Dispersion(bool do_grads);

   private:
    /**
     * Fills the dimers_(i,j) and/or trimers_(i,j,k) vectors, with
     * i < j < k. These i,j,k are the index of the corresponding monomer
     * in the internal order of the system, with i >= istart and i < iend
     * @param[in] nmax Maximum size of the cluster (nmax=2,3)
     * @param[in] cutoff Maximum distance allowed between 2 monomers to
     * consider them for the cluster
     * @param[in] istart Minimum index of i
     * @param[in] iend Maximum index (iend not included) of index i
     */
    void AddClusters(size_t nmax, double cutoff, size_t istart, size_t iend);

    /**
     * Fills the dimers_(i,j) and/or trimers_(i,j,k) vectors, with
     * i < j < k. These i,j,k are the index of the corresponding monomer
     * in the internal order of the system, with i >= istart and i < iend
     * @param[in] nmax Maximum size of the cluster (nmax=2,3)
     * @param[in] cutoff Maximum distance allowed between 2 monomers to
     * consider them for the cluster
     * @param[in] istart Minimum index of i
     * @param[in] iend Maximum index (iend not included) of index i
     * @return Vector of size_t with dimention nclusters * nmax
     */
    std::vector<size_t> AddClustersParallel(size_t nmax, double cutoff, size_t istart, size_t iend);

    /**
     * Fills in the monomer information of the monomers that have been
     * added to the system.
     */
    void AddMonomerInfo();

    /**
     * Sets the charges of the system, including the
     * position dependent charges
     */
    void SetCharges();

    /**
     * Sets the polarizabilities of the system
     */
    void SetPols();

    /**
     * Sets the polarizability factors of the system
     */
    void SetPolfacs();

    /**
     * Sets the position of the electrostatic virtual sites (if any)
     * from the XYZ coordinates set in the system
     */
    void SetVSites();

    void SetC6LongRange();

    /**
     * Private function to internally get the 1b energy.
     * Gradients of the system will be updated.
     * @param[in] do_grads Boolean. If true, gradients will be computed.
     * If false, gradients won't be computed.
     * @return  One-body energy of the system
     */
    double Get1B(bool do_grads);

    /**
     * Private function to internally get the 2b energy.
     * Gradients of the system will be updated.
     * @param[in] do_grads Boolean. If true, gradients will be computed.
     * If false, gradients won't be computed.
     * @return  Two-body energy of the system
     */
    double Get2B(bool do_grads);

    /**
     * Private function to internally get the 3b energy.
     * Gradients of the system will be updated.
     * @param[in] do_grads Boolean. If true, gradients will be computed.
     * If false, gradients won't be computed.
     * @return  Three-body energy of the system
     */
    double Get3B(bool do_grads);

    /**
     * Private function to internally get the electrostatic energy.
     * Gradients of the system will be updated.
     * @param[in] do_grads Boolean. If true, gradients will be computed.
     * If false, gradients won't be computed.
     * @return  Electrostatic energy of the system
     */
    double GetElectrostatics(bool do_grads);

    double GetDispersion(bool do_grads);

   private:
    /**
     * Number of molecules in the system
     */
    size_t nummol;

    /**
     * Number of monomers in the system
     */
    size_t nummon_;

    /**
     * Number of sites in the system. It includes all the electrostatic sites
     */
    size_t numsites_;

    /**
     * Number of sites in the system.
     * It doesn't include the electrostatic sites
     */
    size_t numat_;

    /**
     * Maximum number of monomers to be evaluated in the same batch
     */
    size_t maxNMonEval_;

    /**
     * Maximum number of dimers to be evaluated in the same batch
     */
    size_t maxNDimEval_;

    /**
     * Maximum number of trimers to be evaluated in the same batch
     */
    size_t maxNTriEval_;

    /**
     * Maximum number of iterations allowed in the induced dipole calculation
     */
    size_t maxItDip_;

    /**
     * Maximum error squared per dipole to assume convergence in the
     * induced dipole calculation
     */
    double diptol_;

    /**
     * Cutoff in the search for clusters for the dimers.
     * Molecules which first atoms are at a larger distance than this cutoff
     * will not be considered a 2b cluster
     */
    double cutoff2b_;

    /**
     * Cutoff in the search for clusters for the trimers.
     * Molecules which first atoms are at a larger distance than this cutoff
     * will not be considered a 3b cluster
     */
    double cutoff3b_;

    /**
     * Stores the energy of the system
     */
    double energy_;

    /**
     * Set to true when the system is initialized.
     * Initialization is a requirement in order to perform any
     * energy calculation
     */
    bool initialized_;

    /**
     * If set to tru, the box and periodic boundary conditions will be used and
     * taken into account for the clusters, energy calculations, and any
     * other operation within the system.
     */
    bool use_pbc_;

    /**
     * This variable is set to false when one of the monomer energies is larger
     * than 60 kcal/mol. Due to their construction, the polynomials do not
     * include configurations that distorted.
     */
    bool allMonGood_;

    /**
     * Dispersion class that will be used to get the dispersion energy
     */
    disp::Dispersion dispersionE_;

    /**
     * Electrostatic class that will be used to get the electrostatic energy
     */
    elec::Electrostatics electrostaticE_;

    /**
     * Method used in order to calculate the induced dipoles
     */
    std::string dipole_method_;

    /**
     * Vector that contains, in the internal order of the system, the
     * number of sites of each monomer
     */
    std::vector<size_t> sites_;

    /**
     * Vector that contains, in the internal order of the system, the
     * number of atoms of each monomer
     */
    std::vector<size_t> nat_;

    /**
     * Vector that contains the first index of the first atom of the
     * monomers in the atom list. As an example, first_index_[4]
     * will be the position of the first atom of the 5th monomer
     * in the atom list. This first index accounts also for
     * electrostatic virtual sites
     */
    std::vector<size_t> first_index_;

    /**
     * Vector that stores the dimers computed by the AddClusters functions.
     * The vector stores the two indeces of a dimer, one after the other one,
     * in the internal order of the system.
     */
    std::vector<size_t> dimers_;

    /**
     * Vector that stores the trimers computed by the AddClusters functions.
     * The vector stores the three indeces of a trimer, one after the other one,
     * in the internal order of the system.
     */
    std::vector<size_t> trimers_;

    /**
     * Vector that stores the gradients of the system in the onternal order
     * of the system.
     */
    std::vector<double> grad_;

    /**
     * Vector that stores the charge gradient of each site.
     * This charge gradient exists if the charges are position dependent
     */
    std::vector<double> chggrad_;

    /**
     * Vector that stores the coordinates of all sites in the internal order of
     * the system
     */
    std::vector<double> xyz_;

    /**
     * Vector that stores the charges of all sites in the internal order of
     * the system
     */
    std::vector<double> chg_;

    /**
     * Vector that stores the polarizabilites of all sites in the internal
     * order of the system
     */
    std::vector<double> pol_;

    /**
     * Vector that stores the polarizability factor of all sites in the internal
     * order of the system
     */
    std::vector<double> polfac_;

    std::vector<double> c6_lr_;

    /**
     * Vector that stores the simulation box.
     * The center of the box is origin of coordinates
     * @warning For now, only cubic or rectangular boxes are allowed.
     */
    std::vector<double> box_;

    /**
     * Vector that stores the id of each monomer in the internal order
     * of the system
     */
    std::vector<std::string> monomers_;

    /**
     * Vector that stores the atom names of all sites in the internal order
     * of the system. If a site is a virtual site, its name is "virt"
     */
    std::vector<std::string> atoms_;

    /**
     * Vector that stores the molecules of system. All the monomers that
     * belong to the same molecule are in the same vector inside this vector.
     */
    std::vector<std::vector<size_t> > molecules_;

    /**
     * Important vector that stores a pair with the monomer id and the
     * number of monomers of that type. This vector is used for parallelization
     * and vectorization purposes.
     */
    std::vector<std::pair<std::string, size_t> > mon_type_count_;

    /**
     * Vector that contains the relation between the input monomer
     * order and the internal monomer order. The position i of this
     * vector contains the position in the current
     * monomer vector of the ith initial monomer
     */
    std::vector<size_t> original2current_order_;

    /**
     * Input order of monomers <monomer, site first index>. The position i of this
     * vector contains a pair with the position of monomer "i" in the internal
     * order when it was inputed in the system. The second index of the pair
     * is the first index of that monomer in the input order.
     */
    std::vector<std::pair<size_t, size_t> > initial_order_;

    /**
     * Input order of monomers <monomer, site first index>. The position i of this
     * vector contains a pair with the position of monomer "i" in the internal
     * order when it was inputed in the system. The second index of the pair
     * is the first index of that monomer in the input order, but taking
     * into account only real sites.
     */
    std::vector<std::pair<size_t, size_t> > initial_order_realSites_;
};

}  // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif  // CU_INCLUDE_BBLOCK_SYSTEM_H

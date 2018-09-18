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

// Potential
// 1B
#include "potential/1b/energy1b.h"
// 2B
#include "potential/2b/energy2b.h"
// 3B
#include "potential/3b/energy3b.h"
// DISPERSION
#include "potential/dispersion/dispersion2b.h"
// ELECTROSTATICS
#include "potential/electrostatics/electrostatics.h"


////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: System
////////////////////////////////////////////////////////////////////////////////

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
   */
  std::vector<size_t> GetPairList(size_t nmax, double cutoff, 
                                  size_t istart, size_t iend);


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





  std::vector<std::string> GetSysAtNames();
  std::vector<std::string> GetOriginalOrderSysAtNames();
  std::string GetMonId(size_t n);
  // Modifiers
  void SetSysXyz(std::vector<double> xyz);
  void SetOriginalOrderSysXyz(std::vector<double> xyz);
  void SetOriginalOrderRealSysXyz(std::vector<double> xyz);
  void AddMonomer(std::vector<double> xyz, 
             std::vector<std::string> atoms, std::string id);
  void AddMolecule(std::vector<size_t> molec);
  void Initialize();
  void AddMonomerInfo();
  
  void Set2bCutoff(double cutoff2b);
  void Set3bCutoff(double cutoff3b);
  void SetNMaxEval1b(size_t nmax);
  void SetNMaxEval2b(size_t nmax);
  void SetNMaxEval3b(size_t nmax);
  void SetStepEval2b(size_t step);
  void SetStepEval3b(size_t step);

  void SetDipoleTol(double tol);
  void SetDipoleMaxIt(double maxit);
  void SetDipoleMethod(std::string method);

  void ResetDipoleHistory();

  void SetPBC(bool use_pbc, std::vector<double> box);
  
  // Energy Functions
  // Energy computing gradients. The new gradients of ALL sites 
  // are returned in grad. 
  double Energy(std::vector<double> &grad, bool do_grads);
  double OneBodyEnergy(bool do_grads);
  double TwoBodyEnergy(bool do_grads);
  double ThreeBodyEnergy(bool do_grads);

 private:
  void AddClusters(size_t nmax, double cutoff, size_t istart, size_t iend);
  std::vector<size_t> AddClustersParallel(size_t n_max, double cutoff,
                                          size_t istart, size_t iend);
  void SetCharges();
  void SetPols();
  void SetPolfacs();
  void SetVSites();

  double Get1B(bool do_grads);
  double Get2B(bool do_grads);
  double Get3B(bool do_grads);
  double GetElectrostatics(bool do_grads);


 private:
  size_t nmol_;                              // Number of molecules
  size_t nmon_;                              // Number of monomers
  size_t nsites_;                            // Number of sites in sys
  size_t numat_;                            // Number of sites in sys
  size_t maxNMonEval_;                       // Max number of mons to be eval
  size_t maxNDimEval_;                       // Max number of dimers to be eval
  size_t maxNTriEval_;                       // Max number of trimers to be eval
  size_t stepEval2b_;                        // Number of monomers i to find all
                                             // pairs i,j , with j>i
  size_t stepEval3b_;                        // Number of monomers i to find all
                                             // trimers i,j,k , with j,k > i
  size_t maxItDip_;                          // Max number of it in induced dip
  double diptol_;                            // Tolerance for (mu_i - mu_i+1)^2
  double cutoff2b_;                          // Cutoff for dim and trim search 
  double cutoff3b_;                          // Cutoff for dim and trim search 
  double energy_;                            // Energy of the system
  bool initialized_;                         // Systes is initialized?
  bool use_pbc_;                             // Periodic boundary conditions?
  bool allMonGood_;                          // True if all monomers have low e1b 
                                             // (<60.0kcal/mol)
  // Electrostatics class
  elec::Electrostatics electrostaticE_;
  std::string dipole_method_;

  std::vector<size_t> sites_;                // Number of sites of each mo
  std::vector<size_t> nat_;                  // Number of atoms of each mo
  std::vector<size_t> first_index_;          // First index of mon in sys
  std::vector<size_t> dimers_;               // Dimers of the molecule
  std::vector<size_t> trimers_;              // Trimers of the molecule
  std::vector<double> grad_;                 // Gradients of all sites
  std::vector<double> chggrad_;              // Gradients of pos dep chg
  std::vector<double> xyz_;                  // Coords of all sites
  std::vector<double> chg_;                  // Charges of all sites
  std::vector<double> pol_;                  // Polarizabilities of all sites
  std::vector<double> polfac_;               // Polfacs of all sites
  std::vector<double> box_;                  // Box, in case we use PBC
                                             // Center of the box is origin of coordinates
  std::vector<std::string> monomers_;        // Monomer ids
  std::vector<std::string> atoms_;           // Atom names of sys
  // Molecules of system
  std::vector<std::vector<size_t> > molecules_; 
  // Mon type and # mon of each
  std::vector<std::pair<std::string,size_t> > mon_type_count_;  
  // The position i of this vector contains the position in the current
  // monomer vector of the ith initial monomer
  std::vector<size_t> original2current_order_;       
  // Input order of monomers <monomer, site first index>
  std::vector<std::pair<size_t,size_t> > initial_order_;       
  // Input order of monomers <monomer, site first index only Real sites>
  std::vector<std::pair<size_t,size_t> > initial_order_realSites_;       
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

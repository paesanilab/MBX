#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>

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

// System Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class contains a whole chemical system, which is made of molecules,
// which are made of monomers.
class System {
 public:
  System();
  ~System();
  // Getters
  size_t GetNumMon();
  size_t GetNumMol();
  size_t GetNumSites();
  size_t GetMonNat(size_t n);
  size_t GetFirstInd(size_t n);
  std::vector<size_t> GetDimers();
  std::vector<size_t> GetTrimers();
  std::vector<size_t> GetMolecule(size_t n);
  std::vector<double> GetSysXyz();
  std::vector<double> GetCharges();
  std::vector<double> GetPols();
  std::vector<double> GetPolfacs();
  std::vector<std::string> GetSysAtNames();
  std::string GetMonId(size_t n);
  // Modifiers
  void SetSysXyz(std::vector<double> xyz);
  void AddMonomer(std::vector<double> xyz, 
             std::vector<std::string> atoms, std::string id);
  void AddMolecule(std::vector<size_t> molec);
  void Initialize();
  void AddMonomerInfo();
  void AddClusters(size_t nmax, double cutoff, size_t istart, size_t iend);
  std::vector<size_t> AddClustersParallel(size_t n_max, double cutoff,
                                          size_t istart, size_t iend);
  void SetCharges();
  void SetPols();
  void SetPolfacs();
  void SetVSites();
  // Energy Functions
  // Energy computing gradients. The new gradients of ALL sites 
  // are returned in grad. 
  double Energy(std::vector<double> &grad, bool do_grads);
  double Get1B(bool do_grads);
  double Get2B(bool do_grads);
  double Get3B(bool do_grads);
  double GetElectrostatics(bool do_grads);
 private:
  size_t nmol_;                              // Number of molecules
  size_t nmon_;                              // Number of monomers
  size_t nsites_;                            // Number of sites in sys
  size_t maxNMonEval_;                       // Max number of mons to be eval
  size_t maxNDimEval_;                       // Max number of dimers to be eval
  size_t maxNTriEval_;                       // Max number of trimers to be eval
  size_t maxItDip_;
  double cutoff2b_;                          // Cutoff for dim and trim search 
  double cutoff3b_;                          // Cutoff for dim and trim search 
  double diptol_;
  double energy_;                            // Energy of the system
  bool initialized_;                         // Systes is initialized?
  std::vector<size_t> sites_;                // Number of sites of each mo
  std::vector<size_t> nat_;                  // Number of atoms of each mo
  std::vector<size_t> initial_order_;        // Input order of monomers
  std::vector<size_t> first_index_;          // First index of mon in sys
  std::vector<size_t> dimers_;               // Dimers of the molecule
  std::vector<size_t> trimers_;              // Trimers of the molecule
  std::vector<double> grad_;                  // Gradients of all sites
  std::vector<double> chggrad_;                  // Gradients of pos dep chg
  std::vector<double> xyz_;                  // Coords of all sites
  std::vector<double> chg_;                  // Charges of all sites
  std::vector<double> pol_;                  // Polarizabilities of all sites
  std::vector<double> polfac_;               // Polfacs of all sites
  std::vector<std::string> monomers_;        // Monomer ids
  std::vector<std::string> atoms_;           // Atom names of sys
  // Molecules of system
  std::vector<std::vector<size_t>> molecules_; 
  // Mon type and # mon of each
  std::vector<std::pair<std::string,size_t>> mon_type_count_;   
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

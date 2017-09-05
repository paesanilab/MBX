#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>

// Tools
#include "nanoflann.hpp"
#include "kdtree_utils.h"
#include "sys_tools.h"

// Potential
#include "potential/1b/ps.h"

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
  size_t GetNumMol();
  size_t GetNumSites();
  size_t GetMonNat(size_t n);
  size_t GetFirstInd(size_t n);
  std::vector<size_t> GetDimers();
  std::vector<size_t> GetTrimers();
  std::vector<size_t> GetMolecule(size_t n);
  std::vector<double> GetSysXyz();
  std::vector<std::string> GetSysAtNames();
  std::string GetMonId(size_t n);
  // Modifiers
  void SetSysXyz(std::vector<double> xyz);
  void AddMonomer(std::vector<double> xyz, 
             std::vector<std::string> atoms, std::string id);
  void AddMolecule(std::vector<size_t> molec);
  void Initialize();
  void AddMonomerInfo();
  void AddClusters(size_t nmax);
//  void SetNumMol(size_t n);
//  void AddMolecule(std::shared_ptr<bblock::Molecule> molec);
//  void SetXyz(double * coords);
  // Energy Functions
  double Energy();
  double Energy(double * grd);
 private:
  size_t nmol_;                              // Number of molecules
  size_t nmon_;                              // Number of monomers
  size_t nsites_;                            // Number of sites in sys
  double cutoff_;                            // Cutoff for dim and trim search 
  double energy_;                            // Energy of the system
  bool initialized_;                         // Systes is initialized?
  std::vector<size_t> sites_;                // Number of sites of each mo
  std::vector<size_t> nat_;                  // Number of atoms of each mo
  std::vector<size_t> initial_order_;        // Input order of monomers
  std::vector<size_t> first_index_;          // First index of mon in sys
  std::vector<size_t> dimers_;               // Dimers of the molecule
  std::vector<size_t> trimers_;              // Trimers of the molecule
  std::vector<double> grd_;                  // Gradients of all sites
  std::vector<double> xyz_;                  // Coords of all sites
  std::vector<double> chg_;                  // Charges of all sites
  std::vector<double> pol_;                  // Polarizabilities of all sites
  std::vector<double> polfac_;               // Polfacs of all sites
  std::vector<std::string> monomers_;        // Monomer ids
  std::vector<std::string> atoms_;           // Atom names of sys
  std::vector<std::vector<size_t>> molecules_;  // Molecules of system
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

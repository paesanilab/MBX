#ifndef CU_INCLUDE_BBLOCK_SYSTEM_H
#define CU_INCLUDE_BBLOCK_SYSTEM_H

#include <vector>
#include <string>

#include "nanoflann.hpp"
#include "kdtree_utils.h"

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
  std::vector<size_t> GetDimers();
  std::vector<size_t> GetTrimers();
//  size_t GetNumSites();
  std::vector<std::string> GetSysAtNames();
  std::vector<double> GetSysXyz();
  std::vector<size_t> GetMolecule(size_t n);
  std::string GetMonId(size_t n);
  size_t GetMonNat(size_t n);
  size_t GetFirstInd(size_t n);
  // Modifiers
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
//  double Energy();
//  double Energy(double * grd);
 private:
  size_t nmol_;                                // Number of molecules
  size_t nsites_;                              // Number of sites in sys
  double cutoff_;
  bool initialized_;                           // Systes is initialized?
  std::vector<size_t> sites_;                  // Number of sites of each mo
  std::vector<size_t> nat_;                    // Number of atoms of each mo
  std::vector<size_t> initial_order_;          // Input order of monomers
  std::vector<size_t> first_index_;            // First index of mon in sys
  std::vector<size_t> dimers_;                 // Dimers of the molecule
  std::vector<size_t> trimers_;                // Trimers of the molecule
  std::vector<double> grd_;                    // Gradients of all sites
  std::vector<double> xyz_;                    // Coords of all sites
  std::vector<double> chg_;                    // Coords of all sites
  std::vector<double> pol_;                    // Coords of all sites
  std::vector<double> polfac_;                    // Coords of all sites
  std::vector<std::string> monomers_;          // Monomer ids
  std::vector<std::string> atoms_;
  std::vector<std::vector<size_t>> molecules_;
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_SYSTEM_H

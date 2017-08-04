#ifndef CU_INCLUDE_BBLOCK_MONOMER_H
#define CU_INCLUDE_BBLOCK_MONOMER_H

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>

#include <string.h>

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: Monomer
////////////////////////////////////////////////////////////////////////////////

// Monomer Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class describes an individual monomer. This is the general class.
// Depending on the identity of the monomer, it will be initialized with the 
// appropriate identity.
class Monomer {
 public:
  Monomer();
  ~Monomer();
  // Getters
  size_t GetNumSites();
  size_t GetNumRealSites();
  size_t GetNumVirtSites();
  size_t GetFirstIndex();
  std::vector<std::string> GetAtomNames();
  std::string GetMonomerId();
  // Setters
  
 protected:
  size_t nreals_;                              // Number of real sites
  size_t nvirts_;                              // Number of virtual sites
  size_t ntotls_;                              // Total number of sites
  size_t first_index_;                         // Position of 1st atom in sys
  std::vector<double> xyz_;                    // Coordinates of all sites
  std::vector<double> grd_;                    // Gradients of all sites
  std::vector<double> chg_;                    // Charges of all sites
  std::vector<double> pol_;                    // Pols of all sites
  std::vector<double> polfac_;                 // Polfacs of all sites
  std::string id_;                             // Identity of the monomer
  std::vector<std::string> atoms_;             // Vector with the atom names
  std::vector<excluded_set_type> excluded12_;  // 1-2 distance excluded pairs
  std::vector<excluded_set_type> excluded13_;  // 1-3 distance excluded pairs
  std::vector<excluded_set_type> excluded14_;  // 1-4 distance excluded pairs
 
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONOMER_H

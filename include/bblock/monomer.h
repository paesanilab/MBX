#ifndef CU_INCLUDE_BBLOCK_MONOMER_H
#define CU_INCLUDE_BBLOCK_MONOMER_H

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: monomer

////////////////////////////////////////////////////////////////////////////////

class Monomer {
 public:
  Monomer();
  virtual ~Monomer();
  size_t GetNumSites();
  size_t GetNumRealSites();
  size_t GetNumVirtSites();
  std::shared_ptr<double> GetXyz();
  void SetXyz(double * coords);
  void GetRealSitesXyz(double * coords);
  std::vector<std::string> GetAtomNames();
  std::string GetMonomerId();
 protected:
  size_t n_real_sites;
  size_t n_virt_sites;
  size_t n_sites;
  std::shared_ptr<double>  xyz;
  std::string mon_id;
  std::vector<std::string> at_names;
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONOMER_H

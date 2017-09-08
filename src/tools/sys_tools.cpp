#include "sys_tools.h"

namespace systools {

std::vector<std::pair<std::string,size_t>> OrderMonomers
                   (std::vector<std::string> &mon, 
                    std::vector<size_t> &original_order) {
  std::vector<std::string> monomers = mon;
  mon.clear();
  while (monomers.size() > 0) {
    std::string min_mon = monomers[0];
    size_t index_min = 0;
    for (size_t j = 1; j < monomers.size(); j++) {
      if (monomers[j] < min_mon) {
        min_mon = monomers[j];
        index_min = j;
      }
    }
    mon.push_back(min_mon);
    // TODO Fix this bc it doesnt keep the original order
    original_order.push_back(index_min);
    monomers.erase(monomers.begin() + index_min);
  }

  std::vector<std::pair<std::string,size_t>> mon_types_count;
  mon_types_count.push_back(std::make_pair(mon[0],1));
  size_t count = 0;
  std::string prev_mon = mon[0];
  for (size_t i = 1; i < mon.size(); i++) {
    if (mon[i] == prev_mon) {
      mon_types_count[count].second += 1;
    } else {
      prev_mon = mon[i];
      mon_types_count.push_back(std::make_pair(mon[i],1));
      count += 1;
    } 
  }

  return mon_types_count;

}

size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites,
                     std::vector<size_t> &nat, std::vector<size_t> &fstind,
                     std::vector<double> &chg, std::vector<double> &pol,
                     std::vector<double> &polfac) {
  // Clearing Vectors
  sites.clear();
  nat.clear();
  fstind.clear();
  chg.clear();
  pol.clear();
  polfac.clear();
  
  size_t count = 0;
  for (size_t i = 0; i < mon.size(); i++) {
    if (mon[i] == "h2o") {
      // Filling things for water.
      // TODO: WARNING. For water, charges, and position of the M-site
      // depends on the 3 atom coordinate.  

      // Site Info
      // TODO Maybe we can read this from a database
      sites.push_back(4);
      nat.push_back(3);
      fstind.push_back(count);
      count += sites[i];

      // Charge info
      chg.push_back(0.0);
      chg.push_back(0.0);
      chg.push_back(0.0);
      chg.push_back(0.0);

      // Pol info
      pol.push_back(0.0);
      pol.push_back(0.0);
      pol.push_back(0.0);
      pol.push_back(0.0);

      // Polfac info
      polfac.push_back(0.0);
      polfac.push_back(0.0);
      polfac.push_back(0.0);
      polfac.push_back(0.0);
    } else {
      std::cerr << "No data in the dataset for monomer: "
                << mon[i] << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Setting total number of sites
  return count;
}




} //systools

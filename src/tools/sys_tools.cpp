#include "sys_tools.h"

namespace systools {

void OrderMonomers(std::vector<std::string> &mon, 
                   std::vector<size_t> &original_order) {

}

size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites
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

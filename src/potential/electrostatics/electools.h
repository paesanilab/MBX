#include <cstddef>
#include <vector>
#include <string>

#include "ps.h"
#include "constants.h"

namespace elec {

void SetVSites(std::vector<double> &xyz, std::string mon_id,
               size_t n_mon, size_t nsites, size_t fst_ind);
void SetCharges(std::vector<double> &xyz, std::vector<double> &charges,
                std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);
void SetPolfac (std::vector<double> &polfac, std::string mon_id, 
                size_t n_mon, size_t nsites, size_t fst_ind);
void SetPol (std::vector<double> &atmpolar, std::vector<double> &polfac,
             std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind);

} // elec

#include "energy1b.h"

namespace e1b {

double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz) {

  if (mon == "h2o") {
    return ps::pot_nasa(xyz.data(), 0, nm);
  } else {
    return 0.0;
  }
}


double get_1b_energy(std::string mon,size_t nm,
                     std::vector<double> xyz, std::vector<double> &grad) {

  if (mon == "h2o") {
    return ps::pot_nasa(xyz.data(), grad.data(), nm);
  } else {
    return 0.0;
  }
}


}  // namespace e1b

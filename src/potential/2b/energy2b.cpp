#include "energy2b.h"

namespace e2b {

double get_2b_energy(std::string m1, std::string m2, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2) {
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
  }

  if (m1 == "h2o" and m2 == "h2o") {
    x2o::x2b_v9x pot;
    return pot.eval(xyz1.data(),xyz2.data(),nm);
  } else {
    return 0.0;
  }
}


double get_2b_energy(std::string m1, std::string m2,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2) {
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
  }

  if (m1 == "h2o" and m2 == "h2o") {
    x2o::x2b_v9x pot;
    return pot.eval(xyz1.data(),xyz2.data(),grd1.data(),grd2.data(),nm);
  } else {
    return 0.0;
  }

}


}  // namespace e2b

#include "dispersion2b.h"

namespace e2b {

double GetDispersion(std::string m1, std::string m2, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2) {
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
  }

  if (m1 == "h2o" and m2 == "h2o") {
    return 0.0;
  } else {
    return 0.0;
  }
}


double GetDispersion(std::string m1, std::string m2,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2) {
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
  }

  if (m1 == "h2o" and m2 == "h2o") {
    return 0.0;
  } else {
    return 0.0;
  }

}


}  // namespace e2b

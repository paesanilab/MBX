#include "energy2b.h"

namespace e2b {

double get_2b_energy(std::string m1, std::string m2, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2) {

  // Order the two monomer names and corresponding xyz
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
    std::vector<double> tmp2 = xyz1;
    xyz1 = xyz2;
    xyz2 = tmp2;
  }

  if (m1 == "h2o" and m2 == "h2o") {
    x2o::x2b_v9x pot;
    return pot.eval(xyz1.data(),xyz2.data(),nm);
  } else if (m1 == "f" and m2 == "h2o") {
    // The order is bc the poly were generated this way
    // First water and then ion
    h2o_ion::x2b_h2o_ion_v2x pot(m2, m1);
    return pot.eval(xyz2.data(), xyz1.data(), nm);
  } else {
    return 0.0;
  }
}


double get_2b_energy(std::string m1, std::string m2,size_t nm, 
                     std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2) {
  // Order the two monomer names and corresponding xyz
  if (m2 < m1) {
    std::string tmp = m1;
    m1 = m2;
    m2 = tmp;
    std::vector<double> tmp2 = xyz1;
    xyz1 = xyz2;
    xyz2 = tmp2;
    tmp2 = grd1;
    grd1 = grd2;
    grd2 = tmp2;
  }

  double energy = 0.0;
  // Note: in the conditional, m2 >= m1 ALWAYS
  if (m1 == "h2o" and m2 == "h2o") {
    x2o::x2b_v9x pot;
    energy = pot.eval(xyz1.data(),xyz2.data(),grd1.data(),grd2.data(),nm);
  } else if (m1 == "f" and m2 == "h2o") {
    // The order is bc the poly were generated this way
    // First water and then ion
    h2o_ion::x2b_h2o_ion_v2x pot(m2, m1);
    energy = pot.eval(xyz2.data(),xyz1.data(),grd2.data(),grd1.data(),nm);
  } else {
    return 0.0;
  }

  if (m2 < m1) {
    std::vector<double> tmp2 = xyz1;
    tmp2 = grd2;
    grd2 = grd1;
    grd1 = tmp2;
  }

  return energy;

}


}  // namespace e2b

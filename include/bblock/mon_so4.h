#ifndef CU_INCLUDE_BBLOCK_MONSO4_H
#define CU_INCLUDE_BBLOCK_MONSO4_H

#include "monomer.h"
#include "x1b_A1B4_deg3.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: so4
////////////////////////////////////////////////////////////////////////////////

class SO4 : public Monomer {
 public:
  SO4();
  SO4(double * coords, std::vector<std::string> names);
  ~SO4();
  double Calc1BEnergy();
  double Calc1BEnergy(double * grad);
 private:
  A1B4::x1b_v1x pot_1b;
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONSO4_H

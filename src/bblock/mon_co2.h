#ifndef CU_INCLUDE_BBLOCK_MONCO2_H
#define CU_INCLUDE_BBLOCK_MONCO2_H

#include "monomer.h"
#include "x1b_A1B2_deg4.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: so4
////////////////////////////////////////////////////////////////////////////////

class CO2 : public Monomer {
 public:
  CO2();
  CO2(double * coords, std::vector<std::string> names);
  ~CO2();
  double Calc1BEnergy();
  double Calc1BEnergy(double * grad);
 private:
  A1B2::x1b_v1x pot_1b;
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONCO2_H

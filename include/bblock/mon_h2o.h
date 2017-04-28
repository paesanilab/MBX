#ifndef CU_INCLUDE_BBLOCK_MONH2O_H
#define CU_INCLUDE_BBLOCK_MONH2O_H

#include "monomer.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: h2o
////////////////////////////////////////////////////////////////////////////////

class H2O : public Monomer {
 public:
  H2O();
  H2O(double * coords, std::vector<std::string> names);
  ~H2O();
  double Calc1BEnergy() {return 0;}
  double Calc1BEnergy(double * grad) {return 0;}
// private:
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONH2O_H

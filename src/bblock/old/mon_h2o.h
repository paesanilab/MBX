#ifndef CU_INCLUDE_BBLOCK_MONH2O_H
#define CU_INCLUDE_BBLOCK_MONH2O_H

#include "monomer.h"
#include "ps.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: h2o
////////////////////////////////////////////////////////////////////////////////

class H2O : public Monomer {
 public:
  H2O();
  H2O(double * coords, std::vector<std::string> names);
  ~H2O();
  double Calc1BEnergy();
  double Calc1BEnergy(double * grad);
// private:
};
} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONH2O_H

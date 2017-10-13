#include "poly-3b-v2x.h"

namespace x2o {

double poly_3b_v2x::eval(const double a[1163],
                         const double x[36])
{
  double t[13943];
  poly1(a, x, t);
  poly2(a, x, t);

  return(t[9857]+t[13942]);
}

} // namespace x2o

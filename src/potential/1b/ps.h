#ifndef PS_H
#define PS_H

#include <algorithm>

namespace ps {

double pot_nasa(const double*, double*);
void dms_nasa(const double&, const double&, const double&,
              const double*, double* q3, double* dq3, bool ttm3);
double pot_nasa(const double*, double*, size_t);

// r1 = r2 = 0.9587, \theta = 104.3850
const double e_zero = 0.0; // TODO: fix this

} // namespace ps

#endif // PS_H

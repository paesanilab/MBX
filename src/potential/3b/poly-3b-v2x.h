#ifndef POLY_3B_V2X_H
#define POLY_3B_V2X_H

#include <stdlib.h>
#include <vector>

namespace x2o {

//
// this is the polynomial used by x3b_v2 (including gradients)
//

struct poly_3b_v2x {
    static const unsigned n_vars = 36;
    static const unsigned size = 1163;

    static std::vector<double> eval(const size_t nt, const double* a, const double* x);

    static std::vector<double> eval(const size_t nt, const double* a, const double* x, double* g);
};

}  // namespace x2o

#endif  // POLY_3B_V2X_H

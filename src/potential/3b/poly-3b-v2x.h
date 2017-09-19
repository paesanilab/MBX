#ifndef POLY_3B_V2X_H
#define POLY_3B_V2X_H

namespace x2o {

//
// this is the polynomial used by x3b_v2 (including gradients)
//

struct poly_3b_v2x {
    static const unsigned n_vars = 36;
    static const unsigned size = 1163;

    static double eval(const double a[1163],
                       const double x[36]);

    static double eval(const double a[1163],
                       const double x[36],
                             double g[36]);
};

} // namespace x2o

#endif // POLY_3B_V2X_H

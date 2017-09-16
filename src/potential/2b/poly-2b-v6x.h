#ifndef POLY_2B_V6X_H
#define POLY_2B_V6X_H

namespace x2o {

//
// this is the polynomial used by x2b_v6<4> (including gradients)
//

struct poly_2b_v6x {
    static const unsigned degree = 4;
    static const unsigned n_vars = 31;
    static const unsigned size = 1153;

    static double eval(const double a[1153],
                       const double x[31]);

    static double eval(const double a[1153],
                       const double x[31],
                             double g[31]);
};

} // namespace x2o

#endif // POLY_2B_V6X_H

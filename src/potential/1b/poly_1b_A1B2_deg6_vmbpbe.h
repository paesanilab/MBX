
#ifndef POLY_1B_MBNRG_A1B2_DEG6_VMBPBE_H
#define POLY_1B_MBNRG_A1B2_DEG6_VMBPBE_H

namespace mbnrg_A1B2_deg6 {

struct poly_A1B2_deg6_vmbpbe {
    static const unsigned degree = 6;
    static const unsigned n_vars = 3;

    static const unsigned size = 49;

    double eval(const double x[3], const double a[49]);
    double eval_direct(const double x[3], const double a[49]);
    double eval(const double x[3], const double a[49], double g[3]);
    double eval_direct(const double x[3], const double a[49], double g[3]);
};

}  // namespace mbnrg_A1B2_deg6

#endif  // POLY_1B_MBNRG_A1B2_DEG6_VMBPBE_H

// Polynomial input used to generate these files:

//  add_molecule['A1B2']
//
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//

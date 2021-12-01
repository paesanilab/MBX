
#ifndef POLY_2B_MBNRG_A1B3_A1B3_DEG4_H
#define POLY_2B_MBNRG_A1B3_A1B3_DEG4_H

namespace mbnrg_A1B3_A1B3_deg4 {

struct poly_A1B3_A1B3_deg4_v1 {
    static const unsigned degree = 4;
    static const unsigned n_vars = 28;

    static const unsigned size = 1115;

    double eval(const double x[28], const double a[1115]);
    double eval_direct(const double x[28], const double a[1115]);
    double eval(const double x[28], const double a[1115], double g[28]);
    double eval_direct(const double x[28], const double a[1115], double g[28]);
};

}  // namespace mbnrg_A1B3_A1B3_deg4

#endif  // POLY_2B_MBNRG_A1B3_A1B3_DEG4_H

// Polynomial input used to generate these files:

//  add_molecule['A1B3']
//  add_molecule['A1B3']
//
//  add_variable['A', '1', 'a', 'A', '2', 'b', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '3', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '5', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '6', 'b', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '3', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '4', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '5', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '6', 'b', 'x-intra-A+B-1']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'B', '3', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '5', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '6', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '3', 'a', 'x-intra-B+B-1']
//  add_variable['B', '2', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '5', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '6', 'b', 'x-inter-B+B-0']
//  add_variable['B', '3', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '3', 'a', 'B', '5', 'b', 'x-inter-B+B-0']
//  add_variable['B', '3', 'a', 'B', '6', 'b', 'x-inter-B+B-0']
//  add_variable['B', '4', 'b', 'B', '5', 'b', 'x-intra-B+B-1']
//  add_variable['B', '4', 'b', 'B', '6', 'b', 'x-intra-B+B-1']
//  add_variable['B', '5', 'b', 'B', '6', 'b', 'x-intra-B+B-1']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']

#ifndef POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_VMBPBE_H
#define POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_VMBPBE_H

namespace mbnrg_A1B2_A1B2_A1B2_deg4 {

struct poly_A1B2_A1B2_A1B2_deg4_vmbpbe {
    static const unsigned degree = 4;
    static const unsigned n_vars = 36;

    static const unsigned size = 1163;

    double eval(const double x[36], const double a[1163]);
    double eval_direct(const double x[36], const double a[1163]);
    double eval(const double x[36], const double a[1163], double g[36]);
    double eval_direct(const double x[36], const double a[1163], double g[36]);
};

}  // namespace mbnrg_A1B2_A1B2_A1B2_deg4

#endif  // POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_VMBPBE_H

// Polynomial input used to generate these files:

//  add_molecule['A1B2']
//  add_molecule['A1B2']
//  add_molecule['A1B2']
//
//  add_variable['A', '1', 'a', 'A', '2', 'b', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'A', '3', 'c', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '3', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '5', 'c', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '6', 'c', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'A', '3', 'c', 'x-inter-A+A-0']
//  add_variable['A', '2', 'b', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '3', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '4', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '5', 'c', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '6', 'c', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '3', 'b', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '5', 'c', 'x-intra-A+B-1']
//  add_variable['A', '3', 'c', 'B', '6', 'c', 'x-intra-A+B-1']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '3', 'b', 'B', '4', 'b', 'x-intra-B+B-1']
//  add_variable['B', '3', 'b', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '3', 'b', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '4', 'b', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '4', 'b', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '5', 'c', 'B', '6', 'c', 'x-intra-B+B-1']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//  add_filter['num-fragments', 'x-inter-*+*-*', '2-']
//  add_filter['sum-degree', '*', '2', 'and', sum-degree', 'x-intra-A+B-*', '1+']
//  add_filter['sum-degree', '*', '2', 'and', sum-degree', 'x-intra-B+B-*', '1+']
//  add_filter['sum-degree', '*', '3', 'and', sum-degree', 'x-intra-A+B-*', '2+']
//  add_filter['sum-degree', '*', '3', 'and', sum-degree', 'x-intra-B+B-*', '2+']
//  add_filter['sum-degree', '*', '4', 'and', sum-degree', 'x-intra-A+B-*', '2+']
//  add_filter['sum-degree', '*', '4', 'and', sum-degree', 'x-intra-B+B-*', '2+']
//  add_filter['sum-degree', '*', '4', 'and', sum-degree', 'x-inter-A+A-*', '2+']
//  add_filter['sum-degree', '*', '4', 'and', sum-degree', 'x-inter-B+B-*', '2+']

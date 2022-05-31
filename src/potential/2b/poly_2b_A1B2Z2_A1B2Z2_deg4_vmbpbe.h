
#ifndef POLY_2B_MBNRG_A1B2Z2_A1B2Z2_DEG4_VMBPBE_H
#define POLY_2B_MBNRG_A1B2Z2_A1B2Z2_DEG4_VMBPBE_H

namespace mbnrg_A1B2Z2_A1B2Z2_deg4 {

struct poly_A1B2Z2_A1B2Z2_deg4_vmbpbe {
    static const unsigned degree = 4;
    static const unsigned n_vars = 31;

    static const unsigned size = 1153;

    double eval(const double x[31], const double a[1153]);
    double eval_direct(const double x[31], const double a[1153]);
    double eval(const double x[31], const double a[1153], double g[31]);
    double eval_direct(const double x[31], const double a[1153], double g[31]);
};

}  // namespace mbnrg_A1B2Z2_A1B2Z2_deg4

#endif  // POLY_2B_MBNRG_A1B2Z2_A1B2Z2_DEG4_VMBPBE_H

// Polynomial input used to generate these files:

//  add_molecule['A1B2Z2']
//  add_molecule['A1B2Z2']
//
//  add_variable['A', '1', 'a', 'A', '2', 'b', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '3', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'Z', '3', 'b', 'x-inter-A+Z-0']
//  add_variable['A', '1', 'a', 'Z', '4', 'b', 'x-inter-A+Z-0']
//  add_variable['A', '2', 'b', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '3', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '4', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'Z', '1', 'a', 'x-inter-A+Z-0']
//  add_variable['A', '2', 'b', 'Z', '2', 'a', 'x-inter-A+Z-0']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'Z', '3', 'b', 'x-inter-B+Z-0']
//  add_variable['B', '1', 'a', 'Z', '4', 'b', 'x-inter-B+Z-0']
//  add_variable['B', '2', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'Z', '3', 'b', 'x-inter-B+Z-0']
//  add_variable['B', '2', 'a', 'Z', '4', 'b', 'x-inter-B+Z-0']
//  add_variable['B', '3', 'b', 'B', '4', 'b', 'x-intra-B+B-1']
//  add_variable['B', '3', 'b', 'Z', '1', 'a', 'x-inter-B+Z-0']
//  add_variable['B', '3', 'b', 'Z', '2', 'a', 'x-inter-B+Z-0']
//  add_variable['B', '4', 'b', 'Z', '1', 'a', 'x-inter-B+Z-0']
//  add_variable['B', '4', 'b', 'Z', '2', 'a', 'x-inter-B+Z-0']
//  add_variable['Z', '1', 'a', 'Z', '3', 'b', 'x-inter-Z+Z-0']
//  add_variable['Z', '1', 'a', 'Z', '4', 'b', 'x-inter-Z+Z-0']
//  add_variable['Z', '2', 'a', 'Z', '3', 'b', 'x-inter-Z+Z-0']
//  add_variable['Z', '2', 'a', 'Z', '4', 'b', 'x-inter-Z+Z-0']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//  add_filter['sum-degree', '*', '1', 'and', 'sum-degree', 'x-intra-*+*-*', '1+']
//  add_filter['sum-degree', '*', '2', 'and', 'sum-degree', 'x-intra-*+*-*', '2']
//  add_filter['sum-degree', '*', '3', 'and', 'sum-degree', 'x-intra-*+*-*', '3']
//  add_filter['sum-degree', '*', '4', 'and', 'sum-degree', 'x-intra-*+*-*', '1-/3+']


#ifndef POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_H
#define POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_H

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4 {

struct poly_A1_B1C2X2_B1C2X2_deg4_v1 {
    static const unsigned degree = 4;
    static const unsigned n_vars = 41;

    static const unsigned size = 1575;

    double eval(const double x[41], const double a[1575]);
    double eval_direct(const double x[41], const double a[1575]);
    double eval(const double x[41], const double a[1575], double g[41]);
    double eval_direct(const double x[41], const double a[1575], double g[41]);
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_oldswitch {

struct poly_A1_B1C2X2_B1C2X2_deg4_v1 {
    static const unsigned degree = 4;
    static const unsigned n_vars = 41;

    static const unsigned size = 1575;

    double eval(const double x[41], const double a[1575]);
    double eval_direct(const double x[41], const double a[1575]);
    double eval(const double x[41], const double a[1575], double g[41]);
    double eval_direct(const double x[41], const double a[1575], double g[41]);
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4

#endif  // POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_H

// Polynomial input used to generate these files:

//  add_molecule['A1']
//  add_molecule['B1C2X2']
//  add_molecule['B1C2X2']
//
//  add_variable['A', '1', 'a', 'B', '1', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '2', 'c', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'C', '1', 'b', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '2', 'b', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '3', 'c', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '4', 'c', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'X', '1', 'b', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '2', 'b', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '3', 'c', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '4', 'c', 'x-inter-A+X-0']
//  add_variable['B', '1', 'b', 'B', '2', 'c', 'x-inter-B+B-0']
//  add_variable['B', '1', 'b', 'C', '1', 'b', 'x-intra-B+C-1']
//  add_variable['B', '1', 'b', 'C', '2', 'b', 'x-intra-B+C-1']
//  add_variable['B', '1', 'b', 'C', '3', 'c', 'x-inter-B+C-0']
//  add_variable['B', '1', 'b', 'C', '4', 'c', 'x-inter-B+C-0']
//  add_variable['B', '1', 'b', 'X', '3', 'c', 'x-inter-B+X-0']
//  add_variable['B', '1', 'b', 'X', '4', 'c', 'x-inter-B+X-0']
//  add_variable['B', '2', 'c', 'C', '1', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'c', 'C', '2', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'c', 'C', '3', 'c', 'x-intra-B+C-1']
//  add_variable['B', '2', 'c', 'C', '4', 'c', 'x-intra-B+C-1']
//  add_variable['B', '2', 'c', 'X', '1', 'b', 'x-inter-B+X-0']
//  add_variable['B', '2', 'c', 'X', '2', 'b', 'x-inter-B+X-0']
//  add_variable['C', '1', 'b', 'C', '2', 'b', 'x-intra-C+C-1']
//  add_variable['C', '1', 'b', 'C', '3', 'c', 'x-inter-C+C-0']
//  add_variable['C', '1', 'b', 'C', '4', 'c', 'x-inter-C+C-0']
//  add_variable['C', '1', 'b', 'X', '3', 'c', 'x-inter-C+X-0']
//  add_variable['C', '1', 'b', 'X', '4', 'c', 'x-inter-C+X-0']
//  add_variable['C', '2', 'b', 'C', '3', 'c', 'x-inter-C+C-0']
//  add_variable['C', '2', 'b', 'C', '4', 'c', 'x-inter-C+C-0']
//  add_variable['C', '2', 'b', 'X', '3', 'c', 'x-inter-C+X-0']
//  add_variable['C', '2', 'b', 'X', '4', 'c', 'x-inter-C+X-0']
//  add_variable['C', '3', 'c', 'C', '4', 'c', 'x-intra-C+C-1']
//  add_variable['C', '3', 'c', 'X', '1', 'b', 'x-inter-C+X-0']
//  add_variable['C', '3', 'c', 'X', '2', 'b', 'x-inter-C+X-0']
//  add_variable['C', '4', 'c', 'X', '1', 'b', 'x-inter-C+X-0']
//  add_variable['C', '4', 'c', 'X', '2', 'b', 'x-inter-C+X-0']
//  add_variable['X', '1', 'b', 'X', '3', 'c', 'x-inter-X+X-0']
//  add_variable['X', '1', 'b', 'X', '4', 'c', 'x-inter-X+X-0']
//  add_variable['X', '2', 'b', 'X', '3', 'c', 'x-inter-X+X-0']
//  add_variable['X', '2', 'b', 'X', '4', 'c', 'x-inter-X+X-0']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//
//  add_filter['degree', 'x-intra-*+*-*', '2+','2+']
//  add_filter['num-fragments', 'x-inter-*+*-*', '2-']
//
//  add_filter['degree', 'x-inter-B+B-*', '2', '4']
//  add_filter['degree', 'x-inter-B+B-*', '3+', '2+']
//  add_filter['degree', 'x-inter-C+C-*', '2', '4']
//  add_filter['degree', 'x-inter-C+C-*', '3+', '3+']
//
//  add_filter['degree', 'x-inter-A+B-*', '4+', '4']
//
//  add_filter['degree', 'x-inter-A+A-*', '3+', '2+']
//  add_filter['degree', 'x-inter-A+C-*', '4+', '2+']
//  add_filter['degree', 'x-inter-B+C-*', '4+', '2+']
//
//  add_filter['degree', 'x-inter-X+X-*', '2+', '2+']
//  add_filter['degree', 'x-inter-*+X-*', '2+', '3']
//  add_filter['degree', 'x-inter-*+X-*', '1+', '4+']
//

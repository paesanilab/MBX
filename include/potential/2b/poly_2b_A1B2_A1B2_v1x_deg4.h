#ifndef POLY_MODEL_H
#define POLY_MODEL_H

namespace mb_system {

struct poly_model {
    static const unsigned n_vars = 15;
    static const unsigned size = 597;

    static double eval(const double a[597],
                       const double x[15]);

    static double eval(const double a[597],
                       const double x[15],
                             double g[15]);

    static double eval_direct(const double a[597],
                              const double x[15]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace mb_system

#endif // POLY_MODEL_H
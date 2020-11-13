#ifndef XMIN_H
#define XMIN_H

namespace tools {

struct xmin_opt {
    int mol_struct_opt;
    int maxiter;
    double grms_tol;
    int method;
    int numdiff;
    int m_lbfgs;
    int iter;
    double xmin_time;
    int ls_method;
    int ls_maxiter;
    double ls_maxatmov;
    double beta_armijo;
    double c_armijo;
    double mu_armijo;
    double ftol_wolfe;
    double gtol_wolfe;
    int ls_iter;
    int print_level;
    int error_flag;
};

int xmin_opt_init(struct xmin_opt *);

double xmin(double (*func)(double *, double *, int *), int *natm, double *xyz, double *grad, double *energy,
            double *grms, struct xmin_opt *xo);

}  // namespace tools

#endif  // XMIN_H

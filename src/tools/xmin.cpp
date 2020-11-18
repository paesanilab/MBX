/****************************************************************************/
/*        XMIN: Written by Istvan Kolossvary     time stamp: 8/16/2009      */
/****************************************************************************/

/*  Note: there are two "public" routines here:
 *  xminC():  a fairly low-level interface (currently used by sqm), which
 *            requires its own driver that understands the reverse
 *            communication needed
 *  xmin():   a higher-level, NAB-like interface, which takes the function
 *            to be minimized as an argument, and handles the reverse
 *            communication internally
 */

#include "xmin.h"

#define MQS 1
#define INT_T int
#define REAL_T double

#include <ctype.h>
#include <errno.h>
#include <float.h> /* for DBL_EPSILON machine precision */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SQR(a) ((a) * (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define DONE 0
#define CALCENRG 1
#define CALCGRAD 2
#define CALCBOTH 3
#define CALCENRG_NEWNBL 4
#define CALCGRAD_NEWNBL 5
#define CALCBOTH_NEWNBL 6
#define CALCENRG_OLDNBL 7
#define CALCGRAD_OLDNBL 8
#define CALCBOTH_OLDNBL 9

#define PARAMS_ERROR -1
#define ILLEGAL_STATUS -2
#define MALLOC_ERROR -3
#define DIVZERO_ERROR -4
#define MINIMZ_ERROR -5
#define LSEARCH_ERROR -6

#define ZERO 0.0
#define ONE 1.0
#define BIG 1e+20
#define TINY 1e-20
#define YES 1
#define NO 0
#define TRUE 1
#define FALSE 0

/* Options for minimization methods: */
#define PRCG 1
#define LBFGS 2
#define TNCG 3
/* Options for line search methods: */
#define ARMIJO 1
#define WOLFE 2
#define LS_MINATMOV 1e-04
/* Options for calculating finite difference Hv formula: */
#define FORWARD_DIFF 1
#define CENTRAL_DIFF 2
/* Options for updating L-BFGS matrix:  */
#define UNITMATRIX 1
#define SCALING 2

/* TNCG: */
#define CG_QTOL 0.5
#define CG_ITERMAX 50

static int PRINT_MINIMZ = YES;
static int DEBUG_MINIMZ = NO;
static int nfunc; /* total number of calls to func()  */

static struct lbfgs {
    double rho;
    double gamma;
    double *s;
    double *y;
} * lbfgs_matrix, *lbfgs_matrix_buf;

#if defined(SQM) || defined(MQS)
static FILE *nabout = stdout;
static int get_mytaskid() { return 0; }
#else
/* Defined elsewhere: (prm.c) */
extern int get_mytaskid(); /* for MPI */
#endif

/*
    Top XMIN calling function:
*/
#ifdef SQM
#define xminC xminc_
#endif

/*
    Private to libxmin.c:
*/

#if 0
static void ls_armijo();
static void ls_wolfe();
static void update_wolfe_step();
static void hessvec_central();
static void hessvec_forward();
static void init_lbfgs_matrix();
static void init_lbfgs_matrix_buf();
static void lbfgs_minim();
static void line_search();
static int  load_lbfgs();
static void minim();
//static void my_free();
//static void *my_malloc();
static void nocedal();
static void prcg_minim();
static void tncg_minim();
#endif

/*****
        Dynamic memory allocation:
*****/
static void *my_malloc(void *(*malloc_method)(size_t), const char *s, size_t nmemb, size_t size, int *error_flag)
/*
    malloc_method is a pointer to a particular malloc function.
*/
{
    void *poi;
    *error_flag = FALSE;
    if ((poi = (void *)(*malloc_method)(nmemb * size)) == NULL) {
        perror(s);
        fflush(stderr);
        *error_flag = MALLOC_ERROR;
        return NULL;
    }
    memset(poi, 0, nmemb * size); /* clear memory */
                                  /* printf("\n Allocated %10d bytes at %p for %s\n",(int)(nmemb*size),poi,s+10);
                                     fflush(stdout); */
    return poi;
}

static void my_free(void *poi) {
    if (poi != NULL) free(poi);
    /* printf("\n Deallocated                   %p\n",poi);
       fflush(stdout); */
}

/*****
        Finite difference formula to calculate Hv:
*****/
static void hessvec_central(int *ndim, double *vec_in, double *vec_out, double *xyz, double *grad, int *return_flag,
                            int *label)
/*
  Central Difference:
  H(xyz)*v = ( grad(xyz+tiny_step*v) - grad(xyz-tiny_step*v) ) / (2*tiny_step)
*/
{
    static int i, n;
    static double *xyz_save = NULL, *grad_save = NULL, *grad_orig = NULL, sqrt_epsmach, tiny_step, dot, xyz_norm,
                  vec_in_norm, max, vec_in_max;
    static int allocated, error_flag;
    switch (*label) {
        case 0:
            allocated = NO;
            error_flag = FALSE;
            n = (*ndim);
            if (!allocated) {
                xyz_save = (double *)my_malloc(malloc, "\nERROR in hessvec_central/my_malloc(double *xyz_save)", n,
                                               sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                grad_save = (double *)my_malloc(malloc, "\nERROR in hessvec_central/my_malloc(double *grad_save)", n,
                                                sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                grad_orig = (double *)my_malloc(malloc, "\nERROR in hessvec_central/my_malloc(double *grad_orig)", n,
                                                sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            sqrt_epsmach = sqrt(DBL_EPSILON);
            memcpy(grad_orig, grad, n * sizeof(double));
            goto L00;
        case 1:
            goto L01;
        case 2:
            goto L02;
        default:
            fprintf(stderr, "\nERROR in hessvec_central(): Illegal status.\n");
            fflush(stderr);
            if (allocated) allocated = NO;
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    memcpy(xyz_save, xyz, n * sizeof(double));
    for (i = 0, dot = ZERO; i < n; i++) dot += SQR(xyz_save[i]);
    xyz_norm = sqrt(dot);
    for (i = 0, dot = ZERO; i < n; i++) dot += SQR(vec_in[i]);
    vec_in_norm = sqrt(dot);
    for (i = 0, vec_in_max = ZERO; i < n; i++)
        if ((max = fabs(vec_in[i])) >= vec_in_max) vec_in_max = max;
    /* Derreumaux, Zhang, Schlick, Brooks,
      J. Comput. Chem. 15, 532-552 (1994), p. 541: */
    tiny_step = MIN((2. * sqrt_epsmach * (ONE + xyz_norm) / vec_in_norm), (sqrt_epsmach / vec_in_max));
    for (i = 0; i < n; i++) xyz[i] -= tiny_step * vec_in[i];
    *return_flag = CALCGRAD_OLDNBL;
    *label = 1;
    return;
L01:
    memcpy(grad_save, grad, n * sizeof(double));
    memcpy(xyz, xyz_save, n * sizeof(double));
    for (i = 0; i < n; i++) xyz[i] += tiny_step * vec_in[i];
    *return_flag = CALCGRAD_OLDNBL;
    *label = 2;
    return;
L02:
    for (i = 0; i < n; i++) vec_out[i] = (grad[i] - grad_save[i]) / (2. * tiny_step); /* load vec_out[] */
    memcpy(xyz, xyz_save, n * sizeof(double));                                        /* restore  xyz[] */
    memcpy(grad, grad_orig, n * sizeof(double));                                      /* restore grad[] */
    if (allocated) {
        my_free(xyz_save);
        my_free(grad_save);
        my_free(grad_orig);
        allocated = NO;
    }
    *label = 0; /* hessvec_central() done */
    return;

error_cleanup:
    my_free(xyz_save);
    my_free(grad_save);
    my_free(grad_orig);
    return;
}

static void hessvec_forward(int *ndim, double *vec_in, double *vec_out, double *xyz, double *grad, int *return_flag,
                            int *label)
/*
    Forward Difference:
    H(xyz)*v = ( grad(xyz+tiny_step*v) - grad(xyz) ) / tiny_step

    !!! Make sure that on entry grad[] is up-to-date wrt xyz[],
        because it is not calculated here. !!!
*/
{
    static int i, n;
    static double *xyz_save = NULL, *grad_save = NULL, sqrt_epsmach, tiny_step, dot, xyz_norm, vec_in_norm;
    // MRR. Remove variables not used..
    //   static double *xyz_save = NULL, *grad_save =
    //       NULL, sqrt_epsmach, tiny_step, dot, xyz_norm, vec_in_norm, max,
    //       vec_in_max;
    static int allocated, error_flag;
    switch (*label) {
        case 0:
            allocated = NO;
            error_flag = FALSE;
            n = (*ndim);
            if (!allocated) {
                xyz_save = (double *)my_malloc(malloc, "\nERROR in hessvec_forward/my_malloc(double *xyz_save)", n,
                                               sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                grad_save = (double *)my_malloc(malloc, "\nERROR in hessvec_forward/my_malloc(double *grad_save)", n,
                                                sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            sqrt_epsmach = sqrt(DBL_EPSILON);
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in hessvec_forward(): Illegal status.\n");
            fflush(stderr);
            if (allocated) allocated = NO;
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    memcpy(grad_save, grad, n * sizeof(double));
    memcpy(xyz_save, xyz, n * sizeof(double));
    for (i = 0, dot = ZERO; i < n; i++) dot += SQR(xyz_save[i]);
    xyz_norm = sqrt(dot);
    for (i = 0, dot = ZERO; i < n; i++) dot += SQR(vec_in[i]);
    vec_in_norm = sqrt(dot);

    /* Derreumaux, Zhang, Schlick, Brooks,
       J. Comput. Chem. 15, 532-552 (1994), p. 541: */
    tiny_step = 2. * sqrt_epsmach * (ONE + xyz_norm) / vec_in_norm;
    for (i = 0; i < n; i++) xyz[i] += tiny_step * vec_in[i];
    *return_flag = CALCGRAD_OLDNBL;
    *label = 1;
    return;
L01:
    for (i = 0; i < n; i++) vec_out[i] = (grad[i] - grad_save[i]) / tiny_step; /* load vec_out[] */
    memcpy(xyz, xyz_save, n * sizeof(double));                                 /* restore  xyz[] */
    memcpy(grad, grad_save, n * sizeof(double));                               /* restore grad[] */
    if (allocated) {
        my_free(xyz_save);
        my_free(grad_save);
        allocated = NO;
    }
    *label = 0; /* hessvec_forward() done */
    return;

error_cleanup:
    my_free(xyz_save);
    my_free(grad_save);
    return;
}

/*****
        Line search:
******/
static void update_wolfe_step(double *stx, double *fx, double *dx, double *sty, double *fy, double *dy, double *stp,
                              double fp, double dp, int *brackt, double stpmin, double stpmax, int *error_flag) {
    /* MCSTEP by:
       Jorge J. More', David J. Thuente
       Argonne National Laboratory. MINPACK project. June 1983

       C version by IK.

       The purpose of MCSTEP is to compute a safeguarded step for
       a linesearch and to update an interval of uncertainty for
       a minimizer of the function.

       The parameter stx contains the step with the least function
       value. The parameter stp contains the current step. It is
       assumed that the derivative at stx is negative in the
       direction of the step. If brackt is set true then a
       minimizer has been bracketed in an interval of uncertainty
       with endpoints stx and sty.

       stx, fx, and dx are variables which specify the step,
       the function, and the derivative at the best step obtained
       so far. The derivative must be negative in the direction
       of the step, that is, dx and stp-stx must have opposite
       signs. On output these parameters are updated appropriately.

       sty, fy, and dy are variables which specify the step,
       the function, and the derivative at the other endpoint of
       the interval of uncertainty. On output these parameters are
       updated appropriately.

       stp, fp, and dp are variables which specify the step,
       the function, and the derivative at the current step.
       If brackt is set true then on input stp must be
       between stx and sty. On output stp is set to the new step.

       brackt is a logical variable which specifies if a minimizer
       has been bracketed. If the minimizer has not been bracketed
       then on input brackt must be set false. If the minimizer
       is bracketed then on output brackt is set true.

       stpmin and stpmax are input variables which specify lower
       and upper bounds for the step. */

    double gamma, p, q, r, s, sgnd, stpc, stpf, stpq, theta;
    int bound;

    /* Check input arguments for inconsistencies: */

    if ((*brackt && (*stp <= MIN(*stx, *sty) || *stp >= MAX(*stx, *sty))) || (*dx * (*stp - *stx) >= 0) ||
        stpmax < stpmin) {
        *error_flag = LSEARCH_ERROR;
        return;
    }

    /* Determine if the derivatives have opposite sign: */
    sgnd = dp * (*dx / fabs(*dx));

    if (fp > *fx) {
        /* First case. A higher function value.
           The minimum is bracketed. If the cubic step is closer
           to stx than the quadratic step, the cubic step is taken,
           else the average of the cubic and quadratic steps is taken. */

        bound = TRUE;
        theta = 3 * (*fx - fp) / (*stp - *stx) + *dx + dp;
        s = MAX(fabs(theta), MAX(fabs(*dx), fabs(dp)));
        gamma = s * sqrt(SQR(theta / s) - (*dx / s) * (dp / s));
        if (*stp < *stx) gamma = -gamma;
        p = (gamma - *dx) + theta;
        q = ((gamma - *dx) + gamma) + dp;
        r = p / q;
        stpc = *stx + r * (*stp - *stx);
        stpq = *stx + ((*dx / ((*fx - fp) / (*stp - *stx) + *dx)) / 2) * (*stp - *stx);
        if (fabs(stpc - *stx) < fabs(stpq - *stx))
            stpf = stpc;
        else
            stpf = stpc + (stpq - stpc) / 2;
        *brackt = TRUE;

    } else if (sgnd < 0) {
        /* Second case. A lower function value and derivatives of
           opposite sign. The minimum is bracketed. If the cubic
           step is closer to stx than the quadratic (secant) step,
           the cubic step is taken, else the quadratic step is taken. */

        bound = FALSE;
        theta = 3 * (*fx - fp) / (*stp - *stx) + *dx + dp;
        s = MAX(fabs(theta), MAX(fabs(*dx), fabs(dp)));
        gamma = s * sqrt(SQR(theta / s) - (*dx / s) * (dp / s));
        if (*stp > *stx) gamma = -gamma;
        p = (gamma - dp) + theta;
        q = ((gamma - dp) + gamma) + *dx;
        r = p / q;
        stpc = *stp + r * (*stx - *stp);
        stpq = *stp + (dp / (dp - *dx)) * (*stx - *stp);
        if (fabs(stpc - *stp) > fabs(stpq - *stp))
            stpf = stpc;
        else
            stpf = stpq;
        *brackt = TRUE;
    } else if (fabs(dp) < fabs(*dx)) {
        /* Third case. A lower function value, derivatives of the
           same sign, and the magnitude of the derivative decreases.
           The cubic step is only used if the cubic tends to infinity
           in the direction of the step or if the minimum of the cubic
           is beyond stp. Otherwise the cubic step is defined to be
           either stpmin or stpmax. The quadratic (secant) step is also
           computed and if the minimum is bracketed then the the step
           closest to stx is taken, else the step farthest away is taken. */

        bound = TRUE;
        theta = 3 * (*fx - fp) / (*stp - *stx) + *dx + dp;
        s = MAX(fabs(theta), MAX(fabs(*dx), fabs(dp)));

        /* The case gamma=0 only arises if the cubic does not
           tend to infinity in the direction of the step. */

        gamma = s * sqrt(MAX(ZERO, SQR(theta / s) - (*dx / s) * (dp / s)));
        if (*stp > *stx) gamma = -gamma;
        p = (gamma - dp) + theta;
        q = (gamma + (*dx - dp)) + gamma;
        r = p / q;
        if (r < 0 && gamma != 0)
            stpc = *stp + r * (*stx - *stp);
        else if (*stp > *stx)
            stpc = stpmax;
        else
            stpc = stpmin;
        stpq = *stp + (dp / (dp - *dx)) * (*stx - *stp);
        if (*brackt) {
            if (fabs(*stp - stpc) < fabs(*stp - stpq))
                stpf = stpc;
            else
                stpf = stpq;
        } else {
            if (fabs(*stp - stpc) > fabs(*stp - stpq))
                stpf = stpc;
            else
                stpf = stpq;
        }
    } else {
        /* Fourth case. A lower function value, derivatives of the
           same sign, and the magnitude of the derivative does
           not decrease. If the minimum is not bracketed, the step
           is either stpmin or stpmax, else the cubic step is taken. */

        bound = FALSE;
        if (*brackt) {
            theta = 3 * (fp - *fy) / (*sty - *stp) + *dy + dp;
            s = MAX(fabs(theta), MAX(fabs(*dy), fabs(dp)));
            gamma = s * sqrt(SQR(theta / s) - (*dy / s) * (dp / s));
            if (*stp > *sty) gamma = -gamma;
            p = (gamma - dp) + theta;
            q = ((gamma - dp) + gamma) + *dy;
            r = p / q;
            stpc = *stp + r * (*sty - *stp);
            stpf = stpc;
        } else if (*stp > *stx)
            stpf = stpmax;
        else
            stpf = stpmin;
    }

    /* Update the interval of uncertainty. This update does not
       depend on the new step or the case analysis above. */

    if (fp > *fx) {
        *sty = *stp;
        *fy = fp;
        *dy = dp;
    } else {
        if (sgnd < 0) {
            *sty = *stx;
            *fy = *fx;
            *dy = *dx;
        }
        *stx = *stp;
        *fx = fp;
        *dx = dp;
    }

    /* Compute the new step and safeguard it. */

    stpf = MIN(stpmax, stpf);
    stpf = MAX(stpmin, stpf);
    *stp = stpf;
    if (*brackt && bound) {
        if (*sty > *stx) {
            *stp = MIN(*stx + 0.66 * (*sty - *stx), *stp);
        } else {
            *stp = MAX(*stx + 0.66 * (*sty - *stx), *stp);
        }
    }

    return;
}

static void ls_wolfe(int k, int n, double *x_k, double *fx_k, double *g_k, double *x_diff, double *g_diff,
                     int ls_maxiter, int *ls_tot_iter, double ls_maxatmov, double beta_armijo, double c_armijo,
                     double mu_armijo, double ftol_wolfe, double gtol_wolfe, double *d_k, double *alfa_k, int *ls_iter,
                     int *return_flag, int *label)
/* MCSRCH by:
   Jorge J. More', David J. Thuente
   Argonne National Laboratory. MINPACK project. June 1983

   Slightly modified and rewritten in C by IK.
   Still the best line minimizer I have encountered.

   The purpose of MCSRCH is to find a step which satisfies
   a sufficient decrease condition and a curvature condition.

   At each stage the subroutine updates an interval of
   uncertainty with endpoints stx and sty. The interval of
   uncertainty is initially chosen so that it contains a
   minimizer of the modified function

     f(x+stp*s) - f(x) - ftol*stp*(gradf(x)'s).

   If a step is obtained for which the modified function
   has a nonpositive function value and nonnegative derivative,
   then the interval of uncertainty is chosen so that it
   contains a minimizer of f(x+stp*s).

   The algorithm is designed to find a step which satisfies
   the sufficient decrease condition

     f(x+stp*s) <= f(x) + ftol*stp*(gradf(x)'s),

   and the curvature condition

     abs(gradf(x+stp*s)'s)) <= gtol*abs(gradf(x)'s).

   If ftol is less than gtol and if, for example, the function
   is bounded below, then there is always a step which satisfies
   both conditions. If no step can be found which satisfies both
   conditions, then the algorithm usually stops when rounding
   errors prevent further progress. In this case stp only
   satisfies the sufficient decrease condition. */
{
    static int status_flag, error_flag;
    static double fx_k_save, *x_k_save = NULL;
    static double dg_init, dg_try, s_k, dmax, dnorm, dlimit;
    static double lhs_f_wolfe, rhs_f_wolfe, lhs_g_wolfe, rhs_g_wolfe;
    static int allocated = NO, i, j;
    static int brackt, stage1;
    static double dg, dgm, dgtest, dgx, dgxm, dgy, dgym;
    static double fm, fx, fxm, fy, fym;
    static double stp, stpmin, stpmax;
    static double stx, sty, stmin, stmax, width, width1;

    //  MRR - Using the variables to avoid warnings
    status_flag = -1;
    status_flag++;
    dg = 0.0;
    dg = dg + 1.0;
    //  END MRR

    switch (*label) {
        case 0:
            status_flag = 0;
            error_flag = FALSE;
            if (!allocated) {
                x_k_save = (double *)my_malloc(malloc, "\nERROR in ls_wolfe/my_malloc(double *x_k_save)", n,
                                               sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            for (i = 0, dg_init = ZERO; i < n; i++) {
                dg_init += g_k[i] * d_k[i];
            }
            /* scale back if d_k[] involves agressive atomic displacement */
            for (i = 0, dmax = dnorm = ZERO; i < n; i++) {
                dnorm += SQR(d_k[i]);
                if (fabs(d_k[i]) > dmax) dmax = fabs(d_k[i]);
            }
            dnorm = sqrt(dnorm / n);
            dlimit = BIG;
            if (dmax > ls_maxatmov) {
                dlimit = ls_maxatmov / dmax; /* limit max coord movement */
            }
            s_k = ONE; /* initial LS step is full step given in d_k[] */
            rhs_g_wolfe = gtol_wolfe * fabs(dg_init);
            stpmax = BIG;
            if (dnorm < ONE)
                stpmin = TINY;
            else
                stpmin = LS_MINATMOV / dmax; /* force min coord movement */
                                             /* unless close to  minimum */
            brackt = FALSE;
            stage1 = TRUE;
            width = stpmax - stpmin;
            width1 = 2 * width;
            dgtest = ftol_wolfe * fabs(dg_init);
            stx = ZERO;
            fx = *fx_k;
            dgx = dg_init;
            sty = ZERO;
            fy = *fx_k;
            dgy = dg_init;
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in ls_wolfe(): Illegal status.\n");
            fflush(stderr);
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    for (i = 0; i < n; i++) x_k_save[i] = x_k[i];
    fx_k_save = *fx_k;
    for( i=1, (*alfa_k)=stp=s_k; i<=ls_maxiter; i++/* alfa_k is updated via
                                                      update_wolfe_step() */ ){
        /* Set the minimum and maximum steps to correspond
           to the present interval of uncertainty: */

        if (brackt) {
            stmin = MIN(stx, sty);
            stmax = MAX(stx, sty);
        } else {
            stmin = stx;
            stmax = stp + 4 * (stp - stx);
        }

        /* Force the step to be within the bounds stpmax and stpmin: */

        stp = MAX(stp, stpmin);
        stp = MIN(stp, stpmax);

        /* If an unusual termination is to occur then
           let stp be the lowest point obtained so far: */

        if ((brackt && (stp <= stmin || stp >= stmax)) || (brackt && (stmax - stmin <= DBL_EPSILON * stmax))) stp = stx;

        if (stp > dlimit)
            stp = dlimit; /* limit max coord displacement
                             to ls_maxatmov */

        /* Compute energy and gradient at x_k[]+stp*d_k[]: */
        for (j = 0; j < n; j++) {
            x_k[j] += stp * d_k[j];
        }
        /* Wolfe needs both energy and gradient */
        if (i == 1) {
            *return_flag = CALCBOTH_NEWNBL; /* force NBL update at LS start */
        } else {
            *return_flag = CALCBOTH_OLDNBL; /* keep same NBL until LS done */
        }
        *label = 1;
        return;
    L01:
        lhs_f_wolfe = *fx_k - fx_k_save;
        rhs_f_wolfe = stp * ftol_wolfe * dg_init;
        for (j = 0, dg_try = ZERO; j < n; j++) {
            dg_try += g_k[j] * d_k[j];
        }
        lhs_g_wolfe = fabs(dg_try);
        if (get_mytaskid() == 0) {
            if (DEBUG_MINIMZ) {
                fprintf(nabout, "  LS: i=%2d  lhs_f=%16.8g  rhs_f=%16.8g\n", i, lhs_f_wolfe, rhs_f_wolfe);
                fprintf(nabout, "            lhs_g=%16.8g  rhs_g=%16.8g\n", lhs_g_wolfe, rhs_g_wolfe);
                fflush(nabout);
            }
        }
        /* Test for convergence: */
        if (lhs_f_wolfe <= rhs_f_wolfe && lhs_g_wolfe <= rhs_g_wolfe) {
            *label = 0; /* ls_wolfe() done */
            break;      /* LINE SEARCH SUCCESSFUL */
        }
        /* Numerical havoc: */
        else if (brackt && (stp <= stmin || stp >= stmax)) {
            if (DEBUG_MINIMZ) {
                fprintf(stderr, "Line minimizer aborted: rounding error\n");
            }
            *label = error_flag = LSEARCH_ERROR;
            goto error_cleanup;
        } else if (stp == stpmax && (lhs_f_wolfe <= rhs_f_wolfe && dg_try <= dgtest)) {
            if (DEBUG_MINIMZ) {
                fprintf(stderr, "Line minimizer aborted: step at upper bound\n");
            }
            *label = error_flag = LSEARCH_ERROR;
            goto error_cleanup;
        } else if (stp == stpmin && (lhs_f_wolfe > rhs_f_wolfe || dg_try >= dgtest)) {
            if (DEBUG_MINIMZ) {
                fprintf(stderr, "Line minimizer aborted: step at lower bound\n");
            }
            *label = error_flag = LSEARCH_ERROR;
            goto error_cleanup;
        } else if (brackt && (stmax - stmin <= DBL_EPSILON * stmax)) {
            if (DEBUG_MINIMZ) {
                fprintf(stderr, "Line minimizer aborted: interval of uncertainty too small\n");
            }
            *label = error_flag = LSEARCH_ERROR;
            goto error_cleanup;
        }

        /* It is always the last error message, which is relevant, but it will
           not abort minimization, only exit this particular line search step. */

        /* MORE WORK TO DO.
           In the first stage we seek a step for which the modified
           function has a nonpositive value and a nonnegative derivative: */

        if (stage1 && lhs_f_wolfe <= rhs_f_wolfe && dg_try >= MIN(ftol_wolfe, gtol_wolfe) * dg_init) stage1 = FALSE;

        /* A modified function is used to predict the step only if
           we have not obtained a step for which the modified
           function has a nonpositive function value and nonnegative
           derivative, and if a lower function value has been
           obtained but the decrease is not sufficient: */

        if (stage1 && *fx_k <= fx && lhs_f_wolfe > rhs_f_wolfe) {
            /* Define the modified function and derivative values: */

            fm = *fx_k - stp * dgtest;
            fxm = fx - stx * dgtest;
            fym = fy - sty * dgtest;
            dgm = dg_try - dgtest;
            dgxm = dgx - dgtest;
            dgym = dgy - dgtest;

            /* Update the interval of uncertainty and compute the new step: */

            update_wolfe_step(&stx, &fxm, &dgxm, &sty, &fym, &dgym, &stp, fm, dgm, &brackt, stmin, stmax, &error_flag);

            if (error_flag) {
                *label = error_flag;
                goto error_cleanup;
            }

            /* Reset the function and gradient values for f: */

            fx = fxm + stx * dgtest;
            fy = fym + sty * dgtest;
            dgx = dgxm + dgtest;
            dgy = dgym + dgtest;

        } else {
            update_wolfe_step(&stx, &fx, &dgx, &sty, &fy, &dgy, &stp, *fx_k, dg_try, &brackt, stmin, stmax,
                              &error_flag);

            if (error_flag) {
                *label = error_flag;
                goto error_cleanup;
            }
        }

        /* Force a sufficient decrease in the size
           of the interval of uncertainty: */

        if (brackt) {
            if (fabs(sty - stx) >= .66 * width1) stp = stx + .5 * (sty - stx);
            width1 = width;
            width = fabs(sty - stx);
        }

        if (i < ls_maxiter) memcpy(x_k, x_k_save, n * sizeof(double));
    }

error_cleanup:
    *alfa_k = stp;
    *ls_iter = MIN(i, ls_maxiter);
    (*ls_tot_iter) += *ls_iter;

    if (allocated) {
        my_free(x_k_save);
        allocated = NO;
    }
    return;
}

static void ls_armijo(int k, int n, double *x_k, double *fx_k, double *g_k, double *x_diff, double *g_diff,
                      int ls_maxiter, int *ls_tot_iter, double ls_maxatmov, double beta_armijo, double c_armijo,
                      double mu_armijo, double ftol_wolfe, double gtol_wolfe, double *d_k, double *alfa_k, int *ls_iter,
                      int *return_flag, int *label) {
    static int status_flag, error_flag;
    static double fx_k_save, *x_k_save = NULL;
    static double dir_grad, d_k_norm, dmax, dlimit, lhs_armijo, rhs_armijo;
    static double l_k, s_k, sum_g_k, sum_x_k, mod_test;
    static int allocated = NO, i, j;
    //  MRR - Using the variables to avoid warnings
    status_flag = -1;
    status_flag++;
    //  END MRR

    switch (*label) {
        case 0:
            status_flag = 0;
            error_flag = FALSE;
            if (!allocated) {
                x_k_save = (double *)my_malloc(malloc, "\nERROR in ls_armijo/my_malloc(double *x_k_save)", n,
                                               sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            for (i = 0, dir_grad = d_k_norm = ZERO; i < n; i++) {
                dir_grad += g_k[i] * d_k[i];
                d_k_norm += SQR(d_k[i]);
            }
            if (k) {
                for (i = 0, sum_g_k = sum_x_k = ZERO; i < n; i++) {
                    sum_g_k += SQR(g_diff[i]);
                    sum_x_k += SQR(x_diff[i]);
                }
                l_k = sqrt(sum_g_k) / sqrt(sum_x_k);
            } else {
                l_k = 1.0;
            }
            s_k = -dir_grad / l_k / d_k_norm;
            mod_test = 0.5 * mu_armijo * l_k * d_k_norm;
            for (i = 0, dmax = ZERO; i < n; i++) {
                if (fabs(d_k[i]) > dmax) dmax = fabs(d_k[i]);
            }
            dlimit = BIG;
            if (dmax > ls_maxatmov) {
                dlimit = ls_maxatmov / dmax; /* limit max coord movement */
            }
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in ls_armijo(): Illegal status.\n");
            fflush(stderr);
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    for (i = 0; i < n; i++) x_k_save[i] = x_k[i];
    fx_k_save = *fx_k;
    for (i = 1, (*alfa_k) = s_k; i <= ls_maxiter; (*alfa_k) *= beta_armijo) {
        if ((*alfa_k) > dlimit)
            continue; /* do not compute the energy
                         when it is likely too big */
        /* Compute energy at x_k[]+alfa_k*d_k[]: */
        for (j = 0; j < n; j++) {
            x_k[j] += *alfa_k * d_k[j];
        }
        /* Armijo only needs ene, but grad also has to be current upon exit */
        if (i == 1) {
            *return_flag = CALCBOTH_NEWNBL; /* force NBL update at LS start */
        } else {
            *return_flag = CALCBOTH_OLDNBL; /* keep same NBL until LS done */
        }
        *label = 1;
        return;
    L01:
        lhs_armijo = *fx_k - fx_k_save;
        rhs_armijo = *alfa_k * c_armijo * (dir_grad + *alfa_k * mod_test);
        if (get_mytaskid() == 0) {
            if (DEBUG_MINIMZ) {
                fprintf(nabout, "  LS: i=%2d  lhs=%16.8g  rhs=%16.8g\n", i, lhs_armijo, rhs_armijo);
                fflush(nabout);
            }
        }
        if (lhs_armijo <= rhs_armijo) {
            break; /* line search successful */
        }
        if (i < ls_maxiter) memcpy(x_k, x_k_save, n * sizeof(double));

        i++; /* increment only when energy computed and Armijo test taken */
    }
    *ls_iter = MIN(i, ls_maxiter);
    (*ls_tot_iter) += *ls_iter;

    if (allocated) {
        my_free(x_k_save);
        allocated = NO;
    }
    *label = 0; /* ls_armijo() done */
    return;

error_cleanup:
    my_free(x_k_save);
    return;
}

typedef void (*LS_METHOD_TYPE)(int k, int n, double *x_k, double *fx_k, double *g_k, double *x_diff, double *g_diff,
                               int ls_maxiter, int *ls_tot_iter, double ls_maxatmov, double beta_armijo,
                               double c_armijo, double mu_armijo, double ftol_wolfe, double gtol_wolfe, double *d_k,
                               double *alfa_k, int *ls_iter, int *return_flag, int *label);

static void line_search(LS_METHOD_TYPE line_search_method, int min_iter, int ndim, double *xyz, double *enrg,
                        double *grad, double *x_diff, double *g_diff, double *dir, int ls_maxiter, int *ls_tot_iter,
                        double ls_maxatmov, double beta_armijo, double c_armijo, double mu_armijo, double ftol_wolfe,
                        double gtol_wolfe, int *return_flag, int *label)
/* Wrapper for line search routines. */
{
    /* Local: */
    static int status_flag, error_flag;
    static int ls_iter;
    static double alfa;

    //  MRR - Using the variables to avoid warnings
    error_flag = -1;
    error_flag++;
    //  END MRR

    switch (*label) {
        case 0:
            status_flag = 0;
            error_flag = FALSE;
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in line_search(): Illegal status.\n");
            fflush(stderr);
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    for (status_flag = 0;;) {
    L01:
        line_search_method(min_iter, ndim, xyz, enrg, grad, x_diff, g_diff, ls_maxiter, ls_tot_iter, ls_maxatmov,
                           beta_armijo, c_armijo, mu_armijo, ftol_wolfe, gtol_wolfe, dir, &alfa, &ls_iter, return_flag,
                           &status_flag);
        if (status_flag > 0) {
            *label = 1;
            return; /* line search continue */
        } else if (status_flag < 0) {
            /*
               *label = status_flag;
               goto error_cleanup; */ /* line search error    */

            break; /* line search exit     */
        } else
            break; /* line search done     */
    }
    if (get_mytaskid() == 0) {
        if (DEBUG_MINIMZ) {
            fprintf(nabout, "  LS: step=%16.8g  it=%2d\n", alfa, ls_iter);
            fflush(nabout);
        }
    }
    *label = 0; /* line_search done */
    return;

error_cleanup:
    return;
}

/*****
        Minimization routines:
*****/
static void init_lbfgs_matrix(int m, int ndim, int *error_flag) {
    static int allocated = NO, old_mlbfgs;
    int i, j;
    if (allocated) {
        for (i = 0; i < old_mlbfgs; i++) {
            my_free(lbfgs_matrix[i].s);
            my_free(lbfgs_matrix[i].y);
        }
        my_free(lbfgs_matrix);
        allocated = NO;
    }
    if (!allocated) {
        lbfgs_matrix =
            (struct lbfgs *)my_malloc(malloc, "\nERROR in init_lbfgs_matrix/my_malloc(struct lbfgs *lbfgs_matrix)", m,
                                      sizeof(struct lbfgs), error_flag);
        if (*error_flag) return;
        for (i = 0; i < m; i++) {
            lbfgs_matrix[i].s =
                (double *)my_malloc(malloc, "\nERROR in init_lbfgs_matrix/my_malloc(double *lbfgs_matrix[i].s)", ndim,
                                    sizeof(double), error_flag);
            if (*error_flag) {
                for (j = 0; j < i; j++) {
                    my_free(lbfgs_matrix[j].s);
                    my_free(lbfgs_matrix[j].y);
                }
                return;
            }
            lbfgs_matrix[i].y =
                (double *)my_malloc(malloc, "\nERROR in init_lbfgs_matrix/my_malloc(double *lbfgs_matrix[i].y)", ndim,
                                    sizeof(double), error_flag);
            if (*error_flag) {
                for (j = 0; j < i; j++) {
                    my_free(lbfgs_matrix[j].s);
                    my_free(lbfgs_matrix[j].y);
                }
                my_free(lbfgs_matrix[i].s);
                return;
            }
        }
        allocated = YES;
        old_mlbfgs = m;
    }
}

static void init_lbfgs_matrix_buf(int m, int ndim, int *error_flag) {
    static int allocated = NO, old_mlbfgs;
    int i, j;
    if (allocated) {
        for (i = 0; i < old_mlbfgs; i++) {
            my_free(lbfgs_matrix_buf[i].s);
            my_free(lbfgs_matrix_buf[i].y);
        }
        my_free(lbfgs_matrix_buf);
        allocated = NO;
    }
    if (!allocated) {
        lbfgs_matrix_buf = (struct lbfgs *)my_malloc(
            malloc, "\nERROR in init_lbfgs_matrix_buf/my_malloc(struct lbfgs *lbfgs_matrix_buf)", m,
            sizeof(struct lbfgs), error_flag);
        if (*error_flag) return;
        for (i = 0; i < m; i++) {
            lbfgs_matrix_buf[i].s =
                (double *)my_malloc(malloc, "\nERROR in init_lbfgs_matrix_buf/my_malloc(double *lbfgs_matrix_buf[i].s)",
                                    ndim, sizeof(double), error_flag);
            if (*error_flag) {
                for (j = 0; j < i; j++) {
                    my_free(lbfgs_matrix_buf[j].s);
                    my_free(lbfgs_matrix_buf[j].y);
                }
                return;
            }
            lbfgs_matrix_buf[i].y =
                (double *)my_malloc(malloc, "\nERROR in init_lbfgs_matrix_buf/my_malloc(double *lbfgs_matrix_buf[i].y)",
                                    ndim, sizeof(double), error_flag);
            if (*error_flag) {
                for (j = 0; j < i; j++) {
                    my_free(lbfgs_matrix_buf[j].s);
                    my_free(lbfgs_matrix_buf[j].y);
                }
                my_free(lbfgs_matrix_buf[i].s);
                return;
            }
        }
        allocated = YES;
        old_mlbfgs = m;
    }
}

static int load_lbfgs(struct lbfgs *lbfgsmat, int current, int ndim, double *s, double *y, int *error_flag) {
    int i;
    double rho, gamma;
    for (i = 0, rho = gamma = ZERO; i < ndim; i++) {
        lbfgsmat[current].s[i] = s[i];
        lbfgsmat[current].y[i] = y[i];
        rho += s[i] * y[i];
        gamma += y[i] * y[i];
    }
    if (rho == ZERO) {
        fprintf(stderr, "\nERROR in load_lbfgs(): YS=0.\n");
        fflush(stderr);
        *error_flag = DIVZERO_ERROR;
        return current;
    }
    if (gamma == ZERO) {
        fprintf(stderr, "\nERROR in load_lbfgs(): YY=0.\n");
        fflush(stderr);
        *error_flag = DIVZERO_ERROR;
        return current;
    }
    lbfgsmat[current].rho = ONE / rho;
    lbfgsmat[current].gamma = fabs(rho / gamma);
    return current;
}

static void nocedal(int m, int current, int ndim, double sign, int update, double *vec_in, double *vec_out,
                    int *error_flag) {
    int i, j;
    double *alpha = NULL, beta, dot, *tmp_in = NULL;
    static int allocated = NO;
    if (!allocated) {
        alpha =
            (double *)my_malloc(malloc, "\nERROR in nocedal/my_malloc(double *alpha)", m, sizeof(double), error_flag);
        if (*error_flag) goto error_cleanup;
        tmp_in = (double *)my_malloc(malloc, "\nERROR in nocedal/my_malloc(double *tmp_in)", ndim, sizeof(double),
                                     error_flag);
        if (*error_flag) goto error_cleanup;

        allocated = YES;
    }
    for (i = 0; i < ndim; i++) tmp_in[i] = sign * vec_in[i];
    for (j = m - 1; j >= 0; j--) {
        for (i = 0, dot = ZERO; i < ndim; i++) dot += lbfgs_matrix[j].s[i] * tmp_in[i];
        alpha[j] = lbfgs_matrix[j].rho * dot;
        for (i = 0; i < ndim; i++) tmp_in[i] -= alpha[j] * lbfgs_matrix[j].y[i];
    }
    switch (update) {
        case SCALING:
            for (i = 0; i < ndim; i++) vec_out[i] = tmp_in[i] * lbfgs_matrix[current].gamma; /* scaling */
            break;
        default:
            memcpy(vec_out, tmp_in, ndim * sizeof(double)); /* unit matrix */
            break;
    }
    for (j = 0; j < m; j++) {
        for (i = 0, dot = ZERO; i < ndim; i++) dot += lbfgs_matrix[j].y[i] * vec_out[i];
        beta = lbfgs_matrix[j].rho * dot;
        for (i = 0; i < ndim; i++) vec_out[i] += lbfgs_matrix[j].s[i] * (alpha[j] - beta);
    }
    if (allocated) {
        my_free(alpha);
        my_free(tmp_in);
        allocated = NO;
    }
    return; /* end nocedal() */

error_cleanup:
    my_free(alpha);
    my_free(tmp_in);
}

typedef void (*HESSVEC_METHOD_TYPE)(int *ndim, double *vec_in, double *vec_out, double *xyz, double *grad,
                                    int *return_flag, int *label);

static void tncg_minim(int ndim, int maxiter, double grms_tol, int m_lbfgs, HESSVEC_METHOD_TYPE hessvec,
                       LS_METHOD_TYPE ls_method, double *xyz, double *enrg, double *grad, double *grms_out,
                       int *iter_out, double *total_time, int ls_maxiter, int *ls_iter_out, double ls_maxatmov,
                       double beta_armijo, double c_armijo, double mu_armijo, double ftol_wolfe, double gtol_wolfe,
                       int *return_flag, int *label) {
    static int i, j, min_iter, min_conv, cg_iter, cg_conv, convex;
    static int lbfgs_matrix_reloaded, n_lbfgs, current_update, last_update;
    static double energy, energy_old, grms, rs_old, rs_new;
    static double dq, alpha, beta, qtest;
    static double *p = NULL, *r = NULL, *s = NULL, *q = NULL, *d = NULL;
    static double *s_vec = NULL, *r_old = NULL, *y_vec = NULL, *p_old = NULL;
    static double *xyz_old = NULL, *g_old = NULL, *step = NULL, *g_dif = NULL;
    static double *hp = NULL, php, gp, quad_energy_old, quad_energy_new;
    static int allocated, status_flag, error_flag;
    static clock_t time_stamp;

    //  MRR - Using the variables to avoid warnings
    energy_old = -1.0;
    energy_old = energy_old + 1.0;
    //  END MRR

    switch (*label) {
        case 0:
            *total_time = ZERO;
            energy_old = BIG;
            allocated = NO;
            status_flag = 0;
            error_flag = FALSE;
            convex = YES;
            if (!allocated) {
                p = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *p)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                r = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *r)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                s = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *s)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                q = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *q)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                d = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *d)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                s_vec = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *s_vec)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                r_old = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *r_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                y_vec = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *y_vec)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                p_old = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *p_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                hp = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *hp)", ndim, sizeof(double),
                                         &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                xyz_old = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *xyz_old)", ndim,
                                              sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_old = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *g_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                step = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *step)", ndim,
                                           sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_dif = (double *)my_malloc(malloc, "\nERROR in tncg_minim/my_malloc(double *g_dif)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            goto L00;
        case 1:
            goto L01;
        case 2:
            goto L02;
        case 3:
            goto L03;
        case 4:
            goto L04;
        default:
            fprintf(stderr, "\nERROR in tncg_minim(): Illegal status.\n");
            fflush(stderr);
            if (allocated) allocated = NO;
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    if (m_lbfgs) {
        init_lbfgs_matrix(m_lbfgs, ndim, &error_flag);
        init_lbfgs_matrix_buf(m_lbfgs, ndim, &error_flag);
    }
    if (error_flag) {
        if (allocated) allocated = NO;
        *label = error_flag;
        return;
    }
    /*
        MAIN MINIMIZATION LOOP:
        ( CG preconditioning from J. Nocedal and J. L. Morales:
          Automatic preconditioning by limited memory quasi-Newton updating,
          SIAM J. Optimization, 10, 4, 1079-1096, 2000. )
    */
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "________________________________________________________________\n");
        }
    }
    min_conv = min_iter = 0;
    *iter_out = 1;
    *ls_iter_out = 0;
    lbfgs_matrix_reloaded = NO;
    do {
        time_stamp = clock();
        /*
              Update derivs:
        */
        if (min_iter == 0) {
            *return_flag = CALCBOTH_NEWNBL; /* force NBL update */
            *label = 1;
            return;
        }
    L01:
        if (min_iter) {
            for (i = 0; i < ndim; i++) {
                step[i] = xyz[i] - xyz_old[i];
                g_dif[i] = grad[i] - g_old[i];
            }
        }
        for (i = 0, grms = ZERO; i < ndim; i++) grms += SQR(grad[i]);
        grms = sqrt(grms / ndim);
        if (get_mytaskid() == 0) {
            if (PRINT_MINIMZ) {
                energy = *enrg;
                fprintf(nabout, " MIN:          It= %4d  nfunc= %5d  E= %15.5f (%9.2e)\n", min_iter, nfunc, energy,
                        grms);
            }
        }
        if (grms <= grms_tol || min_iter == maxiter) {
            min_conv = YES;
            break;
        }
        for (i = 0; i < ndim; i++) {
            p[i] = ZERO;
            r[i] = -grad[i];
            hp[i] = ZERO;
        }
        /*
              Preconditioning: M * s = r
        */
        if (lbfgs_matrix_reloaded) {
            nocedal(m_lbfgs, last_update, ndim, ONE, SCALING, r, s, &error_flag);
            if (error_flag) {
                if (allocated) allocated = NO;
                *label = error_flag;
                goto error_cleanup;
            }
        } else {
            for (i = 0; i < ndim; i++) s[i] = r[i]; /* trivial preconditioning */
        }
        for (i = 0, rs_old = ZERO; i < ndim; i++) {
            d[i] = s[i];
            rs_old += r[i] * s[i];
        }
        /*
              CONJUGATE GRADIENT ITERATION LOOP:
        */
        cg_conv = cg_iter = n_lbfgs = 0;
        qtest = 999.999;
        do {
            for (status_flag = 0;;) { /* calc q = H*d     */
            L02:
                hessvec(&ndim, d, q, xyz, grad, return_flag, &status_flag);
                if (status_flag > 0) {
                    *label = 2;
                    return; /* hessvec continue */
                } else if (status_flag < 0) {
                    if (allocated) allocated = NO;
                    *label = status_flag;
                    goto error_cleanup; /* hessvec error    */
                } else
                    break; /* hessvec done     */
            }              /* end hessvec() */
            for (i = 0, dq = ZERO; i < ndim; i++) dq += d[i] * q[i];
            /*
                        Test for negative curvature:
            */
            if (dq <= ZERO) {
                convex = NO;
                if (!cg_iter) {
                    for (i = 0; i < ndim; i++) {
                        p[i] = -grad[i]; /* p[] not yet calculated, use -grad[] */
                    }
                }
                break; /* bail out with current p[] */
            } else {
                convex = YES;
            }
            /*
                        TEST FOR CG TERMINATION:

                        Quadratic reduction test.

                            Quadratic model that is minimized by CG:
                            Q( p[]_cg_iter ) =
                                f( xyz[]_min_iter + p[]_cg_iter ) - f( xyz[]_min_iter ) =
                                grad[]_min_iter * p[]_cg_iter +
                                1/2 * p[]_cg_iter * H_min_iter * p[]_cg_iter

                            cg_iter * ( 1 - Q( p[]_cg_iter_-1 ) / Q( p[]_cg_iter ) ) < 0.5
                            measures the reduction of the quadratic approximation at the
                            current CG iterate with respect to the average reduction.
            */
            alpha = rs_old / dq;
            for (i = 0; i < ndim; i++) {
                p_old[i] = p[i];
                r_old[i] = r[i];
                p[i] += alpha * d[i]; /* update external/minim search dir
                                         (p[] is the CG iterate) */
                r[i] -= alpha * q[i]; /* update residual vector */
                s_vec[i] = p[i] - p_old[i];
                y_vec[i] = r_old[i] - r[i]; /* residual is defined as
                                               "b-Ax" and not "Ax-b". */
                hp[i] += alpha * q[i];      /* Hessian * p[] for quadratic test
                                              ( p[] = alpha*d[] ) */
            }
            if (m_lbfgs) { /* update LBFGS matrix with s_vec[] (iterate)
                              and y_vec[] (residual) */
                n_lbfgs++;
                current_update = load_lbfgs(lbfgs_matrix_buf, cg_iter % m_lbfgs, ndim, s_vec, y_vec, &error_flag);
                if (error_flag) {
                    if (allocated) allocated = NO;
                    *label = error_flag;
                    goto error_cleanup;
                }
            }
            for (i = 0, gp = php = ZERO; i < ndim; i++) {
                gp += grad[i] * p[i];
                php += p[i] * hp[i];
            }
            quad_energy_new = gp + 0.5 * php;
            if (cg_iter && cg_iter >= (m_lbfgs - 1)) {
                qtest = (cg_iter + 1) * (ONE - quad_energy_old / quad_energy_new);
                if (qtest < CG_QTOL) { /* CG convergence achieved
                                          by quadratic test */
                    cg_conv = YES;
                    break;
                }
            }
            quad_energy_old = quad_energy_new;
            /*
                      Preconditioning: M * s = r
            */
            if (lbfgs_matrix_reloaded) {
                nocedal(m_lbfgs, last_update, ndim, ONE, SCALING, r, s, &error_flag);
                if (error_flag) {
                    if (allocated) allocated = NO;
                    *label = error_flag;
                    goto error_cleanup;
                }
            } else {
                for (i = 0; i < ndim; i++) s[i] = r[i]; /* trivial preconditioning */
            }
            /*
                      Update CG direction (d[] is the current vector in a sequence of
                      conjugate (internal) search directions):
            */
            for (i = 0, rs_new = ZERO; i < ndim; i++) rs_new += r[i] * s[i];
            beta = rs_new / rs_old;
            rs_old = rs_new;
            for (i = 0; i < ndim; i++) d[i] = s[i] + beta * d[i];

        } while (++cg_iter < CG_ITERMAX);

        /*
              End conjugate gradient iteration loop.
        */

        if (get_mytaskid() == 0) {
            if (DEBUG_MINIMZ) {
                fprintf(nabout, "  CG:   It= %4d (%7.3f)q  %s%s\n",
                        (convex == NO || cg_iter == CG_ITERMAX) ? cg_iter : cg_iter + 1, qtest,
                        cg_conv == YES ? ":-)" : ":-(", convex == YES ? "" : "(");
            }
        }

        /*
              Update LBFGS preconditioner:
        */
        if (m_lbfgs && n_lbfgs >= m_lbfgs /* && convex */) {
            lbfgs_matrix_reloaded = YES;
            last_update = current_update;
            for (i = 0; i < m_lbfgs; i++) {
                for (j = 0; j < ndim; j++) {
                    lbfgs_matrix[i].s[j] = lbfgs_matrix_buf[i].s[j];
                    lbfgs_matrix[i].y[j] = lbfgs_matrix_buf[i].y[j];
                }
                lbfgs_matrix[i].rho = lbfgs_matrix_buf[i].rho;
                lbfgs_matrix[i].gamma = lbfgs_matrix_buf[i].gamma;
            }
        }

        memcpy(g_old, grad, ndim * sizeof(double));  /* save old grad
                                                        before LS move */
        memcpy(xyz_old, xyz, ndim * sizeof(double)); /* save old xyz
                                                        before LS move */
                                                     /*
                                                           At this point TNCG search direction is given in p[].
                                                           Find the line minimum:
                                                     */
        for (status_flag = 0;;) {
        L03:
            line_search(ls_method, min_iter, ndim, xyz, enrg, grad, step, g_dif, p, ls_maxiter, ls_iter_out,
                        ls_maxatmov, beta_armijo, c_armijo, mu_armijo, ftol_wolfe, gtol_wolfe, return_flag,
                        &status_flag);
            if (status_flag > 0) {
                *label = 3;
                return; /* line_search continue */
            } else if (status_flag < 0) {
                if (allocated) allocated = NO;
                *label = status_flag;
                goto error_cleanup; /* line_search error    */
            } else {
                energy = *enrg;
                break; /* line_search done     */
            }
        } /* end line_search() */
        energy_old = energy;
        (*iter_out)++;
        *total_time += clock() - time_stamp;
    } while (++min_iter <= maxiter);
    /*
        End main minimization loop.
    */
    /* Sanity check: */
    *return_flag = CALCBOTH_NEWNBL;
    *iter_out = min_iter;
    *label = 4;
    return;
L04:
    energy = *enrg;
    for (i = 0, grms = ZERO; i < ndim; i++) grms += SQR(grad[i]);
    grms = sqrt(grms / ndim);
    *grms_out = grms;
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "----------------------------------------------------------------\n");
            fprintf(nabout, " FIN:             %s                    E= %10.2f (%7.3f)\n\n",
                    min_conv == YES ? ":-)" : ":-(", energy, grms);
        }
    }
    /* Deallocate local arrays: */
    if (allocated) {
        my_free(p);
        my_free(r);
        my_free(s);
        my_free(q);
        my_free(d);
        my_free(s_vec);
        my_free(r_old);
        my_free(y_vec);
        my_free(p_old);
        my_free(hp);
        my_free(xyz_old);
        my_free(g_old);
        my_free(step);
        my_free(g_dif);
        allocated = NO;
    }
    *total_time += clock() - time_stamp;
    *total_time /= CLOCKS_PER_SEC;
    *label = 0; /* tncg_minim() done */
    return;

error_cleanup:
    my_free(p);
    my_free(r);
    my_free(s);
    my_free(q);
    my_free(d);
    my_free(s_vec);
    my_free(r_old);
    my_free(y_vec);
    my_free(p_old);
    my_free(hp);
    my_free(xyz_old);
    my_free(g_old);
    my_free(step);
    my_free(g_dif);
}

static void prcg_minim(int ndim, int maxiter, double grms_tol, int m_lbfgs, HESSVEC_METHOD_TYPE hessvec,
                       LS_METHOD_TYPE ls_method, double *xyz, double *enrg, double *grad, double *grms_out,
                       int *iter_out, double *total_time, int ls_maxiter, int *ls_iter_out, double ls_maxatmov,
                       double beta_armijo, double c_armijo, double mu_armijo, double ftol_wolfe, double gtol_wolfe,
                       int *return_flag, int *label) {
    static int i, min_iter, min_conv;
    static double energy, energy_old, grms, ggnew, ggold, gamma, dgrad;
    static double *p = NULL, *p_old = NULL, *g_old = NULL;
    static double *xyz_old = NULL, *step = NULL, *g_dif = NULL;
    static int allocated, status_flag, error_flag;
    static clock_t time_stamp;

    //  MRR - Using the variables to avoid warnings
    energy_old = -1.0;
    energy_old = energy_old + 1.0;
    //  END MRR

    switch (*label) {
        case 0:
            *total_time = ZERO;
            energy_old = BIG;
            allocated = NO;
            status_flag = 0;
            error_flag = FALSE;
            if (!allocated) {
                p = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *p)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                p_old = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *p_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_old = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *g_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                xyz_old = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *xyz_old)", ndim,
                                              sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                step = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *step)", ndim,
                                           sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_dif = (double *)my_malloc(malloc, "\nERROR in prcg_minim/my_malloc(double *g_dif)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            goto L00;
        case 1:
            goto L01;
        case 2:
            goto L02;
        case 3:
            goto L03;
        default:
            fprintf(stderr, "\nERROR in prcg_minim(): Illegal status.\n");
            fflush(stderr);
            if (allocated) allocated = NO;
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    /*
        Main minimization loop:
    */
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "________________________________________________________________\n");
        }
    }
    min_conv = min_iter = 0;
    *iter_out = 1;
    *ls_iter_out = 0;
    do {
        time_stamp = clock();
        /*
                Update derivs:
        */
        if (min_iter == 0) {
            *return_flag = CALCBOTH_NEWNBL; /* force NBL update */
            *label = 1;
            return;
        }
    L01:
        if (min_iter) {
            for (i = 0; i < ndim; i++) {
                step[i] = xyz[i] - xyz_old[i];
                g_dif[i] = grad[i] - g_old[i];
            }
        }
        for (i = 0, grms = ZERO, ggnew = ZERO, ggold = ZERO; i < ndim; i++) {
            grms += SQR(grad[i]);
            ggold += SQR(g_old[i]);
            /*       ggnew += SQR(grad[i]);                            Fletcher-Reeves */
            ggnew += (grad[i] - g_old[i]) * grad[i]; /* Polak-Ribiere   */
        }
        grms = sqrt(grms / ndim);
        if (get_mytaskid() == 0) {
            if (PRINT_MINIMZ) {
                energy = *enrg;
                fprintf(nabout, " MIN:                          It= %4d  E= %10.2f (%7.3f)\n", min_iter, energy, grms);
            }
        }
        if (grms <= grms_tol || min_iter == maxiter) {
            min_conv = YES;
            break;
        }
        if (min_iter)
            gamma = ggnew / ggold;
        else
            gamma = ZERO;
        /*
              Update CG search direction:
        */
        for (i = 0; i < ndim; i++) {
            p[i] = -grad[i] + gamma * p_old[i];
        }
        /*
               Test for downhill search direction:
        */
        for (i = 0, dgrad = ZERO; i < ndim; i++) dgrad += grad[i] * p[i];
        if (dgrad > ZERO) { /* Uphill movement!
                               Replace conjgrad step with -grad */
            for (i = 0; i < ndim; i++) p[i] = -grad[i];
        }
        for (i = 0; i < ndim; i++) p_old[i] = p[i];
        memcpy(g_old, grad, ndim * sizeof(double));  /* save old grad
                                                        before LS move */
        memcpy(xyz_old, xyz, ndim * sizeof(double)); /* save old xyz
                                                        before LS move */
                                                     /*
                                                           At this point PRCG search direction is given in p[].
                                                           Find the line minimum:
                                                     */
        for (status_flag = 0;;) {
        L02:
            line_search(ls_method, min_iter, ndim, xyz, enrg, grad, step, g_dif, p, ls_maxiter, ls_iter_out,
                        ls_maxatmov, beta_armijo, c_armijo, mu_armijo, ftol_wolfe, gtol_wolfe, return_flag,
                        &status_flag);
            if (status_flag > 0) {
                *label = 2;
                return; /* line_search continue */
            } else if (status_flag < 0) {
                if (allocated) allocated = NO;
                *label = status_flag;
                goto error_cleanup; /* line_search error    */
            } else {
                energy = *enrg;
                break; /* line_search done     */
            }
        } /* end line_search() */
        energy_old = energy;
        (*iter_out)++;
        *total_time += clock() - time_stamp;
    } while (++min_iter <= maxiter);
    /*
        End main minimization loop.
    */
    /* Sanity check: */
    *return_flag = CALCBOTH_NEWNBL;
    *iter_out = min_iter;
    *label = 3;
    return;
L03:
    energy = *enrg;
    for (i = 0, grms = ZERO; i < ndim; i++) grms += SQR(grad[i]);
    grms = sqrt(grms / ndim);
    *grms_out = grms;
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "----------------------------------------------------------------\n");
            fprintf(nabout, " FIN:             %s                    E= %10.2f (%7.3f)\n\n",
                    min_conv == YES ? ":-)" : ":-(", energy, grms);
        }
    }
    /* Deallocate local arrays: */
    if (allocated) {
        my_free(p);
        my_free(p_old);
        my_free(g_old);
        my_free(xyz_old);
        my_free(step);
        my_free(g_dif);
        allocated = NO;
    }
    *total_time += clock() - time_stamp;
    *total_time /= CLOCKS_PER_SEC;
    *label = 0; /* prcg_minim() done */
    return;

error_cleanup:
    my_free(p);
    my_free(p_old);
    my_free(g_old);
    my_free(xyz_old);
    my_free(step);
    my_free(g_dif);
}

static void lbfgs_minim(int ndim, int maxiter, double grms_tol, int m_lbfgs, HESSVEC_METHOD_TYPE hessvec,
                        LS_METHOD_TYPE ls_method, double *xyz, double *enrg, double *grad, double *grms_out,
                        int *iter_out, double *total_time, int ls_maxiter, int *ls_iter_out, double ls_maxatmov,
                        double beta_armijo, double c_armijo, double mu_armijo, double ftol_wolfe, double gtol_wolfe,
                        int *return_flag, int *label) {
    static int i, min_iter, min_conv;
    static double energy, energy_old, grms, ggnew, ggold, gamma, dgrad;
    static double *p = NULL, *p_old = NULL, *g_old = NULL;
    static double *g_dif = NULL, *step = NULL, *xyz_old = NULL;
    static int allocated, status_flag, error_flag;
    static clock_t time_stamp;

    //  MRR - Using the variables to avoid warnings
    energy_old = -1.0;
    energy_old = energy_old + 1.0;
    //  END MRR

    switch (*label) {
        case 0:
            *total_time = ZERO;
            energy_old = BIG;
            allocated = NO;
            status_flag = 0;
            error_flag = FALSE;
            if (!allocated) {
                p = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *p)", ndim, sizeof(double),
                                        &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                p_old = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *p_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_old = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *g_old)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                g_dif = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *g_dif)", ndim,
                                            sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                step = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *step)", ndim,
                                           sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                xyz_old = (double *)my_malloc(malloc, "\nERROR in lbfgs_minim/my_malloc(double *xyz_old)", ndim,
                                              sizeof(double), &error_flag);
                if (error_flag) {
                    *label = error_flag;
                    goto error_cleanup;
                }
                allocated = YES;
            }
            goto L00;
        case 1:
            goto L01;
        case 2:
            goto L02;
        case 3:
            goto L03;
        default:
            fprintf(stderr, "\nERROR in lbfgs_minim(): Illegal status.\n");
            fflush(stderr);
            if (allocated) allocated = NO;
            *label = ILLEGAL_STATUS;
            goto error_cleanup;
    }
L00:
    if (!m_lbfgs) {
        fprintf(stderr, "\nERROR in lbfgs_minim(): m_lbfgs=0.\n");
        fflush(stderr);
        if (allocated) allocated = NO;
        *label = MINIMZ_ERROR;
        goto error_cleanup;
    }
    init_lbfgs_matrix(m_lbfgs, ndim, &error_flag);
    if (error_flag) {
        if (allocated) allocated = NO;
        *label = error_flag;
        goto error_cleanup;
    }
    /*
        Main minimization loop:
    */
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "________________________________________________________________\n");
        }
    }
    min_conv = min_iter = 0;
    *iter_out = 1;
    *ls_iter_out = 0;
    do {
        time_stamp = clock();
        /*
                Update derivs:
        */
        if (min_iter == 0) {
            *return_flag = CALCBOTH_NEWNBL; /* force NBL update */
            *label = 1;
            return;
        }
    L01:
        if (min_iter) { /* store last m_lbfgs step[] and g_dif[]
                           vectors in circular order */
            for (i = 0; i < ndim; i++) {
                step[i] = xyz[i] - xyz_old[i];
                g_dif[i] = grad[i] - g_old[i];
            }
            load_lbfgs(lbfgs_matrix, (min_iter - 1) % m_lbfgs, ndim, step, g_dif, &error_flag);
            if (error_flag) {
                if (allocated) allocated = NO;
                *label = error_flag;
                /* goto error_cleanup;  */
                /* add new code here to force a finish */
                break;
            }
        }
        if (min_iter < m_lbfgs) { /* pre-PRCG */
            for (i = 0, grms = ZERO, ggnew = ZERO, ggold = ZERO; i < ndim; i++) {
                grms += SQR(grad[i]);
                ggold += SQR(g_old[i]);
                /*            ggnew += SQR(grad[i]);                       Fletcher-Reeves */
                ggnew += (grad[i] - g_old[i]) * grad[i]; /* Polak-Ribiere   */
            }
            grms = sqrt(grms / ndim);
            if (get_mytaskid() == 0) {
                if (PRINT_MINIMZ) {
                    energy = *enrg;
                    fprintf(nabout, " MIN:                          It= %4d  E= %10.2f (%7.3f)\n", min_iter, energy,
                            grms);
                }
            }
            if (grms <= grms_tol || min_iter == maxiter) {
                min_conv = YES;
                break;
            }
            if (min_iter)
                gamma = ggnew / ggold;
            else
                gamma = ZERO;
            /*
                      Update CG search direction:
            */
            for (i = 0; i < ndim; i++) {
                p[i] = -grad[i] + gamma * p_old[i];
            }
            /*
                     Test for downhill search direction:
            */
            for (i = 0, dgrad = ZERO; i < ndim; i++) dgrad += grad[i] * p[i];
            if (dgrad > ZERO) { /* Uphill movement!
                                   Replace conjgrad step with -grad */
                for (i = 0; i < ndim; i++) p[i] = -grad[i];
            }
            for (i = 0; i < ndim; i++) p_old[i] = p[i];
        } else { /* LBFGS */
            for (i = 0, grms = ZERO; i < ndim; i++) grms += SQR(grad[i]);
            grms = sqrt(grms / ndim);
            if (get_mytaskid() == 0) {
                if (PRINT_MINIMZ) {
                    energy = *enrg;
                    fprintf(nabout, " MIN:                          It= %4d  E= %10.2f (%7.3f)\n", min_iter, energy,
                            grms);
                }
            }
            if (grms <= grms_tol || min_iter == maxiter) {
                min_conv = YES;
                break;
            }
            /*
                     Update LBFGS search direction:
            */
            nocedal(m_lbfgs, (min_iter - 1) % m_lbfgs, ndim, -ONE, SCALING, grad, p, &error_flag);
            if (error_flag) {
                if (allocated) allocated = NO;
                *label = error_flag;
                goto error_cleanup;
            }
            /*
                     Test for downhill search direction:
            */
            for (i = 0, dgrad = ZERO; i < ndim; i++) dgrad += grad[i] * p[i];
            if (dgrad > ZERO) { /* Uphill movement!
                                   Replace LBFGS step with -grad */
                for (i = 0; i < ndim; i++) p[i] = -grad[i];
            }
        }
        memcpy(xyz_old, xyz, ndim * sizeof(double)); /* save old coord
                                                        before LS move */
        memcpy(g_old, grad, ndim * sizeof(double));  /* save old grad
                                                        before LS move */
                                                     /*
                                                           At this point pre-PRCG/LBFGS search direction is given in p[].
                                                           Find the line minimum:
                                                     */
        for (status_flag = 0;;) {
        L02:
            line_search(ls_method, min_iter, ndim, xyz, enrg, grad, step, g_dif, p, ls_maxiter, ls_iter_out,
                        ls_maxatmov, beta_armijo, c_armijo, mu_armijo, ftol_wolfe, gtol_wolfe, return_flag,
                        &status_flag);
            if (status_flag > 0) {
                *label = 2;
                return; /* line_search continue */
            } else if (status_flag < 0) {
                if (allocated) allocated = NO;
                *label = status_flag;
                goto error_cleanup; /* line_search error    */
            } else {
                energy = *enrg;
                break; /* line_search done     */
            }
        } /* end line_search() */
        energy_old = energy;
        (*iter_out)++;
        *total_time += clock() - time_stamp;
    } while (++min_iter <= maxiter);
    /*
        End main minimization loop.
    */
    /* Sanity check: */
    *return_flag = CALCBOTH_NEWNBL;
    *label = 3;
    *iter_out = min_iter;
    return;
L03:
    energy = *enrg;
    for (i = 0, grms = ZERO; i < ndim; i++) grms += SQR(grad[i]);
    grms = sqrt(grms / ndim);
    *grms_out = grms;
    if (get_mytaskid() == 0) {
        if (PRINT_MINIMZ) {
            fprintf(nabout, "----------------------------------------------------------------\n");
            fprintf(nabout, " FIN:             %s                    E= %10.2f (%7.3f)\n\n",
                    min_conv == YES ? ":-)" : ":-(", energy, grms);
        }
    }
    /* Deallocate local arrays: */
    if (allocated) {
        my_free(p);
        my_free(p_old);
        my_free(g_old);
        my_free(g_dif);
        my_free(step);
        my_free(xyz_old);
        allocated = NO;
    }
    *total_time += clock() - time_stamp;
    *total_time /= CLOCKS_PER_SEC;
    *label = 0; /* lbfgs_minim() done */
    return;

error_cleanup:
    my_free(p);
    my_free(p_old);
    my_free(g_old);
    my_free(g_dif);
    my_free(step);
    my_free(xyz_old);
}

typedef void (*MINIM_METHOD_TYPE)(int ndim, int maxiter, double grms_tol, int m_lbfgs, HESSVEC_METHOD_TYPE hessvec,
                                  LS_METHOD_TYPE ls_method, double *xyz, double *enrg, double *grad, double *grms_out,
                                  int *iter_out, double *total_time, int ls_maxiter, int *ls_iter_out,
                                  double ls_maxatmov, double beta_armijo, double c_armijo, double mu_armijo,
                                  double ftol_wolfe, double gtol_wolfe, int *return_flag, int *label);

static void minim(MINIM_METHOD_TYPE minim_method, HESSVEC_METHOD_TYPE numdiff_method, LS_METHOD_TYPE lsearch_method,
                  int ndim, int maxiter, double grms_tol, int m_lbfgs, double *xyz, double *enrg, double *grad,
                  double *grms, int *iter, double *total_time, int ls_maxiter, int *ls_iter, double ls_maxatmov,
                  double beta_armijo, double c_armijo, double mu_armijo, double ftol_wolfe, double gtol_wolfe,
                  int *return_flag, int *label) {
    static int status_flag;
    switch (*label) {
        case 0:
            status_flag = 0;
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in minim(): Illegal status.\n");
            fflush(stderr);
            *label = ILLEGAL_STATUS;
            return;
    }
L00:
    for (status_flag = 0;;) {
    L01:
        minim_method(ndim, maxiter, grms_tol, m_lbfgs, numdiff_method, lsearch_method, xyz, enrg, grad, grms, iter,
                     total_time, ls_maxiter, ls_iter, ls_maxatmov, beta_armijo, c_armijo, mu_armijo, ftol_wolfe,
                     gtol_wolfe, return_flag, &status_flag);
        if (status_flag > 0) {
            *label = 1;
            return; /* minim continue */
        } else if (status_flag < 0) {
            *label = status_flag;
            return; /* minim error    */
        } else {
            break; /* minim done     */
        }
    }           /* end minim() */
    *label = 0; /* minim() done */
}

namespace tools {

/*****
        Reverse communication XMIN function:
*****/

double xminC(int *xyz_min, int *minim_method, int *maxiter, double *grms_tol, int *natm_ext, int *m_lbfgs, int *numdiff,
             double *xyz_ext, double *enrg, double *grad_ext, double *grms, int *iter, double *total_time,
             int *print_level, int *ls_method, int *ls_maxiter, int *ls_iter, double *ls_maxatmov, double *beta_armijo,
             double *c_armijo, double *mu_armijo, double *ftol_wolfe, double *gtol_wolfe, int *return_flag,
             int *label) {
    static int i, j, ndim, natm_local, *atm_indx = NULL;
    static double *xyz_local = NULL, *grad_local = NULL;
    static int allocated, error_flag;
    static int status_flag;

#ifdef SQM
    nabout = stdout;
#endif
    switch (*label) {
        case 0:
            if (*maxiter < 0) {
                fprintf(stderr, "\nERROR in xmin(): Requested number of iterations negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*grms_tol < ZERO) {
                fprintf(stderr, "\nERROR in xmin(): Requested grad RMS negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*m_lbfgs < 0) {
                fprintf(stderr, "\nERROR in xmin(): LBFGS dimension negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*print_level == 0) {
                PRINT_MINIMZ = NO;
                DEBUG_MINIMZ = NO;
            } else if (*print_level == 1) {
                PRINT_MINIMZ = YES;
                DEBUG_MINIMZ = NO;
            } else if (*print_level == 2) {
                PRINT_MINIMZ = YES;
                DEBUG_MINIMZ = YES;
            } else {
                fprintf(stderr, "\nERROR in xmin(): Print level out of range.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*natm_ext < 0) {
                fprintf(stderr, "\nERROR in xmin(): Number of atoms negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*natm_ext < 2) {
                fprintf(stderr, "\nERROR in xmin(): Too few atoms.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*ls_maxiter < 1) {
                fprintf(stderr, "\nERROR in xmin(): Requested number of LS steps negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*ls_maxatmov <= ZERO) {
                fprintf(stderr, "\nERROR in xmin(): Max LS move negative.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*beta_armijo <= 0 || *beta_armijo >= 1) {
                fprintf(stderr, "\nERROR in xmin(): Armijo_beta out of range.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*c_armijo <= 0 || *c_armijo >= 0.5) {
                fprintf(stderr, "\nERROR in xmin(): Armijo_c out of range.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*mu_armijo < 0 || *mu_armijo >= 2) {
                fprintf(stderr, "\nERROR in xmin(): Armijo_mu out of range.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
            }
            if (*xyz_min) { /* Molecular structure optimization */

                /*
                      Check for frozen atoms and create a local
                      environment with only moving atoms:
                */
                natm_local = (*natm_ext);
                for (i = 0; i < (*natm_ext); i++) {
                    if (grad_ext[i * 3] == 0 && grad_ext[i * 3 + 1] == 0 && grad_ext[i * 3 + 2] == 0) natm_local--;
                }
                if (natm_local < 2) {
                    fprintf(stderr, "\nERROR in xmin(): Too few moving atoms.\n");
                    fflush(stderr);
                    *label = PARAMS_ERROR;
                    return 0.;
                }
                ndim = 3 * natm_local;
                allocated = NO;
                error_flag = FALSE;
                if (!allocated) {
                    xyz_local = (double *)my_malloc(malloc, "\nERROR in xminC/my_malloc(double *xyz_local)", ndim,
                                                    sizeof(double), &error_flag);
                    if (error_flag) {
                        *label = error_flag;
                        goto error_cleanup;
                    }
                    grad_local = (double *)my_malloc(malloc, "\nERROR in xminC/my_malloc(double *grad_local)", ndim,
                                                     sizeof(double), &error_flag);
                    if (error_flag) {
                        *label = error_flag;
                        goto error_cleanup;
                    }
                    atm_indx = (int *)my_malloc(malloc, "\nERROR in xminC/my_malloc(int *atm_indx)", natm_local,
                                                sizeof(int), &error_flag);
                    if (error_flag) {
                        *label = error_flag;
                        goto error_cleanup;
                    }
                    allocated = YES;
                }
                for (i = j = 0; i < (*natm_ext); i++) { /* generate local -> ext mapping */
                    if (grad_ext[i * 3] != 0 || grad_ext[i * 3 + 1] != 0 || grad_ext[i * 3 + 2] != 0) atm_indx[j++] = i;
                }
                for (i = 0; i < natm_local; i++) { /* load xyz_local[] */
                    j = atm_indx[i];
                    xyz_local[3 * i] = xyz_ext[3 * j];
                    xyz_local[3 * i + 1] = xyz_ext[3 * j + 1];
                    xyz_local[3 * i + 2] = xyz_ext[3 * j + 2];
                }
            } else { /* General function optimization */

                ndim = (*natm_ext);
                allocated = NO;
                error_flag = FALSE;
                if (!allocated) {
                    xyz_local = (double *)my_malloc(malloc, "\nERROR in xminC/my_malloc(double *xyz_local)", ndim,
                                                    sizeof(double), &error_flag);
                    if (error_flag) {
                        *label = error_flag;
                        goto error_cleanup;
                    }
                    grad_local = (double *)my_malloc(malloc, "\nERROR in xminC/my_malloc(double *grad_local)", ndim,
                                                     sizeof(double), &error_flag);
                    if (error_flag) {
                        *label = error_flag;
                        goto error_cleanup;
                    }
                }
                for (i = 0; i < ndim; i++) {
                    xyz_local[i] = xyz_ext[i]; /* shouldn't be needed; ugly hack */
                }
            }
            status_flag = 0;
            goto L00;
        case 1:
            goto L01;
        default:
            fprintf(stderr, "\nERROR in xmin(): Illegal status.\n");
            fflush(stderr);
            *label = ILLEGAL_STATUS;
            return 0.;
    }

L00:

    for (status_flag = 0;;) {
    L01:

        /* Load grad_local[]: */
        if (xyz_min) {
            for (i = 0; i < natm_local; i++) {
                j = atm_indx[i];
                grad_local[3 * i] = grad_ext[3 * j];
                grad_local[3 * i + 1] = grad_ext[3 * j + 1];
                grad_local[3 * i + 2] = grad_ext[3 * j + 2];
            }
        } else { /* general optimization */
            for (i = 0; i < ndim; i++) {
                grad_local[i] = grad_ext[i]; /* shouldn't be needed; ugly hack */
            }
        }
        switch (*minim_method) {
            case PRCG:
                switch (*ls_method) {
                    case ARMIJO:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(prcg_minim, hessvec_forward, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(prcg_minim, hessvec_central, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    case WOLFE:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(prcg_minim, hessvec_forward, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(prcg_minim, hessvec_central, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    default:
                        fprintf(stderr, "\nERROR in xmin(): Unknown line search method.\n");
                        fflush(stderr);
                        *label = PARAMS_ERROR;
                        return 0.;
                }
                break;
            case LBFGS:
                switch (*ls_method) {
                    case ARMIJO:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(lbfgs_minim, hessvec_forward, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(lbfgs_minim, hessvec_central, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    case WOLFE:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(lbfgs_minim, hessvec_forward, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(lbfgs_minim, hessvec_central, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    default:
                        fprintf(stderr, "\nERROR in xmin(): Unknown line search method.\n");
                        fflush(stderr);
                        *label = PARAMS_ERROR;
                        return 0.;
                }
                break;
            case TNCG:
                switch (*ls_method) {
                    case ARMIJO:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(tncg_minim, hessvec_forward, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(tncg_minim, hessvec_central, ls_armijo, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    case WOLFE:
                        switch (*numdiff) {
                            case FORWARD_DIFF:
                                minim(tncg_minim, hessvec_forward, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            case CENTRAL_DIFF:
                                minim(tncg_minim, hessvec_central, ls_wolfe, ndim, *maxiter, *grms_tol, *m_lbfgs,
                                      xyz_local, enrg, grad_local, grms, iter, total_time, *ls_maxiter, ls_iter,
                                      *ls_maxatmov, *beta_armijo, *c_armijo, *mu_armijo, *ftol_wolfe, *gtol_wolfe,
                                      return_flag, &status_flag);
                                break;
                            default:
                                fprintf(stderr, "\nERROR in xmin(): Unknown finite difference method.\n");
                                fflush(stderr);
                                *label = PARAMS_ERROR;
                                return 0.;
                        }
                        break;
                    default:
                        fprintf(stderr, "\nERROR in xmin(): Unknown line search method.\n");
                        fflush(stderr);
                        *label = PARAMS_ERROR;
                        return 0.;
                }
                break;
            default:
                fprintf(stderr, "\nERROR in xmin(): Unknown minimization method.\n");
                fflush(stderr);
                *label = PARAMS_ERROR;
                return 0.;
        }
        if (status_flag > 0) {
            if (xyz_min) {
                for (i = 0; i < natm_local; i++) { /* load xyz_ext[] */
                    j = atm_indx[i];
                    xyz_ext[3 * j] = xyz_local[3 * i];
                    xyz_ext[3 * j + 1] = xyz_local[3 * i + 1];
                    xyz_ext[3 * j + 2] = xyz_local[3 * i + 2];
                }
            } else { /* general optimization */
                for (i = 0; i < ndim; i++) {
                    xyz_ext[i] = xyz_local[i]; /* shouldn't be needed; ugly hack */
                }
            }
            *label = 1;
            return 0.; /* xminC continue */
        } else if (status_flag < 0) {
            *label = status_flag;
            return 0.; /* xminC error    */
        } else {
            break; /* xminC done     */
        }
    } /* end xminC() */

    /* Load xyz_ext[]: */
    if (xyz_min) {
        for (i = 0; i < natm_local; i++) {
            j = atm_indx[i];
            xyz_ext[3 * j] = xyz_local[3 * i];
            xyz_ext[3 * j + 1] = xyz_local[3 * i + 1];
            xyz_ext[3 * j + 2] = xyz_local[3 * i + 2];
        }
    } else { /* general optimization */
        for (i = 0; i < ndim; i++) {
            xyz_ext[i] = xyz_local[i]; /* shouldn't be needed; ugly hack */
        }
    }

    if (allocated) {
        my_free(xyz_local);
        my_free(grad_local);
        my_free(atm_indx);
        allocated = NO;
    }

    *return_flag = DONE;
    *label = 0; /* xminC() done */
    return (*enrg);

error_cleanup:
    my_free(xyz_local);
    my_free(grad_local);
    my_free(atm_indx);
    return 0.;
}

#ifndef SQM
/*
    libXMIN reverse communication external minimization package.
    Written by Istvan Kolossvary.

    This file used to be xmin.nab, but it had to be translated to standard C
    to accommodate a function pointer to be passed in the first arg to xmin().

    Time stamp 8/16/2009.
*/

INT_T xmin_opt_init(struct xmin_opt *xo) {
    xo->mol_struct_opt = 1;
    xo->maxiter = 1000;
    xo->grms_tol = 0.05;
    xo->method = 3;
    xo->numdiff = 1;
    xo->m_lbfgs = 3;
    xo->ls_method = 2;
    xo->ls_maxiter = 20;
    xo->ls_maxatmov = 0.1;
    xo->beta_armijo = 0.5;
    xo->c_armijo = 0.4;
    xo->mu_armijo = 1.0;
    xo->ftol_wolfe = 0.0001;
    xo->gtol_wolfe = 0.9;
    xo->print_level = 0;

    return 0;
}

REAL_T xmin(REAL_T (*func)(REAL_T *, REAL_T *, INT_T *), INT_T *natm, REAL_T *xyz, REAL_T *grad, REAL_T *energy,
            REAL_T *grms, struct xmin_opt *xo)

//  This package can be used with any program that can calculate the energy
//  and the gradient for a particular [x, y, z] atomic configuration. There is
//  no size limit, but the xyz[] and grad[] arrays must be allocated by the
//  calling program.  Input params:  Number of atoms, xyz[] and grad[] arrays,
//  minimization method, max number of minimization steps, convergence
//  criterion (see below).  Output params: Number of minimization steps
//  completed, final energy and the RMS of the gradient, the CPU time spent in
//  the xmin() routine, and possibly an error message (see below).  On exit,
//  xmin() loads the minimized coordinates into xyz[] and grad[] will be
//  up-to-date, too.

{
    //  Params:
    //
    //  func        The name of the function that computes the function value and
    //              gradient of the objective function to be minimized.
    //  natm        Number of atoms for molecular structure optimization,
    //              but for general optimization natm is the number of dimensions.
    //              xo.mol_struct_opt should be set to zero for general optim. and
    //              func set accordingly.
    //  xyz         Allocated array of (x, y, z) atomic coordinates.
    //  grad        Allocated array of the gradient.
    //  energy      Energy value for structure optimization,
    //              function value otherwise.
    //  grms        RMS of the gradient.
    //  maxiter     Max number of minimization steps.
    //  ls_maxiter  Max number of line search steps per minimization step.
    //  grms_tol    Convergence criterion in terms of the RMS of the gradient.
    //  mol_struct_opt  Switch to select molecular structure optimization or
    //                  general optimization of a function pointed to by func.
    //  method      Minimization method: 1= PRCG, 2= LBFGS,
    //                                   3= LBFGS-preconditioned TNCG.
    //  ls_method   Line search method:  1= modified Armijo,
    //                                   2= Wolfe (J. J. More', D. J. Thuente).
    //  numdiff     Method used in finite difference Hv matrix-vector products:
    //              1= forward difference, 2= central difference.
    //  m_lbfgs     Depth of LBFGS memory for LBFGS minimization or TNCG
    //              preconditioning.
    //              Suggested value 3. m_lbfgs=0 with TNCG minimization turns off
    //              preconditioning.
    //  ls_maxatmov Max atomic coord movement allowed in line search, range > 0.
    //  beta_armijo Armijo beta param, range (0, 1).
    //  c_armijo    Armijo c param,    range (0, 0.5 ).
    //  mu_armijo   Armijo mu param,   range [0, 2).
    //  ftol_wolfe  Wolfe ftol param,  range (0, 0.5 ).
    //  gtol_wolfe  Wolfe gtol param,  range (ftol_wolfe, 1 ).
    //  print_level Verbosity: 0= none, 1= minim details,
    //                         2= minim and line search details plus CG details
    //                            in TNCG
    //  iter        Number of minimization steps completed.
    //  ls_iter     Number of line search steps completed.
    //  xmin_time   Time spent in the xmin() routine in CPU sec.
    //  error_flag  Error flag. xmin() will print a descriptive error message.

    INT_T return_flag, status_flag;
    INT_T NEG_TWO = -2, NEG_FOUR = -4;

    /*
        Code below is specific to mme() in terms of passing the int
        argument. As long as a general 'func' doesn't care about this
        int arg, the code is fine, 'func' just needs to accept a dummy
        int. However, if a 'func' wants to iterpret the int arg,
        a special branch needs to be added here.
    */

    nfunc = 0;
    for (status_flag = xo->error_flag = 0;;) {
        xminC(&(xo->mol_struct_opt), &(xo->method), &(xo->maxiter), &(xo->grms_tol), natm, &(xo->m_lbfgs),
              &(xo->numdiff), xyz, energy, grad, grms, &(xo->iter), &(xo->xmin_time), &(xo->print_level),
              &(xo->ls_method), &(xo->ls_maxiter), &(xo->ls_iter), &(xo->ls_maxatmov), &(xo->beta_armijo),
              &(xo->c_armijo), &(xo->mu_armijo), &(xo->ftol_wolfe), &(xo->gtol_wolfe), &return_flag, &status_flag);

        //      Finished minimization:
        if (return_flag == 0) {
            if (status_flag >= 0)
                xo->error_flag = 0;
            else
                xo->error_flag = status_flag;
        }

        //      Current value of 'iter' is passed to func"="mme() allowing
        //      control of NB list updates from calling program:

        else if (return_flag == 1 || return_flag == 2 || return_flag == 3) {
            if (status_flag >= 0) {
                *energy = func(xyz, grad, &xo->iter);
                nfunc++;
            } else
                xo->error_flag = status_flag;
        }

        //      Force NB list update by passing '-4' to func"="mme():

        else if (return_flag == 4 || return_flag == 5 || return_flag == 6) {
            if (status_flag >= 0) {
                *energy = func(xyz, grad, &NEG_FOUR);
                nfunc++;
            } else
                xo->error_flag = status_flag;
        }

        //      Prevent NB list update by passing '-2' to func"="mme():

        else if (return_flag == 7 || return_flag == 8 || return_flag == 9) {
            if (status_flag >= 0) {
                *energy = func(xyz, grad, &NEG_TWO);
                nfunc++;
            } else
                xo->error_flag = status_flag;
        } else {
            printf("\n XMIN ERROR: return_flag corrupted.\n");
            xo->error_flag = -100;
            return 0.0;
        }

        if (xo->error_flag || !return_flag) break;
    }

    if (xo->error_flag) {
        printf("\n XMIN ERROR: %d\n", status_flag);
        return 0.0;
    }

    return *energy;
}

}  // namespace tools

#endif /* !SQM */

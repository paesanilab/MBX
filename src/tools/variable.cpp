#include "variable.h"

void variable::grads(const double& gg, double* grd1, double* grd2, const double* p1, const double* p2) {
    for (size_t i = 0; i < 3; i++) {
        double d = gg * g[i];
        grd1[i] += d;
        grd2[i] -= d;
    }
}

double variable::v_gau(const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

    const double exp1 = std::exp(-k * r * r);
    const double gg = -2 * k * r * exp1 / r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_gau0(const double& r0, const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

    const double exp1 = std::exp(-k * (r0 - r) * (r0 - r));
    const double gg = 2 * k * (r0 - r) * exp1 / r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_exp0(const double& r0, const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

    const double exp1 = std::exp(k * (r0 - r));
    const double gg = -k * exp1 / r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_exp(const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

    const double exp1 = std::exp(k * (-r));
    const double gg = -k * exp1 / r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_coul0(const double& r0, const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double rsq = g[0] * g[0] + g[1] * g[1] + g[2] * g[2];
    const double r = std::sqrt(rsq);

    const double exp1 = std::exp(k * (r0 - r));
    const double rinv = 1.0 / r;
    const double val = exp1 * rinv;

    const double gg = -(k + rinv) * val * rinv;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return val;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_coul(const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double rsq = g[0] * g[0] + g[1] * g[1] + g[2] * g[2];
    const double r = std::sqrt(rsq);

    const double exp1 = std::exp(k * (-r));
    const double rinv = 1.0 / r;
    const double val = exp1 * rinv;

    const double gg = -(k + rinv) * val * rinv;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return val;
}

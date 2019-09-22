#ifndef VARIABLE_H
#define VARIABLE_H

#include <cmath>

struct variable {
    double v_exp0(const double& r0, const double& k,
                 const double * p1, const double * p2 );
                 
    double v_exp(const double& k,
                 const double * p1, const double * p2 );

    double v_coul0(const double& r0, const double& k,
                  const double * p1, const double * p2 );
                  
    double v_coul(const double& k,
                  const double * p1, const double * p2 );

    double v_gau0(const double& r0, const double& k,
                 const double * p1, const double * p2 );

    double g[3]; // diff(value, p1 - p2)
};

#endif // VARIABLE_H

#ifndef X3B_V2X_H
#define X3B_V2X_H

////////////////////////////////////////////////////////////////////////////////

namespace x2o {

//----------------------------------------------------------------------------//

struct x3b_v2x {

    static double eval(const double* w1,
                       const double* w2,
                       const double* w3);

    static double eval(const double* w1,
                       const double* w2,
                       const double* w3,
                       double* g1,
                       double* g2,
                       double* g3);

    static const double r3i;
    static const double r3f;

    static const double kOH_intra;
    static const double kHH_intra;

    static const double kOO;
    static const double kOH;
    static const double kHH;

    static const double dOH_intra;
    static const double dHH_intra;

    static const double dOO;
    static const double dOH;
    static const double dHH;

private:
    static double f_switch(const double& r, double& g);
};

//----------------------------------------------------------------------------//

} // namespace x2o

////////////////////////////////////////////////////////////////////////////////

#endif // X3B_V2X_H

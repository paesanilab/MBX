#ifndef X2B_V9X_H
#define X2B_V9X_H

////////////////////////////////////////////////////////////////////////////////

namespace x2o {

//----------------------------------------------------------------------------//

//
//  x2b_v9<4> with gradients and no fitting interface
//

//----------------------------------------------------------------------------//

struct x2b_v9x {
    static double eval(const double* w1, const double* w2);

    static double eval(const double* w1, const double* w2,
                             double* g1,       double* g2);

    static const double k_HH_intra;
    static const double k_OH_intra;

    static const double k_HH_coul;
    static const double k_OH_coul;
    static const double k_OO_coul;

    static const double k_XH_main;
    static const double k_XO_main;
    static const double k_XX_main;

    static const double in_plane_gamma;
    static const double out_of_plane_gamma;

    static const double r2i;
    static const double r2f;

    static double f_switch(const double&, double&); // O-O separation
};

//----------------------------------------------------------------------------//

} // namespace x2o

////////////////////////////////////////////////////////////////////////////////

#endif // X2B_V9X_H

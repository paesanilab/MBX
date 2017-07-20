#include "poly_1b_A1B2_deg4.h"

namespace A1B2 {

double poly_1b_v1x::eval(const double a[21], const double x[3])
{
    const double t12 = a[0];
    const double t3 = x[2];
    const double t20 = (a[3]+(t3*a[14]+a[8])*t3)*t3;
    const double t21 = a[5];
    const double t26 = (a[18]*t3+a[7])*t3;
    const double t27 = a[13];
    const double t30 = t3*a[17];
    const double t31 = a[6];
    const double t16 = x[1];
    const double t45 = a[15]*t16;
    const double t47 = a[12]*t3;
    const double t48 = a[10];
    const double t59 = x[0];
    return((a[1]+(a[4]+(t3*a[20]+a[11])*t3)*t3)*t3+(t12+t20+(t21+t26+(t16*t27+t30+t31)*t16)*t16)*t16+(t12+t20+(a[2]+(t3*a[16]+a[9])*t3+(t45+t47+t48)*t16)*t16
+(t21+t26+(t16*a[19]+t47+t48)*t16+(t27*t59+t30+t31+t45)*t59)*t59)*t59);

}

} // namespace A1B2

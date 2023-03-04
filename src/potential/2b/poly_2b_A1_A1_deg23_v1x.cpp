#include "poly_2b_A1_A1_deg23_v1x.h"

namespace x2b_A1_A1_deg23 {

double poly_2b_A1_A1_deg23_v1x::eval(const double a[23], const double x[1],
                        double g[1])
{
    const double t1 = a[0];
    const double t2 = a[1];
    const double t3 = a[2];
    const double t4 = a[3];
    const double t5 = a[4];
    const double t6 = a[5];
    const double t7 = a[6];
    const double t8 = a[7];
    const double t9 = a[8];
    const double t10 = a[9];
    const double t11 = a[10];
    const double t12 = a[11];
    const double t13 = a[12];
    const double t14 = a[13];
    const double t15 = a[14];
    const double t16 = a[15];
    const double t17 = a[16];
    const double t18 = a[17];
    const double t19 = a[18];
    const double t20 = a[19];
    const double t21 = a[20];
    const double t25 = x[0];
    const double t23 = a[22]*t25;
    const double t24 = a[21];
    const double t26 = (t23+t24)*t25;
    const double t28 = (t21+t26)*t25;
    const double t30 = (t20+t28)*t25;
    const double t32 = (t19+t30)*t25;
    const double t34 = (t18+t32)*t25;
    const double t36 = (t17+t34)*t25;
    const double t38 = (t16+t36)*t25;
    const double t40 = (t15+t38)*t25;
    const double t42 = (t14+t40)*t25;
    const double t44 = (t13+t42)*t25;
    const double t46 = (t12+t44)*t25;
    const double t48 = (t11+t46)*t25;
    const double t50 = (t10+t48)*t25;
    const double t52 = (t9+t50)*t25;
    const double t54 = (t8+t52)*t25;
    const double t56 = (t7+t54)*t25;
    const double t58 = (t6+t56)*t25;
    const double t60 = (t5+t58)*t25;
    const double t62 = (t4+t60)*t25;
    const double t64 = (t3+t62)*t25;
    const double t66 = (t2+t64)*t25;
    g[0] = (((((((((((((((((((((2.0*t23+t24)*t25+t21+t26)*t25+t20+t28)*
t25+t19+t30)*t25+t18+t32)*t25+t17+t34)*t25+t16+t36)*t25+t15+t38)*t25+t14+t40)*
t25+t13+t42)*t25+t12+t44)*t25+t11+t46)*t25+t10+t48)*t25+t9+t50)*t25+t8+t52)*t25
+t7+t54)*t25+t6+t56)*t25+t5+t58)*t25+t4+t60)*t25+t3+t62)*t25+t2+t64)*t25+t1+t66
;
    return (t1+t66)*t25;

}

} // namespace mb_system

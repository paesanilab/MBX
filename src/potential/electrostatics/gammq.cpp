#include <cmath>
#include <cassert>

#include <iostream>

#include <limits>
#include <algorithm>  // for max

#include "gammq.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

const double EPS = std::numeric_limits<double>::epsilon();
const double FPMIN = std::numeric_limits<double>::min() / EPS;

const int ngau = 18;

const double y[18] = {0.0021695375159141994, 0.011413521097787704, 0.027972308950302116, 0.051727015600492421,
                      0.082502225484340941,  0.12007019910960293,  0.16415283300752470,  0.21442376986779355,
                      0.27051082840644336,   0.33199876341447887,  0.39843234186401943,  0.46931971407375483,
                      0.54413605556657973,   0.62232745288031077,  0.70331500465597174,  0.78649910768313447,
                      0.87126389619061517,   0.95698180152629142};

const double w[18] = {0.0055657196642445571, 0.012915947284065419, 0.020181515297735382, 0.027298621498568734,
                      0.034213810770299537,  0.040875750923643261, 0.047235083490265582, 0.053244713977759692,
                      0.058860144245324798,  0.064039797355015485, 0.068745323835736408, 0.072941885005653087,
                      0.076598410645870640,  0.079687828912071670, 0.082187266704339706, 0.084078218979661945,
                      0.085346685739338721,  0.085983275670394821};

////////////////////////////////////////////////////////////////////////////////

double gammpapprox(const double& a, const double& x, int psig) {
    const double gln = elec::gammln(a);

    const double a1 = a - 1.0;
    const double lna1 = std::log(a1);
    const double sqrta1 = std::sqrt(a1);

    double xu, t, sum, ans;

    if (x > a1)
        xu = std::max(a1 + 11.5 * sqrta1, x + 6.0 * sqrta1);
    else
        xu = std::max(0.0, std::min(a1 - 7.5 * sqrta1, x - 5.0 * sqrta1));

    sum = 0.0;
    for (int j = 0; j < ngau; ++j) {
        t = x + (xu - x) * y[j];
        sum += w[j] * std::exp(-(t - a1) + a1 * (std::log(t) - lna1));
    }

    ans = sum * (xu - x) * std::exp(a1 * (lna1 - 1.0) - gln);

    return (psig ? (ans > 0.0 ? 1.0 - ans : -ans) : (ans >= 0.0 ? ans : 1.0 + ans));
}

////////////////////////////////////////////////////////////////////////////////

double gser(const double& a, const double& x) {
    const double gln = elec::gammln(a);

    double ap = a;
    double sum = 1.0 / a;
    double del = sum;

    for (;;) {
        ++ap;
        del *= x / ap;
        sum += del;
        if (std::fabs(del) < std::fabs(sum) * EPS) {
            return sum * std::exp(-x + a * std::log(x) - gln);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

double gcf(const double& a, const double& x) {
    const double gln = elec::gammln(a);

    double b = x + 1.0 - a;
    double c = 1.0 / FPMIN;
    double d = 1.0 / b;
    double h = d;

    for (int i = 1;; ++i) {
        const double an = -i * (i - a);

        b += 2.0;
        d = an * d + b;
        if (std::fabs(d) < FPMIN) d = FPMIN;

        c = b + an / c;

        if (std::fabs(c) < FPMIN) c = FPMIN;

        d = 1.0 / d;
        const double del = d * c;
        h *= del;

        if (std::fabs(del - 1.0) <= EPS) break;
    }

    return std::exp(-x + a * std::log(x) - gln) * h;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace elec {

////////////////////////////////////////////////////////////////////////////////

double gammq(const double& a, const double& x) {
    const int ASWITCH = 100;

    if (!(x >= 0.0 && a > 0.0)) {
        std::cerr << "gammq: x = " << x << ", a = " << a << std::endl;
    }

    assert(x >= 0.0 && a > 0.0);

    if (x == 0.0)
        return 1.0;
    else if (int(a) >= ASWITCH)
        return gammpapprox(a, x, 0);
    else if (x < a + 1.0)
        return 1.0 - gser(a, x);
    else
        return gcf(a, x);
}

////////////////////////////////////////////////////////////////////////////////

double gammln(const double& xx) {
    static const double cof[14] = {57.1562356658629235,     -59.5979603554754912,    14.1360979747417471,
                                   -0.491913816097620199,   .339946499848118887e-4,  .465236289270485756e-4,
                                   -.983744753048795646e-4, .158088703224912494e-3,  -.210264441724104883e-3,
                                   .217439618115212643e-3,  -.164318106536763890e-3, .844182239838527433e-4,
                                   -.261908384015814087e-4, .368991826595316234e-5};

    assert(xx > 0.0);

    double x = xx;
    double y = xx;

    double tmp = x + 5.24218750000000000;
    tmp = (x + 0.5) * std::log(tmp) - tmp;

    double ser = 0.999999999999997092;
    for (int j = 0; j < 14; ++j) ser += cof[j] / ++y;

    return tmp + std::log(2.5066282746310005 * ser / x);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace elec

////////////////////////////////////////////////////////////////////////////////

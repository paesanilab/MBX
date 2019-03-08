/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "gammq.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

const double EPS = std::numeric_limits<double>::epsilon();
const double SMALL = std::numeric_limits<double>::min() / EPS;
const double BIG = std::numeric_limits<double>::max() * EPS;

////////////////////////////////////////////////////////////////////////////////

double GammaPse(const double& a, const double& x) {
    // power series expansion
    // See https://en.wikipedia.org/wiki/Incomplete_gamma_function#Evaluation_formulae
    // Look at lower case gamma(s,z)

    // We can say that:
    // a*(a+1)*)a+2)*...*(a+k) = gamma(a+k+1)/gamma(a)

    // We also know that gamma(a+1) = a*gamma(a)

    const double gammaln = elec::gammln(a);

    // Start with k=0
    double ak = a;
    double sum = 1.0 / a;
    double delta = sum;

    // Loop until convergence
    while (true) {
        // Increase ak
        ak++;
        // Update delta from previous by multiplying by x/ak
        delta *= x / ak;
        // Add the new term to the sum
        sum += delta;

        // Check convergence
        if (std::fabs(delta) < std::fabs(sum) * EPS) {
            break;
        }
    }

    // Return P(a,x)
    return sum * std::exp(a * std::log(x) - x - gammaln);
}

////////////////////////////////////////////////////////////////////////////////

double GammaCf(const double a, const double x) {
    // Modified Lentz's algorithm for continued fractions evaluation
    // See Thompson, I.J., and Barnett, A.R. 1986, Journal of Computational Physics, vol. 64, pp. 490–509.
    // Concretely, the appendix.
    // See https://en.wikipedia.org/wiki/Incomplete_gamma_function#Evaluation_formulae

    const double gammaln = elec::gammln(a);

    // We will compute lim(n->inf) hn
    // h will be:
    // h = b0 + a1/(b1 + a2/(b2 + a3/...))

    // Set b0
    double b = x + 1.0 - a;
    // Set h0
    double h = b;
    if (std::fabs(b) < SMALL) h = SMALL;
    // Set d0
    double d = 0.0;
    // Set c0
    double c = h;

    int i = 0;
    while (true) {
        i++;

        // Get an and bn
        const double an = i * (a - i);
        b += 2.0;

        // get dn
        d = an * d + b;
        if (std::fabs(d) < SMALL) d = SMALL;

        // Get cn
        c = b + an / c;
        if (std::fabs(c) < SMALL) c = SMALL;

        // Update d
        d = 1.0 / d;

        // Get Delta_n
        const double deltan = d * c;

        // Get hn
        h *= deltan;

        if (std::fabs(deltan - 1.0) <= EPS) break;
    }

    // Now we need to return gamma(a,x)/gamma(a)
    return std::exp(-x + a * std::log(x) - gammaln) / h;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace elec {

////////////////////////////////////////////////////////////////////////////////

double gammq(const double a, const double x) {
    if (!(x >= 0.0 && a > 0.0)) {
        std::cerr << "gammq: x = " << x << ", a = " << a << std::endl;
    }

    assert(x >= 0.0 && a > 0.0);

    if (x < SMALL)
        return 1.0;
    else if (x > BIG)
        return 0.0;
    else if (x < a + 1.0)
        return 1.0 - GammaPse(a, x);
    else
        return GammaCf(a, x);
}

////////////////////////////////////////////////////////////////////////////////

double gammln(const double x) {
    // Coefficients obtained with Paul Godfrey method
    // https://mrob.com/pub/ries/lanczos-gamma.html

    // Using g = 4.7421875, n = 15
    static const double cof[14] = {57.1562356658629235,     -59.5979603554754912,    14.1360979747417471,
                                   -0.491913816097620199,   .339946499848118887e-4,  .465236289270485756e-4,
                                   -.983744753048795646e-4, .158088703224912494e-3,  -.210264441724104883e-3,
                                   .217439618115212643e-3,  -.164318106536763890e-3, .844182239838527433e-4,
                                   -.261908384015814087e-4, .368991826595316234e-5};

    assert(x > 0.0);

    // Lanczos approximation
    // https://en.wikipedia.org/wiki/Lanczos_approximation
    // We will compute ln(gamma(x))

    // Compute x + g + 0.5
    double prefactor = x + 5.24218750000000000;

    // Compute exponent 1 (x + 0.5)
    double exp1 = x + 0.5;

    // Compute exponent 2 (x + g + 0.5)
    double exp2 = prefactor;

    // Compute sqrt(2*PI)
    double sqrt2pi = 2.5066282746310005;

    // Compute Ag
    double Ag = 0.999999999999997092;
    for (int i = 0; i < 14; i++) {
        double den = x + double(i + 1);
        Ag += cof[i] / den;
    }

    // Return ln(gamma(x)).
    // Using relation ln(gamma(x) = ln(gamma(x + 1)) - ln(x)
    // Which means: ln(gamma(x) = ln(gamma(x+1)/x)
    return std::log(sqrt2pi * Ag / x) - exp2 + std::log(prefactor) * exp1;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace elec

////////////////////////////////////////////////////////////////////////////////

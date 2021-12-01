#include <cmath>
#include <cassert>

#include "macros.h"
#include "sphere.h"

////////////////////////////////////////////////////////////////////////////////

namespace kit {

//----------------------------------------------------------------------------//

sphere::sphere() : m_R(10.0), m_U(100.0) {}

//----------------------------------------------------------------------------//

sphere::sphere(const double& cR, const double& cU) : m_R(cR), m_U(cU) {
    assert(cR > 0.0);
    assert(cU >= 0.0);
}

//----------------------------------------------------------------------------//

void sphere::setup(const double& nR, const double& nU) {
    assert(nR > 0.0);
    assert(nU >= 0.0);

    m_R = nR;
    m_U = nU;
}

//----------------------------------------------------------------------------//

double sphere::operator()(size_t natom, const double* RESTRICT mass, const double* RESTRICT x,
                          double* RESTRICT g) const {
    assert(natom > 0);
    assert(mass != 0 && x != 0);

    // center of mass

    double total_mass(0);
    double cm[3] = {0.0, 0.0, 0.0};

    for (size_t n = 0; n < natom; ++n) {
        const size_t n3 = 3 * n;
        for (size_t k = 0; k < 3; ++k) cm[k] += mass[n] * x[n3 + k];

        total_mass += mass[n];
    }

    for (size_t k = 0; k < 3; ++k) cm[k] /= total_mass;

    // the thing

    double E(0), xx[3] = {0.0, 0.0, 0.0};

    for (size_t n = 0; n < natom; ++n) {
        const size_t n3 = 3 * n;

        double Rsq(0), Rij[3];
        for (size_t k = 0; k < 3; ++k) {
            Rij[k] = x[n3 + k] - cm[k];
            Rsq += Rij[k] * Rij[k];
        }

        const double d = std::sqrt(Rsq);
        const double delta = d - m_R;

        if (delta > 0.0) {
            E += m_U * delta * delta;

            if (g != 0) {
                const double tmp1 = m_U * delta / d;

                for (size_t k = 0; k < 3; ++k) {
                    const double tmp2 = tmp1 * Rij[k];
                    g[n3 + k] += tmp2;
                    xx[k] += tmp2;
                }
            }
        }
    }

    if (g != 0) {
        for (size_t n = 0; n < natom; ++n) {
            const size_t n3 = 3 * n;
            for (size_t k = 0; k < 3; ++k) g[n3 + k] -= (mass[n] / total_mass) * xx[k];
        }
    }

    return E / 2;
}

//----------------------------------------------------------------------------//

}  // namespace kit

////////////////////////////////////////////////////////////////////////////////

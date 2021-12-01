#ifndef SPHERE_H
#define SPHERE_H

#include <cstddef>

namespace kit {

//
// (U/2)*[f(|r1 - Rc| - R0) + ... + f(|rN - Rc| - R0)],
//   f(x < 0) = 0, f(x > 0) = x^2
//

struct sphere {
    sphere();
    sphere(const double& cR, const double& cU);

    // returns energy; adds to gradients
    double operator()(size_t natom, const double* mass, const double* x, double* g = 0) const;

    const double& R() const { return m_R; }
    const double& U() const { return m_U; }

    void setup(const double& nR, const double& nU);

   private:
    double m_R;  // A
    double m_U;  // kcal mol
};

}  // namespace kit

#endif  // SPHERE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include <cmath>
#include <cassert>
#include <algorithm>

#include "necklace.h"

#define M_N2C(i, j) m_transform[m_nbead * (i) + (j)]

////////////////////////////////////////////////////////////////////////////////

namespace kit {

//----------------------------------------------------------------------------//

necklace::necklace() : m_natom(0), m_nbead(0) {}

//----------------------------------------------------------------------------//

necklace::~necklace() { setup(0, 0); }

//----------------------------------------------------------------------------//

void necklace::setup(size_t natoms, size_t nbeads) {
    if (natoms == m_natom && nbeads == m_nbead) return;

    if (m_nbead > 0 || m_natom > 0) {
        assert(m_nbead > 0 && m_natom > 0);

#ifdef ENABLE_FFTW3

        fftw_destroy_plan(m_plan_cart2nmode);
        fftw_destroy_plan(m_plan_nmode2cart);

        fftw_free(m_pos_cart);
        fftw_free(m_pos_nmode);

        fftw_free(m_vel_nmode);

        fftw_free(m_frc_cart);
        fftw_free(m_frc_nmode);

#else /* no FFTW3 */

        delete[] m_transform;
        delete[] m_pos_cart;
        delete[] m_pos_nmode;
        delete[] m_vel_nmode;
        delete[] m_frc_cart;
        delete[] m_frc_nmode;

#endif /* ENABLE_FFTW3 */

        delete[] m_lambda;
    }

    m_natom = natoms;
    m_nbead = nbeads;

    const size_t n_extra_doubles = 2;  // 1 for Epot_sum and 1 for Econ

    if (natoms > 0 || nbeads > 0) {
        assert(natoms > 0 && nbeads > 0);
        assert(nbeads == 1 || nbeads % 2 == 0);

        m_lambda = new double[nbeads];

#ifdef ENABLE_FFTW3

        const size_t nbytes = 3 * sizeof(double) * nbeads * natoms;
        const size_t nbytes_frc = nbytes + sizeof(double) * n_extra_doubles;

        m_pos_cart = (double*)fftw_malloc(nbytes);
        m_pos_nmode = (double*)fftw_malloc(nbytes);

        m_vel_nmode = (double*)fftw_malloc(nbytes);

        m_frc_cart = (double*)fftw_malloc(nbytes_frc);
        m_frc_nmode = (double*)fftw_malloc(nbytes_frc);

        {
            const int n = (int)nbeads;
            const int howmany = (int)3 * natoms;

            const fftw_r2r_kind c2n_kind = FFTW_R2HC;
            const fftw_r2r_kind n2c_kind = FFTW_HC2R;

            m_plan_cart2nmode = fftw_plan_many_r2r(1, &n, howmany, m_pos_cart, 0, howmany, 1, m_pos_nmode, 0, howmany,
                                                   1, &c2n_kind, FFTW_MEASURE | FFTW_PRESERVE_INPUT);

            m_plan_nmode2cart = fftw_plan_many_r2r(1, &n, howmany, m_pos_nmode, 0, howmany, 1, m_pos_cart, 0, howmany,
                                                   1, &n2c_kind, FFTW_MEASURE | FFTW_PRESERVE_INPUT);
        }

        // nmodes are "scrambled" by fftw
        // (comparing to Francesco's codes [below])

        m_lambda[0] = 0.0;
        m_lambda[nbeads / 2] = 4.0 * nbeads;

        for (size_t n = 1; n < nbeads / 2; ++n)
            m_lambda[nbeads - n] = m_lambda[n] = 4 * nbeads * (1.0 - std::cos(2 * n * (M_PI / nbeads)));

#else /* no FFTW3 */

        m_transform = new double[nbeads * nbeads];

        m_pos_cart = new double[3 * natoms * nbeads];
        m_pos_nmode = new double[3 * natoms * nbeads];
        m_vel_nmode = new double[3 * natoms * nbeads];
        m_frc_cart = new double[3 * natoms * nbeads + n_extra_doubles];
        m_frc_nmode = new double[3 * natoms * nbeads + n_extra_doubles];

        // initialize the transformation matrix

        for (size_t i = 0; i < nbeads; ++i) M_N2C(i, 0) = 1.0;

        for (size_t i = 0; i < nbeads / 2; ++i) {
            M_N2C(2 * i, nbeads - 1) = +1.0;
            M_N2C(2 * i + 1, nbeads - 1) = -1.0;
        }

        for (size_t i = 0; i < nbeads; ++i) {
            const double phase = 2 * i * (M_PI / nbeads);
            for (size_t j = 1; j < nbeads / 2; ++j) {
                M_N2C(i, 2 * j - 1) = std::sqrt(2.0) * std::cos(phase * j);
                M_N2C(i, 2 * j) = -std::sqrt(2.0) * std::sin(phase * j);
            }
        }

        // eigenvalues of the nmode -> cart transform

        m_lambda[0] = 0.0;  // path centroid
        m_lambda[nbeads - 1] = 4.0 * nbeads;

        for (size_t n = 1; n < nbeads / 2; ++n)
            m_lambda[2 * n - 1] = m_lambda[2 * n] = 2 * nbeads * (1.0 - std::cos(2 * n * (M_PI / nbeads)));

#endif /* ENABLE_FFTW3 */

    }  // if (natoms > 0 || nbeads > 0)
}

//----------------------------------------------------------------------------//

void necklace::pos_c2n() {
    assert(m_nbead > 0 && m_natom > 0);
    assert(m_nbead % 2 == 0 || m_nbead == 1);

#ifdef ENABLE_FFTW3

    fftw_execute_r2r(m_plan_cart2nmode, m_pos_cart, m_pos_nmode);

    const double factor = 1.0 / m_nbead;
    for (size_t n = 0; n < 3 * m_nbead * m_natom; ++n) m_pos_nmode[n] *= factor;

#else /* no FFTW3 */

    const size_t natom3 = 3 * m_natom;
    std::fill(m_pos_nmode, m_pos_nmode + natom3 * m_nbead, 0.0);

    for (size_t i = 0; i < m_nbead; ++i) {
        const size_t i0 = i * natom3;
        for (size_t j = 0; j < m_nbead; ++j) {
            const size_t j0 = j * natom3;
            const double factor = M_N2C(j, i) / m_nbead;
            for (size_t k = 0; k < natom3; ++k) m_pos_nmode[i0 + k] += factor * m_pos_cart[j0 + k];
        }
    }

#endif /* ENABLE_FFTW3 */
}

//----------------------------------------------------------------------------//

void necklace::pos_n2c() {
    assert(m_nbead > 0 && m_natom > 0);
    assert(m_nbead % 2 == 0 || m_nbead == 1);

#ifdef ENABLE_FFTW3

    fftw_execute_r2r(m_plan_nmode2cart, m_pos_nmode, m_pos_cart);

#else  /* no FFTW3 */

    const size_t natom3 = 3 * m_natom;
    std::fill(m_pos_cart, m_pos_cart + natom3 * m_nbead, 0.0);

    for (size_t i = 0; i < m_nbead; ++i) {
        const size_t i0 = i * natom3;
        for (size_t j = 0; j < m_nbead; ++j) {
            const size_t j0 = j * natom3;
            const double factor = M_N2C(i, j);
            for (size_t k = 0; k < natom3; ++k) m_pos_cart[i0 + k] += factor * m_pos_nmode[j0 + k];
        }
    }
#endif /* ENABLE_FFTW3 */
}

//----------------------------------------------------------------------------//

void necklace::frc_c2n() {
    assert(m_nbead > 0 && m_natom > 0);
    assert(m_nbead % 2 == 0 || m_nbead == 1);

#ifdef ENABLE_FFTW3

    fftw_execute_r2r(m_plan_cart2nmode, m_frc_cart, m_frc_nmode);

    double factor = 1.0 / m_nbead;

    const size_t natom3 = 3 * m_natom;
    for (size_t i = 0; i < natom3; ++i) {
        m_frc_nmode[i] *= factor;
        m_frc_nmode[i + natom3 * (m_nbead / 2)] *= factor;
    }

    factor *= 2;

    for (size_t b = 1; b < m_nbead / 2; ++b)
        for (size_t i = 0; i < natom3; ++i) {
            m_frc_nmode[i + b * natom3] *= factor;
            m_frc_nmode[i + (m_nbead - b) * natom3] *= factor;
        }

#else /* no FFTW3 */

    const size_t natom3 = 3 * m_natom;
    std::fill(m_frc_nmode, m_frc_nmode + natom3 * m_nbead, 0.0);

    for (size_t i = 0; i < m_nbead; ++i) {
        const size_t i0 = i * natom3;
        for (size_t j = 0; j < m_nbead; ++j) {
            const size_t j0 = j * natom3;
            // !!! 1/M factor that is different from Francesco's codes
            const double factor = M_N2C(j, i) / m_nbead;
            for (size_t k = 0; k < natom3; ++k) m_frc_nmode[i0 + k] += factor * m_frc_cart[j0 + k];
        }
    }

#endif /* ENABLE_FFTW3 */
}

//----------------------------------------------------------------------------//

}  // namespace kit

////////////////////////////////////////////////////////////////////////////////

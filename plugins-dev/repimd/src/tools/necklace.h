#ifndef NECKLACE_H
#define NECKLACE_H

#include <cassert>
#include <cstddef>

#ifdef ENABLE_FFTW3
#include <fftw3.h>
#endif /* ENABLE_FFTW3 */

namespace kit {

//
// necklace for PIMD/CMD; manages memory for cartesian/normal mode
// positions, velocities, forces and handles the transforms
//

struct necklace {
    necklace();
    ~necklace();

    inline size_t natom() const;
    inline size_t nbead() const;

    inline const double& lambda(size_t) const;

   private:
    necklace(const necklace&);
    necklace& operator=(const necklace&);

    size_t m_natom;
    size_t m_nbead;

#ifdef ENABLE_FFTW3

    fftw_plan m_plan_cart2nmode;
    fftw_plan m_plan_nmode2cart;

#else /* no FFTW3 */

    double* m_transform;

#endif /* ENABLE_FFTW3 */

    double* m_lambda;

   protected:
    void setup(size_t natoms, size_t nbeads);

    void pos_c2n();
    void pos_n2c();

    void frc_c2n();

    // layout is bead1, bead2, ..., beadN, where
    // each bead consists of ndofs elements

    double* m_pos_cart;
    double* m_pos_nmode;

    double* m_vel_nmode;

    double* m_frc_cart;
    double* m_frc_nmode;
};

inline size_t necklace::natom() const { return m_natom; }

inline size_t necklace::nbead() const { return m_nbead; }

inline const double& necklace::lambda(size_t b) const {
    assert(b < nbead());

    return m_lambda[b];
}

}  // namespace kit

#endif  // NECKLACE_H

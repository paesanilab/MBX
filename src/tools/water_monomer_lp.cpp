#include "water_monomer_lp.h"

void monomer::setup(const double* ohh, const double& in_plane_g, const double& out_of_plane_g, double* x1, double* x2) {
    for (int i = 0; i < 3; ++i) {
        oh1[i] = ohh[i + 3] - ohh[i];
        oh2[i] = ohh[i + 6] - ohh[i];
    }

    const double v[3] = {oh1[1] * oh2[2] - oh1[2] * oh2[1], oh1[2] * oh2[0] - oh1[0] * oh2[2],
                         oh1[0] * oh2[1] - oh1[1] * oh2[0]};

    for (int i = 0; i < 3; ++i) {
        const double in_plane = ohh[i] + 0.5 * in_plane_g * (oh1[i] + oh2[i]);
        const double out_of_plane = out_of_plane_g * v[i];

        x1[i] = in_plane + out_of_plane;
        x2[i] = in_plane - out_of_plane;
    }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

void monomer::grads(const double* g1, const double* g2, const double& in_plane_g, const double& out_of_plane_g,
                    double* grd) const {
    const double gm[3] = {g1[0] - g2[0], g1[1] - g2[1], g1[2] - g2[2]};

    const double t1[3] = {oh2[1] * gm[2] - oh2[2] * gm[1], oh2[2] * gm[0] - oh2[0] * gm[2],
                          oh2[0] * gm[1] - oh2[1] * gm[0]};

    const double t2[3] = {oh1[1] * gm[2] - oh1[2] * gm[1], oh1[2] * gm[0] - oh1[0] * gm[2],
                          oh1[0] * gm[1] - oh1[1] * gm[0]};

    for (int i = 0; i < 3; ++i) {
        const double gsum = g1[i] + g2[i];
        const double in_plane = 0.5 * in_plane_g * gsum;

        const double gh1 = in_plane + out_of_plane_g * t1[i];
        const double gh2 = in_plane - out_of_plane_g * t2[i];

        grd[i + 0] += gsum - (gh1 + gh2);  // O
        grd[i + 3] += gh1;                 // H1
        grd[i + 6] += gh2;                 // H2
    }
}

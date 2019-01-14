#include "potential/dispersion/disptools.h"

namespace disp {

double tang_toennies(int n, const double& x) {
    assert(n >= 0);
    int nn = n;
    double sum = 1.0 + x / nn;

    while (--nn != 0) sum = 1.0 + sum * x / nn;

    double tt = 1.0 - sum * std::exp(-x);

    if (std::fabs(tt) < 1.0e-8) {
        double term(1);
        for (nn = n; nn != 0; --nn) term *= x / nn;

        sum = 0.0;
        for (nn = n + 1; nn < 1000; ++nn) {
            term *= x / nn;
            sum += term;

            if (std::fabs(term / sum) < 1.0e-8) break;
        }

        tt = sum * std::exp(-x);
    }

    return tt;
}

//----------------------------------------------------------------------------//

/* Block commented since C8 is not used for now.

double disp68(const double& C6, const double& d6,
               const double& C8, const double& d8,
               const double* p1, const double* p2,
                     double* g1,       double* g2, bool do_grads) {

  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = disp::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = disp::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  const double grd = (6*e6 + 8*e8)*inv_rsq
      - (C6*std::pow(d6, 7)*if6*std::exp(-d6r)
      +  C8*std::pow(d8, 9)*if8*std::exp(-d8r))/r;

  g1[0] += dx*grd;
  g2[0] -= dx*grd;

  g1[1] += dy*grd;
  g2[1] -= dy*grd;

  g1[2] += dz*grd;
  g2[2] -= dz*grd;

  return - (e6 + e8);
}

//----------------------------------------------------------------------------//

double disp68(const double& C6, const double& d6,
              const double& C8, const double& d8,
              const double* p1, const double* p2) {

  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = disp::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = disp::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  return - (e6 + e8);
}

******************************************************************************/

//----------------------------------------------------------------------------//

double disp6(const double C6, const double d6,
             const double* p1, const double* xyz2,
             double* grad1, double* grad2, 
             const size_t nmon1, const size_t nmon2, const size_t start2, const size_t end2,
             const size_t atom_index1, const size_t atom_index2, const double disp_scale_factor, bool do_grads,
             const double cutoff, const std::vector<double>& box) {

    double disp = 0.0;

    size_t nmon22 = nmon2*2;
    
    size_t shift2 = atom_index2 * nmon2 * 3;

    bool use_cutoff = box.size();
    double g1[3], g2[3 * nmon2];
    std::fill(g1, g1 + 3, 0.0);
    std::fill(g2, g2 + 3 * nmon2, 0.0);
    //    #pragma simd
    for (size_t nv = start2; nv < end2; nv++) {
        const double dx = p1[0] - xyz2[shift2 + nv];
        const double dy = p1[1] - xyz2[nmon2 + shift2 + nv];
        const double dz = p1[2] - xyz2[nmon22 + shift2 + nv];

        const double rsq = dx * dx + dy * dy + dz * dz;
        const double r = std::sqrt(rsq);

        // If using cutoff, check for distances and get proper dispersion
        if (!use_cutoff || r <= cutoff) {
            const double d6r = d6 * r;
            const double tt6 = disp::tang_toennies(6, d6r);

            const double inv_rsq = 1.0 / rsq;
            const double inv_r6 = inv_rsq * inv_rsq * inv_rsq;

            const double e6 = C6 * tt6 * inv_r6;

            disp -= e6;

            if (do_grads) {
                const double grd = 6 * e6 * inv_rsq - C6 * std::pow(d6, 7) * if6 * std::exp(-d6r) / r;

                g1[0] += dx * grd;
                g2[nv] -= dx * grd;

                g1[1] += dy * grd;
                g2[nmon2 + nv] -= dy * grd;

                g1[2] += dz * grd;
                g2[nmon22 + nv] -= dz * grd;
            }
        }
    }

    if (do_grads) {
        grad1[0] += g1[0];
        grad1[1] += g1[1];
        grad1[2] += g1[2];
        for (size_t i = start2; i < end2; i++) {
            grad2[shift2 + i] += g2[i];
            grad2[shift2 + nmon2 + i] += g2[nmon2 + i];
            grad2[shift2 + nmon22 + i] += g2[nmon22 + i];
        }
    }

    return disp * disp_scale_factor;
}

void GetC6(std::string mon_id1, std::string mon_id2, size_t index1, size_t index2, double &out_C6, double &out_d6) {

    std::vector<double> C6,d6;
    std::vector<size_t> types1, types2;
    size_t nt2;

    if (mon_id1 == "h2o" && mon_id2 == "h2o") {
        // Define the type of atom in each mon
        types1.push_back(0);
        types1.push_back(1);
        types1.push_back(1);

        types2.push_back(0);
        types2.push_back(1);
        types2.push_back(1);

        // Set the number of different types
        nt2 = 2;

        // Fill in (in order) the C6 and d6 coefficients
        C6.push_back(2.373212214147944e+02);  // kcal/mol * A^(-6) O -- O
        C6.push_back(8.349556669872743e+01);  // kcal/mol * A^(-6) O -- H
        C6.push_back(8.349556669872743e+01);  // kcal/mol * A^(-6) H -- O
        C6.push_back(2.009358600184719e+01);  // kcal/mol * A^(-6) H -- H

        d6.push_back(9.295485815062264e+00);  // A^(-1)
        d6.push_back(9.775202425217957e+00);  // A^(-1)
        d6.push_back(9.775202425217957e+00);  // A^(-1)
        d6.push_back(9.406475169954112e+00);  // A^(-1)

        size_t i = types1[index1];
        size_t j = types2[index2];

        out_C6 = C6[i*nt2 + j];
        out_d6 = d6[i*nt2 + j];
    }

}








}  // namespace disp

//----------------------------------------------------------------------------//

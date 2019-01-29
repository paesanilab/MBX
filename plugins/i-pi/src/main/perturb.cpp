#include "perturb.h"

namespace noneq {

    Perturbation::Perturbation() {}
    Perturbation::~Perturbation() {}

    void Perturbation::Initialize(std::vector<double> xyz, std::vector<double> chg, std::vector<double> alpha, std::vector<double> polfac) {
        xyz_ = xyz;
        chg_ = chg;
        alpha_ = alpha;
        polfac_ = polfac;

        nsites_ = chg.size();
        
    }

    void Perturbation::GetTij_a_ab() {
        // Loop over all pairs
        double r, r2;
        double s1, s2, s3;
        double A, a_thole;
        double one_over_six = 1.0/6.0;
        std::vector<double> dr(3,0.0);
        for (size_t i = 0; i < nsites_ - 1; i++) {
            size_t i3 = 3*i;
            size_t i9 = 3*i3;
            size_t i3n = nsites_ * i3;
            size_t i9n = nsites_ * i9;
            for (size_t j = i+1; j < nsites_; j++) {
                size_t j3 = 3*j;
                size_t j9 = 3*j3;
                size_t j3n = nsites_ * j3;
                size_t j9n = nsites_ * j9;
                // Obtain proper a
                a_thole = 0.055;
                // Obtain A
                A = std::pow(polfac_[i] * polfac_[j], one_over_six);

                dr[0] = xyz_[i3] - xyz_[j3];
                dr[1] = xyz_[i3 + 1] - xyz_[j3 + 1];
                dr[2] = xyz_[i3 + 2] - xyz_[j3 + 2];
                r2 = dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2];
                r = sqrt(r2);

                GetS1S2S3(r, A, a_thole, s1, s2, s3);

                for (size_t a = 0; a < 3; a++) {
                    tij_a_[i3n + j3 + a] = -s1 * dr[a] / (r2*r);
                    tij_a_[j3n + i3 + a] = -tij_a_[i3n + j3 + a];
                    for (size_t b = 0; b < 3; b++) {
                        tij_ab_[i9n + j9 + 3*a + b] = s2 * dr[a]*dr[b]/(r2*r2*r) - s1*Delta(a,b)/(r2*r);
                        tij_ab_[j9n + i9 + 3*a + b] = tij_ab_[i9n + j9 + 3*a + b];
                    }
                }
            }
        }
    }

    void Perturbation::GetTij_ab_abc() {
        // Loop over all pairs
        double r, r2;
        double s1, s2, s3;
        double A, a_thole;
        double one_over_six = 1.0/6.0;
        std::vector<double> dr(3,0.0);
        for (size_t i = 0; i < nsites_ - 1; i++) {
            size_t i3 = 3*i;
            size_t i9 = 3*i3;
            size_t i27 = 3*i9;
            size_t i9n = nsites_ * i9;
            size_t i27n = nsites_ * i27;
            for (size_t j = i+1; j < nsites_; j++) {
                size_t j3 = 3*j;
                size_t j9 = 3*j3;
                size_t j27 = 9*j3;
                size_t j9n = nsites_ * j9;
                size_t j27n = nsites_ * j27;
                // Obtain proper a
                a_thole = 0.055;
                // Obtain A
                A = std::pow(polfac_[i] * polfac_[j], one_over_six);

                dr[0] = xyz_[i3] - xyz_[j3];
                dr[1] = xyz_[i3 + 1] - xyz_[j3 + 1];
                dr[2] = xyz_[i3 + 2] - xyz_[j3 + 2];
                r2 = dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2];
                r = sqrt(r2);
                double r4 = r2*r2;

                GetS1S2S3(r, A, a_thole, s1, s2, s3);

                for (size_t a = 0; a < 3; a++) {
                    for (size_t b = 0; b < 3; b++) {
                        tij_ab_[i9n + j9 + 3*a + b] = s2 * dr[a]*dr[b]/(r2*r2*r) - s1*Delta(a,b)/(r2*r);
                        tij_ab_[j9n + i9 + 3*a + b] = tij_ab_[i9n + j9 + 3*a + b];
                        for (size_t c = 0; c < 3; c++) {
                            tij_abc[i27n + j27+9*a + 3*b + c] = -s3*15.0 / (r4*r2*r) *dr[a]*dr[b]*dr[c] + s2*3/(r4*r)*(dr[a]*Delta(b,c) + dr[b]*Delta(a,c) + dr[c]*Delta(a,b));
                            tij_abc[j27n + i27+9*a + 3*b + c] = -tij_abc[i27n + j27+9*a + 3*b + c];
                        }
                    }
                }
            }
        }
    }

    void GetS1S2S3(double r, double A, double a, double &s1, double &s2, double &s3) {
        if (A > 1E-50) {
            double rA = r/A;
            double rA2 = rA*rA;
            double rA4 = rA2*rA2;
            double arA4 = a * rA4
            double exparA4 = std::exp(-arA4);
            s1 = 1.0 - exparA4;
            s2 = s1 - 4.0/3.0*arA4*exparA4;
            s3 = s2 -4.0/15.0*arA4*exparA4*(4*arA4 - 1.0);
        } else {
            s1 = 1.0;
            s2 = 1.0;
            s3 = 1.0;
        }
    }

    size_t Delta(size_t i, size_t j) {
        if (i == j) return 1;
        else return 0; 
    }

} // namespace noneq

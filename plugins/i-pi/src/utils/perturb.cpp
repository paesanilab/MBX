#include "perturb.h"

namespace noneq {

    Perturbation::Perturbation() {}
    Perturbation::~Perturbation() {}

    void Perturbation::Initialize(std::vector<double> xyz, std::vector<double> chg, std::vector<double> alpha, std::vector<double> polfac, std::vector<double> box) {

        // Setting some predefined values
        tol_ = 1E-16;
        maxit_ = 1000;

        xyz_ = xyz;
        chg_ = chg;
        alpha_ = alpha;
        polfac_ = polfac;

        nsites_ = chg.size();
        
        tij_a_ = std::vector<double>(nsites_ * nsites_ * 3,0.0);
        tij_ab_ = std::vector<double>(nsites_ * nsites_ * 9,0.0);
        tij_abc_ = std::vector<double>(nsites_ * nsites_ * 27,0.0);

        pi_all_ = std::vector<double>(nsites_ * 9,0.0);

        pi_ = std::vector<double>(9,0.0);
        dk_pi_all_ = std::vector<double>(nsites_ * nsites_ * 27,0.0);
        dk_pi_ = std::vector<double>(nsites_ * 27,0.0);

        dk_mu_ind_all_ = std::vector<double>(3*nsites_*3*nsites_,0.0);
        dk_mu_perm_all_ = std::vector<double>(3*nsites_*3*nsites_,0.0);
        dk_mu_ind_ = std::vector<double>(9*nsites_,0.0);
        dk_mu_perm_ = std::vector<double>(9*nsites_,0.0);
        dk_mu_ = std::vector<double>(9*nsites_,0.0);
        
        box_ = box;
    }

    void Perturbation::SetDipoles(std::vector<double> mu_perm, std::vector<double> mu_ind) {
        mu_ind_all_ = mu_ind;
        mu_perm_all_ = mu_perm;
    }

    void Perturbation::SetChargeDerivative(std::vector<double> chg_der) {
        dchg_ = chg_der;
    }

    std::vector<double> Perturbation::GetPerturbationPolarizability(double E1, double E2, size_t a, size_t b, double direction) {
        std::vector<double> force_perturb(3*nsites_,0.0);
        
        // Obtain tensors, polarizability, and its derivative
        GetTij_ab_abc();
        size_t num_it_pol_all = CalculatePolarizabilityAll();
        CalculateDkPi();

        double voltA2voltm = 1E+10; // Volts per angstrom to volt per meter
        double angstromcube2faradaymetersq = 1.11265E-40; 
        double kcalmol2joule = 6.9477E-21;
        
        double unit_conv = voltA2voltm*voltA2voltm*angstromcube2faradaymetersq/kcalmol2joule;
        double field_perturb = 0.5*E1*E2*unit_conv*direction;
        for (size_t k = 0; k < nsites_; k++) {
            for (size_t u = 0; u < 3; u++) {
                force_perturb[3*k + u] = field_perturb*dk_pi_[27*k + 9*u + 3*a +b];
            } 
        }

        // Return force perturbation in kcal/mol / Angstrom
        return force_perturb;
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
                if (i%4 == 0 and (j == i+1 or j== i+2)) a_thole = 0.626;
                else a_thole = 0.055;
                // Obtain A
                A = std::pow(polfac_[i] * polfac_[j], one_over_six);

                dr[0] = xyz_[i3] - xyz_[j3];
                dr[1] = xyz_[i3 + 1] - xyz_[j3 + 1];
                dr[2] = xyz_[i3 + 2] - xyz_[j3 + 2];
                
                if (box_.size()) DoPBC(dr,box_);

                r2 = dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2];
                r = sqrt(r2);

                GetS1S2S3(r, A, a_thole, s1, s2, s3);

                for (size_t a = 0; a < 3; a++) {
                    tij_a_[i3n + j3 + a] = -s1 * dr[a] / (r2*r);
                    tij_a_[j3n + i3 + a] = -tij_a_[i3n + j3 + a];
                    for (size_t b = 0; b < 3; b++) {
                        tij_ab_[i9n + j9 + 3*a + b] = s2 * 3.0* dr[a]*dr[b]/(r2*r2*r) - s1*Delta(a,b)/(r2*r);
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
                // Obtain proper a TODO
                //a_thole = 0.055;
                if (i%4 == 0 and (j == i+1 or j== i+2)) a_thole = 0.626;
                else a_thole = 0.055;
                // Obtain A
                A = std::pow(polfac_[i] * polfac_[j], one_over_six);

                dr[0] = xyz_[i3] - xyz_[j3];
                dr[1] = xyz_[i3 + 1] - xyz_[j3 + 1];
                dr[2] = xyz_[i3 + 2] - xyz_[j3 + 2];

                if (box_.size()) DoPBC(dr,box_);

                r2 = dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2];
                r = sqrt(r2);

                double r4 = r2*r2;

                GetS1S2S3(r, A, a_thole, s1, s2, s3);

                for (size_t a = 0; a < 3; a++) {
                    for (size_t b = 0; b < 3; b++) {
                        tij_ab_[i9n + j9 + 3*a + b] = s2 * 3.0 * dr[a]*dr[b]/(r2*r2*r) - s1*Delta(a,b)/(r2*r);
                        tij_ab_[j9n + i9 + 3*a + b] = tij_ab_[i9n + j9 + 3*a + b];
                        for (size_t c = 0; c < 3; c++) {
                            tij_abc_[i27n + j27+9*a + 3*b + c] = -s3*15.0 / (r4*r2*r) *dr[a]*dr[b]*dr[c] + s2*3/(r4*r)*(dr[a]*Delta(b,c) + dr[b]*Delta(a,c) + dr[c]*Delta(a,b));
                            tij_abc_[j27n + i27+9*a + 3*b + c] = -tij_abc_[i27n + j27+9*a + 3*b + c];
                        }
                    }
                }
            }
        }
    }

    void Perturbation::GetElectrostaticTensors(std::vector<double> &Tij_a, std::vector<double> &Tij_ab, std::vector<double> &Tij_abc) {
        GetTij_a_ab();
        GetTij_ab_abc();
        
        Tij_a = tij_a_;
        Tij_ab = tij_ab_;
        Tij_abc = tij_abc_;
    }

    void Perturbation::GetPolarizabilityAll(std::vector<double> &pi_all) {
        CalculatePolarizabilityAll();
        pi_all = pi_all_;
    }

    void Perturbation::GetDkPi(std::vector<double> &dk_pi) {
        CalculateDkPi();
        dk_pi = dk_pi_;
    }

    void Perturbation::GetDkMu(std::vector<double> &dk_mu) {
        GetTij_a_ab();
        GetTij_ab_abc();
        CalculateDkMu();
        for (size_t m = 0; m < dk_mu_ind_.size(); m++) 
            dk_mu_[m] = dk_mu_ind_[m] + dk_mu_perm_[m];

        dk_mu = dk_mu_;
    }

    void Perturbation::GetDkMuInd(std::vector<double> &dk_mu) {
        dk_mu = dk_mu_ind_;
    }

    size_t Perturbation::CalculatePolarizabilityAll() {
        // Initialize with isotropic polarizability * identity
        std::fill(pi_all_.begin(), pi_all_.end(), 0.0);
        for (size_t i = 0; i < nsites_; i++) {
            double alpha_i = alpha_[i];
            size_t i9 = i*9;
            for (size_t a = 0; a < 3; a++) {
                pi_all_[i9 + 3*a + a] = alpha_i;
            }
        }

        std::vector<double> pi_all_old;
        double learn_param = 0.8;
        // Iterate til convergence
        size_t it = 0;
        while (true) {
            // Save previous iteration
            pi_all_old = pi_all_;
 
            // Update pi_all_
            std::fill(pi_all_.begin(), pi_all_.end(), 0.0);
            for (size_t i = 0; i < nsites_; i++) {
                double alpha_i = alpha_[i];
                size_t i9 = i*9;
                size_t i9n = nsites_ * i9;
                // Isotropic polarizability
                for (size_t a = 0; a < 3; a++) {
                    pi_all_[i9 + 3*a + a] += alpha_i;
                }
 
                // Add the sum
                for (size_t j = 0; j < nsites_; j++) { 
                    size_t j9 = 9 * j;
                    if (i == j) continue;
                    for(size_t a = 0; a < 3; a++) {
                        size_t a3 = 3*a;
                        for(size_t b=0; b<3;b++) {
                            for(size_t m = 0; m < 3; m++) {
                                pi_all_[i9 + a3 + b] += alpha_i*tij_ab_[i9n + j9 + a3 + m] * pi_all_old[j9 + 3*m +b] ;
                            }
                        }
                    }
                }
            }

            for (size_t i = 0; i < pi_all_.size(); i++) 
                pi_all_[i] = pi_all_[i]*learn_param + (1-learn_param) *pi_all_old[i];

            // Check difference
            double err2 = 0.0;
            for (size_t i = 0; i < pi_all_.size(); i++) {
                double err = pi_all_[i] - pi_all_old[i];
                err2 += err*err;
            }

            // Break if convergence
            if (sqrt(err2) / nsites_ < tol_) break;
//            std::cout << "Iteration: " << it << " " << sqrt(err2) << std::endl;
            
            // Error if maxit have been reached
            if (it > maxit_) {
                std::cerr << "Polarizability reached max number of iterations\n";
                break;
            }
            
            it++;
                                
        }       

        return it;
            
    }

    void Perturbation::CalculatePolarizability() {
        std::fill(pi_.begin(),pi_.end(),0.0);

        for (size_t i = 0; i < nsites_; i++) 
            for (size_t m = 0; m < 9; m++) 
                pi_[m] += pi_all_[9*i + m];
    }

    void Perturbation::GetPolarizability(std::vector<double> &pi) {
        GetTij_ab_abc();
        CalculatePolarizabilityAll();
        CalculatePolarizability();
        pi = pi_;
    }

    size_t Perturbation::CalculateDkPiAll(size_t k) {
        size_t size = nsites_ * 27;
        size_t shift = k * size;

        std::vector<double> dk_new(size,0.0);
        std::vector<double> dk_old(size,0.0);

        // Zero the derivatives
        //std::fill(dk_pi_all_.begin() + shift, dk_pi_all_.begin() + shift + size, 0.0);
        std::vector<double> pi_alpha_all(9,0.0);
        
        size_t it = 0;
        //std::cout << "pi_alpha_all\n";
        //for (auto i = pi_alpha_all.begin(); i < pi_alpha_all.end(); i++) std::cout << *i << std::endl;
        while (true) {
        
            dk_old = dk_new;
            std::fill(dk_new.begin(),dk_new.end(),0.0);

            for (size_t i = 0; i < nsites_; i++) {
                for (size_t j = 0; j < nsites_; j++) {
                    if (i == j) continue;
                    for (size_t m = 0; m < 3; m++)
                        pi_alpha_all[3*m + m] = alpha_[j];
                    for (size_t u = 0; u < 3; u++) 
                        for (size_t a = 0; a < 3; a++) 
                            for (size_t b = 0; b < 3; b++) 
                                for (size_t c = 0; c < 3; c++) {
                                    dk_new[27*i + 9*u + 3*a + b] += 
                                    (tij_abc_[27*nsites_*i + 27*j + 9*u +3*a + c] 
                                    * (Delta(k,i) - Delta(k,j)) 
                                    //* pi_all_[9*j + 3*c + b] 
                                    * pi_alpha_all[3*c + b]  //* pi_all_[9*j + 3*c + b] 
                                    + tij_ab_[9*nsites_*i + 9*j + 3*a + c]
                                    * dk_old[27*j + 9*u + 3*c + b]);
                                }
                }

                for (size_t m = 0; m < 27; m++) 
                    dk_new[27*i + m] *= alpha_[i];
            }

            //}                                 
            // Check difference
            double err2 = 0.0;
            for (size_t i = 0; i < dk_new.size(); i++) {
                double err = dk_new[i] - dk_old[i];
                err2 += err*err;
            }

            // Break if convergence
            if (sqrt(err2) < tol_) break;
            //std::cout << "Iteration: " << it << " " << sqrt(err2) << std::endl;

            // Error if maxit have been reached
            if (it > maxit_) {
                std::cerr << "Dk polarizability reached max number of iterations\n";
                break;
            }

            it++;

        }

        std::copy(dk_new.begin(), dk_new.end(),dk_pi_all_.begin() + shift);
        
        return it;
    }


    void Perturbation::CalculateDkPi() {

        // Initialize d_pi to 0
        std::fill(dk_pi_.begin(), dk_pi_.end(), 0.0);

        for (size_t k = 0; k < nsites_ ; k++) {
            CalculateDkPiAll(k);
        }

        for (size_t i = 0; i < nsites_; i++) 
            for (size_t k = 0; k < nsites_ ; k++) 
                for (size_t m = 0; m < 27; m++) 
                    dk_pi_[27*k + m] += dk_pi_all_[27*nsites_*k + 27*i + m];
    }

    void Perturbation::CalculateDkMuAll(size_t k) {
        size_t size = nsites_ * 9;  // 3*N*3
        size_t shift = k * size;
        std::vector<double> dk_new(size,0.0);
        std::vector<double> dk_old(size,0.0);

        size_t it = 0;
        while (true) {

            dk_old = dk_new;
            std::fill(dk_new.begin(),dk_new.end(),0.0);
            for (size_t i = 0; i < nsites_; i++) {
                for (size_t j = 0; j < nsites_; j++) {
                    if (i == j) continue;
                    if (!IsIJBonded(i,j)) {
                        for (size_t u = 0; u < 3; u++) {
                            for (size_t a = 0; a < 3; a++) {
                                dk_new[9*i + 3*u + a] += tij_ab_[9*nsites_*i + 9*j + 3*u + a]
                                                      * (Delta(k,i) - Delta(k,j))
                                                      * chg_[j];
                                dk_new[9*i + 3*u + a] += tij_a_[3*nsites_*i +3*j +a] 
                                                      * dchg_[3*nsites_*j + 3*k + u];
                            }
                        }
                    }

                    for (size_t u = 0; u < 3; u++) {
                        for (size_t a = 0; a < 3; a++) {
                            for (size_t b = 0; b < 3; b++) {
                                dk_new[9*i + 3*u + a] += 
                                         tij_abc_[27*nsites_*i + 27*j + 9*u +3*a + b] 
                                       * (Delta(k,i) - Delta(k,j))
                                       * mu_ind_all_[3*j + b];
                                dk_new[9*i + 3*u + a] += tij_ab_[9*nsites_*i + 9*j + 3*a + b]
                                       * dk_old[9*j + 3*u * b];
                            }
                        }
                    }
                }
                for (size_t m = 0; m < 9; m++)
                    dk_new[9*i + m] *= alpha_[i];
            }           

            // Check difference
            double err2 = 0.0;
            for (size_t i = 0; i < dk_new.size(); i++) {
                double err = dk_new[i] - dk_old[i];
                err2 += err*err;
            }

            // Break if convergence
            if (sqrt(err2) < tol_) break;
            //std::cout << "Iteration: " << it << " " << sqrt(err2) << std::endl;

            // Error if maxit have been reached
            if (it > maxit_) {
                std::cerr << "Dk dipole reached max number of iterations\n";
                break;
            }

            it++;

        }

        std::copy(dk_new.begin(), dk_new.end(),dk_mu_ind_all_.begin() + shift);

        // Now calculate permanent dipole derivative
        std::fill(dk_new.begin(),dk_new.end(),0.0);
        for (size_t i = 0; i < nsites_; i++) {
            for (size_t u = 0; u < 3; u++) {
                for (size_t a = 0; a < 3; a++) {
                    dk_new[9*i + 3*u + a] = dchg_[3*nsites_*i + 3*k + u]
                                          * xyz_[3*i + a]
                                          + chg_[i] * Delta(i,k) * Delta(a,u);
                }
            }
        } 

        for (size_t m = 0; m < dk_new.size(); m++) 
            dk_mu_perm_all_[shift + m] = dk_new[m];

    }

    void Perturbation::CalculateDkMu() {

        // Initialize dk_mu_ to 0
        std::fill(dk_mu_ind_.begin(), dk_mu_ind_.end(), 0.0);
        std::fill(dk_mu_perm_.begin(), dk_mu_perm_.end(), 0.0);

        for (size_t k = 0; k < nsites_ ; k++) {
            CalculateDkMuAll(k);
        }

        for (size_t i = 0; i < nsites_; i++) {
            for (size_t k = 0; k < nsites_ ; k++) {
                for (size_t m = 0; m < 9; m++) {
                    dk_mu_ind_[9*k + m] += dk_mu_ind_all_[9*nsites_*k + 9*i + m];
                    dk_mu_perm_[9*k + m] += dk_mu_perm_all_[9*nsites_*k + 9*i + m];
                }
            }
        }
    }

    void GetS1S2S3(double r, double A, double a, double &s1, double &s2, double &s3) {
        if (A > 1E-50) {
            double rA = r/A;
            double rA2 = rA*rA;
            double rA4 = rA2*rA2;
            double arA4 = a * rA4;
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

    int Delta(int i, int j) {
        if (i == j) return 1;
        else return 0; 
    }

    void DoPBC(std::vector<double> &r, const std::vector<double> &box) {
        for (size_t i = 0; i < 3; i++) {
            while (r[i] < -box[3*i + i]/2.0) r[i] += box[3*i + i];
            while (r[i] > box[3*i + i]/2.0) r[i] -= box[3*i + i];
        }
    }

    bool IsIJBonded(size_t i, size_t j) {
        return i/4 == j/4;
    }

} // namespace noneq

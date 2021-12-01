#include <iostream>
#include <vector>
#include <cmath>


namespace noneq {
class Perturbation {
  public:
    Perturbation();
    ~Perturbation();
    
    void Initialize(std::vector<double> xyz, std::vector<double> chg, std::vector<double> alpha, std::vector<double> polfac, std::vector<double> box = {});

    void SetDipoles(std::vector<double> mu_perm, std::vector<double> mu_ind);
    void SetChargeDerivative(std::vector<double> chg_der);

    // E1,E2 magnitude of the electric field
    // a,b are the axiz (x(0), y(1) or z(2)) of E1 and E2 respectively
    // Direction is +-1.0 for non eq perturbation
    std::vector<double> GetPerturbationPolarizability(double E1, double E2, size_t a, size_t b, double direction);
    std::vector<double> GetPerturbationDipole(double E1, double direction);

    void GetElectrostaticTensors(std::vector<double> &Tij_a, std::vector<double> &Tij_ab, std::vector<double> &Tij_abc);

    void GetPolarizabilityAll(std::vector<double> &pi_all);

    void GetPolarizability(std::vector<double> &pi);

    void GetDkPi(std::vector<double> &dk_pi);

    void GetDkMu(std::vector<double> &dk_mu);
    void GetDkMuInd(std::vector<double> &dk_mu);

  private:
    void GetTij_a_ab();
    void GetTij_ab_abc();
    size_t CalculatePolarizabilityAll();
    void CalculatePolarizability();
    size_t CalculateDkPiAll(size_t k);
    void CalculateDkPi();
    void CalculateDkMuAll(size_t k);
    void CalculateDkMu();

  private:
    // number of sites
    size_t nsites_;

    // tolerance for convergence. Value per site.
    double tol_;
    // Max number of iterations
    size_t maxit_;

    // coordinates, 3N
    std::vector<double> xyz_;
    // charges, N
    std::vector<double> chg_;
    // isotropic polarizability, N
    std::vector<double> alpha_;
    // scaling factor for screening, N
    // Paesani lab typically uses polfac = alpha, but we are not recstricted to that.
    std::vector<double> polfac_;
    
    // Order 1 tensor, N*N*3
    std::vector<double> tij_a_;
    // Order 2 tensor, N*N*3*3
    std::vector<double> tij_ab_;
    // Order 3 tensor, N*N*3*3*3
    std::vector<double> tij_abc_;
    
    // Effective polarizability, N*3*3
    std::vector<double> pi_all_;

    // Many Body polarizability 3*3
    std::vector<double> pi_;

    // Derivative of pi_all, N*N*3*3*3
    std::vector<double> dk_pi_all_;

    // Derivative of pi_, N*3*3*3
    std::vector<double> dk_pi_;

    // Box of the system
    std::vector<double> box_;

    // Charge derivatives N*3N 
    std::vector<double> dchg_;

    // Induced dipoles 3*N
    std::vector<double> mu_ind_all_;

    // Permanent dipoles 3*N
    std::vector<double> mu_perm_all_;

    // Derivative of mu_ind_all_  3*N*3*N
    std::vector<double> dk_mu_ind_all_;

    // Derivative of mu_perm_all_  3*N*3*N
    std::vector<double> dk_mu_perm_all_;

    // Derivative of mu_ind_  3*N*3
    std::vector<double> dk_mu_ind_;

    // Derivative of mu_perm_  3*N*3
    std::vector<double> dk_mu_perm_;

    // Derivative of mu_  3*N*3
    std::vector<double> dk_mu_;
};

// Get screening functions s1, s2, and s3.
// r is the distance between sites i and j
// A = (polfac_i*polfac_j)^(1/6)
// a is the thole damping
// For MB-pol, 0.626 1-2 distances, 0.055 for everything else
void GetS1S2S3(double r, double A, double a, double &s1, double &s2, double &s3);

int Delta(int i, int j); 
    
// Assuming orthorombic box.
void DoPBC(std::vector<double> &r, const std::vector<double> &box);

// Hardcoded for water. Need to generalize.
// Returns if i and j are bonded
bool IsIJBonded(size_t i, size_t j);

} // namespace noneq

#include <iostream>


namespace noneq {
class Perturbation {
  public:
    Perturbation();
    ~Perturbation();
    
    void Initialize(std::vector<double> xyz, std::vector<double> chg, std::vector<double> alpha, std::vector<double> polfac);

    std::vector<double> GetPerturbationPolarizability(double E1, double E2, double direction);
    std::vector<double> GetPerturbationDipole(double E1, double direction);

  private:
    void GetTij_a_ab();
    void GetTij_ab_abc();

  private:
    // number of sites
    size_t nsites_;

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
    std::vector<double> tij_a;
    // Order 2 tensor, N*N*3*3
    std::vector<double> tij_ab;
    // Order 3 tensor, N*N*3*3*3
    std::vector<double> tij_abc;
    
    // Effective polarizability, N*3*3
    std::vector<double> pi_all;


};

// Get screening functions s1, s2, and s3.
// r is the distance between sites i and j
// A = (polfac_i*polfac_j)^(1/6)
// a is the thole damping
// For MB-pol, 0.626 1-2 distances, 0.055 for everything else
void GetS1S2S3(double r, double A, double a, double &s1, double &s2, double &s3);

double Delta(size_t i, size_t j); 
    
} // namespace noneq

#ifndef CU_INCLUDE_BBLOCK_DIMER_H   
#define CU_INCLUDE_BBLOCK_DIMER_H   

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

#include "molecule.h"
#include "tang-toennies.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: Dimer
////////////////////////////////////////////////////////////////////////////////

// Dimer Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
// This class is a dimer, with its own potential. All the possible dimers
// will be in the system inside a vector.
class Dimer : public Molecule {
 public:
  Dimer();
  Dimer(std::shared_ptr<Monomer> mon1, std::shared_ptr<Monomer> mon2);
  ~Dimer();
  // Getters
  double * GetRealXyz();
  // Virtual Functions
  virtual double GetDispersion() = 0;
  virtual double GetShortRange() = 0;
  virtual double GetDispersion(double * grd) = 0;
  virtual double GetShortRange(double * grd) = 0;
 private:
  size_t n_mon;                         // Number of Monomers
  size_t n_sites;                       // Number of sites in the dimer
  std::vector<size_t> mon_n_sites;      // Vector with mon site num
  std::vector<size_t> mon_n_real_sites; // Vector with mon real site num
  std::shared_ptr<double> xyz;
  std::shared_ptr<double> real_xyz;

  // Dispersion
  inline double x6(const double& C6, const double& d6,
                  const double& C8, const double& d8,
                  const double* p1, const double* p2);
  inline double x6(const double& C6, const double& d6,
                  const double& C8, const double& d8,
                  const double* p1, const double* p2,
                        double* g1,       double* g2);
  // Buckingham
  inline double buck(const double A, const double  B,
      const double* p1, const double* p2);
  inline double buck(const double A, const double B,
      const double* p1, const double* p2, double* g1, double* g2);
  
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_DIMER_H

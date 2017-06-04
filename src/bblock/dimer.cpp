#include "dimer.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Dimer

////////////////////////////////////////////////////////////////////////////////

Dimer::Dimer() {
  n_mon=0;
}

Dimer::Dimer(std::shared_ptr<Monomer> mon1, 
             std::shared_ptr<Monomer> mon2) {  
  n_mon = 2;
  n_sites = mon1->GetNumRealSites() + mon2->GetNumRealSites();
  monomers.clear();
  monomers.push_back(mon1);
  monomers.push_back(mon2);
}

Dimer::~Dimer() {
}

double * Dimer::GetRealXyzSys() {
  std::copy(monomers[0]->GetFirstIndex(),
            syscoords + 3 * monomers[0]->GetFirstIndex()
            + 3 * monomers[0]->GetNumRealSites(),
            real_xyz.get()) 
  std::copy(syscoords + 3 * monomers[1]->GetFirstIndex(),
            syscoords + 3 * monomers[1]->GetFirstIndex() 
            + 3 * monomers[1]->GetNumRealSites(),
            real_xyz.get() + 3 * monomers[0]->GetFirstIndex())
  return real_xyz.get();
}

inline double Dimer::x6(const double& C6, const double& d6,
                const double& C8, const double& d8,
                const double* p1, const double* p2,
                      double* g1,       double* g2)
{
  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = x2o::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = x2o::tang_toennies(8, d8r);


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

inline double Dimer::x6(const double& C6, const double& d6,
                const double& C8, const double& d8,
                const double* p1, const double* p2)
{
  const double dx = p1[0] - p2[0];
  const double dy = p1[1] - p2[1];
  const double dz = p1[2] - p2[2];

  const double rsq = dx*dx + dy*dy + dz*dz;
  const double r = std::sqrt(rsq);

  const double d6r = d6*r;
  const double tt6 = x2o::tang_toennies(6, d6r);

  const double d8r = d8*r;
  const double tt8 = x2o::tang_toennies(8, d8r);


  const double inv_rsq = 1.0/rsq;
  const double inv_r6 = inv_rsq*inv_rsq*inv_rsq;
  const double inv_r8 = inv_r6*inv_rsq;

  const double e6 = C6*tt6*inv_r6;
  const double e8 = C8*tt8*inv_r8;

  return - (e6 + e8);
}
  
double buck(const double A, const double  B,
      const double* p1, const double* p2)
{
    const double r = tools::distance(p1, p2);
    return A*exp(-B*r);
}

double buck(const double A, const double B,
      const double* p1, const double* p2, double* g1, double* g2)
{
    const double r = tools::distance(p1, p2);

    const double fac = A*exp(-B*r);
    //const double fac = a/b*exp(-b*r);

    for(size_t i = 0; i < 3; ++i){
  g1[i] -= (p1[i] - p2[i])*B/r*fac;
  g2[i] += (p1[i] - p2[i])*B/r*fac;
    }

    return fac;
}


////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Dimer

////////////////////////////////////////////////////////////////////////////////

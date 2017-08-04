#ifndef CU_INCLUDE_BBLOCK_DIM_CO2_CO2_H   
#define CU_INCLUDE_BBLOCK_DIM_CO2_CO2_H   

#include "dimer.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: CO2_CO2
////////////////////////////////////////////////////////////////////////////////

// CO2_CO2 Class
// @author: Marc Riera
// @email:  mrierari@ucsd.edu
class CO2_CO2 : public Dimer {
 public:
  CO2_CO2();
  ~CO2_CO2();
  // Getters
  double GetDispersion();
  double GetShortRange();
  double GetDispersion(double * grd);
  double GetShortRange(double * grd);

 private:
  x2b_A1B2_A1B2::x2b_A1B2_A1B2_v1x pot;

  const double m_C6_AA = 310.915 ;
  const double m_C6_AB = 216.702 ;
  const double m_C6_BB = 172.445 ;
  const double m_d6_AA = 3.20676 ;
  const double m_d6_AB = 3.38985 ;
  const double m_d6_BB = 3.74833 ;


  const double m_C8 = 0.0;
  const double m_d8 = 0.0;

  const double if6 = 1.0/x2o::factorial<6>();
  const double if8 = 1.0/x2o::factorial<8>();
  
  
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_DIMER_H

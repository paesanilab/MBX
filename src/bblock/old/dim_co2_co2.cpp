#include "dim_co2_co2.h"

////////////////////////////////////////////////////////////////////////////////
namespace bblock { // Building Block :: CO2_CO2
////////////////////////////////////////////////////////////////////////////////

CO2_CO2::CO2_CO2() {

// USER SHOULD DEFINE HERE
  char rel_path[80] = "/src/potential/2b/params/co2_co2.nc";

// DO NOT TOUCH REST

  char * env = std::getenv("CU_HOME");
  char path_to_dat[1000];
  if (env == NULL) {
    std::cerr << "Please set the environment variable C_HOME"
              << " to where the home directory of Clusters is."
              << std::endl;
    exit(EXIT_FAILURE);
  } else {
    strcpy(path_to_dat,env);
    strcat(path_to_dat,rel_path);
  }

  pot.load_netcdf(path_to_dat);
  
}

CO2_CO2::~CO2_CO2(){}

double CO2_CO2::GetDispersion() {

  double * xyz1 = monomers[0]->GetXyz(); 
  double * xyz2 = monomers[1]->GetXyz(); 
  double disp = 0.0;
  const double* A_1_a= xyz1 + 0;
  const double* B_1_a= xyz1 + 3;
  const double* B_2_a= xyz1 + 6;

  const double* A_1_b= xyz2 + 0;
  const double* B_1_b= xyz2 + 3;
  const double* B_2_b= xyz2 + 6;

  disp += x6(m_C6_AA, m_d6_AA, m_C8, m_d8, A_1_a, A_1_b);
  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, A_1_a, B_1_b);
  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, A_1_a, B_2_b);

  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, B_1_a, A_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_1_a, B_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_1_a, B_2_b);

  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, B_2_a, A_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_2_a, B_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_2_a, B_2_b);


  return disp;
}

double CO2_CO2::GetDispersion(double * grd) {

  double * xyz1 = monomers[0]->GetXyz();
  double * xyz2 = monomers[1]->GetXyz();
  double * grd1[3*monomers[0]->GetNumRealSites()];
  double * grd2[3*monomers[1]->GetNumRealSites()];

  double disp = 0.0;

  std::fill(grd1,grd1 + 3*monomers[0]->GetNumRealSites()], 0.0);
  std::fill(grd1,grd2 + 3*monomers[1]->GetNumRealSites()], 0.0);

  const double* A_1_a= xyz1 + 0;
  double* gA_1_a= grd1 + 0;
  const double* B_1_a= xyz1 + 3;
  double* gB_1_a= grd1 + 3;
  const double* B_2_a= xyz1 + 6;
  double* gB_2_a= grd1 + 6;

  const double* A_1_b= xyz2 + 0;
  double* gA_1_b= grd2 + 0;
  const double* B_1_b= xyz2 + 3;
  double* gB_1_b= grd2 + 3;
  const double* B_2_b= xyz2 + 6;
  double* gB_2_b= grd2 + 6;

  disp += x6(m_C6_AA, m_d6_AA, m_C8, m_d8, A_1_a, A_1_b, gA_1_a, gA_1_b);
  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, A_1_a, B_1_b, gA_1_a, gB_1_b);
  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, A_1_a, B_2_b, gA_1_a, gB_2_b);

  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, B_1_a, A_1_b, gB_1_a, gA_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_1_a, B_1_b, gB_1_a, gB_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_1_a, B_2_b, gB_1_a, gB_2_b);

  disp += x6(m_C6_AB, m_d6_AB, m_C8, m_d8, B_2_a, A_1_b, gB_2_a, gA_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_2_a, B_1_b, gB_2_a, gB_1_b);
  disp += x6(m_C6_BB, m_d6_BB, m_C8, m_d8, B_2_a, B_2_b, gB_2_a, gB_2_b);

  std::copy(grd1,grd1 + 3*monomers[0]->GetNumRealSites()],grd);
  std::copy(grd2,grd2 + 3*monomers[1]->GetNumRealSites()],grd + 
            3*monomers[0]->GetNumRealSites()]);

  return disp;
}

double CO2_CO2::GetShortRange() {
  
}



} //bblock

/* Original code from Greg Medders. Adapted by Marc Riera in 20171005*/
#include "bblock/system.h"
#include "io_tools/read_nrg.h"
#include "tools/mt19937.h"
#include "tools/random-rotation.h"

namespace {
  const double step_size = 0.05;//Angstrom
  const double r_cut = 2.2;
  const double rcut_check = 2.0;
  const double kB = 0.0019872041; //kcal/mol/K from wiki
  const double PITIMES2 = 2*M_PI;
  const double max_dist = 50;//Angstrom
  double emin = 0.0;

  std::vector<bblock::System> sys_m1;
  std::vector<bblock::System> sys_m2;
  std::vector<bblock::System> sys_dim;
}

////////////////////////////////////////////////////////////////////////////////

double SetMass(const std::string atom) {
  // The masses are the ones from the most abundant isotope
  // Taken from NIST on November 7th 2015 by Marc R.
  // http://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl?ele=&all=all&ascii=html
  double mass = 0.0;
  if (atom ==  "H") {
    mass = 1.007825032;
  } else if (atom ==  "D") { 
    mass = 2.01410178;
  } else if (atom ==  "C") { 
    mass = 12.0000000;
  } else if (atom ==  "N") { 
    mass = 14.0;
  } else if (atom ==  "O") { 
    mass = 15.994914619;
  } else if (atom ==  "F") { 
    mass = 18.998403162;
  } else if (atom ==  "Cl") {
    mass = 34.968852682;
  } else if (atom ==  "Br") {
    mass = 78.9183376;
  } else if (atom ==  "I") { 
    mass = 126.9044719;
  } else if (atom ==  "Li") {
    mass = 7.0160034366;
  } else if (atom ==  "Na") {
    mass = 22.989769282;
  } else if (atom ==  "K") {
    mass = 38.9637064864;
  } else if (atom ==  "Rb") {
    mass = 84.9114977282;
  } else if (atom ==  "Cs") {
    mass = 132.905429 ;
  } else {
    std::cerr << "Atom " << atom << " not found." << std::endl;
    throw std::exception();
  }

  return mass;

}

////////////////////////////////////////////////////////////////////////////////

inline double dist(const double* r1, const double* r2)
{
    const double dx = r1[0] - r2[0];
    const double dy = r1[1] - r2[1];
    const double dz = r1[2] - r2[2];

    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

////////////////////////////////////////////////////////////////////////////////

void com_to_origin( double* crd, std::vector<std::string> at_names) {
  // Center the molecule at the center of mass
  // Finc COM coordinate:
  std::vector<double> COM(3,0.0);
  double tot_mass = 0.0;
  for (size_t i = 0; i < at_names.size(); i++) {
    double mass = SetMass(at_names[i]);
    for (size_t j = 0; j < 3; j++) {
      COM[j] += crd[3*i + j]*mass;
    }
    tot_mass += mass;
  }

  for (size_t j = 0; j < 3; j++) {
    COM[j] = COM[j]/tot_mass;
  }

  // Center molecule
  for (size_t i = 0; i < at_names.size(); i++)
    for (size_t j = 0; j < 3; j++)
      crd[3*i + j] = crd[3*i + j] - COM[j];
}

////////////////////////////////////////////////////////////////////////////////

void rotate_atom(const double* rot, double* xyz) {
  double old[3] = {xyz[0], xyz[1], xyz[2]};

  for (size_t i = 0; i < 3; ++i) {
    xyz[i] = 0.0;
    for (size_t j = 0; j < 3; ++j)
      xyz[i] += rot[i*3 + j]*old[j];
  }
}

////////////////////////////////////////////////////////////////////////////////

void rotate_monomer(kit::mt19937& prg, double* xyz, size_t n) {
  double x[3];
  for (size_t k = 0; k < 3; ++k)
    x[k] = prg.random_double();

  double rot[9];
  random_rotation(x, rot);

  for (size_t k = 0; k < n; ++k)
    rotate_atom(rot, xyz + 3*k);
}

////////////////////////////////////////////////////////////////////////////////

void average_mayer_function (const double& R,
  std::vector<bblock::System>& mon1, 
  std::vector<bblock::System>& mon2,
  std::vector<bblock::System>& dimer,
  int ns,
  kit::mt19937& prg, std::vector<double> temps, std::vector<double>& B2T) {

  for (size_t i = 0; i < ns; i++) { 
    size_t nat = mon1[0].GetNumRealSites();
    size_t nat2 = mon2[0].GetNumRealSites();
    double m1[3*nat], m2[3*nat2], dim[3*(nat2+nat)];

    std::vector<double> sys_xyz1 = mon1[0].GetRealXyz();
    std::vector<double> sys_xyz2 = mon2[0].GetRealXyz();

    std::copy(sys_xyz1.data(), sys_xyz1.data() + 3*nat, m1);
    std::copy(sys_xyz2.data(), sys_xyz2.data() + 3*nat2, m2);

    com_to_origin(m1, mon1[0].GetRealAtomNames());
    com_to_origin(m2, mon1[0].GetRealAtomNames());

    std::copy(m1, m1 + 3*nat, dim);
    std::copy(m2, m2 + 3*nat2, dim + 3*nat);

    rotate_monomer(prg, dim, nat);
    rotate_monomer(prg, dim + 3*nat, nat2);

    // Translate COMs away along X axis
    for (size_t j = 0; j < nat; j++) {
      dim[3*j] += R;
    }

    bool good = true;
    if (R < rcut_check) {
      for (size_t k = 0; k < nat; k++) {
        for (size_t j = 0; j < nat2; j++) {
          if (dist(dim + 3*k, dim+3*j+3*nat) < 1.0) {
            good = false;
            k = nat;
            j = nat2;
          }
        }
      }
    }
    
    double mon1_pot = mon1[0].Energy(false);
    double mon2_pot = mon2[0].Energy(false);

    std::vector<double> dim_xyz(3*(nat2+nat));
    std::copy(dim, dim+3*(nat2+nat),dim_xyz.begin());
    dimer[0].SetRealXyz(dim_xyz);
    double dim_pot = dimer[0].Energy(false);

        
    if (good && dim_pot > emin) {
      double Eint = dim_pot - mon1_pot - mon2_pot;
      for (size_t j = 0; j < temps.size(); j++) 
        B2T[j] += std::exp(-Eint/(kB*temps[j])) - 1.0;
    } else {
      for (size_t j = 0; j < temps.size(); j++)
        B2T[j] -= 1.0;
    }
  }
  
  for (size_t j = 0; j < temps.size(); j++)
    B2T[j] /= double(ns);
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

  if (argc < 7) {
    std::cout << "Usage: " << argv[0] 
              << " <Temperature> <eq_geom1.nrg> <eq_geom2.nrg> <dimer.nrg> <n_samples_x_point> <E_min_dimer> " << std::endl;
    std::cout << "If T < 0, will use the default temperatures:\n " 
              << " {150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400,\n"
              << " 450, 500, 550, 600, 650, 700, 750, 800 900 1000 \n"
              << " 1100 1200 1400 1600 1800}\n\n"; 
    return EXIT_FAILURE;
  }

  // 1. Load Configurations
  const double dR = step_size;

  const double T = atof(argv[1]);
  std::vector<double> temps;
  if (T < 0) {
    temps = {150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400,
              450, 500, 550, 600, 650, 700, 750, 800, 900, 1000,
              1100, 1200, 1400, 1600, 1800};
  } else {
    temps.push_back(T);
  }

  kit::mt19937 prg(27606);
  std::vector<double> B2T(temps.size(),0.0);
  int ns = 0;
  emin = atof(argv[6]);
  std::cout << "Emin = " << emin << std::endl;

  std::cout << "\nrcut = " << r_cut << std::endl;
  try {
    // Checking each one of the input files in the command line
    tools::ReadNrg(argv[2], sys_m1);
    tools::ReadNrg(argv[3], sys_m2);
    tools::ReadNrg(argv[4], sys_dim);
    ns = atoi(argv[5]);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  // 2. Do trapezoidal rule for numerical integration of radial coord
  //   Don't start at R = 0 because first (and final) points are zero
  for (double R = dR; R < max_dist; R += dR) {
    std::vector<double> Mayer(temps.size(),0.0);
    std::cout << "Working on d = " << R << std::endl;
    if(R < r_cut){
      for (size_t i = 0; i < temps.size(); i++)
        Mayer[i] = -1.0;
    } else {
      average_mayer_function(R, sys_m1, sys_m2, sys_dim, ns, prg, temps, Mayer);
    }

    for (size_t i = 0; i < temps.size(); i++) 
      B2T[i] += R*R*Mayer[i];
  }

  for (size_t i = 0; i < temps.size(); i++) {
    B2T[i] *= -dR*PITIMES2;
    B2T[i] *= 0.6022;
    std::cout << temps[i] << "    " << B2T[i] << std::endl;
  }

  return 0;
}  

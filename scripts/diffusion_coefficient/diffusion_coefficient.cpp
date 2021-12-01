#include "tools/properties.h"
#include "io_tools/read_xyz.h"
#include "io_tools/read_nrg.h"
#include "bblock/system.h"
#include <iomanip>
#include <iostream>
#include <fstream>

namespace {
  const double kB = 0.0019872041; //kcal/mol/K from wiki
  const double PITIMES2 = 2*M_PI;
  const double bohr2ang = 0.529177; 
  const double au2sec = 2.418884326509E-17;
  const double ang2cm = 1E-08;
  const double ps2sec = 1E-12;

}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

  if (argc < 6) {
    std::cout << "Usage: " << argv[0] 
              << " <trajecotry_i-pi.xyz> <velocities_i-pi.xyz> <input.nrg> <monomer_id> <time between frames (fs)>\n";
    return EXIT_FAILURE;
  }

  std::vector<std::vector<double> > coords;
  std::vector<std::vector<double> > vel;
  std::vector<std::vector<std::string> > at_names;
  std::vector<std::vector<double> > boxes;

  std::vector<bblock::System> systems;

  std::string mon_id;

  double dt = 0.0;

  try {
    // Checking each one of the input files in the command line
    // Read coordinates
    //std::cout << "Processing coordinates...\n";
    //tools::ReadXYZ(argv[1], at_names, coords, boxes, true, 1.0);

    // Read velocities
    std::cout << "Processing velocities...\n";
    tools::ReadXYZ(argv[2], at_names, vel, boxes, true, 0.529177);

    // Read the system
    tools::ReadNrg(argv[3], systems);

    // Read the monomer we want
    mon_id = std::string(argv[4]);

    // Read time step
    dt = atof(argv[5]);
    
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  // Get number of frames
  //size_t nframes = coords.size();
  size_t nframes = vel.size();
  
  // Get dt in s
  double dt_s = dt * 1E-15;

  // Check that coords, boxes, vel and atom_names have the same size
  if (vel.size() != nframes) {
      std::cerr << "Uops... Something went wrong. Velocities have not the same number of frames as the coordinates: " << vel.size() << " vs. " << nframes << std::endl;
      return 1; 
  }

  if (boxes.size() != nframes) {
      std::cerr << "Uops... Something went wrong. Boxes have not the same number of frames as the coordinates: " << boxes.size() << " vs. " << nframes << std::endl;
      return 1;
  }

  if (at_names.size() != nframes) {
      std::cerr << "Uops... Something went wrong. Atom names have not the same number of frames as the coordinates: " << at_names.size() << " vs. " << nframes << std::endl;
      return 1;
  }

  // Up to here everything seems OK. Let's calculate diffusion coefficient.

  std::cout << "Retrieving the monomers of interest...\n";
  // Retrieve system information
  // First thing is to know how many monomers of the type we want do we have
  size_t num_mon = systems[0].GetNumMon();
  std::vector<size_t> num_ats;
  std::vector<size_t> first_ind;
  size_t fi = 0;
  for (size_t i = 0; i < num_mon; i++) {
      std::string id = systems[0].GetMonId(i);
      size_t nat = systems[0].GetMonNumAt(i);
      if (id == mon_id) {
          num_ats.push_back(nat);
          first_ind.push_back(fi);
      }
      fi += nat;
  }

  size_t nmon = num_ats.size();

  // First, calculate the velocities at the center of mass

  std::cout << "Obtaining the velocities of the center of mass...\n";
  // Calculate the total mass of this monomer
  double total_mass = 0.0;
  for (size_t i = 0; i < num_ats[0]; i++) {
      total_mass += tools::GetMass(at_names[0][first_ind[0] + i]);
  }
  // Declare stuff that will be filled
  std::vector<std::vector<double> > v_cm(nframes, std::vector<double>(3*nmon,0.0));

  // Loop over frames
  for (size_t t1 = 0; t1 < nframes; t1++) {
      // Loop over all the monomers of interest
      for (size_t mon = 0; mon < nmon; mon++) {
          size_t nat = num_ats[mon];
          size_t fi = first_ind[mon];
          // Loop over atoms of monomer
          for (size_t j = 0; j < nat; j++) {
              size_t j3 = j*3;
              std::string at = at_names[t1][fi + j];
              double mass = tools::GetMass(at);
              // Loop over dimensions
              for (size_t k = 0; k < 3; k++) {
                  // Get CM velocity and convert it to cm/s
                  v_cm[t1][mon*3 + k] += 
                      vel[t1][fi*3 + j3 + k] * (mass/total_mass) * ang2cm / au2sec;
              }
          }
      }
  }

  // Then calculate the velocity autocorrelation function
  std::cout << "Calculating the velocity autocorrelation function...\n";

  std::vector<size_t> num_av(nframes,0);
  std::vector<double> vel_corr(3*(nframes),0.0);

  for (size_t t1 = 0; t1 < nframes; t1++) {
      for (size_t t2 = t1; t2 < nframes; t2++) {
          size_t t = t2 - t1;
          num_av[t]++;
          for (size_t mon = 0; mon < nmon; mon++) {
              for (size_t i = 0; i < 3; i++) {
                  vel_corr[3*t + i] += v_cm[t1][3*mon + i] * v_cm[t2][3*mon + i];
              }
          }
      }
  }

  // Average the calculated correlation function
  for (size_t t = 0; t < nframes; t++) {
      for (size_t i = 0; i < 3; i++) {
          vel_corr[3*t + i] /= (double(num_av[t]*nmon));
      }
  }

  // And calculate the diffusion coefficient
  std::cout << "Calculating the diffusion coefficient...\n";
  double diff_coef = 0.0;
  std::vector<double> vel_corr_tot(nframes,0.0);
  std::vector<double> time_ps(nframes,0.0);

  for (size_t t = 0; t < nframes; t++) {
      double corr_vel_tot = 0.0;
      double t_ps = double(t)*dt*1E-3;
      double t_s = t_ps * 1E-12;
      time_ps[t] = t_ps;
      for (size_t i = 0; i < 3; i++) {
          double tmp = vel_corr[3*t + i]*dt_s;
          corr_vel_tot += vel_corr[3*t + i];
          if (t == 0 or t == nframes - 1) tmp /= 2.0;
          diff_coef += tmp;
      }

      vel_corr_tot[t] += corr_vel_tot;
  }

  diff_coef /= 3.0;

  // Print results in file
  std::cout << "Printing results in files...\n";

  std::ofstream off_d("diffusion_coefficient.txt");
  off_d << std::scientific << std::setprecision(8) 
        << diff_coef << " cm^2/s" << std::endl;
  off_d.close();

  std::ofstream off("correlation.dat");
  for (size_t i = 0; i < nframes; i++) {
      off << std::scientific << std::setprecision(8)
          << std::setw(20) << time_ps[i]
          << std::setw(20) << vel_corr_tot[i] << std::endl;
  }
  off.close();
  
  std::cout << "Diffusion Coefficient = " << diff_coef << " cm^2/s" << std::endl;
  
  return 0;
}  


























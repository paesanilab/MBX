#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

#define REL_TOL 1E-06
namespace {

static std::vector<bblock::System> systems;

} // namespace

////////////////////////////////////////////////////////////////////////////////

void Compare(std::vector<double> en_ref, std::vector<double> en,
             std::string test, int &exitcode) {
  if (en_ref.size() != en.size()) {
    std::cerr << " ** Error ** : " << "Energy vectors do not have the same size.\n";
    exitcode = 1;
    return;
  }

  for (size_t i = 0; i < en.size(); i++) {
    double diff = std::abs(en_ref[i] - en[i]);
    if (diff > REL_TOL) {
      std::cerr << " ** Error ** : " << " Energies in test: \" " << test
                << " \" do not match for system[" << i << "]\n";
      std::cerr << en_ref[i] << " vs. " << en[i] << std::endl;
      exitcode = 1;
    }
  }
}

void CompareGrads(std::vector<double> grad_ref, std::vector<double> grad,
                  std::string test, size_t sys_index, int &exitcode) {
  if (grad_ref.size() != grad.size()) {
    std::cerr << " ** Error ** : " << "Gradient vectors do not have the same size.\n";
    exitcode = 1;
    return;
  }

  for (size_t i = 0; i < grad.size(); i++) {
    double diff = std::abs(grad_ref[i] - grad[i]);
    if (diff > REL_TOL) {
      std::cerr << " ** Error ** : " << " Energies in test: \" " << test
                << " \" do not match for system[" << sys_index << "]\n";
      std::cerr << grad_ref[i] << " vs. " << grad[i] << std::endl;
      exitcode = 1;
    }
  }
}

void PrintError(std::string test, size_t &exitcode) {
  std::cerr << " ** Error ** : " << "Test: \" " << test
            << " has failed." << std::endl;
  exitcode = 1;
}

template<typename T>
void PrintDifference(T val1, std::string header1,
                     T val2, std::string header2) {
  std::cerr << header1 << "(" << val1 << ") != " 
            << header2 << "(" << val2 << ")" << std::endl;
}

int main(int argc, char** argv)
{

  if (argc != 2) {
    std::cerr << "usage: energy input.nrg"
              << std::endl;
    return 0;
  }

  try {
    std::ifstream ifs(argv[1]);

    if (!ifs){
      throw std::runtime_error("could not open the NRG file");
    }

    tools::ReadNrg(argv[1], systems);
  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }

  // Generate all the data manually, then compare with the data generated
  // by the classes

  // Generate the coordinates internally
  // They must match the input:
  std::vector<double> xyz_ref = {
     1.321764984e+00 , -9.741042851e-02 , -1.766603401e-01,    // O
     1.417497992e+00 ,  6.385845339e-01 ,  4.439868776e-01,    // H
     3.540663798e-01 , -1.588118731e-01 , -2.456073451e-01,    // H
    -2.208131200e+00 ,  1.976693677e-01 ,  2.393048227e-01,    // Br 
     2.944922158e-01 , -2.351535749e+00 ,  1.444824160e+00,    // O
     9.312334658e-01 , -1.788380344e+00 ,  9.860224962e-01,    // H
    -4.776761258e-01 , -1.772172188e+00 ,  1.485185464e+00,    // H
    -1.122835060e+00 , -3.335553877e+00 , -9.798953936e-01,    // O
    -5.748067974e-01 , -3.252236355e+00 , -1.893369508e-01,    // H
    -1.538286573e+00 , -2.467574899e+00 , -1.019958033e+00,    // H
     1.275636119e-01 ,  2.311277753e+00 , -1.655546212e+00,    // O
     7.574431466e-01 ,  1.582562474e+00 , -1.634768274e+00,    // H
    -6.864683765e-01 ,  1.872099084e+00 , -1.368183421e+00,    // H
     5.998702944e-01 ,  2.360680451e+00 ,  1.273512544e+00,    // O
    -2.130729533e-01 ,  1.844059568e+00 ,  1.366935547e+00,    // H
     5.104601172e-01 ,  2.693542675e+00 ,  3.706992464e-01};   // H

  // Atom names and monomers
  std::vector<std::string> atom_names_ref = {
    "O", "H", "H", 
    "Br", 
    "O", "H", "H", 
    "O", "H", "H", 
    "O", "H", "H", 
    "O", "H", "H"};

  std::vector<std::string> monomers_ref = 
    {"h2o", "br", "h2o", "h2o", "h2o", "h2o"};

  // Number of sites and number of atoms
  size_t nat_ref = 16; // 1 + 5*3
  size_t nsites_ref = 21; // 1 + 5*4

  std::vector<size_t> nats_ref = {3, 1, 3, 3, 3, 3};
  std::vector<size_t> nsitesv_ref = {4, 1, 4, 4, 4, 4};
  std::vector<size_t> first_index_ref = {0, 4, 5, 9, 13, 17};

  // Now we create a system that will be the same as the one read
  bblock::System system_ref;
  
  // Add monomers to the system
  size_t count = 0;
  for (size_t i =0; i < monomers_ref.size(); i++) {
    std::vector<double> xyz(xyz_ref.begin() + 3*count, 
                            xyz_ref.begin() + 3*count + 3*nats_ref[i]);
    std::vector<std::string> ats(atom_names_ref.begin() + count,
                                 atom_names_ref.begin() + count + nats_ref[i]);
    std::string monid = monomers_ref[i];
    system_ref.AddMonomer(xyz, ats, monid);
  }

  // Initialize the system to fill in the information
  system_ref.Initialize();

  //////////////////////////////////////////////////////////////////////////////

  size_t exitcode = 0;
  std::string test;
  std::string header_manual = "Manually created System ";
  std::string header_ref = "Reference ";
  std::string header_read = "Read System ";

  // Test GetNumMon
  test = "GetNumMon()";

  size_t nm_ref = system_ref.GetNumMon();
  size_t nm     = systems[0].GetNumMon();

  if (nm_ref != monomers_ref.size()) {
    PrintError(test, exitcode);
    PrintDifference(nm_ref, header_manual,
                    monomers_ref.size(), header_ref);
  }

  if (nm     != monomers_ref.size()) {
    PrintError(test, exitcode);
    PrintDifference(nm, header_read,
                    monomers_ref.size(), header_ref);
  }

  //////////////////////////////////////////////////////////////////////////////

//  // Test GetNumMol()
//  test = "GetNumMol()";
//
//  nm     = systems[0].GetNumMol();
//
//  if (nm     != monomers_ref.size()) {
//    PrintError(test, exitcode);
//    PrintDifference(nm, header_read,
//                    monomers_ref.size(), header_ref);
//  }

  //////////////////////////////////////////////////////////////////////////////

  // Test GetNumSites()
  test = "GetNumSites()";
  
  size_t ns_ref = system_ref.GetNumSites();
  size_t ns     = systems[0].GetNumSites();

  if (ns_ref != nsites_ref) {
    PrintError(test, exitcode);
    PrintDifference(ns_ref, header_manual,
                    nsites_ref, header_ref);
  }

  if (ns     != nsites_ref) {
    PrintError(test, exitcode);
    PrintDifference(ns, header_read,
                    nsites_ref, header_ref);
  }

  //////////////////////////////////////////////////////////////////////////////

  // Test GetNumRealSites()
  test = "GetNumRealSites()";

  ns_ref = system_ref.GetNumRealSites();
  ns     = systems[0].GetNumRealSites();

  if (ns_ref != nat_ref) {
    PrintError(test, exitcode);
    PrintDifference(ns_ref, header_manual,
                    nat_ref, header_ref);
  }

  if (ns     != nat_ref) {
    PrintError(test, exitcode);
    PrintDifference(ns, header_read,
                    nat_ref, header_ref);
  }

  //////////////////////////////////////////////////////////////////////////////

  // Test GetMonNumAt(n)
  test = "GetMonNumAt(n)";

  for (size_t i = 0; i < monomers_ref.size(); i++) {
    size_t nat_ref = system_ref.GetMonNumAt(i);
    size_t nat     = systems[0].GetMonNumAt(i);

    if (nat_ref != nats_ref[i]) {
      PrintError(test, exitcode);
      PrintDifference(nat_ref, header_manual,
                      nats_ref[i], header_ref);
    }

    if (nat     != nats_ref[i]) {
      PrintError(test, exitcode);
      PrintDifference(nat, header_read,
                      nats_ref[i], header_ref);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  // Test GetFirstIndex(n)
  test = "GetFirstIndex(n)";

  for (size_t i = 0; i < monomers_ref.size(); i++) {
    size_t fi_ref = system_ref.GetFirstInd(i);
    size_t fi     = systems[0].GetFirstInd(i);

    if (fi_ref != first_index_ref[i]) {
      PrintError(test, exitcode);
      PrintDifference(fi_ref, header_manual,
                      first_index_ref[i], header_ref);
    }

    if (fi     != first_index_ref[i]) {
      PrintError(test, exitcode);
      PrintDifference(fi, header_read,
                      first_index_ref[i], header_ref);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  // Test GetXyz()
  

  //////////////////////////////////////////////////////////////////////////////

  if (exitcode == 0) {
    std::cout << "All tests passed!\n";
  }

  return exitcode;
}

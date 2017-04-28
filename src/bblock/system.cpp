#include "system.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {
  n_mol=0;
  initialized = false;
}

System::~System() {
}

void System::AddMolecule(std::shared_ptr<bblock::Molecule> molec){
  molecules.push_back(molec);
  return;
}

std::shared_ptr<Molecule> System::GetMolecule(size_t n) {return molecules[n];}
size_t System::GetNumMol() {return n_mol;}
size_t System::GetNumSites() {return n_sites;}
std::vector<std::string> System::GetAtomNames() {return at_names;}
void System::SetNumMol(size_t n) {n_mol = n;}

double System::Energy() {

  double energy1b = 0.0;
  double energy2b = 0.0;
  double energy3b = 0.0;
  double energy_elec = 0.0;

  for (size_t i = 0; i < n_mol; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon = 
          molecules[i]->GetMonomer(j); 
      energy1b += mon->Calc1BEnergy();
    }
  }
  return energy1b + energy2b + energy3b + energy_elec;    
}

double System::Energy(double * grd) {

  double energy1b = 0.0;
  double energy2b = 0.0;
  double energy3b = 0.0;
  double energy_elec = 0.0;

  if ( !initialized) Initialize();
  std::fill(grd, grd + 3*n_sites, 0.0);

  // 1B energy with gradients
  size_t icount = 0;
  for (size_t i = 0; i < n_mol; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon =
          molecules[i]->GetMonomer(j);
      energy1b += mon->Calc1BEnergy(grd + icount);
      icount += 3*(mon->GetNumSites());
    }
  }
  return energy1b + energy2b + energy3b + energy_elec;
}

void System::Initialize() {

  n_sites = 0;
  at_names.clear();

  for (size_t i = 0; i < n_mol ; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon =
          molecules[i]->GetMonomer(j);
      // Update number of sites
      n_sites += mon->GetNumSites();
      // Update atom names
      std::vector<std::string> mon_names = mon->GetAtomNames();
      at_names.insert(at_names.end(), mon_names.begin(), mon_names.end());

    }
  }

  // Initializing coordinates
  xyz = std::shared_ptr<double> (new double[n_sites * 3],
           []( double *p ) { delete[] p; });
  size_t n = 0;
  for (size_t i = 0; i < n_mol ; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon =
          molecules[i]->GetMonomer(j);
      double xyzmon[3 * (mon->GetNumSites())];
      mon->GetXyz(xyzmon);
      std::copy(xyzmon,xyzmon + 3 * mon->GetNumSites(),
                xyz.get() + 3 * n);
      n += mon->GetNumSites();
    }
  }

  initialized = true;
  return;
}

void System::GetXyz(double * coords) {
  size_t n = 0;
  for (size_t i = 0; i < n_mol ; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon =
          molecules[i]->GetMonomer(j);
      double xyzmon[3 * (mon->GetNumSites())];
      mon->GetXyz(xyzmon);
      std::copy(xyzmon,xyzmon + 3 * mon->GetNumSites(), 
                xyz.get() + 3 * n); 
      n += mon->GetNumSites();
    }
  }
  std::copy(xyz.get(), xyz.get() + 3 * n_sites, coords);
}

void System::SetXyz(double * coords) {
  size_t n = 0;
  for (size_t i = 0; i < n_mol ; i++) {
    size_t n_mon = molecules[i]->GetNumMon();
    for (size_t j = 0; j < n_mon; j++) {
      std::shared_ptr<Monomer> mon =
          molecules[i]->GetMonomer(j);
      std::copy(coords + 3 * n,
                coords + 3 * (n + mon->GetNumSites()), 
                xyz.get() + n);
      mon->SetXyz(xyz.get() + n);
      n += mon->GetNumSites();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

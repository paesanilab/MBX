#include "system.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {
  n_mol=0;
}

System::~System() {
}

void System::AddMolecule(std::shared_ptr<bblock::Molecule> molec){
  molecules.push_back(molec);
  return;
}

std::shared_ptr<Molecule> System::GetMolecule(size_t n) {return molecules[n];}

size_t System::GetNumMol() {return n_mol;}

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

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

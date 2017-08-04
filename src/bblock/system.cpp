#include "system.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {
}

System::~System() {
}

void System::AddMonomer(std::vector<double> xyz, 
             std::vector<std::string> atoms, std::string id){
  // Adding coordinates
  for (auto i = xyz.begin() ; i != xyz.end() ; i++)
    xyz_.push_back(*i);
  // Adding atom names
  for (auto i = atoms.begin() ; i != atoms.end() ; i++)
    atoms_.push_back(*i);
  // Adding id
  monomers_.push_back(id);
}

void System::Initialize() {}
////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

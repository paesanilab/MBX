#include "system.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {initialized_ = false;}
System::~System() {}

size_t System::GetNumMol() {return nmol_;}
size_t System::GetNumMon() {return nmon_;}
size_t System::GetNumSites() {return nsites_;}
size_t System::GetMonNat(size_t n) {return nat_[n];}
size_t System::GetFirstInd(size_t n) {return first_index_[n];}

std::vector<size_t> System::GetDimers() {return dimers_;}
std::vector<size_t> System::GetTrimers() {return trimers_;}
std::vector<size_t> System::GetMolecule(size_t n) {return molecules_[n];}
std::vector<std::string> System::GetSysAtNames() {return atoms_;}
std::vector<double> System::GetSysXyz() {return xyz_;}

std::string System::GetMonId(size_t n) {return monomers_[n];}

void System::SetSysXyz(std::vector<double> xyz) {
  // TODO Check that sizes are the same
  std::copy(xyz.begin(), xyz.end(), xyz_.begin());
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

void System::AddMolecule(std::vector<size_t> molec) {
  molecules_.push_back(molec);
}

void System::Initialize() {
  if (initialized_) return;

  cutoff2b_ = 15.0;
  cutoff3b_ =  5.0;
  maxNMonEval_ = 1024;
  maxNDimEval_ = 1024;
  maxNTriEval_ = 1024;
  
  AddMonomerInfo();
  nmol_ = molecules_.size();
  nmon_ = monomers_.size();
  //AddClusters(3, cutoff3b_);
  //AddClusters(2, cutoff2b_);
  // TODO Here should go the order and rearrengement stuff
}

void System::AddMonomerInfo() {
  // TODO Add check if empty or not initialized
  std::vector<double> xyz = xyz_;
  xyz_.clear();

  std::vector<std::string> atoms = atoms_;
  atoms_.clear();

  // TODO Sort the monomers and put them together by type
  mon_type_count_ = systools::OrderMonomers(monomers_, initial_order_); 

  // At this point, the monomers are sorted by name.
  // TODO maybe use a swich would be better than if else
  // TODO maybe we should try to find out number of sites first and reserve
  // memory, rather than pushing back?
  
  nsites_ = systools::SetUpMonomers(monomers_, sites_, nat_, first_index_,
                          chg_, pol_, polfac_);
  
  // Rearranging coordinates to account for virt sites
  xyz_ = std::vector<double> (3*nsites_, 0.0);
  atoms_ = std::vector<std::string> (nsites_, "virt");
  size_t count = 0;
    
  for (size_t i = 0; i < monomers_.size(); i++) {
    //size_t k = initial_order_[i];
    std::copy(xyz.begin() + 3 * count,
              xyz.begin() + 3 * (nat_[i] + count),
              xyz_.begin() + 3 * first_index_[i]);
    std::copy(atoms.begin() + count,
              atoms.begin() + nat_[i] + count,
              atoms_.begin() + first_index_[i]);
    count += nat_[i];
  }

  // Setting Gradients to 0
  grd_ = std::vector<double> (3*nsites_, 0.0);
  

}

void System::AddClusters(size_t n_max, double cutoff, 
                         size_t istart, size_t iend) {
  // istart is the monomer position for which we will look all dimers and 
  // trimers that contain it. iend is the last monomer position.
  // This means, if istart is 0 and iend is 2, we will look for all dimers
  // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!
  systools::AddClusters(n_max, cutoff, istart, iend, monomers_, xyz_,
                        first_index_, dimers_, trimers_);
  
}

double System::Energy(std::vector<double> &grd, bool do_grads) {
  // Reset energy and grads in system to 0
  energy_ = 0.0;
  std::fill(grd_.begin(), grd_.end(), 0.0);

  // Get the NB contributions
  double e1b = Get1B(do_grads);
  double e2b = Get2B(do_grads);
  double e3b = Get3B(do_grads);

  // Set up energy with the new value
  energy_ = e1b + e2b + e3b;

  // Copy gradients to output grd
  grd.clear();
  grd = grd_;

  return energy_;
}

double System::Get1B(bool do_grads) {

  // 1B ENERGY
  // Loop overall the monomers and get their energy
  size_t cmt = 0;
  size_t ccrd = 0;
  double e1b = 0.0;

  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    // Useful variables
    size_t istart = 0;
    size_t iend = 0;
    while (istart < mon_type_count_[k].second) {
      iend = std::min(istart + maxNMonEval_, mon_type_count_[k].second);
      size_t length = iend - istart;


      std::vector<double> energy = std::vector<double>(length, 0.0);
      size_t ncoord = 3 * nat_[cmt] * length;
      // XYZ with real sites
      std::vector<double> xyz = std::vector<double> (ncoord,0.0);

      // Set up real coordinates
      for (size_t i = istart; i < iend; i++) {
        std::copy(xyz_.begin() + ccrd + 3 * i * sites_[cmt],
                  xyz_.begin() + ccrd + 3 * (i * sites_[cmt] + nat_[cmt]),
                  xyz.begin() + 3 * (i - istart) * nat_[cmt]);
      }

      double grd2[ncoord];
      std::fill(grd2, grd2 + ncoord, 0.0);
      // Get energy of the chunk as function of monomer
      if (mon_type_count_[k].first == "h2o") {
        if (do_grads)  {
          energy = ps::pot_nasa(xyz.data(), grd2, length);
        } else {
          energy = ps::pot_nasa(xyz.data(), 0, length);
        }
      } else {
        std::fill(energy.begin(), energy.end(),0.0);
      }
      // Add energy to energy_
      for (size_t i = 0; i < length; i++) {
        e1b += energy[i];

        if (do_grads) {
          // Reorganize gradients
          for (size_t j = 0; j < 3*nat_[cmt]; j++) {
            grd_[ccrd + 3*(i + istart)*sites_[cmt] + j] 
                += grd2[3*i*nat_[cmt] + j];
          }
        }
      }
      istart = iend;
    }

    // Update ccrd and cmt
    ccrd += 3 * mon_type_count_[k].second * sites_[cmt];
    cmt += mon_type_count_[k].second;
  }

  return e1b;
}


double System::Get2B(bool do_grads) {
  // No dimers makes the function return 0.

  // 2B ENERGY
  double e2b = 0;
  double edisp = 0.0;

  // Vectorizable part
  size_t istart = 0;
  size_t iend = 0;
  size_t step = 1024;

  while (istart < nmon_) {
//    if (nmon_ < maxNDimEval_) {
//      iend = nmon_;
//    } else {
//      // TODO the step is arbitrary. Set to one for now
      iend = std::min(istart + step, nmon_);
//    }

    // Adding corresponding clusters      
    AddClusters(2,cutoff2b_,istart,iend);
    if (dimers_.size() < 2) {
      istart = iend;
      continue;
    }

    std::vector<double> xyz1;
    std::vector<double> xyz2;
    std::vector<double> grd1;
    std::vector<double> grd2;
    std::string m1 = monomers_[dimers_[0]];
    std::string m2 = monomers_[dimers_[1]];
    size_t i = 0;
    size_t nd = 0;
    while (i < dimers_.size()) {
      if (monomers_[dimers_[i]] == m1 && 
          monomers_[dimers_[i + 1]] == m2) {
        // Push the coordinates
        for (size_t j = 0; j < 3*nat_[dimers_[i]]; j++) {
          xyz1.push_back(xyz_[3*first_index_[dimers_[i]] + j]);
          grd1.push_back(0.0);
        }
        for (size_t j = 0; j < 3*nat_[dimers_[i+1]]; j++) {
          xyz2.push_back(xyz_[3*first_index_[dimers_[i+1]] + j]);
          grd2.push_back(0.0);
        }
        nd++;
      }
      
  
      // If one of the monomers is different as the previous one
      // since dimers are also ordered, means that no more dimers of that
      // type exist. Thus, do calculation, update m? and clear xyz
      if (monomers_[dimers_[i]] != m1 ||
          monomers_[dimers_[i + 1]] != m2 ||
          i == dimers_.size() - 2 || nd == maxNDimEval_) {
        //std::cerr << "i = " << i << " / " << dimers_.size() - 1 << std::endl;
        if (do_grads) {
          // POLYNOMIALS
          e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2, grd1, grd2);
          // DISPERSION
          edisp += disp::GetDispersion(m1, m2, nd, do_grads,
                                     xyz1, xyz2, grd1, grd2);
          for (size_t k = 0; k < nd ; k++) {
            for (size_t j = 0; j < 3*nat_[dimers_[i - 2*k]]; j++) {
              grd_[3*first_index_[dimers_[i - 2*k]] + j]
                  += grd1[(nd - k - 1)*3*nat_[dimers_[i - 2*k]] + j];
            }
            for (size_t j = 0; j < 3*nat_[dimers_[i- 2*k +1]]; j++) {
              grd_[3*first_index_[dimers_[i -2*k + 1]] + j]
                  += grd2[(nd - k - 1)*3*nat_[dimers_[i - 2*k + 1]] + j];
            }
          }
        } else {
          // POLYNOMIALS
          e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2);
          // DISPERSION
          edisp += disp::GetDispersion(m1, m2, nd, do_grads,
                                     xyz1, xyz2, grd1, grd2);
        }
        nd = 0;
        xyz1.clear();
        xyz2.clear();
        grd1.clear();
        grd2.clear();
        m1 = monomers_[dimers_[i]];
        m2 = monomers_[dimers_[i + 1]];
        if (i != dimers_.size() - 2) i-=2;
      }
      i+=2;
    } 
    istart = iend;
  }
  //std::cout << "disp = " << edisp << "    2b = " << e2b << std::endl;

  //std::cerr << "dimers done: " << e2b + edisp << std::endl;
  return e2b + edisp;
}

double System::Get3B(bool do_grads) {
  // 3B ENERGY
  double e3b = 0;
  // trimers are ordered
  // TODO put this in chunk so can be vectorized
  // TODO Maybe initialize here all possible trimer structs?
  size_t istart = 0;
  size_t iend = 0;
  size_t step = 1024;

  while (istart < nmon_) {
//    if (nmon_ < maxNTriEval_) {
//      iend = nmon_;
//    } else {
//      // TODO the step is arbitrary. Set to one for now
      iend = std::min(istart + step, nmon_);
//    }

    // Adding corresponding clusters      
    AddClusters(3,cutoff3b_,istart,iend);
    if (trimers_.size() < 3) {
      istart = iend;
      continue;
    }

    for (size_t i = 0; i < trimers_.size(); i+=3) {
      if (monomers_[trimers_[i]] == "h2o" and
          monomers_[trimers_[i + 1]] == "h2o" and
          monomers_[trimers_[i + 2]] == "h2o") {
        x2o::x3b_v2x pot;
        if (do_grads) {
          double grdx[27];
          std::fill(grdx, grdx + 27, 0.0);
          e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
                          xyz_.data() + 3*first_index_[trimers_[i + 1]],
                          xyz_.data() + 3*first_index_[trimers_[i + 2]],
                          grdx, grdx + 9, grdx + 18);
          for (size_t j = 0; j < 9; j++) {
            grd_[3*first_index_[trimers_[i]] + j] += grdx[j];
            grd_[3*first_index_[trimers_[i + 1]] + j] += grdx[j + 9];
            grd_[3*first_index_[trimers_[i + 2]] + j] += grdx[j + 18];
          }
        } else {
          e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
                          xyz_.data() + 3*first_index_[trimers_[i + 1]],
                          xyz_.data() + 3*first_index_[trimers_[i + 2]]);
        }
      }
    }
    istart = iend;
    //if ((i +3)%3072 == 0 ) std::cerr << i << " / " << trimers_.size() << std::endl;
  }

  return e3b;
}
//  // 3B ENERGY
//  double e3b = 0;
//  // trimers are ordered
//  // TODO put this in chunk so can be vectorized
//  // TODO Maybe initialize here all possible trimer structs?
//  for (size_t i = 0; i < trimers_.size(); i+=3) {
//    if (monomers_[trimers_[i]] == "h2o" and
//        monomers_[trimers_[i + 1]] == "h2o" and
//        monomers_[trimers_[i + 2]] == "h2o") {
//      double grdx[27];
//      std::fill(grdx, grdx + 27, 0.0);
//      x2o::x3b_v2x pot;
//      e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
//                      xyz_.data() + 3*first_index_[trimers_[i + 1]],
//                      xyz_.data() + 3*first_index_[trimers_[i + 2]], 
//                      grdx, grdx + 9, grdx + 18);
//      for (size_t j = 0; j < 9; j++) {
//        grd_[3*first_index_[trimers_[i]] + j] += grdx[j];
//        grd_[3*first_index_[trimers_[i + 1]] + j] += grdx[j + 9];
//        grd_[3*first_index_[trimers_[i + 2]] + j] += grdx[j + 18];
//      }
//    }
//  }
//
//  energy_ += e3b;
//
//
//  // Putting gradients back to argument. All grads will be put there
//  // Including Virtual Electrostatic sites
//  // TODO Maybe change this
//  grd.clear();
//  grd = grd_;

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////






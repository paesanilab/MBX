#include "system.h"

//#define DEBUG
//#define TIMING

#ifdef TIMING
#  include <chrono>
#  include <iostream>
#endif

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {initialized_ = false;}
System::~System() {}

size_t System::GetNumMol() {return nmol_;}
size_t System::GetNumMon() {return nmon_;}
size_t System::GetNumSites() {return nsites_;}
size_t System::GetNumRealSites() {return numat_;}

size_t System::GetMonNumAt(size_t n) {
  return nat_[original2current_order_[n]];
}

size_t System::GetFirstInd(size_t n) {
  // Obtain position in system of monomer in position 
  // 'n' in the original order
  size_t current_pos = original2current_order_[n];

  // Get the first index that this monomer had in the input order
  return initial_order_[current_pos].second;
}

std::vector<size_t> System::GetPairList(size_t nmax, double cutoff,
                                        size_t istart, size_t iend) {

  // Call the add clusters function to get all the pairs
  AddClusters(nmax, cutoff, 0, monomers_.size());

  // Change the monomer indexes of dimers_ or trimers_ 
  // to match the input order
  // Select also the pairs that we are interested on
  
  std::vector<size_t> pair_list;

  if (nmax == 2) {
    for (size_t i = 0; i < dimers_.size(); i+=nmax) {
      size_t mon1 = initial_order_[dimers_[i]].first;
      size_t mon2 = initial_order_[dimers_[i+1]].first;
      if ((mon1 >= istart && mon1 < iend) || 
          (mon2 >= istart && mon2 < iend)) {
        pair_list.push_back(mon1);
        pair_list.push_back(mon2);
      }
    }
  } else if (nmax == 3) {
    for (size_t i = 0; i < trimers_.size(); i+=nmax) {
      size_t mon1 = initial_order_[trimers_[i]].first;
      size_t mon2 = initial_order_[trimers_[i+1]].first;
      size_t mon3 = initial_order_[trimers_[i+2]].first;
      if ((mon1 >= istart && mon1 < iend) || 
          (mon2 >= istart && mon2 < iend) ||
          (mon3 >= istart && mon3 < iend)) {
        pair_list.push_back(mon1);
        pair_list.push_back(mon2);
        pair_list.push_back(mon3);
      }
    }
  }

  return pair_list;
}

std::vector<size_t> System::GetMolecule(size_t n) {return molecules_[n];}

std::vector<std::string> System::GetAtomNames() {
  return systools::ResetOrderN(atoms_, initial_order_, first_index_, sites_);
}

std::vector<std::string> System::GetRealAtomNames() {
  return systools::ResetOrderRealN(atoms_, initial_order_realSites_,
                                   numat_, first_index_, nat_);
}

std::vector<double> System::GetXyz() {
  return systools::ResetOrder3N(xyz_, initial_order_, first_index_, sites_);
}

std::vector<double> System::GetRealXyz() {
  return systools::ResetOrderReal3N(xyz_, initial_order_realSites_, 
                              numat_, first_index_, nat_);
}

std::vector<double> System::GetGrads() {
  return systools::ResetOrder3N(grad_, initial_order_, 
                                first_index_, sites_);
}

std::vector<double> System::GetRealGrads() {
  return systools::ResetOrderReal3N(grad_, initial_order_realSites_, 
                                    numat_, first_index_, nat_);
}

std::vector<double> System::GetCharges() {
  return systools::ResetOrderN(chg_, initial_order_, 
                               first_index_, sites_);
}

std::vector<double> System::GetRealCharges() {
  return systools::ResetOrderRealN(chg_, initial_order_realSites_, 
                                   numat_, first_index_, nat_);
}

std::vector<double> System::GetPolarizabilities() {
  return systools::ResetOrderN(pol_, initial_order_, 
                               first_index_, sites_);
}

std::vector<double> System::GetRealPolarizabilities() {
  return systools::ResetOrderRealN(pol_, initial_order_realSites_,
                                   numat_, first_index_, nat_);
}

std::vector<double> System::GetPolarizabilityFactors() {
  return systools::ResetOrderN(polfac_, initial_order_,
                               first_index_, sites_);
}

std::vector<double> System::GetRealPolarizabilityFactors() {
  return systools::ResetOrderRealN(pol_, initial_order_realSites_,
                                   numat_, first_index_, nat_);
}

std::string System::GetMonId(size_t n) {
  size_t current_pos = original2current_order_[n];
  return monomers_[current_pos];
}

void System::Set2bCutoff(double cutoff2b) {cutoff2b_ = cutoff2b;} 
void System::Set3bCutoff(double cutoff3b) {cutoff3b_ = cutoff3b;}
void System::SetNMaxEval1b(size_t nmax) {maxNMonEval_ = nmax;} 
void System::SetNMaxEval2b(size_t nmax) {maxNDimEval_ = nmax;}
void System::SetNMaxEval3b(size_t nmax) {maxNTriEval_ = nmax;}
void System::SetStepEval2b(size_t step) {stepEval2b_ = step;}
void System::SetStepEval3b(size_t step) {stepEval3b_ = step;}

void System::SetDipoleTol(double tol) {diptol_ = tol;}
void System::SetDipoleMaxIt(double maxit) {maxItDip_ = maxit;}
void System::SetDipoleMethod(std::string method) {dipole_method_ = method;}

void System::SetPBC(bool use_pbc, 
                    std::vector<double> box = {1000.0,0.0,0.0,
                                               0.0,1000.0,0.0,
                                               0.0,0.0,1000.0}) {
  use_pbc_ = use_pbc;
  box_ = box;
  if (use_pbc_) {
    systools::FixMonomerCoordinates(xyz_,box_,nat_,first_index_);
    SetVSites();
    SetCharges();
    SetPols();
    SetPolfacs();
  }
}

void System::SetSysXyz(std::vector<double> xyz) {
  // TODO Check that sizes are the same
  std::copy(xyz.begin(), xyz.end(), xyz_.begin());
}

void System::SetOriginalOrderSysXyz(std::vector<double> xyz) {
  // TODO Check that sizes are the same
  for (size_t i = 0; i < sites_.size(); i++) {
    size_t ini = 3*initial_order_[i].second;
    size_t fin = ini + 3*sites_[i];
    size_t ini_new = 3*first_index_[i];
    std::copy(xyz.begin() + ini, xyz.begin() + fin,
              xyz_.begin() + ini_new);
  } 
}

void System::SetOriginalOrderRealSysXyz(std::vector<double> xyz) {
  // TODO Check that sizes are the same
  for (size_t i = 0; i < nat_.size(); i++) {
    size_t ini = 3*initial_order_realSites_[i].second;
    size_t fin = ini + 3*nat_[i];
    size_t ini_new = 3*first_index_[i];
    std::copy(xyz.begin() + ini, xyz.begin() + fin,
              xyz_.begin() + ini_new);
  }
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

  cutoff2b_ = 100.0;
  cutoff3b_ =  5.0;
  diptol_ = 1E-16;
  maxNMonEval_ = 1024;
  maxNDimEval_ = 1024;
  maxNTriEval_ = 1024;
  maxItDip_ = 100;

  SetPBC(false);
  
  AddMonomerInfo();
  nmol_ = molecules_.size();
  nmon_ = monomers_.size();

  SetVSites();
  SetCharges();
  SetPols();
  SetPolfacs();

  dipole_method_ = "aspc";

  // TODO: Do grads set to true for now. Needs to be fixed
  electrostaticE_.Initialize(chg_, chggrad_, polfac_, 
                pol_, xyz_, monomers_, sites_, first_index_, 
                mon_type_count_, true, diptol_, maxItDip_, dipole_method_);

  initialized_ = true;
}

void System::AddMonomerInfo() {
  // TODO Add check if empty or not initialized
  std::vector<double> xyz = xyz_;
  xyz_.clear();

  std::vector<std::string> atoms = atoms_;
  atoms_.clear();

  
  std::vector<size_t> fi_at;
  nsites_ = systools::SetUpMonomers(monomers_, sites_, nat_, fi_at);
  
  numat_ = 0;
  for (size_t i = 0; i < nat_.size(); i++) {
    numat_ += nat_[i];
  }

  mon_type_count_ = systools::OrderMonomers(monomers_, sites_, nat_, 
            original2current_order_,initial_order_, initial_order_realSites_); 
  
  // Rearranging coordinates to account for virt sites
  xyz_ = std::vector<double> (3*nsites_, 0.0);
  atoms_ = std::vector<std::string> (nsites_, "virt");

  size_t count = 0;
  first_index_.clear();
  std::vector<size_t> tmpsites;
  std::vector<size_t> tmpnats;
  for (size_t i = 0; i < monomers_.size(); i++) {
    size_t k = initial_order_[i].first;
    std::copy(xyz.begin() + 3 * fi_at[k],
              xyz.begin() + 3 * (fi_at[k] + nat_[k]),
              xyz_.begin() + 3 * count);
    std::copy(atoms.begin() + fi_at[k],
              atoms.begin() + fi_at[k] + nat_[k],
              atoms_.begin() + count);
    first_index_.push_back(count);
    count += sites_[k];
    tmpsites.push_back(sites_[k]);
    tmpnats.push_back(nat_[k]);
  }

  sites_ = tmpsites;
  nat_ = tmpnats;

  // Setting Gradients to 0
  grad_ = std::vector<double> (3*nsites_, 0.0);
  chg_ = std::vector<double> (nsites_, 0.0);
  pol_ = std::vector<double> (nsites_, 0.0);
  polfac_ = std::vector<double> (nsites_, 0.0);
  
  

}

void System::AddClusters(size_t n_max, double cutoff, 
                         size_t istart, size_t iend) {
  // istart is the monomer position for which we will look all dimers and 
  // trimers that contain it. iend is the last monomer position.
  // This means, if istart is 0 and iend is 2, we will look for all dimers
  // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!
  size_t nmon = monomers_.size();
  systools::AddClusters(n_max, cutoff, istart, iend, nmon, 
                        use_pbc_, box_, xyz_,
                        first_index_, dimers_, trimers_);
  
}

std::vector<size_t> System::AddClustersParallel(size_t n_max, double cutoff,
                         size_t istart, size_t iend) {
  // Overloaded function to be compatible with omp
  // Returns dimers if n_max == 2, or trimers if n_max == 3
  size_t nmon = monomers_.size();
  std::vector<size_t> dimers, trimers;
  systools::AddClusters(n_max, cutoff, istart, iend, nmon, 
                        use_pbc_, box_, xyz_,
                        first_index_, dimers, trimers);
  if (n_max == 2)
    return dimers;
  return trimers;
}

double System::Energy(std::vector<double> &grad, bool do_grads) {
  // Reset energy and grads in system to 0
  energy_ = 0.0;
  std::fill(grad_.begin(), grad_.end(), 0.0);

  // Reset the chargers, pols, polfacs and new Vsite
  if (use_pbc_) {
    SetPBC(use_pbc_,box_);
  } else {
    SetVSites();
    SetCharges();
    SetPols();
    SetPolfacs();
  }

  // Get the NB contributions

# ifdef TIMING
  auto t1 = std::chrono::high_resolution_clock::now();
# endif

  allMonGood_ = true;
  double e1b = Get1B(do_grads);

  // If monomers are too distorted, skip 
  if (!allMonGood_) {
    grad = systools::ResetOrder3N(grad_, initial_order_, first_index_, sites_);
    return e1b;
  }

# ifdef TIMING
  auto t2 = std::chrono::high_resolution_clock::now();
# endif

  double e2b = Get2B(do_grads);

# ifdef TIMING
  auto t3 = std::chrono::high_resolution_clock::now();
# endif

  double e3b = Get3B(do_grads);

# ifdef TIMING
  auto t4 = std::chrono::high_resolution_clock::now();
# endif

  // Set charges, polarizabilities and polfacs
  // Note: Pols and Polfacs are constant
  SetVSites();
  SetCharges();
  SetPols();
  SetPolfacs();

# ifdef TIMING
  auto t5 = std::chrono::high_resolution_clock::now();
# endif

  // Electrostatic energy
  double Eelec = GetElectrostatics(do_grads);

# ifdef TIMING
  auto t6 = std::chrono::high_resolution_clock::now();
# endif

  // Set up energy with the new value
  energy_ = e1b + e2b + e3b + Eelec;

# ifdef DEBUG
  std::cerr << "1B = " << e1b << std::endl
            << "2B = " << e2b << std::endl
            << "3B = " << e3b << std::endl
            << "Elec = " << Eelec << std::endl;
# endif
# ifdef TIMING
  std::cerr << "System::1b(grad=" << do_grads << ") "
    << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
    << " milliseconds\n";
  std::cerr << "System::2b(grad=" << do_grads << ") "
    << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count()
    << " milliseconds\n";
  std::cerr << "System::3b(grad=" << do_grads << ") "
    << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()
    << " milliseconds\n";
  std::cerr << "System::electrostatics(grad=" << do_grads << ") "
    << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count()
    << " milliseconds\n";
  std::cerr << "TotalEnergy(grad=" << do_grads << ") "
    << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t1).count()
    << " milliseconds\n";
# endif

  // Copy gradients to output grad
  grad = systools::ResetOrder3N(grad_, initial_order_, first_index_, sites_);

  return energy_;
}

double System::OneBodyEnergy(bool do_grads) {
  energy_ = 0.0;
  std::fill(grad_.begin(), grad_.end(), 0.0);
  
  energy_ = Get1B(do_grads);

  return energy_;
}

double System::Get1B(bool do_grads) {

  // 1B ENERGY
  // Loop overall the monomers and get their energy
  size_t curr_mon_type = 0;
  size_t current_coord = 0;
  double e1b = 0.0;

  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    // Useful variables
    size_t istart = 0;
    size_t iend = 0;
    while (istart < mon_type_count_[k].second) {
      iend = std::min(istart + maxNMonEval_, mon_type_count_[k].second);
      size_t nmon = iend - istart;
      size_t ncoord = 3 * nat_[curr_mon_type] * nmon;
      std::string mon = mon_type_count_[k].first;

      // XYZ with real sites
      std::vector<double> xyz(ncoord,0.0);
      std::vector<double> grad2(ncoord,0.0);

      // Set up real coordinates
      for (size_t i = istart; i < iend; i++) {
        std::copy(xyz_.begin() + current_coord + 3 * i * sites_[curr_mon_type],
                  xyz_.begin() + current_coord + 3 * (i * sites_[curr_mon_type] 
                + nat_[curr_mon_type]),
                  xyz.begin() + 3 * (i - istart) * nat_[curr_mon_type]);
      }

      // Get energy of the chunk as function of monomer
      if (do_grads)  {
        e1b += e1b::get_1b_energy(mon, nmon, xyz, grad2, allMonGood_);
        
        // Reorganize gradients
        for (size_t i = 0; i < nmon; i++) {
          for (size_t j = 0; j < 3*nat_[curr_mon_type]; j++) {
            grad_[current_coord + 3*(i + istart)*sites_[curr_mon_type] + j]
                += grad2[3*i*nat_[curr_mon_type] + j];
          }
        }
      } else {
        e1b += e1b::get_1b_energy(mon, nmon, xyz, allMonGood_);
      }

      istart = iend;
    }

    // Update current_coord and curr_mon_type
    current_coord += 3 * mon_type_count_[k].second * sites_[curr_mon_type];
    curr_mon_type += mon_type_count_[k].second;
  }

  return e1b;
}

double System::TwoBodyEnergy(bool do_grads) {
  energy_ = 0.0;
  std::fill(grad_.begin(), grad_.end(), 0.0);

  energy_ = Get2B(do_grads);

  return energy_;
}

double System::Get2B(bool do_grads) {
  // No dimers makes the function return 0.

  // 2B ENERGY
  double e2b_t = 0.0;
  double edisp_t = 0.0;

  // Variables needed for OMP
  size_t step = 1;
  int num_threads = 1;
  int grad_step = 3*nsites_;

# ifdef _OPENMP
# pragma omp parallel
{
  if (omp_get_thread_num() == 0)
    num_threads = omp_get_num_threads();
}
  grad_step = 3*nsites_ / num_threads;
  step = std::max(size_t(1),std::min(nmon_/num_threads,step));
# endif // _OPENMP

  // dimers are ordered
  size_t first_grad = 0;
  size_t last_grad = 3*nsites_;
  int rank = 0;
  std::vector<double> e2b_pool(num_threads,0.0);
  std::vector<double> edisp_pool(num_threads,0.0);
  std::vector<std::vector<double>> grad_pool(num_threads,std::vector<double>(3*nsites_,0.0));

# ifdef _OPENMP
# pragma omp parallel for schedule(dynamic) private(rank)
# endif // _OPENMP
  for (size_t istart = 0; istart < nmon_; istart += step) {
#   ifdef _OPENMP
    rank = omp_get_thread_num();
#   endif // _OPENMP
    size_t iend = std::min(istart + step, nmon_);

    // Adding corresponding clusters      
#   ifdef _OPENMP
    std::vector<size_t> dimers =
                       AddClustersParallel(2,cutoff2b_,istart,iend);
#   else
    AddClusters(2,cutoff2b_,istart,iend);
    std::vector<size_t> dimers = dimers_;
#   endif

    if (dimers.size() < 2) {
      continue;
    }

    std::vector<double> xyz1;
    std::vector<double> xyz2;
    std::vector<double> grad1;
    std::vector<double> grad2;
    std::string m1 = monomers_[dimers[0]];
    std::string m2 = monomers_[dimers[1]];

    size_t i = 0;
    size_t nd = 0;
    size_t nd_tot = 0;

    while (2*nd_tot < dimers.size()) {
      i = (nd_tot + nd) * 2;
      if (monomers_[dimers[i]] == m1 && 
          monomers_[dimers[i + 1]] == m2) {
        // Push the coordinates
        for (size_t j = 0; j < 3*nat_[dimers[i]]; j++) {
          xyz1.push_back(xyz_[3*first_index_[dimers[i]] + j]);
          grad1.push_back(0.0);
        }
        for (size_t j = 0; j < 3*nat_[dimers[i+1]]; j++) {
          xyz2.push_back(xyz_[3*first_index_[dimers[i+1]] + j]);
          grad2.push_back(0.0);
        }
        nd++;
      }
      
  
      // If one of the monomers is different as the previous one
      // since dimers are also ordered, means that no more dimers of that
      // type exist. Thus, do calculation, update m? and clear xyz
      if (monomers_[dimers[i]] != m1 ||
          monomers_[dimers[i + 1]] != m2 ||
          i == dimers.size() - 2 || nd == maxNDimEval_) {
        if (nd == 0) {
          xyz1.clear();
          xyz2.clear();
          grad1.clear();
          grad2.clear();
          m1 = monomers_[dimers[i]];
          m2 = monomers_[dimers[i + 1]];
          continue;
        }

        // Fix dimer positions if pbc
        if (use_pbc_) {
          systools::GetCloseDimerImage(box_, nat_[dimers[nd_tot * 2]],
                             nat_[dimers[nd_tot * 2 + 1]],
                             nd, xyz1.data(), xyz2.data());
        }

        if (do_grads) {
          // POLYNOMIALS
          e2b_pool[rank] += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2, grad1, grad2);

          // DISPERSION
          edisp_pool[rank] += disp::GetDispersion(m1, m2, nd, do_grads,
                                     xyz1, xyz2, grad1, grad2, cutoff2b_, use_pbc_);
          size_t i0 = nd_tot * 2;
          for (size_t k = 0; k < nd ; k++) {
            for (size_t j = 0; j < 3*nat_[dimers[i0 + 2*k]]; j++) {
              grad_pool[rank][3*first_index_[dimers[i0 + 2*k]] + j]
                  += grad1[k*3*nat_[dimers[i0 + 2*k]] + j];
            }
            for (size_t j = 0; j < 3*nat_[dimers[i0 + 2*k +1]]; j++) {
              grad_pool[rank][3*first_index_[dimers[i0 + 2*k + 1]] + j]
                  += grad2[k*3*nat_[dimers[i0 + 2*k + 1]] + j];
            }
          }
        } else {
          // POLYNOMIALS
          e2b_pool[rank] += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2);
          // DISPERSION
          edisp_pool[rank] += disp::GetDispersion(m1, m2, nd, do_grads,
                                     xyz1, xyz2, grad1, grad2, cutoff2b_, use_pbc_);
        }
        nd_tot += nd;
        nd = 0;
        xyz1.clear();
        xyz2.clear();
        grad1.clear();
        grad2.clear();
        m1 = monomers_[dimers[i]];
        m2 = monomers_[dimers[i + 1]];
      }
    } 
  }

# ifdef _OPENMP
# pragma omp parallel private(first_grad,last_grad,rank)
{
  rank = omp_get_thread_num();

  first_grad = 0 + rank * grad_step;

  last_grad = (rank + 1) * grad_step;

  if (rank == num_threads - 1) {
    last_grad = 3*nsites_;
  }
# pragma omp barrier
# endif

  for (size_t i = 0; i < num_threads; i++) {
    for (size_t j = first_grad; j < last_grad; j++) {
      grad_[j] += grad_pool[i][j];
    }
  }

# ifdef _OPENMP
} // parallel   
# endif

  for (size_t i = 0; i < num_threads; i++) {
    e2b_t += e2b_pool[i];
    edisp_t += edisp_pool[i];
  }


# ifdef DEBUG
  std::cerr << "disp = " << edisp_t << "    2b = " << e2b_t << std::endl;
# endif

  return e2b_t + edisp_t;
}

double System::ThreeBodyEnergy(bool do_grads) {
  energy_ = 0.0;
  std::fill(grad_.begin(), grad_.end(), 0.0);

  energy_ = Get3B(do_grads);

  return energy_;
}

double System::Get3B(bool do_grads) {
  // 3B ENERGY
  double e3b_t = 0.0;

  // Variables needed for OMP
  size_t step = 1;
  int num_threads = 1;
  int grad_step = 3*nsites_;
  
# ifdef _OPENMP
# pragma omp parallel
{
  if (omp_get_thread_num() == 0)
    num_threads = omp_get_num_threads();
}
  grad_step = 3*nsites_ / num_threads;
  step = std::max(size_t(1),std::min(nmon_/num_threads,step));
# endif // _OPENMP

  // trimers are ordered
  size_t first_grad = 0;
  size_t last_grad = 3*nsites_;
  int rank = 0;
  std::vector<double> e3b_pool(num_threads,0.0);
  std::vector<std::vector<double>> grad_pool(num_threads,std::vector<double>(3*nsites_,0.0));

# ifdef _OPENMP
# pragma omp parallel for schedule(dynamic) private(rank)
# endif // _OPENMP
  for (size_t istart = 0; istart < nmon_; istart += step) {
#   ifdef _OPENMP
    rank = omp_get_thread_num();
#   endif
    size_t iend = std::min(istart + step, nmon_);

    // Adding corresponding clusters      
#   ifdef _OPENMP
    std::vector<size_t> trimers = 
                       AddClustersParallel(3,cutoff3b_,istart,iend);
#   else 
    AddClusters(3,cutoff3b_,istart,iend);
    std::vector<size_t> trimers = trimers_;
#   endif

    if (trimers.size() < 3) {
      continue;
    }

    std::vector<double> coord1;
    std::vector<double> coord2;
    std::vector<double> coord3;
    std::string m1 = monomers_[trimers[0]];
    std::string m2 = monomers_[trimers[1]];
    std::string m3 = monomers_[trimers[2]];

    size_t i = 0;
    size_t nt = 0;
    size_t nt_tot = 0;

    while (3*nt_tot < trimers.size()) {
      i = (nt_tot + nt) * 3;
      if (monomers_[trimers[i]] == m1 &&
          monomers_[trimers[i + 1]] == m2 && 
          monomers_[trimers[i + 2]] == m3)  {
         // Push the coordinates
        for (size_t j = 0; j < 3*nat_[trimers[i]]; j++) {
          coord1.push_back(xyz_[3*first_index_[trimers[i]] + j]);
        }
        for (size_t j = 0; j < 3*nat_[trimers[i + 1]]; j++) {
          coord2.push_back(xyz_[3*first_index_[trimers[i + 1]] + j]);
        }
        for (size_t j = 0; j < 3*nat_[trimers[i + 2]]; j++) {
          coord3.push_back(xyz_[3*first_index_[trimers[i + 2]] + j]);
        }
        nt++;
      }

      // If one of the monomers is different as the previous one
      // since trimers are also ordered, means that no more trimers of that
      // type exist. Thus, do calculation, update m? and clear xyz
      if (monomers_[trimers[i]] != m1 ||
          monomers_[trimers[i + 1]] != m2 ||
          monomers_[trimers[i + 2]] != m3 ||
          i == trimers.size() - 3 || nt == maxNTriEval_) {
        if (nt == 0) {
          coord1.clear();
          coord2.clear();
          coord3.clear();
          m1 = monomers_[trimers[i]];
          m2 = monomers_[trimers[i + 1]];
          m3 = monomers_[trimers[i + 2]];
          continue;
        }

        // Fix dimer positions if pbc
        if (use_pbc_) {
          systools::GetCloseTrimerImage(box_, nat_[trimers[nt_tot * 3]],
                             nat_[trimers[nt_tot * 3 + 1]],
                             nat_[trimers[nt_tot * 3 + 2]],
                             nt, coord1.data(), coord2.data(), coord3.data());
        }

        std::vector<double> xyz1(coord1.size(),0.0);
        std::vector<double> xyz2(coord2.size(),0.0);
        std::vector<double> xyz3(coord3.size(),0.0);
        std::copy(coord1.begin(),coord1.end(),xyz1.begin());
        std::copy(coord2.begin(),coord2.end(),xyz2.begin());
        std::copy(coord3.begin(),coord3.end(),xyz3.begin());

        if (do_grads) {
          // POLYNOMIALS
          std::vector<double> grad1(coord1.size(),0.0);
          std::vector<double> grad2(coord2.size(),0.0);
          std::vector<double> grad3(coord3.size(),0.0);
          // POLYNOMIALS
          e3b_pool[rank] += e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3, grad1, grad2, grad3);

          size_t i0 = nt_tot * 3;
          for (size_t k = 0; k < nt ; k++) {
            for (size_t j = 0; j < 3*nat_[trimers[i0 + 3*k]]; j++) {
              grad_pool[rank][3*first_index_[trimers[i0 + 3*k]] + j]
                  += grad1[k*3*nat_[trimers[i0 + 3*k]] + j];
            }
            for (size_t j = 0; j < 3*nat_[trimers[i0 + 3*k + 1]]; j++) {
              grad_pool[rank][3*first_index_[trimers[i0 + 3*k + 1]] + j]
                  += grad2[k*3*nat_[trimers[i0 + 3*k + 1]] + j];
            }
            for (size_t j = 0; j < 3*nat_[trimers[i0 + 3*k + 2]]; j++) {
              grad_pool[rank][3*first_index_[trimers[i0 + 3*k + 2]] + j]
                  += grad3[k*3*nat_[trimers[i0 + 3*k + 2]] + j];
            }
          }
        } else {
          // POLYNOMIALS
          e3b_pool[rank] += e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3);
        }
        nt_tot += nt;
        nt = 0;
        coord1.clear();
        coord2.clear();
        coord3.clear();
        m1 = monomers_[trimers[i]];
        m2 = monomers_[trimers[i + 1]];
        m3 = monomers_[trimers[i + 2]];
      }
    }
  }

# ifdef _OPENMP
# pragma omp parallel private(first_grad,last_grad,rank)
{
  rank = omp_get_thread_num();

  first_grad = 0 + rank * grad_step;

  last_grad = (rank + 1) * grad_step;
  if (rank == num_threads - 1) {
    last_grad = 3*nsites_;
  }
# pragma omp barrier
# endif

  for (size_t i = 0; i < num_threads; i++) {
    for (size_t j = first_grad; j < last_grad; j++) {
      grad_[j] += grad_pool[i][j];
    }
  }

# ifdef _OPENMP
} // parallel   
# endif

  for (size_t i = 0; i < num_threads; i++) {
    e3b_t += e3b_pool[i];
  }

  return e3b_t;
}

////////////////////////////////////////////////////////////////////////////////

void System::SetCharges() {
  size_t fi_mon = 0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    std::string mon = mon_type_count_[k].first;
    size_t nmon = mon_type_count_[k].second;
    size_t nsites = sites_[fi_mon];
    
    systools::SetCharges(xyz_, chg_, mon, nmon, nsites, 
                first_index_[fi_mon], chggrad_);
    fi_mon += nmon;
  }
}

////////////////////////////////////////////////////////////////////////////////

void System::SetPols() {
  size_t fi_mon = 0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    std::string mon = mon_type_count_[k].first;
    size_t nmon = mon_type_count_[k].second;
    size_t nsites = sites_[fi_mon];
    
    systools::SetPol(pol_, mon, nmon, nsites, first_index_[fi_mon]);
    fi_mon += nmon;
  }
}

////////////////////////////////////////////////////////////////////////////////

void System::SetPolfacs() {
  size_t fi_mon = 0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    std::string mon = mon_type_count_[k].first;
    size_t nmon = mon_type_count_[k].second;
    size_t nsites = sites_[fi_mon];

    systools::SetPolfac(polfac_, mon, nmon, nsites, first_index_[fi_mon]);
    fi_mon += nmon;
  }
}

////////////////////////////////////////////////////////////////////////////////

void System::SetVSites() {
  size_t fi_mon = 0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    std::string mon = mon_type_count_[k].first;
    size_t nmon = mon_type_count_[k].second;
    size_t nsites = sites_[fi_mon];

    systools::SetVSites(xyz_, mon, nmon, nsites, first_index_[fi_mon]);
    fi_mon += nmon;
  }
}

////////////////////////////////////////////////////////////////////////////////

double System::GetElectrostatics(bool do_grads) {
  electrostaticE_.SetXyzChgPolPolfac(xyz_, chg_, chggrad_, 
                                     pol_, polfac_, dipole_method_, do_grads);
  return electrostaticE_.GetElectrostatics(grad_);
}

////////////////////////////////////////////////////////////////////////////////

void System::ResetDipoleHistory() {
  electrostaticE_.ResetAspcHistory();
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////






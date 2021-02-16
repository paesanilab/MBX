/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "system.h"

//#define DEBUG
//#define TIMING
//#define PRINT_INDIVIDUAL_TERMS

#ifdef TIMING
#include <chrono>
#include <iostream>
#endif

/**
 * @file system.cpp
 * @brief System class implementation
 */

////////////////////////////////////////////////////////////////////////////////

namespace bblock {  // Building Block :: System

// std::unordered_map<std::string, eff::Conn> bblock::System::connectivity_map_;

////////////////////////////////////////////////////////////////////////////////

System::System() {
    initialized_ = false;
    monomer_json_read_ = false;
    mpi_initialized_ = false;
    simcell_periodic_ = false;

    // Define some of the parameters

    /////////////
    // CUTOFFS //
    /////////////

    // Setting 2B cutoff
    // Affects the 2B dispersion and 2B polynomials
    // TODO make it effective for electrostatics too
    cutoff2b_ = 100.0;

    // Setting 3B cutoff
    // Affects the 3B polynomials
    cutoff3b_ = 5.0;

    ////////////////////////
    // Evaluation batches //
    ////////////////////////

    // Maximum number in the batch for the 1B evaluation
    maxNMonEval_ = 1024;
    // Maximum number in the batch for the 2B evaluation
    maxNDimEval_ = 1024;
    // Maximum number in the batch for the 3B evaluation
    maxNTriEval_ = 1024;

    // Setting dipole tolerance to a consrvative value
    // TODO make it be error/dipole, not total error as it is now
    diptol_ = 1E-16;
    // Sets the maximum number of iteartions in the induced dipole
    // calculation. Will assume no convergence if this number is reached
    maxItDip_ = 100;
    // Sets the default method to calculate induced dipoles to ASPC
    dipole_method_ = "cg";

    // Sets default ewald parameters for elec, disp and lj
    elec_alpha_ = disp_alpha_ = lj_alpha_ = 0.0;
    elec_grid_density_ = disp_grid_density_ = lj_grid_density_ = 2.5;
    elec_spline_order_ = disp_spline_order_ = lj_spline_order_ = 6;

    // Define the virial vector
    virial_ = std::vector<double>(9, 0.0);

    grid_fftdim_elec_ = std::vector<int>{};
    grid_fftdim_disp_ = std::vector<int>{};
    grid_fftdim_lj_ = std::vector<int>{};
}
System::~System() {}

size_t System::GetNumMol() { return nummol; }
size_t System::GetNumMon() { return nummon_; }
size_t System::GetNumSites() { return numsites_; }
size_t System::GetNumRealSites() { return numat_; }

size_t System::GetMonNumAt(size_t n) { return nat_[original2current_order_[n]]; }

std::vector<size_t> System::GetMonNumAt() {
    std::vector<size_t> monnumat(nat_.size(), 0);
    for (size_t i = 0; i < nat_.size(); i++) {
        monnumat[i] = nat_[original2current_order_[i]];
    }
    return monnumat;
}

size_t System::GetFirstInd(size_t n) {
    // Obtain position in system of monomer in position
    // 'n' in the original order
    size_t current_pos = original2current_order_[n];

    // Get the first index that this monomer had in the input order
    return initial_order_[current_pos].second;
}

std::vector<size_t> System::GetPairList(size_t nmax, double cutoff, size_t istart, size_t iend) {
    // Make sure that nmax is 2 or 3
    // Throw exception otherwise
    if (nmax != 2 and nmax != 3) {
        std::string text = "nmax value of " + std::to_string(nmax) + " is not acceptable. Possible values are 2 or 3.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Call the add clusters function to get all the pairs
    AddClusters(nmax, cutoff, 0, monomers_.size());

    // Change the monomer indexes of dimers_ or trimers_
    // to match the input order
    // Select also the pairs that we are interested on

    std::vector<size_t> pair_list;

    if (nmax == 2) {
        // Loop over all the dimers
        for (size_t i = 0; i < dimers_.size(); i += nmax) {
            // Get the initial order of each monomer in the dimer
            size_t mon1 = initial_order_[dimers_[i]].first;
            size_t mon2 = initial_order_[dimers_[i + 1]].first;
            // Add the dimers within the range [istart,iend)
            if ((mon1 >= istart && mon1 < iend) || (mon2 >= istart && mon2 < iend)) {
                pair_list.push_back(mon1);
                pair_list.push_back(mon2);
            }
        }
    } else if (nmax == 3) {
        // Loop over all the trimers
        for (size_t i = 0; i < trimers_.size(); i += nmax) {
            // Get the initial order of each monomer in the trimer
            size_t mon1 = initial_order_[trimers_[i]].first;
            size_t mon2 = initial_order_[trimers_[i + 1]].first;
            size_t mon3 = initial_order_[trimers_[i + 2]].first;
            // Add the dimers within the range [istart,iend)
            if ((mon1 >= istart && mon1 < iend) || (mon2 >= istart && mon2 < iend) || (mon3 >= istart && mon3 < iend)) {
                pair_list.push_back(mon1);
                pair_list.push_back(mon2);
                pair_list.push_back(mon3);
            }
        }
    }

    return pair_list;
}

void System::GetAtomMonIndex(std::vector<size_t> &original_atom_index_to_original_mon_index,
                             std::vector<std::string> &original_atom_index_to_original_mon_id) {
    original_atom_index_to_original_mon_index = std::vector<size_t>(numat_, 0);
    original_atom_index_to_original_mon_id = std::vector<std::string>(numat_, 0);
    for (size_t i = 0; i < nat_.size(); i++) {
        size_t original_index = initial_order_realSites_[i].first;
        for (size_t j = 0; j < nat_[i]; j++) {
            original_atom_index_to_original_mon_index[original_index + j] = i;
            original_atom_index_to_original_mon_id[original_index + j] = monomers_[i];
        }
    }
}

std::vector<size_t> System::GetMolecule(size_t n) { return molecules_[n]; }

std::vector<std::string> System::GetAtomNames() {
    return systools::ResetOrderN(atoms_, initial_order_, first_index_, sites_);
}

std::vector<std::string> System::GetRealAtomNames() {
    return systools::ResetOrderRealN(atoms_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::vector<double> System::GetXyz() { return systools::ResetOrder3N(xyz_, initial_order_, first_index_, sites_); }

std::vector<double> System::GetRealXyz() {
    SetPBC(box_);
    return systools::ResetOrderReal3N(xyz_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::vector<double> System::GetGrads() { return systools::ResetOrder3N(grad_, initial_order_, first_index_, sites_); }

std::vector<double> System::GetRealGrads() {
    return systools::ResetOrderReal3N(grad_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::vector<double> System::GetCharges() { return systools::ResetOrderN(chg_, initial_order_, first_index_, sites_); }

std::vector<double> System::GetRealCharges() {
    return systools::ResetOrderRealN(chg_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::vector<double> System::GetPolarizabilities() {
    return systools::ResetOrderN(pol_, initial_order_, first_index_, sites_);
}

std::vector<double> System::GetRealPolarizabilities() {
    return systools::ResetOrderRealN(pol_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::vector<double> System::GetPolarizabilityFactors() {
    return systools::ResetOrderN(polfac_, initial_order_, first_index_, sites_);
}

std::vector<double> System::GetRealPolarizabilityFactors() {
    return systools::ResetOrderRealN(pol_, initial_order_realSites_, numat_, first_index_, nat_);
}

std::string System::GetMonId(size_t n) {
    size_t current_pos = original2current_order_[n];
    return monomers_[current_pos];
}

std::vector<std::string> System::GetMonId() {
    std::vector<std::string> mon_ids(monomers_.size());
    for (size_t i = 0; i < monomers_.size(); i++) {
        mon_ids[i] = monomers_[original2current_order_[i]];
    }
    return mon_ids;
}

std::vector<double> System::GetVirial() { return virial_; }

std::vector<double> System::GetBox() { return box_; }

size_t System::GetMaxEval1b() { return maxNMonEval_; }

size_t System::GetMaxEval2b() { return maxNDimEval_; }

size_t System::GetMaxEval3b() { return maxNTriEval_; }

double System::GetDipoleTolerance() { return diptol_; }

std::string System::GetDipoleMethod() { return dipole_method_; }

size_t System::GetMaxIterationsDipoles() { return maxItDip_; }

void System::GetEwaldParamsElectrostatics(double &alpha, double &grid_density, size_t &spline_order) {
    alpha = elec_alpha_;
    grid_density = elec_grid_density_;
    spline_order = elec_spline_order_;
}

std::vector<int> System::GetFFTDimensionElectrostatics(int box_id) { return electrostaticE_.GetFFTDimension(box_id); }

std::vector<int> System::GetFFTDimensionDispersion(int box_id) { return dispersionE_.GetFFTDimension(box_id); }

std::vector<int> System::GetFFTDimensionLennardJones(int box_id) { return lennardJonesE_.GetFFTDimension(box_id); }

void System::SetFFTDimensionElectrostatics(std::vector<int> grid) { electrostaticE_.SetFFTDimension(grid); }

void System::SetFFTDimensionDispersion(std::vector<int> grid) { dispersionE_.SetFFTDimension(grid); }

void System::SetFFTDimensionLennardJones(std::vector<int> grid) { lennardJonesE_.SetFFTDimension(grid); }

void System::GetEwaldParamsDispersion(double &alpha, double &grid_density, size_t &spline_order) {
    alpha = disp_alpha_;
    grid_density = disp_grid_density_;
    spline_order = disp_spline_order_;
}

void System::GetEwaldParamsLennardJones(double &alpha, double &grid_density, size_t &spline_order) {
    alpha = lj_alpha_;
    grid_density = lj_grid_density_;
    spline_order = lj_spline_order_;
}

// FIXME As for today, these functions are not used. // MRR 20191022
// Will need to activate them and use them whenever we need them for MB-Spec
// void System::GetMolecularDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind) {
//    std::vector<double> tmp_perm = electrostaticE_.GetMolecularPermanentDipoles();
//    std::vector<double> tmp_ind = electrostaticE_.GetMolecularInducedDipoles();
//
//    mu_perm = std::vector<double>(tmp_perm.size(), 0.0);
//    mu_ind = std::vector<double>(tmp_ind.size(), 0.0);
//    // Reorder to match input order
//    for (size_t i = 0; i < nummon_; i++) {
//        size_t current_pos = original2current_order_[i];
//        for (size_t j = 0; j < 3; j++) {
//            mu_perm[3 * i + j] = tmp_perm[3 * current_pos + j];
//            mu_ind[3 * i + j] = tmp_ind[3 * current_pos + j];
//        }
//    }
//}
//
// void System::GetDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind) {
//    mu_perm = electrostaticE_.GetPermanentDipoles();
//    mu_ind = electrostaticE_.GetInducedDipoles();
//
//    systools::ResetOrderReal3N(mu_perm, initial_order_realSites_, numat_, first_index_, nat_);
//    systools::ResetOrderReal3N(mu_ind, initial_order_realSites_, numat_, first_index_, nat_);
//}
//
// void System::GetTotalDipole(std::vector<double> &mu_perm, std::vector<double> &mu_ind, std::vector<double> &mu_tot) {
//    std::vector<double> all_mu_perm = electrostaticE_.GetPermanentDipoles();
//    std::vector<double> all_mu_ind = electrostaticE_.GetInducedDipoles();
//
//    mu_perm = std::vector<double>(3, 0.0);
//    mu_ind = std::vector<double>(3, 0.0);
//
//    mu_tot = std::vector<double>(3, 0.0);
//
//    for (size_t i = 0; i < numsites_; i++) {
//        for (size_t j = 0; j < 3; j++) {
//            mu_perm[j] += all_mu_perm[3 * i + j];
//            mu_ind[j] += all_mu_ind[3 * i + j];
//        }
//    }
//    for (size_t j = 0; j < 3; j++) {
//        mu_tot[j] = mu_perm[j] + mu_ind[j];
//    }
//}
//
// std::vector<double> System::GetChargeDerivativesOHH() {
//    std::vector<double> chg_der(numat_ * numat_ * 3, 0.0);
//
//    size_t fi_chgder = 0;
//    size_t fi_mon = 0;
//    size_t fi_at = 0;
//    size_t row_length = numat_ * 3;
//    // Loop over all monomer types
//    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
//        size_t nmon = mon_type_count_[mt].second;
//        size_t nat = nat_[fi_mon];
//        // For each monomer, check if it is water. If it is not water, skip.
//        if (mon_type_count_[mt].first == "h2o") {
//            // chg_der will be 27*nwaters
//            // Loop over all water molecules
//            for (size_t m = 0; m < nmon; m++) {
//                size_t shift = 3 * fi_at + 3 * numat_ * fi_at;
//                // chg_der has the order H1 H2 O. We want O H1 H2 M
//                for (size_t u = 0; u < 3; u++) {
//                    chg_der[shift + u] = chggrad_[27 * m + 18 + 6 + u];     // dqo/dro
//                    chg_der[shift + 3 + u] = chggrad_[27 * m + 0 + 6 + u];  // dqo/drh1
//                    chg_der[shift + 6 + u] = chggrad_[27 * m + 9 + 6 + u];  // dqo/drh2
//
//                    chg_der[shift + row_length + u] = chggrad_[27 * m + 18 + 0 + u];     // dqh1/dro
//                    chg_der[shift + row_length + 3 + u] = chggrad_[27 * m + 0 + 0 + u];  // dqh1/drh1
//                    chg_der[shift + row_length + 6 + u] = chggrad_[27 * m + 9 + 0 + u];  // dqh1/drh2
//
//                    chg_der[shift + 2 * row_length + u] = chggrad_[27 * m + 18 + 3 + u];     // dqh2/dro
//                    chg_der[shift + 2 * row_length + 3 + u] = chggrad_[27 * m + 0 + 3 + u];  // dqh2/drh1
//                    chg_der[shift + 2 * row_length + 6 + u] = chggrad_[27 * m + 9 + 3 + u];  // dqh2/drh2
//                }
//                fi_at += nat;
//            }
//        }
//        fi_mon += nmon;
//    }
//    return chg_der;
//}
//
// std::vector<double> System::GetChargeDerivatives() {
//    std::vector<double> chg_der(numsites_ * numsites_ * 3, 0.0);
//
//    size_t fi_chgder = 0;
//    size_t fi_mon = 0;
//    size_t fi_sites = 0;
//    size_t row_length = numsites_ * 3;
//
//    double g21 = systools::gamma21;
//
//    // Loop over all monomer types
//    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
//        size_t nmon = mon_type_count_[mt].second;
//        size_t nsites = sites_[fi_mon];
//        // For each monomer, check if it is water. If it is not water, skip.
//        if (mon_type_count_[mt].first == "h2o") {
//            // chg_der will be 27*nwaters
//            // Loop over all water molecules
//            for (size_t m = 0; m < nmon; m++) {
//                size_t shift = 3 * fi_sites + 3 * numsites_ * fi_sites;
//                // chg_der has the order H1 H2 O. We want O H1 H2 M
//                for (size_t u = 0; u < 3; u++) {
//                    chg_der[shift + u] = 0.0;      // dqo/dro
//                    chg_der[shift + 3 + u] = 0.0;  // dqo/drh1
//                    chg_der[shift + 6 + u] = 0.0;  // dqo/drh2
//                    chg_der[shift + 9 + u] = 0.0;  // dqo/drM
//
//                    chg_der[shift + row_length + u] = 0.0;  // dqh1/dro
//                    chg_der[shift + row_length + 3 + u] =
//                        chggrad_[27 * m + u] + g21 * (chggrad_[27 * m + u] + chggrad_[27 * m + 3 + u]);  // dqh1/drh1
//                    chg_der[shift + row_length + 6 + u] =
//                        chggrad_[27 * m + 9 + u] +
//                        g21 * (chggrad_[27 * m + 9 + u] + chggrad_[27 * m + 9 + 3 + u]);  // dqh1/drh2
//                    chg_der[shift + row_length + 9 + u] =
//                        chggrad_[27 * m + 18 + u] +
//                        g21 * (chggrad_[27 * m + 18 + u] + chggrad_[27 * m + 18 + 3 + u]);  // dqh1/drM
//
//                    chg_der[shift + 2 * row_length + u] = 0.0;  // dqh2/dro
//                    chg_der[shift + 2 * row_length + 3 + u] =
//                        chggrad_[27 * m + 3 + u] +
//                        g21 * (chggrad_[27 * m + u] + chggrad_[27 * m + 3 + u]);  // dqh2/drh1
//                    chg_der[shift + 2 * row_length + 6 + u] =
//                        chggrad_[27 * m + 9 + 3 + u] +
//                        g21 * (chggrad_[27 * m + 9 + u] + chggrad_[27 * m + 9 + 3 + u]);  // dqh2/drh2
//                    chg_der[shift + 2 * row_length + 9 + u] =
//                        chggrad_[27 * m + 18 + 3 + u] +
//                        g21 * (chggrad_[27 * m + 18 + u] + chggrad_[27 * m + 18 + 3 + u]);  // dqh2/drM
//
//                    chg_der[shift + 3 * row_length + u] = 0.0;  // dqM/dro
//                    chg_der[shift + 3 * row_length + 3 + u] =
//                        chggrad_[27 * m + 6 + u] -
//                        2 * g21 * (chggrad_[27 * m + u] + chggrad_[27 * m + 3 + u]);  // dqM/drh1
//                    chg_der[shift + 3 * row_length + 6 + u] =
//                        chggrad_[27 * m + 9 + 6 + u] -
//                        2 * g21 * (chggrad_[27 * m + 9 + u] + chggrad_[27 * m + 9 + 3 + u]);  // dqM/drh2
//                    chg_der[shift + 3 * row_length + 9 + u] =
//                        chggrad_[27 * m + 18 + 6 + u] -
//                        2 * g21 * (chggrad_[27 * m + 18 + u] + chggrad_[27 * m + 18 + 3 + u]);  // dqM/drM
//                }
//                fi_sites += nsites;
//            }
//        }
//        fi_mon += nmon;
//    }
//
//    // FIXME
//    //    for (size_t i = 0; i < numsites_ ; i++) {
//    //        for (size_t j = 0; j < 3*numsites_; j++) {
//    //            std::cout << std::scientific << std::setprecision(4) << std::setw(14) << chg_der[3*numsites_*i + j];
//    //        }
//    //        std::cout << std::endl;
//    //    }
//
//    return chg_der;
//}

void System::Set2bCutoff(double cutoff2b) { cutoff2b_ = cutoff2b; }
void System::Set3bCutoff(double cutoff3b) { cutoff3b_ = cutoff3b; }
double System::Get2bCutoff() { return cutoff2b_; }
double System::Get3bCutoff() { return cutoff3b_; }
void System::SetNMaxEval1b(size_t nmax) { maxNMonEval_ = nmax; }
void System::SetNMaxEval2b(size_t nmax) { maxNDimEval_ = nmax; }
void System::SetNMaxEval3b(size_t nmax) { maxNTriEval_ = nmax; }

void System::SetDipoleTol(double tol) { diptol_ = tol; }
void System::SetDipoleMaxIt(size_t maxit) { maxItDip_ = maxit; }
void System::SetDipoleMethod(std::string method) { dipole_method_ = method; }

void System::SetPBC(std::vector<double> box) {
    // Check that the box has 0 or 9 components
    if (box.size() != 9 && box.size() != 0) {
        std::string text = "Box size of " + std::to_string(box.size()) + " is not acceptable.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

#ifdef DEBUG
    std::cerr << "Entered SetPBC():\n";
    std::cerr << "Coordinate before fixing monomers:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    // Set the box and the bool to use or not pbc
    use_pbc_ = box.size();
    if (use_pbc_) simcell_periodic_ = true;

    box_ = box;
    if (box.size() == 9) {
        box_ = box;
        box_ABCabc_ = BoxVecToBoxABCabc(box);
    } else if (box.size() == 6) {
        box_ABCabc_ = box;
        box_ = BoxABCabcToBoxVec(box);
    }

    box_inverse_ = InvertUnitCell(box_);

    // If we use PBC, we need to make sure that the monomer atoms are all
    // close to the central atom (1st atom of each monomer)
    if (use_pbc_) {
        // Fix monomer coordinates
        systools::FixMonomerCoordinates(xyz_, box_, box_inverse_, nat_, first_index_);
    }

#ifdef DEBUG
    std::cerr << "Coordinate after fixing monomers:\n";
    for (size_t i = 0; i < xyz_.size(); i++) {
        std::cerr << xyz_[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    // Reset the virtual site positions, charges, pols and polfacs
    SetVSites();
    SetCharges();
    SetPols();
    SetPolfacs();
}

void System::SetXyz(std::vector<double> xyz) {
    // Make sure that the xyz of input has the right size
    if (xyz.size() != 3 * numsites_) {
        std::string text =
            "Sizes " + std::to_string(xyz.size()) + " and " + std::to_string(3 * numsites_) + " don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy each coordinate in the apropriate place in the internal
    // xyz vector
    for (size_t i = 0; i < sites_.size(); i++) {
        size_t ini = 3 * initial_order_[i].second;
        size_t fin = ini + 3 * sites_[i];
        size_t ini_new = 3 * first_index_[i];
        std::copy(xyz.begin() + ini, xyz.begin() + fin, xyz_.begin() + ini_new);
    }
}

void System::SetRealXyz(std::vector<double> xyz) {
    // Make sure that the xyz of input has the right size
    if (xyz.size() != 3 * numat_) {
        std::string text =
            "Sizes " + std::to_string(xyz.size()) + " and " + std::to_string(3 * numat_) + " don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy each coordinate in the apropriate place in the internal
    // xyz vector
    for (size_t i = 0; i < nat_.size(); i++) {
        size_t ini = 3 * initial_order_realSites_[i].second;
        size_t fin = ini + 3 * nat_[i];
        size_t ini_new = 3 * first_index_[i];
        std::copy(xyz.begin() + ini, xyz.begin() + fin, xyz_.begin() + ini_new);
    }
}

void System::AddMonomer(std::vector<double> xyz, std::vector<std::string> atoms, std::string id, size_t islocal,
                        int tag) {
    // If the system has been initialized, adding a monomer is not possible
    if (initialized_) {
        std::string text = std::string("The system has already been initialized. ") +
                           std::string("Adding a new monomer is not possible");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Adding coordinates
    for (auto i = xyz.begin(); i != xyz.end(); i++) xyz_.push_back(*i);
    // Adding atom names
    for (auto i = atoms.begin(); i != atoms.end(); i++) atoms_.push_back(*i);
    // Adding id
    monomers_.push_back(id);
    // Adding local/ghost descriptor
    islocal_.push_back(islocal);
    // Adding unique ids
    for (int i = 0; i < atoms.size(); ++i) atom_tag_.push_back(tag + i);
}

void System::AddMolecule(std::vector<size_t> molec) { molecules_.push_back(molec); }

std::vector<std::pair<std::string, std::string>> System::GetTTMnrgPairs() { return buck_pairs_; }

std::vector<std::string> System::GetFFMons() { return ff_mons_; }

std::vector<std::string> System::Get1bIgnorePoly() { return ignore_1b_poly_; }

std::vector<std::vector<std::string>> System::Get2bIgnorePoly() { return ignore_2b_poly_; }

std::vector<std::vector<std::string>> System::Get3bIgnorePoly() { return ignore_3b_poly_; }

void System::AddTTMnrgPair(std::string mon1, std::string mon2) {
    std::pair<std::string, std::string> p = mon2 < mon1 ? std::make_pair(mon2, mon1) : std::make_pair(mon1, mon2);

    if (std::find(buck_pairs_.begin(), buck_pairs_.end(), p) == buck_pairs_.end()) {
        buck_pairs_.push_back(p);
    }
}

void System::SetTTMnrgPairs(std::vector<std::pair<std::string, std::string>> ttm_pairs) {
    buck_pairs_.clear();

    for (auto it = ttm_pairs.begin(); it != ttm_pairs.end(); it++) {
        std::string s1 = (*it).first;
        std::string s2 = (*it).second;
        std::pair<std::string, std::string> p = s2 < s1 ? std::make_pair(s2, s1) : std::make_pair(s1, s2);
        buck_pairs_.push_back(p);
    }
}

void System::SetLennardJonesPairs(std::vector<std::pair<std::string, std::string>> use_lennard_jones) {
    lj_pairs_.clear();

    for (auto it = use_lennard_jones.begin(); it != use_lennard_jones.end(); it++) {
        std::string s1 = (*it).first;
        std::string s2 = (*it).second;
        std::pair<std::string, std::string> p = s2 < s1 ? std::make_pair(s2, s1) : std::make_pair(s1, s2);
        lj_pairs_.push_back(p);
    }
}

void System::SetIgnoreDispersionPairs(std::vector<std::pair<std::string, std::string>> ignore_dispersion) {
    ignore_disp_.clear();

    for (auto it = ignore_dispersion.begin(); it != ignore_dispersion.end(); it++) {
        std::string s1 = (*it).first;
        std::string s2 = (*it).second;
        std::pair<std::string, std::string> p = s2 < s1 ? std::make_pair(s2, s1) : std::make_pair(s1, s2);
        ignore_disp_.push_back(p);
    }
}

void System::SetFFMons(std::vector<std::string> ff_mons) { ff_mons_ = ff_mons; }

void System::AddFFMon(std::string mon) {
    if (std::find(ff_mons_.begin(), ff_mons_.end(), mon) == ff_mons_.end()) {
        ff_mons_.push_back(mon);
    }
}

void System::Add1bIgnorePoly(std::string mon) {
    if (std::find(ignore_1b_poly_.begin(), ignore_1b_poly_.end(), mon) == ignore_1b_poly_.end()) {
        ignore_1b_poly_.push_back(mon);
    }
}

void System::Set1bIgnorePoly(std::vector<std::string> ignore_1b) { ignore_1b_poly_ = ignore_1b; }

void System::Add2bIgnorePoly(std::string mon1, std::string mon2) {
    std::vector<std::string> p = {mon1, mon2};
    std::sort(p.begin(), p.end());

    if (std::find(ignore_2b_poly_.begin(), ignore_2b_poly_.end(), p) == ignore_2b_poly_.end()) {
        ignore_2b_poly_.push_back(p);
    }
}

void System::Set2bIgnorePoly(std::vector<std::vector<std::string>> ignore_2b) {
    ignore_2b_poly_.clear();

    for (size_t i = 0; i < ignore_2b.size(); i++) {
        std::vector<std::string> p = ignore_2b[i];
        std::sort(p.begin(), p.end());
        ignore_2b_poly_.push_back(p);
    }
}

void System::Add3bIgnorePoly(std::string mon1, std::string mon2, std::string mon3) {
    std::vector<std::string> p = {mon1, mon2, mon3};
    std::sort(p.begin(), p.end());

    if (std::find(ignore_3b_poly_.begin(), ignore_3b_poly_.end(), p) == ignore_3b_poly_.end()) {
        ignore_3b_poly_.push_back(p);
    }
}

void System::Set3bIgnorePoly(std::vector<std::vector<std::string>> ignore_3b) {
    ignore_3b_poly_.clear();

    for (size_t i = 0; i < ignore_3b.size(); i++) {
        std::vector<std::string> p = ignore_3b[i];
        std::sort(p.begin(), p.end());
        ignore_3b_poly_.push_back(p);
    }
}

void System::Initialize() {
    // If we try to reinitialize the system, we will get an exception
    if (initialized_) {
        std::string text =
            std::string("The system has already been initialized. ") + std::string("Reinitialization is not possible");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Do not attempt to initiaize if monomers json file has not been read.
    if (!monomer_json_read_) return;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cout << std::scientific << std::setprecision(10);
#endif

    /////////////////////////////
    // Add monomer information //
    /////////////////////////////

    // Retrieves all the monomer information given the coordinates
    // and monomer id, such as number of sites, and orders the monomers
    if (xyz_.size() > 0) {
        AddMonomerInfo();
    }

    // Setting the number of molecules and number of monomers
    if (xyz_.size() == 0) {
        numat_ = 0;
        nummol = 0;
        nummon_ = 0;
    } else {
        nummol = molecules_.size();
        nummon_ = monomers_.size();
    }

    // Setting PBC to false by default
    SetPBC(box_);

    // Set C6 for long range pme
    SetC6LongRange();

    // Set LJ for long range pme
    SetLJLongRange();

    // With the information previously set, we initialize the
    // electrostatics class
    if (mpi_initialized_) electrostaticE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
    electrostaticE_.Initialize(chg_, chggrad_, polfac_, pol_, xyz_, monomers_, sites_, first_index_, mon_type_count_,
                               islocal_, atom_tag_, true, diptol_, maxItDip_, dipole_method_);

    // And the dispersion class
    std::vector<double> xyz_real = numat_ == 0 ? xyz_ : GetRealXyz();
    if (mpi_initialized_) dispersionE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
    dispersionE_.Initialize(c6_lr_, xyz_real, monomers_, nat_, mon_type_count_, islocal_, true, box_);

    // And the Lennard-Jones class
    if (mpi_initialized_) lennardJonesE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
    lennardJonesE_.Initialize(lj_lr_, xyz_real, monomers_, nat_, mon_type_count_, islocal_, true, box_);

    // And buckingham class
    if (numat_ > 0) {
        buckinghamE_.Initialize(xyz_real, monomers_, nat_, mon_type_count_, enforce_ttm_for_idx_, islocal_, true, box_);
    }

    // We are done. Setting initialized_ to true
    initialized_ = true;
}

// void System::InitializePME() {
//    // If we try to reinitialize the system, we will get an exception
//    if (initialized_) {
//        std::string text =
//            std::string("The system has already been initialized. ") + std::string("Reinitialization is not
//            possible");
//        throw CUException(__func__, __FILE__, __LINE__, text);
//    }
//
//#ifdef DEBUG
//    std::cerr << std::scientific << std::setprecision(10);
//    std::cout << std::scientific << std::setprecision(10);
//#endif
//
//    // Retrieves all the monomer information given the coordinates
//    // and monomer id, such as number of sites, and orders the monomers
//    numat_ = 0;
//    // AddMonomerInfo();
//
//    // Setting the number of molecules and number of monomers
//    nummol = 0;
//    nummon_ = 0;
//
//    // Setting PBC to false by default
//    SetPBC(box_);
//
//    // Set C6 for long range pme
//    SetC6LongRange();
//
//    // Set LJ for long range pme
//    SetLJLongRange();
//    // With the information previously set, we initialize the
//    // electrostatics class
//    if (mpi_initialized_) electrostaticE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
//    electrostaticE_.Initialize(chg_, chggrad_, polfac_, pol_, xyz_, monomers_, sites_, first_index_, mon_type_count_,
//                               islocal_, atom_tag_, true, diptol_, maxItDip_, dipole_method_);
//
//    // std::vector<double> xyz_real = GetRealXyz();
//    std::vector<double> xyz_real = {};
//    if (mpi_initialized_) dispersionE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
//    dispersionE_.Initialize(c6_lr_, xyz_real, monomers_, nat_, mon_type_count_, islocal_, true, box_);
//
//    if (mpi_initialized_) lennardJonesE_.SetMPI(world_, proc_grid_x_, proc_grid_y_, proc_grid_z_);
//    lennardJonesE_.Initialize(lj_lr_, xyz_real, monomers_, nat_, mon_type_count_, islocal_, true, box_);
//    // buckinghamE_.Initialize(xyz_real, monomers_, nat_, mon_type_count_, islocal_, true, box_);
//
//    // We are done. Setting initialized_ to true
//    initialized_ = true;
//}

void System::SetUpFromJsonDispersionRepulsion(char *json_file) {
    nlohmann::json j_default = {};
    std::ifstream ifjson;
    nlohmann::json j;
    if (json_file != 0 && std::string(json_file) != "") {
        try {
            ifjson.open(json_file);
            j = nlohmann::json::parse(ifjson);
        } catch (...) {
            j = j_default;
            std::cerr << "There has been a problem loading your json file: " + std::string(json_file) +
                             "... using defaults";
        }
    } else {
        j = j_default;
    }

    SetUpFromJsonDispersionRepulsion(j);

    ifjson.close();
}

void System::SetUpFromJsonMonomers(char *json_file) {
    nlohmann::json j_default = {};
    std::ifstream ifjson;
    nlohmann::json j;
    if (json_file != 0 && std::string(json_file) != "") {
        try {
            ifjson.open(json_file);
            j = nlohmann::json::parse(ifjson);
        } catch (...) {
            j = j_default;
            std::cerr << "There has been a problem loading your json file: " + std::string(json_file) +
                             "... using defaults";
        }
    } else {
        j = j_default;
    }

    SetUpFromJsonMonomers(j);

    ifjson.close();
}

void SetUpFromJsonDispersionRepulsion(std::string json_text) {
    nlohmann::json j = nlohmann::json::parse(json_text);
    SetUpFromJsonDispersionRepulsion(j);
}

void SetUpFromJsonMonomers(std::string json_text) {
    nlohmann::json j = nlohmann::json::parse(json_text);
    SetUpFromJsonMonomers(j);
}

void System::SetUpFromJsonDispersionRepulsion(nlohmann::json j) {
    repdisp_j_ = j;
    dispersionE_.SetJsonDispersionRepulsion(repdisp_j_);
    buckinghamE_.SetJsonDispersionRepulsion(repdisp_j_);
    lennardJonesE_.SetJsonLennardJones(repdisp_j_);
}

void System::SetUpFromJsonMonomers(nlohmann::json j) {
    monomers_j_ = j;
    monomer_json_read_ = true;
#if 0
    if (isPME_) {
        InitializePME();
    } else {
        Initialize();
    }
#else
    Initialize();
#endif
    electrostaticE_.SetJsonMonomers(monomers_j_);
    dispersionE_.SetJsonMonomers(monomers_j_);
    buckinghamE_.SetJsonMonomers(monomers_j_);
    lennardJonesE_.SetJsonMonomers(monomers_j_);
}

void System::SetUpFromJson(nlohmann::json j) {
    // Try to get box
    // Default at initialization: no box (empty vector)
    try {
        std::vector<double> box = j["MBX"]["box"];
        box_ = box;
    } catch (...) {
        // if (mpi_rank_ == 0) std::cerr << "**WARNING** \"box\" is not defined in json file. Using empty box.\n";
    }

    if (box_.size() == 9) {
        box_ABCabc_ = BoxVecToBoxABCabc(box_);
    } else if (box_.size() == 6) {
        box_ABCabc_ = box_;
        box_ = BoxABCabcToBoxVec(box_ABCabc_);
    }

    mbx_j_["MBX"]["box"] = box_;

    // Try to get FFT grid for electrostatics
    // Default at initialization: no user-specified grid (empty vector)
    try {
        std::vector<int> grid_fftdim_elec = j["MBX"]["grid_fftdim_elec"];
        grid_fftdim_elec_ = grid_fftdim_elec;
    } catch (...) {
        // if(mpi_rank_ == 0) std::cerr << "**WARNING** \"grid_fftdim_elec\" is not defined in json file.\n";
    }

    mbx_j_["MBX"]["grid_fftdim_elec"] = grid_fftdim_elec_;

    SetFFTDimensionElectrostatics(grid_fftdim_elec_);

    // Try to get FFT grid for dispersion
    // Default: no user-specified grid (empty vector)
    try {
        std::vector<int> grid_fftdim_disp = j["MBX"]["grid_fftdim_disp"];
        grid_fftdim_disp_ = grid_fftdim_disp;
    } catch (...) {
        // if(mpi_rank_ == 0) std::cerr << "**WARNING** \"grid_fftdim_disp\" is not defined in json file.\n";
    }

    mbx_j_["MBX"]["grid_fftdim_disp"] = grid_fftdim_disp_;

    SetFFTDimensionDispersion(grid_fftdim_disp_);

    // Try to get FFT grid for LJ
    // Default: no user-specified grid (empty vector)
    try {
        std::vector<int> grid_fftdim_lj = j["MBX"]["grid_fftdim_lj"];
        grid_fftdim_lj_ = grid_fftdim_lj;
    } catch (...) {
        // if(mpi_rank_ == 0) std::cerr << "**WARNING** \"grid_fftdim_lj\" is not defined in json file.\n";
    }

    mbx_j_["MBX"]["grid_fftdim_lj"] = grid_fftdim_lj_;

    SetFFTDimensionLennardJones(grid_fftdim_lj_);

    // Try to get 2b cutoff
    // Default: 100 Angstrom if empty box, 9 Angstrom if box
    try {
        cutoff2b_ = j["MBX"]["twobody_cutoff"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"twobody_cutoff\" is not defined in json file. Using " << cutoff2b_ << "\n";
    }
    mbx_j_["MBX"]["twobody_cutoff"] = cutoff2b_;

    // Try to get 3b cutoff
    // Default: 5.0 Angstrom
    try {
        cutoff3b_ = j["MBX"]["threebody_cutoff"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"threebody_cutoff\" is not defined in json file. Using " << cutoff3b_ << "\n";
    }
    mbx_j_["MBX"]["threebody_cutoff"] = cutoff3b_;

    // Try to get maximum number of evaluations for 1b
    // Default: 1024
    try {
        maxNMonEval_ = j["MBX"]["max_n_eval_1b"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"max_n_eval_1b\" is not defined in json file. Using " << maxNMonEval_ << "\n";
    }
    mbx_j_["MBX"]["max_n_eval_1b"] = maxNMonEval_;

    // Try to get maximum number of evaluations for 2b
    // Default: 1024
    try {
        maxNDimEval_ = j["MBX"]["max_n_eval_2b"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"max_n_eval_2b\" is not defined in json file. Using " << maxNDimEval_ << "\n";
    }
    mbx_j_["MBX"]["max_n_eval_2b"] = maxNDimEval_;

    // Try to get maximum number of evaluations for 3b
    // Default: 1024
    try {
        maxNTriEval_ = j["MBX"]["max_n_eval_3b"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"max_n_eval_3b\" is not defined in json file. Using " << maxNTriEval_ << "\n";
    }
    mbx_j_["MBX"]["max_n_eval_3b"] = maxNTriEval_;

    // Try to get dipole convergence criteria
    // Default: 1E-16
    try {
        diptol_ = j["MBX"]["dipole_tolerance"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"dipole_tolerance\" is not defined in json file. Using " << diptol_
        //               << "\n";
    }
    mbx_j_["MBX"]["dipole_tolerance"] = diptol_;

    // Try to get dipole convergence criteria
    // Default: Conjugate gradient cg
    try {
        dipole_method_ = j["MBX"]["dipole_method"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"dipole_method\" is not defined in json file. Using " << dipole_method_ <<
        //     "\n";
    }
    mbx_j_["MBX"]["dipole_method"] = dipole_method_;

    // Try to get dipole max number of iterations
    // Default: 100
    try {
        maxItDip_ = j["MBX"]["dipole_max_it"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"dipole_max_it\" is not defined in json file. Using " << maxItDip_ << "\n";
    }
    mbx_j_["MBX"]["dipole_max_it"] = maxItDip_;

    // Try to get dispersion PME alpha
    // Default: 0.0
    try {
        disp_alpha_ = j["MBX"]["alpha_ewald_disp"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"alpha_ewald_disp\" is not defined in json file. Using " << disp_alpha_ <<
        //     "\n";
    }
    mbx_j_["MBX"]["alpha_ewald_disp"] = disp_alpha_;

    // Try to get dispertion PME grid density
    // Default: 2.5
    try {
        disp_grid_density_ = j["MBX"]["grid_density_disp"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"grid_density_disp\" is not defined in json file. Using " <<
        //     disp_grid_density_
        //               << "\n";
    }
    mbx_j_["MBX"]["grid_density_disp"] = disp_grid_density_;

    // Try to get dispersion PME spline order
    // Default: 6
    try {
        disp_spline_order_ = j["MBX"]["spline_order_disp"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"spline_order_disp\" is not defined in json file. Using " <<
        //     disp_spline_order_
        //               << "\n";
    }
    mbx_j_["MBX"]["spline_order_disp"] = disp_spline_order_;

    SetEwaldDispersion(disp_alpha_, disp_grid_density_, disp_spline_order_);

    // Try to get dispersion PME alpha
    // Default: 0.0
    try {
        lj_alpha_ = j["MBX"]["alpha_ewald_lj"];
    } catch (...) {
        if (mpi_rank_ == 0)
            std::cerr << "**WARNING** \"alpha_ewald_lj\" is not defined in json file. Using " << lj_alpha_ << "\n";
    }
    mbx_j_["MBX"]["alpha_ewald_lj"] = lj_alpha_;

    // Try to get dispertion PME grid density
    // Default: 2.5
    try {
        lj_grid_density_ = j["MBX"]["grid_density_lj"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"grid_density_lj\" is not defined in json file. Using " << lj_grid_density_
        //               << "\n";
    }
    mbx_j_["MBX"]["grid_density_lj"] = lj_grid_density_;

    // Try to get dispersion PME spline order
    // Default: 6
    try {
        lj_spline_order_ = j["MBX"]["spline_order_lj"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"spline_order_lj\" is not defined in json file. Using " << lj_spline_order_
        //               << "\n";
    }
    mbx_j_["MBX"]["spline_order_lj"] = lj_spline_order_;

    SetEwaldLennardJones(lj_alpha_, lj_grid_density_, lj_spline_order_);

    // Try to get electrostatics PME alpha
    // Default: 0.0
    try {
        elec_alpha_ = j["MBX"]["alpha_ewald_elec"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"alpha_ewald_elec\" is not defined in json file. Using " << elec_alpha_ <<
        //     "\n";
    }
    mbx_j_["MBX"]["alpha_ewald_elec"] = elec_alpha_;

    // Try to get electrostatics PME grid density
    // Default: 2.5
    try {
        elec_grid_density_ = j["MBX"]["grid_density_elec"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"grid_density_elec\" is not defined in json file. Using " <<
        //     elec_grid_density_
        //               << "\n";
    }
    mbx_j_["MBX"]["grid_density_elec"] = elec_grid_density_;

    // Try to get electrostatics PME spline order
    // Default: 6
    try {
        elec_spline_order_ = j["MBX"]["spline_order_elec"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"spline_order_elec\" is not defined in json file. Using " <<
        //     elec_spline_order_
        //               << "\n";
    }
    mbx_j_["MBX"]["spline_order_elec"] = elec_spline_order_;

    SetEwaldElectrostatics(elec_alpha_, elec_grid_density_, elec_spline_order_);

    try {
        std::vector<std::pair<std::string, std::string>> ttm_pairs = j["MBX"]["ttm_pairs"];
        buck_pairs_ = ttm_pairs;
    } catch (...) {
        // if (mpi_rank_ == 0) std::cerr << "**WARNING** \"ttm_pairs\" is not defined in json file. Using empty
        // list.\n";
    }
    SetTTMnrgPairs(buck_pairs_);
    mbx_j_["MBX"]["ttm_pairs"] = buck_pairs_;

    try {
        std::vector<std::pair<std::string, std::string>> ignore_dispersion = j["MBX"]["ignore_dispersion"];
        ignore_disp_ = ignore_dispersion;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"ignore_dispersion\" is not defined in json file. Using empty list.\n";
    }
    SetIgnoreDispersionPairs(ignore_disp_);
    mbx_j_["MBX"]["ignore_dispersion"] = ignore_disp_;

    try {
        std::vector<std::pair<std::string, std::string>> use_lennard_jones = j["MBX"]["use_lennard_jones"];
        lj_pairs_ = use_lennard_jones;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"use_lennard_jones\" is not defined in json file. Using empty list.\n";
    }
    SetLennardJonesPairs(lj_pairs_);
    mbx_j_["MBX"]["ttm_pairs"] = lj_pairs_;

    try {
        std::vector<std::string> ff_mons = j["MBX"]["ff_mons"];
        ff_mons_ = ff_mons;
    } catch (...) {
        // if (mpi_rank_ == 0) std::cerr << "**WARNING** \"ff_mons\" is not defined in json file. Using empty list.\n";
    }
    SetFFMons(ff_mons_);
    mbx_j_["MBX"]["ff_mons"] = ff_mons_;

    try {
        std::vector<std::string> ignore_1b_poly = j["MBX"]["ignore_1b_poly"];
        ignore_1b_poly_ = ignore_1b_poly;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"ignore_1b_poly\" is not defined in json file. Using empty list.\n";
    }
    Set1bIgnorePoly(ignore_1b_poly_);
    mbx_j_["MBX"]["ignore_1b_poly"] = ignore_1b_poly_;

    try {
        std::vector<std::vector<std::string>> ignore_2b_poly = j["MBX"]["ignore_2b_poly"];
        ignore_2b_poly_ = ignore_2b_poly;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"ignore_2b_poly\" is not defined in json file. Using empty list.\n";
    }
    Set2bIgnorePoly(ignore_2b_poly_);
    mbx_j_["MBX"]["ignore_2b_poly"] = ignore_2b_poly_;

    try {
        std::vector<std::vector<std::string>> ignore_3b_poly = j["MBX"]["ignore_3b_poly"];
        ignore_3b_poly_ = ignore_3b_poly;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"ignore_3b_poly\" is not defined in json file. Using empty list.\n";
    }
    Set3bIgnorePoly(ignore_3b_poly_);
    mbx_j_["MBX"]["ignore_3b_poly"] = ignore_3b_poly_;

    std::string connectivity_file = "";
    try {
        connectivity_file = j["MBX"]["connectivity_file"];
        tools::ReadConnectivity(connectivity_file.c_str(), connectivity_map_);
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"connectivity_file\" is not defined in json file. Not using 1B TTM-nrg.\n";
    }
    mbx_j_["MBX"]["connectivity_file"] = connectivity_file;

    std::string repdisp_file = "";
    try {
        repdisp_file = j["MBX"]["nonbonded_file"];
        char f[repdisp_file.length() + 1];
        strcpy(f, repdisp_file.c_str());
        SetUpFromJsonDispersionRepulsion(f);
    } catch (...) {
        SetUpFromJsonDispersionRepulsion();
        // if (mpi_rank_ == 0) std::cerr << "**WARNING** \"nonbonded_file\" is not defined in json file.\n";
    }
    mbx_j_["MBX"]["nonbonded_file"] = repdisp_file;

    std::string monomers_json_file = "";
    try {
        monomers_json_file = j["MBX"]["monomers_file"];
        char f[monomers_json_file.length() + 1];
        strcpy(f, monomers_json_file.c_str());
        SetUpFromJsonMonomers(f);
    } catch (...) {
        SetUpFromJsonMonomers();
        // if (mpi_rank_ == 0) std::cerr << "**WARNING** \"monomers_file\" is not defined in json file.\n";
    }
    mbx_j_["MBX"]["monomers_file"] = monomers_json_file;

    SetPBC(box_);
}

nlohmann::json System::GetJsonConfig() { return mbx_j_; }

void System::SetUpFromJson(std::string json_text) {
    nlohmann::json j = nlohmann::json::parse(json_text);
    SetUpFromJson(j);
}

void System::SetUpFromJson(char *json_file) {
    /* Template example for mbx.json
{
   "Note" : "This is a cofiguration file",
   "MBX" : {
       "box" : [100.0 ,0.0,0.0,100.0,0.0,0.0,0.0,100.0],
       "twobody_cutoff"   : 9.0,
       "threebody_cutoff" : 6.5,
       "max_n_eval_1b"    : 500,
       "max_n_eval_2b"    : 500,
       "max_n_eval_3b"    : 500,
       "dipole_tolerance" : 1E-016,
       "dipole_max_it"    : 100,
       "dipole_method"     : "cg",
       "alpha_ewald_elec" : 0.6,
       "grid_density_elec" : 2.5,
       "grid_fftdim_elec" : [],
       "spline_order_elec" : 6,
       "alpha_ewald_disp" : 0.6,
       "grid_density_disp" : 2.5,
       "grid_fftdim_disp" : [],
       "spline_order_disp" : 6,
       "ttm_pairs" : [],
       "ff_mons" : [],
       "ignore_dispersion" : [],
       "use_lennard_jones" : [],
       "ignore_1b_poly" : [],
       "ignore_2b_poly" : [],
       "ignore_3b_poly" : [],
       "connectivity_file" = "";
   } ,
   "i-pi" : {
       "port" : 34543,
       "localhost" : "localhost"
   }
}
     */

    // nlohmann::json j_default = {{"Note", "This is a cofiguration file"},
    //                            {"MBX",
    //                             {{"box", nlohmann::json::array()},
    //                              {"twobody_cutoff", 100.0},
    //                              {"threebody_cutoff", 6.5},
    //                              {"max_n_eval_1b", 500},
    //                              {"max_n_eval_2b", 500},
    //                              {"max_n_eval_3b", 500},
    //                              {"dipole_tolerance", 1E-16},
    //                              {"dipole_max_it", 100},
    //                              {"dipole_method", "cg"},
    //                              {"alpha_ewald_elec", 0.0},
    //                              {"grid_density_elec", 2.5},
    //                              {"grid_fftdim_elec", nlohmann::json::array()},
    //                              {"spline_order_elec", 6},
    //                              {"alpha_ewald_disp", 0.0},
    //                              {"grid_density_disp", 2.5},
    //                              {"grid_fftdim_disp", nlohmann::json::array()},
    //                              {"spline_order_disp", 6},
    //                              {"ttm_pairs", nlohmann::json::array()},
    //                              {"ignore_dispersion", nlohmann::json::array()},
    //                              {"use_lennard_jones", nlohmann::json::array()},
    //                              {"alpha_ewald_lj", 0.0},
    //                              {"grid_density_lj", 2.5},
    //                              {"spline_order_lj", 6},
    //                              {"ff_mons", nlohmann::json::array()},
    //                              {"connectivity_file", ""},
    //                              {"nonbonded_file", ""},
    //                              {"monomers_file", ""},
    //                              {"ignore_1b_poly", nlohmann::json::array()},
    //                              {"ignore_2b_poly", nlohmann::json::array()},
    //                              {"ignore_3b_poly", nlohmann::json::array()}}},
    //                            {"i-pi", {{"port", 34543}, {"localhost", "localhost"}}}};

    nlohmann::json j_default = {};

    std::ifstream ifjson;
    nlohmann::json j;
    if (json_file != 0) {
        try {
            ifjson.open(json_file);
            j = nlohmann::json::parse(ifjson);
        } catch (...) {
            j = j_default;
            std::cerr << "There has been a problem loading your json file: " + std::string(json_file) +
                             "... using defaults";
        }
    } else {
        j = j_default;
    }

    SetUpFromJson(j);

    ifjson.close();
}

std::string System::GetCurrentSystemConfig() {
    std::stringstream ss;

    ss << std::left << std::setw(25) << "Box:";
    for (size_t i = 0; i < box_.size(); i++) {
        ss << std::scientific << std::setprecision(4) << box_[i] << " ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "2B cutoff:" << cutoff2b_ << std::endl;
    ss << std::left << std::setw(25) << "3B cutoff:" << cutoff3b_ << std::endl;
    ss << std::left << std::setw(25) << "Max Eval Mon:" << maxNMonEval_ << std::endl;
    ss << std::left << std::setw(25) << "Max Eval Dim:" << maxNDimEval_ << std::endl;
    ss << std::left << std::setw(25) << "Max Eval Trim:" << maxNTriEval_ << std::endl;
    ss << std::left << std::setw(25) << "Dipole Tol:" << diptol_ << std::endl;
    ss << std::left << std::setw(25) << "Dipole Max Iter:" << maxItDip_ << std::endl;
    ss << std::left << std::setw(25) << "Dipole Method:" << dipole_method_ << std::endl;
    ss << std::left << std::setw(25) << "Ewald Alpha Elec:" << elec_alpha_ << std::endl;
    ss << std::left << std::setw(25) << "Grid Dens Elec:" << elec_grid_density_ << std::endl;
    ss << std::left << std::setw(25) << "Spline Order Elec:" << elec_spline_order_ << std::endl;
    ss << std::left << std::setw(25) << "Ewald Alpha Disp:" << disp_alpha_ << std::endl;
    ss << std::left << std::setw(25) << "Grid Dens Disp:" << disp_grid_density_ << std::endl;
    ss << std::left << std::setw(25) << "Spline Order Disp:" << disp_spline_order_ << std::endl;
    ss << std::left << std::setw(25) << "Ewald Alpha LJ:" << lj_alpha_ << std::endl;
    ss << std::left << std::setw(25) << "Grid Dens LJ:" << lj_grid_density_ << std::endl;
    ss << std::left << std::setw(25) << "Spline Order LJ:" << lj_spline_order_ << std::endl;

    ss << std::left << std::setw(25) << "TTM-pairs:";
    for (size_t i = 0; i < buck_pairs_.size(); i++) {
        ss << "{" << buck_pairs_[i].first << "," << buck_pairs_[i].second << "} ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "Forcefield Mons:";
    for (size_t i = 0; i < ff_mons_.size(); i++) {
        ss << ff_mons_[i] << " ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "Ignore 1B poly:";
    for (size_t i = 0; i < ignore_1b_poly_.size(); i++) {
        ss << ignore_1b_poly_[i] << " ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "Ignore 2B poly:";
    for (size_t i = 0; i < ignore_2b_poly_.size(); i++) {
        ss << "{";
        for (size_t j = 0; j < ignore_2b_poly_[i].size(); j++) {
            ss << ignore_2b_poly_[i][j] << " ";
        }
        ss << "} ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "Ignore 3B poly:";
    for (size_t i = 0; i < ignore_3b_poly_.size(); i++) {
        ss << "{";
        for (size_t j = 0; j < ignore_3b_poly_[i].size(); j++) {
            ss << ignore_3b_poly_[i][j] << " ";
        }
        ss << "} ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "FFT grid electrostatics:";
    for (size_t i = 0; i < grid_fftdim_elec_.size(); i++) {
        ss << grid_fftdim_elec_[i] << " ";
    }
    ss << std::endl;

    ss << std::left << std::setw(25) << "FFT grid dispersion:";
    for (size_t i = 0; i < grid_fftdim_disp_.size(); i++) {
        ss << grid_fftdim_disp_[i] << " ";
    }
    ss << std::endl;

    return ss.str();
}

void System::AddMonomerInfo() {
    // If xyz_ is empty, not possible to add any information
    if (xyz_.size() < 3) {
        std::string text = std::string("Cannot initialize the system with ") +
                           std::string("less than 3 coordinates, and current ") + std::string("size of xyz is ") +
                           std::to_string(xyz_.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy xyz_ and clear it.
    std::vector<double> xyz = xyz_;
    xyz_.clear();

    // If the size of xyz is not 3* size(atoms_), the system has
    // not been defined properly
    if (xyz.size() != 3 * atoms_.size()) {
        std::string text = std::string("Number of coordinates (") + std::to_string(xyz.size()) +
                           std::string(") is not consistent with number of atoms (") + std::to_string(atoms_.size()) +
                           std::string(")");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy the atoms_ vector and clear it
    std::vector<std::string> atoms = atoms_;
    atoms_.clear();

    // Copy atom_tag_ vector and clear it.
    std::vector<int> atom_tag = atom_tag_;
    atom_tag_.clear();

    // Adding the number of sites of each monomer and storing the first index
    std::vector<size_t> fi_at;
    numsites_ = systools::SetUpMonomers(monomers_, sites_, nat_, fi_at, monomers_j_);

#ifdef DEBUG
    std::cerr << "Finished SetUpMonomers.\n";
    std::cerr << "Monomer vector:\n";
    for (size_t i = 0; i < monomers_.size(); i++) {
        std::cerr << monomers_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Sites vector:\n";
    for (size_t i = 0; i < sites_.size(); i++) {
        std::cerr << sites_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Local/Ghost vector:\n";
    for (size_t i = 0; i < islocal_.size(); i++) {
        std::cerr << islocal_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Atom tag vector:\n";
    for (size_t i = 0; i < atom_tag.size(); i++) {
        std::cerr << atom_tag[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Atoms vector:\n";
    for (size_t i = 0; i < nat_.size(); i++) {
        std::cerr << nat_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "First Index vector:\n";
    for (size_t i = 0; i < fi_at.size(); i++) {
        std::cerr << fi_at[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    // Calculating the number of atoms
    numat_ = 0;
    for (size_t i = 0; i < nat_.size(); i++) {
        numat_ += nat_[i];
    }

    // Ordering monomers by monomer type, from less to more monomers of each type
    mon_type_count_ = systools::OrderMonomers(monomers_, islocal_, sites_, nat_, original2current_order_,
                                              initial_order_, initial_order_realSites_);

#ifdef DEBUG
    std::cerr << "Finished OrderMonomers():\n";
    std::cerr << "mon_type_count:\n";
    for (size_t i = 0; i < mon_type_count_.size(); i++) {
        std::cerr << "{\"" << mon_type_count_[i].first << "\"," << mon_type_count_[i].second << "},";
    }
    std::cerr << std::endl;

    std::cerr << "New monomer vector:\n";
    for (size_t i = 0; i < monomers_.size(); i++) {
        std::cerr << monomers_[i] << ",";
    }
    std::cerr << std::endl;

    std::cerr << "New Local/Ghost vector:\n";
    for (size_t i = 0; i < islocal_.size(); i++) {
        std::cerr << islocal_[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Original2Current:\n";
    for (size_t i = 0; i < original2current_order_.size(); i++) {
        std::cerr << original2current_order_[i] << ",";
    }
    std::cerr << std::endl;

    std::cerr << "original_order:\n";
    for (size_t i = 0; i < initial_order_.size(); i++) {
        std::cerr << "{" << initial_order_[i].first << "," << initial_order_[i].second << "} , ";
    }
    std::cerr << std::endl;

    std::cerr << "original_order_realsites:\n";
    for (size_t i = 0; i < initial_order_realSites_.size(); i++) {
        std::cerr << "{" << initial_order_realSites_[i].first << "," << initial_order_realSites_[i].second << "} , ";
    }
    std::cerr << std::endl;
#endif

    // Rearranging coordinates to account for virt sites
    xyz_ = std::vector<double>(3 * numsites_, 0.0);
    atoms_ = std::vector<std::string>(numsites_, "virt");
    atom_tag_ = std::vector<int>(numsites_, 0);

    size_t count = 0;
    first_index_.clear();
    std::vector<size_t> tmpsites;
    std::vector<size_t> tmpnats;
    // Loop over all the monomers
    for (size_t i = 0; i < monomers_.size(); i++) {
        // For each monomer, copy the coordinates and atoms of the input
        // order xyz and atoms to the new position in the ordered system
        size_t k = initial_order_[i].first;
        // Positions
        std::copy(xyz.begin() + 3 * fi_at[k], xyz.begin() + 3 * (fi_at[k] + nat_[k]), xyz_.begin() + 3 * count);
        // Atom names
        std::copy(atoms.begin() + fi_at[k], atoms.begin() + fi_at[k] + nat_[k], atoms_.begin() + count);
        // Atom tags
        std::copy(atom_tag.begin() + fi_at[k], atom_tag.begin() + fi_at[k] + nat_[k], atom_tag_.begin() + count);
        // Atom tags for virtual sites
        for (size_t j = 0; j < (sites_[k] - nat_[k]); ++j) atom_tag_[count + nat_[k] + j] = -atom_tag[fi_at[k] + j];
        // Adding the first index of sites
        first_index_.push_back(count);
        // Update count
        count += sites_[k];
        // Updating the sites and nat vectors
        tmpsites.push_back(sites_[k]);
        tmpnats.push_back(nat_[k]);
    }

    // Set sites_ and nat_
    sites_ = tmpsites;
    nat_ = tmpnats;

    // Initialize gradients, charges, pols and polfacs to the right size
    grad_ = std::vector<double>(3 * numsites_, 0.0);
    chg_ = std::vector<double>(numsites_, 0.0);
    pol_ = std::vector<double>(numsites_, 0.0);
    polfac_ = std::vector<double>(numsites_, 0.0);

#ifdef DEBUG
    std::cerr << "New Atom tag vector w/ sites:\n";
    for (size_t i = 0; i < atom_tag_.size(); i++) {
        std::cerr << atom_tag_[i] << " , ";
    }
    std::cerr << std::endl;
#endif
}

void System::AddClusters(size_t nmax, double cutoff, size_t istart, size_t iend, bool use_ghost_) {
    // istart is the monomer position for which we will look all dimers and
    // trimers that contain it. iend is the last monomer position.
    // This means, if istart is 0 and iend is 2, we will look for all dimers
    // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!

    // Make sure that nmax is 2 or 3
    // Throw exception otherwise
    // Commented for now since this functiuon is private and unlikely to
    // be called from the outside
    // if (nmax != 2 and nmax != 3) {
    //    std::string text = "nmax value of " + std::to_string(nmax) + " is not acceptable. Possible values are 2
    //    or 3."; throw CUException(__func__, __FILE__, __LINE__, text);
    //}

    size_t nmon = monomers_.size();
    systools::AddClusters(nmax, cutoff, istart, iend, nmon, use_pbc_, box_, box_inverse_, xyz_, first_index_, islocal_,
                          dimers_, trimers_, use_ghost_);
}

std::vector<size_t> System::AddClustersParallel(size_t nmax, double cutoff, size_t istart, size_t iend,
                                                bool use_ghost_) {
    // Overloaded function to be compatible with omp
    // Returns dimers if nmax == 2, or trimers if nmax == 3

    // Make sure that nmax is 2 or 3
    // Throw exception otherwise
    // Commented for now since this functiuon is private and unlikely to
    // be called from the outside
    // if (nmax != 2 and nmax != 3) {
    //    std::string text = "nmax value of " + std::to_string(nmax) + " is not acceptable. Possible values are 2
    //    or 3."; throw CUException(__func__, __FILE__, __LINE__, text);
    //}

    size_t nmon = monomers_.size();
    std::vector<size_t> dimers, trimers;
    systools::AddClusters(nmax, cutoff, istart, iend, nmon, use_pbc_, box_, box_inverse_, xyz_, first_index_, islocal_,
                          dimers, trimers, use_ghost_);
    if (nmax == 2) return dimers;
    return trimers;
}

void System::SetConnectivity(std::unordered_map<std::string, eff::Conn> connectivity_map) {
    connectivity_map_ = connectivity_map;
}

double System::Energy(bool do_grads) {
    // Check if system has been initialized
    // If not, throw exception

    if (!initialized_) {
        SetUpFromJson();
    }

    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Reset energy and grads in system to 0
    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    // Reset the charges, pols, polfacs and new Vsite
    SetPBC(box_);

    // Call the get energy function
    allMonGood_ = true;

    // Get the NB contributions

#ifdef TIMING
    auto t1 = std::chrono::high_resolution_clock::now();
#endif

    double eff = GetFF(do_grads);
    double e1b = Get1B(do_grads);

    // If monomers are too distorted, skip 2b and 3b calculation
    // Return only
    // if (!allMonGood_) {
    //    return e1b;
    //}

#ifdef TIMING
    auto t2 = std::chrono::high_resolution_clock::now();
#endif

    // double e2b = 0.0;
    double e2b = Get2B(do_grads);

#ifdef TIMING
    auto t2a = std::chrono::high_resolution_clock::now();
#endif

    // double edisp = 0.0;
    double edisp = GetDispersion(do_grads);

#ifdef TIMING
    auto t2b = std::chrono::high_resolution_clock::now();
#endif

    // double ebuck = 0.0;
    double ebuck = GetBuckingham(do_grads);
#ifdef TIMING
    auto t3 = std::chrono::high_resolution_clock::now();
#endif

    double elj = 0.0;
    if (lj_pairs_.size() > 0) elj = GetLennardJones(do_grads);

#ifdef TIMING
    auto t31 = std::chrono::high_resolution_clock::now();
#endif

    // double e3b = 0.0;
    double e3b = Get3B(do_grads);

#ifdef TIMING
    auto t4 = std::chrono::high_resolution_clock::now();
#endif

    // Electrostatic energy
    double Eelec = GetElectrostatics(do_grads);
    // double Eelec = 0.0;

#ifdef TIMING
    auto t5 = std::chrono::high_resolution_clock::now();
#endif

    // Set up energy with the new value
    energy_ = eff + e1b + e2b + e3b + edisp + ebuck + elj + Eelec;

#ifdef PRINT_INDIVIDUAL_TERMS
    std::cerr << std::setprecision(10) << std::scientific;
    std::cerr << "1B = " << e1b << std::endl
              << "FF = " << eff << std::endl
              << "2B = " << e2b << std::endl
              << "3B = " << e3b << std::endl
              << "Disp = " << edisp << std::endl
              << "Buck = " << ebuck << std::endl
              << "LJ = " << elj << std::endl
              << "Elec = " << Eelec << std::endl
              << "Total = " << energy_ << std::endl;
#endif
#ifdef TIMING
    std::cerr << "System::1b(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds\n";
    std::cerr << "System::2b(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2a - t2).count() << " milliseconds\n";
    std::cerr << "System::disp(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2b - t2a).count() << " milliseconds\n";
    std::cerr << "System::rep(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2b).count() << " milliseconds\n";
    std::cerr << "System::LJ(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t31 - t3).count() << " milliseconds\n";
    std::cerr << "System::3b(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t31).count() << " milliseconds\n";
    std::cerr << "System::electrostatics(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count() << " milliseconds\n";
    std::cerr << "TotalEnergy(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t1).count() << " milliseconds\n";
#endif

    return energy_;
}

double System::OneBodyEnergy(bool do_grads) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("1B Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Reset energy and gradients
    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    // Calculate the 1b energy
    energy_ = Get1B(do_grads);
    return energy_;
}

double System::ClassicPotential(bool do_grads) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("1B Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Reset energy and gradients
    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    // Calculate the 1b energy
    energy_ = GetFF(do_grads);
    return energy_;
}

double System::GetFF(bool do_grads) {
    // 1B CLASSICAL ENERGY
    // Loop overall the monomers and get their energy
    size_t curr_mon_type = 0;
    size_t current_coord = 0;
    double eff = 0.0;

    // EY: WARNING. THE FOLLOWING CODE HAS BEEN ANNOTATED BY ERIC YEH. DELETE AS
    // APPROPRIATE. All lines commented by ERIC YEH have "EY:" in front of the line

    // EY: Loop over the number of types. so the number of unique id's. Say you have
    // EY: CO2 and H2O. This loop runs twice. Once for co2 and another for h2o
    // EY: Each iteration SET a single id that you consider
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        // Useful variables
        size_t istart = 0;
        size_t iend = 0;

        if (std::find(ff_mons_.begin(), ff_mons_.end(), mon_type_count_[k].first) == ff_mons_.end()) {
            continue;
        }

        // EY: Loops over the number monomer of the current id or type
        while (istart < mon_type_count_[k].second) {
            iend = std::min(istart + maxNMonEval_, mon_type_count_[k].second);
            size_t nmon = iend - istart;
            size_t ncoord = 3 * nat_[curr_mon_type] * nmon;  // EY: Number of coordinates that need to be assigned
            std::string mon = mon_type_count_[k].first;      // EY: Monomer id

            // XYZ with real sites
            std::vector<double> xyz(ncoord, 0.0);
            std::vector<double> grad2(ncoord, 0.0);

            // Set up real coordinates
            for (size_t i = istart; i < iend; i++) {
                std::copy(xyz_.begin() + current_coord + 3 * i * sites_[curr_mon_type],
                          xyz_.begin() + current_coord + 3 * (i * sites_[curr_mon_type] + nat_[curr_mon_type]),
                          xyz.begin() + 3 * (i - istart) * nat_[curr_mon_type]);
            }

            // Get energy of the chunk as function of monomer
            if (do_grads) {
                // EY: Overloaded function to get BOTH gradient and virial.
                // EY: Note: grad2 is passed by reference.
                try {
                    eff += eff::get_ff_energy(bblock::System::connectivity_map_.at(mon), nmon, xyz, grad2, allMonGood_,
                                              nat_[curr_mon_type], box_, box_inverse_, &virial_);
                } catch (const std::exception &e) {
                    std::string text =
                        std::string("Monomer id not contained in connectivity map. System monomer id given is: ") +
                        std::to_string(nat_[curr_mon_type]) +
                        std::string(". Is correct monomer id defined in connectivity file?");
                    throw CUException(__func__, __FILE__, __LINE__, text);
                }

                // Reorganize gradients
                for (size_t i = 0; i < nmon; i++) {
                    for (size_t j = 0; j < 3 * nat_[curr_mon_type]; j++) {
                        grad_[current_coord + 3 * (i + istart) * sites_[curr_mon_type] + j] +=
                            grad2[3 * i * nat_[curr_mon_type] + j];
                    }
                }
            } else {
                try {
                    // EY: ONLY get the energy
                    eff += eff::get_ff_energy(bblock::System::connectivity_map_.at(mon), nmon, xyz, allMonGood_,
                                              nat_[curr_mon_type], box_, box_inverse_);
                } catch (const std::exception &e) {
                    std::string text =
                        std::string("Monomer id not contained in connectivity map. System monomer id given is: ") +
                        std::to_string(nat_[curr_mon_type]) +
                        std::string(". Is correct monomer id defined in connectivity file?");
                    throw CUException(__func__, __FILE__, __LINE__, text);
                }
            }

            istart = iend;
        }

        // Update current_coord and curr_mon_type
        current_coord += 3 * mon_type_count_[k].second * sites_[curr_mon_type];
        curr_mon_type += mon_type_count_[k].second;
    }

    return eff;
}

double System::Get1B(bool do_grads) {
    // 1B ENERGY
    // Loop overall the monomers and get their energy
    size_t curr_mon_type = 0;
    size_t current_coord = 0;
    size_t current_mon = 0;
    double e1b = 0.0;
    enforce_ttm_for_idx_.clear();

    size_t indx = 0;

    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        // Useful variables
        size_t istart = 0;
        size_t iend = 0;

        if (std::find(ignore_1b_poly_.begin(), ignore_1b_poly_.end(), mon_type_count_[k].first) !=
            ignore_1b_poly_.end()) {
            continue;
        }

        while (istart < mon_type_count_[k].second) {
            std::vector<size_t> mon_idxs;
            iend = std::min(istart + maxNMonEval_, mon_type_count_[k].second);
            std::vector<size_t> indexes;
            size_t nmon = 0;
            for (size_t i = istart; i < iend; i++) {
                if (islocal_[indx + i]) {
                    mon_idxs.push_back(current_mon);
                    nmon++;
                }
                current_mon++;
            }

            size_t ncoord = 3 * nat_[curr_mon_type] * nmon;
            std::string mon = mon_type_count_[k].first;

            // XYZ with real sites
            std::vector<double> xyz(ncoord, 0.0);
            std::vector<double> grad2(ncoord, 0.0);

            // Set up real coordinates
            size_t ii = istart;
            for (size_t i = istart; i < iend; i++) {
                if (islocal_[indx + i]) {
                    std::copy(xyz_.begin() + current_coord + 3 * i * sites_[curr_mon_type],
                              xyz_.begin() + current_coord + 3 * (i * sites_[curr_mon_type] + nat_[curr_mon_type]),
                              xyz.begin() + 3 * (ii - istart) * nat_[curr_mon_type]);
                    ii++;
                }
            }

            // Get energy of the chunk as function of monomer
            if (do_grads) {
                e1b += e1b::get_1b_energy(mon, nmon, xyz, grad2, indexes, &virial_);

                // Reorganize gradients
                size_t ii = 0;
                for (size_t i = istart; i < iend; i++) {
                    if (islocal_[indx + i]) {
                        for (size_t j = 0; j < 3 * nat_[curr_mon_type]; j++) {
                            grad_[current_coord + 3 * (ii + istart) * sites_[curr_mon_type] + j] +=
                                grad2[3 * ii * nat_[curr_mon_type] + j];
                        }
                        ii++;
                    }
                }
            } else {
                e1b += e1b::get_1b_energy(mon, nmon, xyz, indexes);
            }

            for (size_t i = 0; i < indexes.size(); i++) {
                enforce_ttm_for_idx_.push_back(mon_idxs[indexes[i]]);
            }

            istart = iend;
        }

        // Update current_coord and curr_mon_type
        current_coord += 3 * mon_type_count_[k].second * sites_[curr_mon_type];
        curr_mon_type += mon_type_count_[k].second;
        indx = iend;
    }

    return e1b;
}

double System::TwoBodyEnergy(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("2B Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Reset energy and gradients
    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    // Calculate the 2b energy
    energy_ = Get2B(do_grads, use_ghost);

    return energy_;
}

double System::Get2B(bool do_grads, bool use_ghost) {
    // No dimers makes the function return 0.

    // 2B ENERGY
    double e2b_t = 0.0;
    double edisp_t = 0.0;

    // Variables needed for OMP
    size_t step = 1;
    int num_threads = 1;

#ifdef _OPENMP
#pragma omp parallel
    {
        // Get the number of threads
        if (omp_get_thread_num() == 0) num_threads = omp_get_num_threads();
    }
    // Define variables to be used later in the condensation of data
    int grad_step = 3 * numsites_ / num_threads;
    step = std::max(size_t(1), std::min(nummon_ / num_threads, step));
#endif  // _OPENMP

    // Variables to be used for both serial and parallel implementation
    size_t first_grad = 0;
    size_t last_grad = 3 * numsites_;
    int rank = 0;

    // Vector pools that allow compatibility between
    // serial and parallel implementation
    std::vector<double> e2b_pool(num_threads, 0.0);
    std::vector<std::vector<double>> grad_pool(num_threads, std::vector<double>(3 * numsites_, 0.0));
    std::vector<std::vector<double>> virial_pool(num_threads, std::vector<double>(9, 0.0));  // declare virial pool
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic) private(rank)
#endif  // _OPENMP
    for (size_t istart = 0; istart < nummon_; istart += step) {
#ifdef _OPENMP
        rank = omp_get_thread_num();
#endif  // _OPENMP

        // We loop over all the monomers, and we get all the dimers
        // in which this monomer is involved

        // In this function, istart and iend refer to the first
        // monomer of the dimer. The second one will be found from istart+1
        // to the total number of monomers

        // This iend definition is making sure that we don't go passed
        // the number of monomers
        size_t iend = std::min(istart + step, nummon_);

// Adding corresponding clusters depending on if we are within
// OPENMP or not

// This call will get the dimers that have as first index a monomer
// with index between istart and iend (iend not included)
#ifdef _OPENMP
        std::vector<size_t> dimers = AddClustersParallel(2, cutoff2b_, istart, iend, use_ghost);
#else
        AddClusters(2, cutoff2b_, istart, iend, use_ghost);
        std::vector<size_t> dimers = dimers_;
#endif

        // In order to continue, we need at least one dimer
        // If the size of the dimer vector is not at least 2, means
        // that we don't have any dimer
        if (dimers.size() < 2) {
            continue;
        }

        // The way the XYZ are set, they include the virtual site,
        // but we don't need the electrostatic virtual site for teh 2B
        // polynomials. Thus, we need to create a pair of vectors with the right
        // coordinates to pass to polynomials and dispersion
        std::vector<double> xyz1;
        std::vector<double> xyz2;
        std::vector<double> grad1;
        std::vector<double> grad2;
        std::vector<double> virial(9, 0.0);  // declare virial tensor

        // Define the two monomer ids that we are currently looking at
        std::string m1 = monomers_[dimers[0]];
        std::string m2 = monomers_[dimers[1]];

        // Initialize the iteration variables
        size_t i = 0;
        size_t nd = 0;
        size_t nd_tot = 0;
        size_t nd_bad = 0;

        // Loop over all the dimers
        while (2 * nd_tot < dimers.size()) {
            i = (nd_tot + nd_bad + nd) * 2;
            if (i >= dimers.size()) break;
            bool m1_is_good = true;
            bool m2_is_good = true;
            // Check if we are still in the same type of pair
            // We will pas the entire batch in the 2b calculator, but they need
            // to be the same pair (e.g., h2o-h2o, h2o-i, cl-na...)
            if (std::find(enforce_ttm_for_idx_.begin(), enforce_ttm_for_idx_.end(), dimers[i]) !=
                enforce_ttm_for_idx_.end())
                m1_is_good = false;
            if (std::find(enforce_ttm_for_idx_.begin(), enforce_ttm_for_idx_.end(), dimers[i + 1]) !=
                enforce_ttm_for_idx_.end())
                m2_is_good = false;
            if (monomers_[dimers[i]] == m1 && monomers_[dimers[i + 1]] == m2 && m1_is_good && m2_is_good) {
                // Push the coordinates
                for (size_t j = 0; j < 3 * nat_[dimers[i]]; j++) {
                    xyz1.push_back(xyz_[3 * first_index_[dimers[i]] + j]);
                    grad1.push_back(0.0);
                }
                for (size_t j = 0; j < 3 * nat_[dimers[i + 1]]; j++) {
                    xyz2.push_back(xyz_[3 * first_index_[dimers[i + 1]] + j]);
                    grad2.push_back(0.0);
                }
                nd++;
            }
            if (!m1_is_good || !m2_is_good) nd_bad++;

            // If one of the monomers is different as the previous one
            // since dimers are also ordered, means that no more dimers of that
            // type exist. Thus, do calculation, update m? and clear xyz
            if (monomers_[dimers[i]] != m1 || monomers_[dimers[i + 1]] != m2 || i == dimers.size() - 2 ||
                nd == maxNDimEval_ || !m1_is_good || !m2_is_good) {
                if (nd == 0) {
                    xyz1.clear();
                    xyz2.clear();
                    grad1.clear();
                    grad2.clear();
                    std::fill(virial.begin(), virial.end(), 0.0);
                    m1 = monomers_[dimers[i]];
                    m2 = monomers_[dimers[i + 1]];
                    continue;
                }

                // Fix dimer positions if pbc
                if (use_pbc_) {
                    systools::GetCloseDimerImage(box_, box_inverse_, nat_[dimers[nd_tot * 2]],
                                                 nat_[dimers[nd_tot * 2 + 1]], nd, xyz1.data(), xyz2.data());
                }

                // Check if this pair needs to use MB-nrg
                bool use_poly = true;
                for (size_t i2b = 0; i2b < ignore_2b_poly_.size(); i2b++) {
                    std::vector<std::string> v1 = ignore_2b_poly_[i2b];
                    std::vector<std::string> v2 = {m1, m2};
                    std::sort(v1.begin(), v1.end());
                    std::sort(v2.begin(), v2.end());

                    if (v1 == v2) {
                        use_poly = false;
                        break;
                    }
                }

                if (use_poly) {
                    if (do_grads) {
                        // POLYNOMIALS
                        e2b_pool[rank] += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2, grad1, grad2, &virial);

                        for (size_t k = 0; k < 9; k++) {  // accumulate virial tensor from pool

                            virial_pool[rank][k] += virial[k];
                        }
                        // Update gradients in system
                        size_t i0 = nd_tot * 2;
                        for (size_t k = 0; k < nd; k++) {
                            // Monomer 1
                            for (size_t j = 0; j < 3 * nat_[dimers[i0 + 2 * k]]; j++) {
                                grad_pool[rank][3 * first_index_[dimers[i0 + 2 * k]] + j] +=
                                    grad1[k * 3 * nat_[dimers[i0 + 2 * k]] + j];
                            }
                            // Monomer 2
                            for (size_t j = 0; j < 3 * nat_[dimers[i0 + 2 * k + 1]]; j++) {
                                grad_pool[rank][3 * first_index_[dimers[i0 + 2 * k + 1]] + j] +=
                                    grad2[k * 3 * nat_[dimers[i0 + 2 * k + 1]] + j];
                            }
                        }
                    } else {
                        e2b_pool[rank] += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2);
                    }
                }

                // Update loop variables and clear other temporary variable
                nd_tot += nd + nd_bad;
                nd = 0;
                nd_bad = 0;
                xyz1.clear();
                xyz2.clear();
                grad1.clear();
                grad2.clear();
                std::fill(virial.begin(), virial.end(), 0.0);  // clear virial tensor
                m1 = monomers_[dimers[i]];
                m2 = monomers_[dimers[i + 1]];
            }
        }
    }

#ifdef _OPENMP
#pragma omp parallel private(first_grad, last_grad, rank)
    {
        rank = omp_get_thread_num();

        first_grad = 0 + rank * grad_step;

        last_grad = (rank + 1) * grad_step;

        if (rank == num_threads - 1) {
            last_grad = 3 * numsites_;
        }
#pragma omp barrier
#endif

        // Condensate gradients
        //	const double scale = use_ghost ? 0.5 : 1.0;
        const double scale = 1.0;  // only accumulate force on local particles in LAMMPS
        for (int i = 0; i < num_threads; i++) {
            for (size_t j = first_grad; j < last_grad; j++) {
                grad_[j] += scale * grad_pool[i][j];
            }
        }

#ifdef _OPENMP
    }  // parallel
#endif

    // Condensate energy
    for (int i = 0; i < num_threads; i++) {
        e2b_t += e2b_pool[i];
    }

    if (use_ghost) e2b_t *= 0.5;
    // Condensate virial
    const double scalev = use_ghost ? 0.5 : 1.0;
    for (int i = 0; i < num_threads; i++) {
        for (size_t j = 0; j < 9; j++) {
            virial_[j] += scalev * virial_pool[i][j];
        }
    }

#ifdef DEBUG
    std::cerr << "disp = " << edisp_t << "    2b = " << e2b_t << std::endl;
#endif

    return e2b_t + edisp_t;
}

double System::ThreeBodyEnergy(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("3B Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = Get3B(do_grads, use_ghost);

    return energy_;
}

double System::Get3B(bool do_grads, bool use_ghost) {
    // 3B ENERGY
    double e3b_t = 0.0;

    const double one_third = 1.0 / 3.0;

    // Variables needed for OMP
    size_t step = 1;
    int num_threads = 1;

#ifdef _OPENMP
#pragma omp parallel
    {
        // Get the number of threads
        if (omp_get_thread_num() == 0) num_threads = omp_get_num_threads();
    }
    // Define variables to be used later in the condensation of data
    int grad_step = 3 * numsites_ / num_threads;
    step = std::max(size_t(1), std::min(nummon_ / num_threads, step));
#endif  // _OPENMP

    // Variables to be used for both serial and parallel implementation
    size_t first_grad = 0;
    size_t last_grad = 3 * numsites_;
    int rank = 0;

    // Vector pools that allow compatibility between
    // serial and parallel implementation
    std::vector<double> e3b_pool(num_threads, 0.0);
    std::vector<std::vector<double>> grad_pool(num_threads, std::vector<double>(3 * numsites_, 0.0));
    std::vector<std::vector<double>> virial_pool(num_threads, std::vector<double>(9, 0.0));  // declare virial pool

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic) private(rank)
#endif  // _OPENMP
    for (size_t istart = 0; istart < nummon_; istart += step) {
#ifdef _OPENMP
        rank = omp_get_thread_num();
#endif

        // We loop over all the monomers, and we get all the trimers
        // in which this monomer is involved

        // In this function, istart and iend refer to the first
        // monomer of the trimer. The second and third ones will
        // be found from istart+1 to the total number of monomers

        // This iend definition is making sure that we don't go passed
        // the number of monomers

        size_t iend = std::min(istart + step, nummon_);

#ifdef _OPENMP
        std::vector<size_t> trimers = AddClustersParallel(3, cutoff3b_, istart, iend, use_ghost);
#else
        AddClusters(3, cutoff3b_, istart, iend, use_ghost);
        std::vector<size_t> trimers = trimers_;
#endif

        // In order to continue, we need at least one dimer
        // If the size of the dimer vector is not at least 2, means
        // that we don't have any dimer
        if (trimers.size() < 3) {
            continue;
        }

        // The way the XYZ are set, they include the virtual site,
        // but we don't need the electrostatic virtual site for teh 2B
        // polynomials. Thus, we need to create a pair of vectors with the right
        // coordinates to pass to polynomials and dispersion
        std::vector<double> coord1;
        std::vector<double> coord2;
        std::vector<double> coord3;
        std::string m1 = monomers_[trimers[0]];
        std::string m2 = monomers_[trimers[1]];
        std::string m3 = monomers_[trimers[2]];

        // Initialize the iteration variables
        size_t i = 0;
        size_t nt = 0;
        size_t nt_tot = 0;

        // if ghost monomers included, then force maxNTriEval == 1 to properly tally energy+virial
        // should we just overwrite maxNTriEval_?
        size_t _maxNTriEval = (use_ghost) ? 1 : maxNTriEval_;

        // Loop over all the trimers
        while (3 * nt_tot < trimers.size()) {
            i = (nt_tot + nt) * 3;

            // Check if we are still in the same type of trimer
            if (monomers_[trimers[i]] == m1 && monomers_[trimers[i + 1]] == m2 && monomers_[trimers[i + 2]] == m3) {
                // Push the coordinates
                for (size_t j = 0; j < 3 * nat_[trimers[i]]; j++) {
                    coord1.push_back(xyz_[3 * first_index_[trimers[i]] + j]);
                }
                for (size_t j = 0; j < 3 * nat_[trimers[i + 1]]; j++) {
                    coord2.push_back(xyz_[3 * first_index_[trimers[i + 1]] + j]);
                }
                for (size_t j = 0; j < 3 * nat_[trimers[i + 2]]; j++) {
                    coord3.push_back(xyz_[3 * first_index_[trimers[i + 2]] + j]);
                }
                nt++;
            }

            // If one of the monomers is different as the previous one
            // since trimers are also ordered, means that no more trimers of that
            // type exist. Thus, do calculation, update m? and clear xyz
            if (monomers_[trimers[i]] != m1 || monomers_[trimers[i + 1]] != m2 || monomers_[trimers[i + 2]] != m3 ||
                i == trimers.size() - 3 || nt == _maxNTriEval) {
                if (nt == 0) {
                    coord1.clear();
                    coord2.clear();
                    coord3.clear();
                    m1 = monomers_[trimers[i]];
                    m2 = monomers_[trimers[i + 1]];
                    m3 = monomers_[trimers[i + 2]];
                    continue;
                }

                // Fix trimer positions if pbc
                if (use_pbc_) {
                    systools::GetCloseTrimerImage(box_, box_inverse_, nat_[trimers[nt_tot * 3]],
                                                  nat_[trimers[nt_tot * 3 + 1]], nat_[trimers[nt_tot * 3 + 2]], nt,
                                                  coord1, coord2, coord3);
                }

                // Check if this pair needs to use MB-nrg
                bool use_poly = true;
                for (size_t i3b = 0; i3b < ignore_3b_poly_.size(); i3b++) {
                    std::vector<std::string> v1 = ignore_3b_poly_[i3b];
                    std::vector<std::string> v2 = {m1, m2, m3};
                    std::sort(v1.begin(), v1.end());
                    std::sort(v2.begin(), v2.end());

                    if (v1 == v2) {
                        use_poly = false;
                        break;
                    }
                }

                if (use_poly) {
                    std::vector<double> xyz1(coord1.size(), 0.0);
                    std::vector<double> xyz2(coord2.size(), 0.0);
                    std::vector<double> xyz3(coord3.size(), 0.0);
                    std::copy(coord1.begin(), coord1.end(), xyz1.begin());
                    std::copy(coord2.begin(), coord2.end(), xyz2.begin());
                    std::copy(coord3.begin(), coord3.end(), xyz3.begin());

                    if (do_grads) {
                        // POLYNOMIALS
                        std::vector<double> grad1(coord1.size(), 0.0);
                        std::vector<double> grad2(coord2.size(), 0.0);
                        std::vector<double> grad3(coord3.size(), 0.0);
                        std::vector<double> virial(9, 0.0);  // declare virial tensor
                        // POLYNOMIALS
                        double e = e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);

                        double escale = 1.0;
                        if (use_ghost)
                            escale = (islocal_[trimers[i]] + islocal_[trimers[i + 1]] + islocal_[trimers[i + 2]]) *
                                     one_third;
                        e3b_pool[rank] += escale * e;

                        // Update gradients
                        size_t i0 = nt_tot * 3;
                        for (size_t k = 0; k < nt; k++) {
                            // Monomer 1
                            for (size_t j = 0; j < 3 * nat_[trimers[i0 + 3 * k]]; j++) {
                                grad_pool[rank][3 * first_index_[trimers[i0 + 3 * k]] + j] +=
                                    grad1[k * 3 * nat_[trimers[i0 + 3 * k]] + j];
                            }
                            // Monomer 2
                            for (size_t j = 0; j < 3 * nat_[trimers[i0 + 3 * k + 1]]; j++) {
                                grad_pool[rank][3 * first_index_[trimers[i0 + 3 * k + 1]] + j] +=
                                    grad2[k * 3 * nat_[trimers[i0 + 3 * k + 1]] + j];
                            }
                            // Monomer 3
                            for (size_t j = 0; j < 3 * nat_[trimers[i0 + 3 * k + 2]]; j++) {
                                grad_pool[rank][3 * first_index_[trimers[i0 + 3 * k + 2]] + j] +=
                                    grad3[k * 3 * nat_[trimers[i0 + 3 * k + 2]] + j];
                            }
                        }
                        // Virial Tensor
                        for (size_t j = 0; j < 9; j++) {
                            virial_pool[rank][j] += escale * virial[j];
                        }

                    } else {
                        // POLYNOMIALS
                        double e = e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3);
                        double escale = 1.0;
                        if (use_ghost)
                            escale = (islocal_[trimers[i]] + islocal_[trimers[i + 1]] + islocal_[trimers[i + 2]]) *
                                     one_third;
                        e3b_pool[rank] += escale * e;
                    }
                }

                // Update iteration variables
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

#ifdef _OPENMP
#pragma omp parallel private(first_grad, last_grad, rank)
    {
        rank = omp_get_thread_num();

        first_grad = 0 + rank * grad_step;

        last_grad = (rank + 1) * grad_step;
        if (rank == num_threads - 1) {
            last_grad = 3 * numsites_;
        }
#pragma omp barrier
#endif

        // Condensate gradients
        for (int i = 0; i < num_threads; i++) {
            for (size_t j = first_grad; j < last_grad; j++) {
                grad_[j] += grad_pool[i][j];
            }
        }

#ifdef _OPENMP
    }  // parallel
#endif

    // Condensate energy
    for (int i = 0; i < num_threads; i++) {
        e3b_t += e3b_pool[i];
    }
    // Condensate virial
    for (int i = 0; i < num_threads; i++) {
        for (size_t j = 0; j < 9; j++) {
            virial_[j] += virial_pool[i][j];
        }
    }

    return e3b_t;
}

////////////////////////////////////////////////////////////////////////////////

void System::SetCharges() {
    // Set charges for each monomer type
    size_t fi_mon = 0;
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t nsites = sites_[fi_mon];

        systools::SetCharges(xyz_, chg_, mon, nmon, nsites, first_index_[fi_mon], chggrad_, monomers_j_);
        fi_mon += nmon;
    }

#ifdef DEBUG
    // Get charges of real and virtual sites in the input order
    std::vector<double> real_chg =
        systools::ResetOrderRealN(chg_, initial_order_realSites_, numat_, first_index_, nat_);
    std::vector<double> all_chg = systools::ResetOrderN(chg_, initial_order_, first_index_, sites_);

    // Print them
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "Real sites charges\n";
    std::cerr << real_chg[0];
    for (size_t i = 1; i < real_chg.size(); i++) std::cerr << ", " << real_chg[i];
    std::cerr << "\nAll charges\n";
    std::cerr << all_chg[0];
    for (size_t i = 1; i < all_chg.size(); i++) std::cerr << ", " << all_chg[i];
    std::cerr << std::endl;
    std::cerr << "\nCharge Derivatives {size " << chggrad_.size() << "\n";
    for (size_t i = 0; i < chggrad_.size(); i++) std::cerr << ", " << chggrad_[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

////////////////////////////////////////////////////////////////////////////////

void System::SetPols() {
    // Set polarizabilities for each monomer type
    size_t fi_mon = 0;
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t nsites = sites_[fi_mon];

        systools::SetPol(pol_, mon, nmon, nsites, first_index_[fi_mon], monomers_j_);
        fi_mon += nmon;
    }

#ifdef DEBUG
    // Get charges of real and virtual sites in the input order
    std::vector<double> real_pol =
        systools::ResetOrderRealN(pol_, initial_order_realSites_, numat_, first_index_, nat_);
    std::vector<double> all_pol = systools::ResetOrderN(pol_, initial_order_, first_index_, sites_);

    // Print them
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "Real sites polarizabilities\n";
    std::cerr << real_pol[0];
    for (size_t i = 1; i < real_pol.size(); i++) std::cerr << ", " << real_pol[i];
    std::cerr << "\nAll polarizabilities\n";
    std::cerr << all_pol[0];
    for (size_t i = 1; i < all_pol.size(); i++) std::cerr << ", " << all_pol[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

////////////////////////////////////////////////////////////////////////////////

void System::SetPolfacs() {
    // Set polarizability factors for each monomer type
    size_t fi_mon = 0;
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t nsites = sites_[fi_mon];

        systools::SetPolfac(polfac_, mon, nmon, nsites, first_index_[fi_mon], monomers_j_);
        fi_mon += nmon;
    }

#ifdef DEBUG
    // Get charges of real and virtual sites in the input order
    std::vector<double> real_polfac =
        systools::ResetOrderRealN(polfac_, initial_order_realSites_, numat_, first_index_, nat_);
    std::vector<double> all_polfac = systools::ResetOrderN(polfac_, initial_order_, first_index_, sites_);

    // Print them
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "Real sites polarizability factors\n";
    std::cerr << real_polfac[0];
    for (size_t i = 1; i < real_polfac.size(); i++) std::cerr << ", " << real_polfac[i];
    std::cerr << "\nAll polarizability factors\n";
    std::cerr << all_polfac[0];
    for (size_t i = 1; i < all_polfac.size(); i++) std::cerr << ", " << all_polfac[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

////////////////////////////////////////////////////////////////////////////////

void System::SetC6LongRange() {
    // Set virtual sites for each monomer type
    size_t fi_mon = 0;
    size_t fi_atoms = 0;
    c6_lr_ = std::vector<double>(numat_, 0.0);
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t natoms = nat_[fi_mon];

        systools::SetC6LongRange(c6_lr_, mon, nmon, natoms, fi_atoms, monomers_j_);
        fi_mon += nmon;
        fi_atoms += nmon * natoms;
    }

#ifdef DEBUG
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "All c6_lr after setting them\n";
    std::cerr << c6_lr_[0];
    for (size_t i = 1; i < c6_lr_.size(); i++) std::cerr << ", " << c6_lr_[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

void System::SetLJLongRange() {
    // Set virtual sites for each monomer type
    size_t fi_mon = 0;
    size_t fi_atoms = 0;
    lj_lr_ = std::vector<double>(numat_, 0.0);
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t natoms = nat_[fi_mon];

        systools::SetLJLongRange(lj_lr_, mon, nmon, natoms, fi_atoms, lj_pairs_, repdisp_j_);
        fi_mon += nmon;
        fi_atoms += nmon * natoms;
    }

#ifdef DEBUG
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "All lj_lr after setting them\n";
    std::cerr << lj_lr_[0];
    for (size_t i = 1; i < lj_lr_.size(); i++) std::cerr << ", " << lj_lr_[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

////////////////////////////////////////////////////////////////////////////////

void System::SetVSites() {
    // Set virtual sites for each monomer type
    size_t fi_mon = 0;
    for (size_t k = 0; k < mon_type_count_.size(); k++) {
        std::string mon = mon_type_count_[k].first;
        size_t nmon = mon_type_count_[k].second;
        size_t nsites = sites_[fi_mon];

        systools::SetVSites(xyz_, mon, nmon, nsites, first_index_[fi_mon]);
        fi_mon += nmon;
    }

#ifdef DEBUG
    // Get charges of real and virtual sites in the input order
    std::vector<double> all_xyz = systools::ResetOrder3N(xyz_, initial_order_, first_index_, sites_);

    // Print them
    std::cerr << "Entered " << __func__ << std::endl;
    std::cerr << "All coordinates after setting vsites\n";
    std::cerr << all_xyz[0];
    for (size_t i = 1; i < all_xyz.size(); i++) std::cerr << ", " << all_xyz[i];
    std::cerr << std::endl;
#endif  // DEBUG
}

////////////////////////////////////////////////////////////////////////////////

double System::Dispersion(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Dispersion Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetDispersion(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

double System::LennardJones(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Dispersion Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetLennardJones(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

double System::DispersionPME(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Dispersion Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    if (islocal_.size() > 0) std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetDispersionPME(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

double System::DispersionPMElocal(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Dispersion Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    if (islocal_.size() > 0) std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetDispersionPMElocal(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

double System::Buckingham(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Buckingham Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetBuckingham(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

double System::Electrostatics(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Electrostatic Energy calculation ") + std::string("not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetElectrostatics(do_grads, use_ghost);

    return energy_;
}

double System::ElectrostaticsMPI(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Electrostatic Energy calculation ") + std::string("not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetElectrostatics(do_grads, use_ghost);

    // correct energy and forces for number of processors

    double rnprocs = 1.0 / (double)(proc_grid_x_ * proc_grid_y_ * proc_grid_z_);

    energy_ *= rnprocs;
    for (int i = 0; i < grad_.size(); ++i) grad_[i] *= rnprocs;
    for (int i = 0; i < virial_.size(); ++i) virial_[i] *= rnprocs;

    return energy_;
}

double System::ElectrostaticsMPIlocal(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text = std::string("System has not been initialized. ") +
                           std::string("Electrostatic Energy calculation ") + std::string("not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    if (islocal_.size() > 0) std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    energy_ = GetElectrostaticsMPIlocal(do_grads, use_ghost);

    return energy_;
}

////////////////////////////////////////////////////////////////////////////////

void System::SetEwaldElectrostatics(double alpha, double grid_density, int spline_order) {
    elec_alpha_ = alpha;
    elec_grid_density_ = grid_density;
    elec_spline_order_ = spline_order;
    electrostaticE_.SetEwaldAlpha(alpha);
    electrostaticE_.SetEwaldGridDensity(grid_density);
    electrostaticE_.SetEwaldSplineOrder(spline_order);
}

////////////////////////////////////////////////////////////////////////////////

void System::SetEwaldDispersion(double alpha, double grid_density, int spline_order) {
    disp_alpha_ = alpha;
    disp_grid_density_ = grid_density;
    disp_spline_order_ = spline_order;
    dispersionE_.SetEwaldAlpha(alpha);
    dispersionE_.SetEwaldGridDensity(grid_density);
    dispersionE_.SetEwaldSplineOrder(spline_order);
}

////////////////////////////////////////////////////////////////////////////////

void System::SetEwaldLennardJones(double alpha, double grid_density, int spline_order) {
    lj_alpha_ = alpha;
    lj_grid_density_ = grid_density;
    lj_spline_order_ = spline_order;
    lennardJonesE_.SetEwaldAlpha(alpha);
    lennardJonesE_.SetEwaldGridDensity(grid_density);
    lennardJonesE_.SetEwaldSplineOrder(spline_order);
}

////////////////////////////////////////////////////////////////////////////////

void System::SetEwald(double alpha, double grid_density, int spline_order) {
    SetEwaldElectrostatics(alpha, grid_density, spline_order);
    SetEwaldDispersion(alpha, grid_density, spline_order);
    SetEwaldLennardJones(alpha, grid_density, spline_order);
}

////////////////////////////////////////////////////////////////////////////////

void System::SetMPI(MPI_Comm comm, int nx, int ny, int nz) {
#if HAVE_MPI == 1
    mpi_initialized_ = true;
    world_ = comm;
    MPI_Comm_rank(world_, &mpi_rank_);
    proc_grid_x_ = nx;
    proc_grid_y_ = ny;
    proc_grid_z_ = nz;
#else
    world_ = 0;
    mpi_rank_ = 0;
    proc_grid_x_ = 1;
    proc_grid_y_ = 1;
    proc_grid_z_ = 1;
#endif
}

////////////////////////////////////////////////////////////////////////////////

int System::TestMPI() {
#if HAVE_MPI == 1
    if (mpi_initialized_)
        return 1;
    else
        return -1;
#else
    return -2;
#endif
}

////////////////////////////////////////////////////////////////////////////////

void System::SetPeriodicity(bool periodic) {
    simcell_periodic_ = periodic;
    electrostaticE_.SetPeriodicity(periodic);
}

////////////////////////////////////////////////////////////////////////////////

double System::GetElectrostatics(bool do_grads, bool use_ghost) {
    electrostaticE_.SetNewParameters(xyz_, chg_, chggrad_, pol_, polfac_, dipole_method_, do_grads, box_, cutoff2b_);
    electrostaticE_.SetDipoleTolerance(diptol_);
    electrostaticE_.SetDipoleMaxIt(maxItDip_);
    electrostaticE_.SetEwaldAlpha(elec_alpha_);
    electrostaticE_.SetEwaldGridDensity(elec_grid_density_);
    electrostaticE_.SetEwaldSplineOrder(elec_spline_order_);
    electrostaticE_.SetFFTDimension(grid_fftdim_elec_);

    return electrostaticE_.GetElectrostatics(grad_, &virial_, use_ghost);
}

double System::GetElectrostaticsMPIlocal(bool do_grads, bool use_ghost) {
    electrostaticE_.SetNewParameters(xyz_, chg_, chggrad_, pol_, polfac_, dipole_method_, do_grads, box_, cutoff2b_);
    electrostaticE_.SetDipoleTolerance(diptol_);
    electrostaticE_.SetDipoleMaxIt(maxItDip_);
    electrostaticE_.SetEwaldAlpha(elec_alpha_);
    electrostaticE_.SetEwaldGridDensity(elec_grid_density_);
    electrostaticE_.SetEwaldSplineOrder(elec_spline_order_);
    electrostaticE_.SetFFTDimension(grid_fftdim_elec_);

    return electrostaticE_.GetElectrostaticsMPIlocal(grad_, &virial_, use_ghost);
}

////////////////////////////////////////////////////////////////////////////////

double System::GetDispersion(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }
    dispersionE_.SetNewParameters(xyz_real, ignore_disp_, do_grads, cutoff2b_, box_);
    dispersionE_.SetEwaldAlpha(disp_alpha_);
    dispersionE_.SetEwaldGridDensity(disp_grid_density_);
    dispersionE_.SetEwaldSplineOrder(disp_spline_order_);
    dispersionE_.SetFFTDimension(grid_fftdim_disp_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = dispersionE_.GetDispersion(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

double System::GetLennardJones(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }
    lennardJonesE_.SetNewParameters(xyz_real, lj_pairs_, do_grads, cutoff2b_, box_);
    lennardJonesE_.SetEwaldAlpha(lj_alpha_);
    lennardJonesE_.SetEwaldGridDensity(lj_grid_density_);
    lennardJonesE_.SetEwaldSplineOrder(lj_spline_order_);
    lennardJonesE_.SetFFTDimension(grid_fftdim_lj_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = lennardJonesE_.GetLennardJones(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

double System::GetDispersionPME(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }

    dispersionE_.SetNewParameters(xyz_real, ignore_disp_, do_grads, cutoff2b_, box_);
    dispersionE_.SetEwaldAlpha(disp_alpha_);
    dispersionE_.SetEwaldGridDensity(disp_grid_density_);
    dispersionE_.SetEwaldSplineOrder(disp_spline_order_);
    dispersionE_.SetFFTDimension(grid_fftdim_disp_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = dispersionE_.GetDispersionPME(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

double System::GetDispersionPMElocal(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }

    dispersionE_.SetNewParameters(xyz_real, ignore_disp_, do_grads, cutoff2b_, box_);
    dispersionE_.SetEwaldAlpha(disp_alpha_);
    dispersionE_.SetEwaldGridDensity(disp_grid_density_);
    dispersionE_.SetEwaldSplineOrder(disp_spline_order_);
    lennardJonesE_.SetFFTDimension(grid_fftdim_lj_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = dispersionE_.GetDispersionPMElocal(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

double System::GetLennardJonesPME(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }

    lennardJonesE_.SetNewParameters(xyz_real, lj_pairs_, do_grads, cutoff2b_, box_);
    lennardJonesE_.SetEwaldAlpha(lj_alpha_);
    lennardJonesE_.SetEwaldGridDensity(lj_grid_density_);
    lennardJonesE_.SetEwaldSplineOrder(lj_spline_order_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = lennardJonesE_.GetLennardJonesPME(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

double System::GetLennardJonesPMElocal(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }

    lennardJonesE_.SetNewParameters(xyz_real, lj_pairs_, do_grads, cutoff2b_, box_);
    lennardJonesE_.SetEwaldAlpha(lj_alpha_);
    lennardJonesE_.SetEwaldGridDensity(lj_grid_density_);
    lennardJonesE_.SetEwaldSplineOrder(lj_spline_order_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = lennardJonesE_.GetLennardJonesPMElocal(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

void System::SetBoxPMElocal(std::vector<double> box) {
    // Check that the box has 0 or 9 components
    if (box.size() != 9 && box.size() != 0) {
        std::string text = "Box size of " + std::to_string(box.size()) + " is not acceptable.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    dispersionE_.SetBoxPMElocal(box);
    electrostaticE_.SetBoxPMElocal(box);
    lennardJonesE_.SetBoxPMElocal(box);
}

////////////////////////////////////////////////////////////////////////////////

double System::GetBuckingham(bool do_grads, bool use_ghost) {
    std::vector<double> xyz_real(3 * numat_);

    size_t count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            xyz_real[count + j] = xyz_[first_index_[i] * 3 + j];
        }
        count += 3 * nat_[i];
    }

    buckinghamE_.SetNewParameters(xyz_real, buck_pairs_, enforce_ttm_for_idx_, do_grads, cutoff2b_, box_);
    // buckinghamE_.SetNewParameters(xyz_real, buck_pairs_, do_grads, cutoff2b_, box_);
    std::vector<double> real_grad(3 * numat_, 0.0);
    double e = buckinghamE_.GetRepulsion(real_grad, &virial_, use_ghost);

    count = 0;
    for (size_t i = 0; i < nummon_; i++) {
        for (size_t j = 0; j < 3 * nat_[i]; j++) {
            grad_[first_index_[i] * 3 + j] += real_grad[count + j];
        }
        count += 3 * nat_[i];
    }
    return e;
}

////////////////////////////////////////////////////////////////////////////////

void System::ResetDipoleHistory() { electrostaticE_.ResetAspcHistory(); }

std::vector<size_t> System::GetInfoElectrostaticsCounts() { return electrostaticE_.GetInfoCounts(); }
std::vector<double> System::GetInfoElectrostaticsTimings() { return electrostaticE_.GetInfoTimings(); }

////////////////////////////////////////////////////////////////////////////////

}  // namespace bblock

////////////////////////////////////////////////////////////////////////////////

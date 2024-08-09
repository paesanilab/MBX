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
#include "../potential/3b/x3b-v2x.h"

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
    std::cerr << std::setprecision(20);
    tag_counter_ = 1;

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

    // Setting 4B cutoff
    // Affects the 4B polynomials
    cutoff4b_ = 0.0;

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

    mbx_j_ = {};
    monomers_j_ = {};
    repdisp_j_ = {};

    // Defaults for when MBX built w/o MPI
    world_ = 0;
    mpi_rank_ = 0;
    proc_grid_x_ = 1;
    proc_grid_y_ = 1;
    proc_grid_z_ = 1;
}
System::~System() {}

size_t System::GetNumMol() { return nummol; }
size_t System::GetNumMon() { return nummon_; }
size_t System::GetNumSites() { return numsites_; }
size_t System::GetNumRealSites() { return numat_; }

size_t System::GetMonNumAt(size_t n) { return nat_[original2current_order_[n]]; }

std::vector<double> System::GetExternalCharges() { return electrostaticE_.GetExternalCharges(); }

std::vector<double> System::GetExternalChargesPositions() { return electrostaticE_.GetExternalChargesPositions(); }

std::vector<double> System::GetExternalChargesGradients() { return electrostaticE_.GetExternalChargesGradients(); }

void System::GetPhiXAndEfX(std::vector<double> &phi, std::vector<double> &ef, std::vector<double> &phid,
                           std::vector<double> &efd) {
    electrostaticE_.GetPhiXAndEfX(phi, ef, phid, efd);
}

void System::GetGradAndGradX(std::vector<double> &grad, std::vector<double> &gradx) {
    electrostaticE_.GetGradAndGradX(grad, gradx);
}

void System::GetElectrostaticFields(std::vector<double> &phi, std::vector<double> &efq, std::vector<double> &efd) {
    phi = systools::ResetOrderN(electrostaticE_.GetSysPhi(), initial_order_, first_index_, sites_);
    efq = systools::ResetOrder3N(electrostaticE_.GetSysEfq(), initial_order_, first_index_, sites_);
    efd = systools::ResetOrder3N(electrostaticE_.GetSysEfd(), initial_order_, first_index_, sites_);
}

// void System::Hack1EfqPhi() { electrostaticE_.Hack1EfqPhi(); }
//
// void System::Hack2CgIter() { electrostaticE_.Hack2CgIter(); }

void System::RedistributeGradients(std::vector<double> &grad) {
    // TODO FOr now, assuming that grad is for all sites.
    size_t nmon = 1;
    for (size_t i = 0; i < monomers_.size(); i++) {
        size_t sys2inp_order = initial_order_[i].second;
        size_t fi_crd = first_index_[sys2inp_order] * 3;
        std::string mon = monomers_[sys2inp_order];
        systools::RedistributeVirtGrads2Real(mon, nmon, fi_crd, grad);
    }
}

void System::Hack3GetPotentialAtPoints(std::vector<double> coordinates) {
    electrostaticE_.Hack3GetPotentialAtPoints(coordinates);
}

double System::GetPermanentElectrostaticEnergy() { return electrostaticE_.GetPermanentElectrostaticEnergy(); }
double System::GetPermanentElectrostaticEnergyExternalFieldContribution() {
    return electrostaticE_.GetPermanentElectrostaticEnergyExternalFieldContribution();
}

double System::GetInducedElectrostaticEnergy() { return electrostaticE_.GetInducedElectrostaticEnergy(); }

void System::SetExternalElectrostaticPotentialAndFieldInSites(std::vector<double> phi, std::vector<double> ef,
                                                              std::vector<double> def, std::vector<double> dmui) {
    // Make sure that the xyz of input has the right size
    if (ef.size() != 3 * numsites_ or phi.size() != numsites_ or (def.size() != 9 * numsites_ and def.size() != 0) or
        (dmui.size() != 9 * numsites_ and dmui.size() != 0)) {
        std::string text =
            "Sizes " + std::to_string(ef.size()) + " and " + std::to_string(3 * numsites_) + " don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy each coordinate in the apropriate place in the internal
    std::vector<double> phi_ord(numsites_, 0.0), ef_ord(3 * numsites_, 0.0), def_ord(def.size(), 0.0),
        dmui_ord(def.size(), 0.0);
    for (size_t i = 0; i < sites_.size(); i++) {
        size_t ini = 3 * initial_order_[i].second;
        size_t fin = ini + 3 * sites_[i];
        size_t ini_new = 3 * first_index_[i];
        std::copy(ef.begin() + ini, ef.begin() + fin, ef_ord.begin() + ini_new);
    }
    for (size_t i = 0; i < sites_.size(); i++) {
        size_t ini = initial_order_[i].second;
        size_t fin = ini + sites_[i];
        size_t ini_new = first_index_[i];
        std::copy(phi.begin() + ini, phi.begin() + fin, phi_ord.begin() + ini_new);
    }
    if (def.size() != 0) {
        for (size_t i = 0; i < sites_.size(); i++) {
            size_t ini = 9 * initial_order_[i].second;
            size_t fin = ini + 9 * sites_[i];
            size_t ini_new = 9 * first_index_[i];
            std::copy(def.begin() + ini, def.begin() + fin, def_ord.begin() + ini_new);
        }
    }

    if (dmui.size() != 0) {
        for (size_t i = 0; i < sites_.size(); i++) {
            size_t ini = 9 * initial_order_[i].second;
            size_t fin = ini + 9 * sites_[i];
            size_t ini_new = 9 * first_index_[i];
            std::copy(dmui.begin() + ini, dmui.begin() + fin, dmui_ord.begin() + ini_new);
        }
    }

    electrostaticE_.SetExternalElectrostaticPotentialAndFieldInSites(phi_ord, ef_ord, def_ord, dmui_ord);
}

void System::SetExternalChargesAndPositions(std::vector<double> chg, std::vector<double> xyz) {
    electrostaticE_.SetExternalChargesAndPositions(chg, xyz);
}

void System::SetExternalChargesAndPositions(std::vector<double> chg, std::vector<double> xyz,
                                            std::vector<size_t> islocal, std::vector<int> tag) {
    electrostaticE_.SetExternalChargesAndPositions(chg, xyz, islocal, tag);
}

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

std::vector<size_t> System::GetFirstInd() {
    std::vector<size_t> fi(monomers_.size());
    for (size_t i = 0; i < monomers_.size(); i++) {
        size_t current_pos = original2current_order_[i];
        fi[i] = initial_order_[current_pos].second;
    }

    // Get the first index that this monomer had in the input order
    return fi;
}

std::vector<size_t> System::GetFirstIndRealSites() {
    std::vector<size_t> fi(monomers_.size());
    size_t count = 0;
    for (size_t i = 0; i < monomers_.size(); i++) {
        size_t current_pos = original2current_order_[i];
        size_t nat = nat_[current_pos];
        fi[i] = count;
        count += nat;
    }

    // Get the first index that this monomer had in the input order
    return fi;
}

std::vector<size_t> System::GetSitesVector() {
    std::vector<size_t> sites(sites_.size());
    size_t count = 0;
    for (size_t i = 0; i < sites_.size(); i++) {
        size_t current_pos = original2current_order_[i];
        size_t nat = sites_[current_pos];
        sites[i] = nat;
    }

    // Get the first index that this monomer had in the input order
    return sites;
}

std::vector<size_t> System::GetAtomsVector() {
    std::vector<size_t> sites(sites_.size());
    size_t count = 0;
    for (size_t i = 0; i < sites_.size(); i++) {
        size_t current_pos = original2current_order_[i];
        size_t nat = nat_[current_pos];
        sites[i] = nat;
    }

    // Get the first index that this monomer had in the input order
    return sites;
}

std::vector<size_t> System::GetPairList(size_t nmax, double cutoff, size_t istart, size_t iend, bool use_ghost) {
    // Make sure that nmax is 2 or 3
    // Throw exception otherwise
    if (nmax != 2 and nmax != 3) {
        std::string text = "nmax value of " + std::to_string(nmax) + " is not acceptable. Possible values are 2 or 3.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Call the add clusters function to get all the pairs
    std::vector<size_t> d, t;
    std::vector<size_t> idxs;
    for (size_t i = 0; i < monomers_.size(); i++) {
        idxs.push_back(i);
    }

    if (nmax == 2) dimers_ = AddClustersParallel(2, cutoff, idxs, use_ghost);
    if (nmax == 3) trimers_ = AddClustersParallel(3, cutoff, idxs, use_ghost);

    for (size_t j = 0; j < dimers_.size(); j++) d.push_back(dimers_[j]);
    for (size_t j = 0; j < trimers_.size(); j++) t.push_back(trimers_[j]);

    dimers_ = d;
    trimers_ = t;

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

// void System::GetAtomMonIndex(std::vector<size_t> &original_atom_index_to_original_mon_index,
//                             std::vector<std::string> &original_atom_index_to_original_mon_id) {
//    original_atom_index_to_original_mon_index = std::vector<size_t>(numat_);
//    original_atom_index_to_original_mon_id = std::vector<std::string>(numat_);
//    for (size_t i = 0; i < nat_.size(); i++) {
//        size_t original_index = initial_order_realSites_[i].first;
//        for (size_t j = 0; j < nat_[i]; j++) {
//            original_atom_index_to_original_mon_index[original_index + j] = i;
//            original_atom_index_to_original_mon_id[original_index + j] = monomers_[i];
//        }
//    }
//}

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

std::vector<double> System::GetRealC6lr() {
    return systools::ResetOrderN(c6_lr_, initial_order_realSites_, first_index_real_sites_, nat_);
}

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

std::vector<double> System::GetBoxABCabc() { return box_ABCabc_; }

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

void System::CheckFFTDimension(std::vector<int> grid) {
    if (grid.size() == 0) return;

#if HAVE_MPI == 1
    if (!mpi_initialized_) {
        std::string text = std::string("MPI not initialized yet");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }
#endif

    // grid points evenly distributed across ranks in each dimension

    if (grid.size() == 3) {
        size_t err = 0;
        if ((grid[0] / proc_grid_x_) * proc_grid_x_ != grid[0]) err += 1;
        if ((grid[1] / proc_grid_y_) * proc_grid_y_ != grid[1]) err += 1;
        if ((grid[2] / proc_grid_z_) * proc_grid_z_ != grid[2]) err += 1;

        if ((grid[0] / proc_grid_x_) % 2 == 1) err += 1;
        if ((grid[1] / proc_grid_y_) % 2 == 1) err += 1;
        if ((grid[2] / proc_grid_z_) % 2 == 1) err += 1;

        if (err > 0) {
            std::string text = std::string("FFT grid dimensions must be evenly divisible by processor grid");
            throw CUException(__func__, __FILE__, __LINE__, text);
        }
    }
}

void System::SetFFTDimensionElectrostatics(std::vector<int> grid) {
    CheckFFTDimension(grid);
    electrostaticE_.SetFFTDimension(grid);
}

void System::SetFFTDimensionDispersion(std::vector<int> grid) {
    CheckFFTDimension(grid);
    dispersionE_.SetFFTDimension(grid);
}

void System::SetFFTDimensionLennardJones(std::vector<int> grid) {
    CheckFFTDimension(grid);
    lennardJonesE_.SetFFTDimension(grid);
}

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
void System::GetMolecularDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind) {
    std::vector<double> tmp_perm = electrostaticE_.GetMolecularPermanentDipoles();
    std::vector<double> tmp_ind = electrostaticE_.GetMolecularInducedDipoles();

    mu_perm = std::vector<double>(tmp_perm.size(), 0.0);
    mu_ind = std::vector<double>(tmp_ind.size(), 0.0);
    // Reorder to match input order
    for (size_t i = 0; i < nummon_; i++) {
        size_t current_pos = original2current_order_[i];
        for (size_t j = 0; j < 3; j++) {
            mu_perm[3 * i + j] = tmp_perm[3 * current_pos + j];
            mu_ind[3 * i + j] = tmp_ind[3 * current_pos + j];
        }
    }
}

void System::GetDipoles(std::vector<double> &mu_perm, std::vector<double> &mu_ind) {
    std::vector<double> mu_perm2 = electrostaticE_.GetPermanentDipoles();
    std::vector<double> mu_ind2 = electrostaticE_.GetInducedDipoles();

    mu_perm = systools::ResetOrder3N(mu_perm2, initial_order_, first_index_, sites_);
    mu_ind = systools::ResetOrder3N(mu_ind2, initial_order_, first_index_, sites_);
}

void System::GetTotalDipole(std::vector<double> &mu_perm, std::vector<double> &mu_ind, std::vector<double> &mu_tot) {
    std::vector<double> all_mu_perm = electrostaticE_.GetPermanentDipoles();
    std::vector<double> all_mu_ind = electrostaticE_.GetInducedDipoles();

    mu_perm = std::vector<double>(3, 0.0);
    mu_ind = std::vector<double>(3, 0.0);

    mu_tot = std::vector<double>(3, 0.0);

    for (size_t i = 0; i < numsites_; i++) {
        for (size_t j = 0; j < 3; j++) {
            mu_perm[j] += all_mu_perm[3 * i + j];
            mu_ind[j] += all_mu_ind[3 * i + j];
        }
    }
    for (size_t j = 0; j < 3; j++) {
        mu_tot[j] = mu_perm[j] + mu_ind[j];
    }
}

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
void System::Set4bCutoff(double cutoff4b) { cutoff4b_ = cutoff4b; }
double System::Get2bCutoff() { return cutoff2b_; }
double System::Get3bCutoff() { return cutoff3b_; }
double System::Get4bCutoff() { return cutoff4b_; }
void System::SetNMaxEval1b(size_t nmax) { maxNMonEval_ = nmax; }
void System::SetNMaxEval2b(size_t nmax) { maxNDimEval_ = nmax; }
void System::SetNMaxEval3b(size_t nmax) { maxNTriEval_ = nmax; }

void System::SetDipoleTol(double tol) { diptol_ = tol; }
void System::SetDipoleMaxIt(size_t maxit) { maxItDip_ = maxit; }
void System::SetDipoleMethod(std::string method) { dipole_method_ = method; }

void System::SetPBC(std::vector<double> box) {
    // Check that the box has 0 or 9 components
    if (box.size() != 9 && box.size() != 6 && box.size() != 0) {
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
        if (xyz_.size() > 0) systools::FixMonomerCoordinates(xyz_, box_, box_inverse_, nat_, first_index_);
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
    for (int i = 0; i < atoms.size(); ++i) {
        atom_tag_.push_back(tag == 0 ? tag_counter_ + i : tag + i);
    }
    tag_counter_ += atoms.size();
}

void System::AddMolecule(std::vector<size_t> molec) { molecules_.push_back(molec); }

std::vector<std::pair<std::string, std::string>> System::GetTTMnrgPairs() { return buck_pairs_; }

std::vector<std::string> System::GetFFMons() { return ff_mons_; }

std::vector<std::string> System::Get1bIgnorePoly() { return ignore_1b_poly_; }

std::vector<std::vector<std::string>> System::Get2bIgnorePoly() { return ignore_2b_poly_; }

std::vector<std::vector<std::string>> System::Get3bIgnorePoly() { return ignore_3b_poly_; }

std::vector<std::vector<std::string>> System::Get4bIgnorePoly() { return ignore_4b_poly_; }

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

void System::Add4bIgnorePoly(std::string mon1, std::string mon2, std::string mon3, std::string mon4) {
    std::vector<std::string> p = {mon1, mon2, mon3, mon4};
    std::sort(p.begin(), p.end());

    if (std::find(ignore_4b_poly_.begin(), ignore_4b_poly_.end(), p) == ignore_4b_poly_.end()) {
        ignore_4b_poly_.push_back(p);
    }
}

void System::Set4bIgnorePoly(std::vector<std::vector<std::string>> ignore_4b) {
    ignore_4b_poly_.clear();

    for (size_t i = 0; i < ignore_4b.size(); i++) {
        std::vector<std::string> p = ignore_4b[i];
        std::sort(p.begin(), p.end());
        ignore_4b_poly_.push_back(p);
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

    // Try to get 4b cutoff
    // Default: 0.0 Angstrom
    try {
        cutoff4b_ = j["MBX"]["fourbody_cutoff"];
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"fourbody_cutoff\" is not defined in json file. Using " << cutoff4b_ << "\n";
    }
    mbx_j_["MBX"]["fourbody_cutoff"] = cutoff4b_;

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
        // if (mpi_rank_ == 0)
        //    std::cerr << "**WARNING** \"alpha_ewald_lj\" is not defined in json file. Using " << lj_alpha_ << "\n";
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

    try {
        std::vector<std::vector<std::string>> ignore_4b_poly = j["MBX"]["ignore_4b_poly"];
        ignore_4b_poly_ = ignore_4b_poly;
    } catch (...) {
        // if (mpi_rank_ == 0)
        //     std::cerr << "**WARNING** \"ignore_4b_poly\" is not defined in json file. Using empty list.\n";
    }
    Set4bIgnorePoly(ignore_4b_poly_);
    mbx_j_["MBX"]["ignore_4b_poly"] = ignore_4b_poly_;

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

std::string System::GetJsonText() {
    std::ostringstream ss;
    ss << mbx_j_;
    return ss.str();
}

std::string System::GetJsonDispersionRepulsionText() {
    std::ostringstream ss;
    ss << repdisp_j_;
    return ss.str();
}

std::string System::GetJsonMonomersText() {
    std::ostringstream ss;
    ss << monomers_j_;
    return ss.str();
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
    ss << std::left << std::setw(25) << "4B cutoff:" << cutoff4b_ << std::endl;
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

    ss << std::left << std::setw(25) << "Ignore 4B poly:";
    for (size_t i = 0; i < ignore_4b_poly_.size(); i++) {
        ss << "{";
        for (size_t j = 0; j < ignore_4b_poly_[i].size(); j++) {
            ss << ignore_4b_poly_[i][j] << " ";
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
    std::vector<size_t> fi_sites;
    numsites_ = systools::SetUpMonomers(monomers_, sites_, nat_, fi_at, fi_sites, monomers_j_);

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
    size_t count_real = 0;
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
        // Adding the first index of real sites
        first_index_real_sites_.push_back(count_real);
        // Update count
        count += sites_[k];
        count_real += nat_[k];
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

std::vector<size_t> System::AddClustersParallel(size_t nmax, double cutoff, std::vector<size_t> idxs, bool use_ghost_) {
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
    std::vector<size_t> nmers;
    systools::AddClusters(nmax, cutoff, idxs, nmon, use_pbc_, box_, box_inverse_, xyz_, first_index_, islocal_,
                          atom_tag_, nmers, use_ghost_);
    return nmers;
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

#ifdef TTMNRG
    double eff = GetFF(do_grads);
#else
    double eff = 0.0;
#endif
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

    // Only calculate buck if compiled with the option
    double ebuck = 0.0;
#ifdef TTMNRG
    ebuck = GetBuckingham(do_grads);
#endif

#ifdef TIMING
    auto t3 = std::chrono::high_resolution_clock::now();
#endif

    double elj = 0.0;
#ifdef TTMNRG
    if (lj_pairs_.size() > 0) elj = GetLennardJones(do_grads);
#endif

#ifdef TIMING
    auto t31 = std::chrono::high_resolution_clock::now();
#endif

    // double e3b = 0.0;
    double e3b = Get3B(do_grads);

#ifdef TIMING
    auto t32 = std::chrono::high_resolution_clock::now();
#endif

    double e4b = 0.0;
    if (cutoff4b_ > std::numeric_limits<double>::epsilon()) {
        e4b = Get4B(do_grads);
    }

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
    energy_ = eff + e1b + e2b + e3b + e4b + edisp + ebuck + elj + Eelec;

#ifdef PRINT_INDIVIDUAL_TERMS
    std::cerr << std::setprecision(20) << std::scientific;
    std::cerr << "1B = " << e1b << std::endl
              << "FF = " << eff << std::endl
              << "2B = " << e2b << std::endl
              << "3B = " << e3b << std::endl
              << "4B = " << e4b << std::endl
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
              << std::chrono::duration_cast<std::chrono::milliseconds>(t32 - t31).count() << " milliseconds\n";
    std::cerr << "System::4b(grad=" << do_grads << ") "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t32).count() << " milliseconds\n";
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
        indx += iend;
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
    step = num_threads;
#endif  // _OPENMP

    // Variables to be used for both serial and parallel implementation
    size_t first_grad = 0;
    size_t last_grad = 3 * numsites_;
    int rank = 0;
    std::vector<size_t> idxs;

    // Vector pools that allow compatibility between
    // serial and parallel implementation
    std::vector<double> e2b_pool(num_threads, 0.0);
    std::vector<std::vector<double>> grad_pool(num_threads);
    std::vector<std::vector<double>> virial_pool(num_threads);  // declare virial pool

    std::vector<std::vector<size_t>> all_dimers(num_threads);

#ifdef _OPENMP
#pragma omp parallel private(rank, idxs)
    {
        rank = omp_get_thread_num();
#endif  // _OPENMP
        for (size_t i = rank; i < nummon_; i += step) {
            idxs.push_back(i);
        }
        // Adding corresponding clusters depending on if we are within
        // OPENMP or not

        // This call will get the dimers that have as first index a monomer
        // with index between istart and iend (iend not included)

        all_dimers[rank] = AddClustersParallel(2, cutoff2b_, idxs, use_ghost);

        grad_pool[rank] = std::vector<double>(3 * numsites_, 0.0);
        virial_pool[rank] = std::vector<double>(9, 0.0);

#ifdef _OPENMP  
    }
#endif

    std::vector<size_t> dimers_pool;

    for(size_t i = 0; i < num_threads; i++) {
        dimers_pool.insert(dimers_pool.end(), all_dimers[i].begin(), all_dimers[i].end());
        all_dimers[i].clear();
    }

    size_t dimers_pool_index = 0;

    // this variable is the maximum number of dimers that will be dispached to a thread at a time.
    // the number of trimers will be smaller near the end of the evaluaton when there are fewer dimers.
    // should probably be a multiple of 8 for compatibility with uncoming SIMD PIP evaluation.
    const size_t batch_size = 16;

    // actually calculate the dimers
#ifdef _OPENMP
#pragma omp parallel private(rank, idxs) shared(dimers_pool_index)
    {
        rank = omp_get_thread_num();
#endif

        std::vector<size_t>& dimers = all_dimers[rank];

        size_t start_index;
        size_t this_batch_size;

        #pragma omp critical(dimers_pool_index)
        {
            start_index = dimers_pool_index;
            this_batch_size = std::min(batch_size, (dimers_pool.size() - start_index) / 2);
            dimers_pool_index += this_batch_size * 2;
        }
        dimers.insert(dimers.end(), dimers_pool.begin() + start_index, dimers_pool.begin() + start_index + this_batch_size*2);

        // In order to continue, we need at least one dimer
        // If the size of the dimer vector is not at least 2, means
        // that we don't have any dimer
        bool skip = false;
        if (dimers.size() < 2) {
            skip = true;
            dimers.clear();
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
        std::string m1;
        std::string m2;

        if (!skip) {
            m1 = monomers_[dimers[0]];
            m2 = monomers_[dimers[1]];
        }

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
            
            if(dimers.size() - 2 * (nd_tot + nd_bad + nd) == 0) {

                size_t start_index;
                size_t this_batch_size;

                #pragma omp critical(dimers_pool_index)
                {
                    start_index = dimers_pool_index;
                    this_batch_size = std::min(batch_size, (dimers_pool.size() - start_index) / 2);
                    dimers_pool_index += this_batch_size * 2;

                }
                dimers.insert(dimers.end(), dimers_pool.begin() + start_index, dimers_pool.begin() + start_index + this_batch_size*2);
            }
        }
#ifdef _OPENMP
    }  // parallel

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

    // if (use_ghost) e2b_t *= 0.5;
    // Condensate virial
    const double scalev = 1.0;
    // const double scalev = use_ghost ? 0.5 : 1.0;
    for (int i = 0; i < num_threads; i++) {
        for (size_t j = 0; j < 9; j++) {
            virial_[j] += scalev * virial_pool[i][j];
        }
    }

    return e2b_t;
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

double System::FourBodyEnergy(bool do_grads, bool use_ghost) {
    // Check if system has been initialized
    // If not, throw exception
    if (!initialized_) {
        std::string text =
            std::string("System has not been initialized. ") + std::string("4B Energy calculation not possible.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    energy_ = 0.0;
    std::fill(grad_.begin(), grad_.end(), 0.0);
    std::fill(virial_.begin(), virial_.end(), 0.0);

    SetPBC(box_);

    if (cutoff4b_ > std::numeric_limits<double>::epsilon()) {
        energy_ = Get4B(do_grads, use_ghost);
    }

    return energy_;
}


const double thefit[] = {
    -1.743377197070875e+01,  // 0
    2.089311656832017e+02,   // 1
    1.087427688530270e+02,   // 2
    -1.283744841245588e+02,  // 3
    5.596663854789029e+01,   // 4
    1.425941898038279e+01,   // 5
    6.128551002039593e+01,   // 6
    1.733353924301668e+00,   // 7
    9.842973046886841e+01,   // 8
    -7.117037275194882e+01,  // 9
    -1.827803643046749e+02,  // 10
    -1.973360263961605e+02,  // 11
    -1.281468919584132e+02,  // 12
    2.444689615336278e-01,   // 13
    4.354289750064604e+00,   // 14
    -7.471440488451695e+00,  // 15
    8.702668612497430e+00,   // 16
    4.763609490185287e+01,   // 17
    1.143722522296538e+00,   // 18
    -1.149353943107063e+02,  // 19
    -8.309578296560531e+00,  // 20
    1.700065724784118e+00,   // 21
    -1.034653523164574e+00,  // 22
    -9.693920276402674e-01,  // 23
    9.585551929246364e+01,   // 24
    -1.214294676227674e+00,  // 25
    1.040853036759069e+02,   // 26
    8.139651134259881e+01,   // 27
    -5.022950429222917e-01,  // 28
    8.062976299999429e+00,   // 29
    -1.053099974111210e+00,  // 30
    -2.487891615528081e-01,  // 31
    -3.657812319314182e+00,  // 32
    2.799893446283987e+01,   // 33
    -1.009606561081473e+00,  // 34
    1.813850975920025e+00,   // 35
    -2.605903397841693e+00,  // 36
    9.996355874102957e+00,   // 37
    -8.208497585239612e-01,  // 38
    -6.016177469157579e+01,  // 39
    -8.970942083101251e+01,  // 40
    -1.611245164498255e-01,  // 41
    1.628202777237015e+01,   // 42
    5.884927653121586e-01,   // 43
    1.189806746324836e+02,   // 44
    1.924383296233176e+00,   // 45
    4.241577037344579e+00,   // 46
    -1.412059381528348e+00,  // 47
    1.374513183815514e+01,   // 48
    -6.880224507175423e+01,  // 49
    -2.555833934325997e-01,  // 50
    1.624855431045398e+00,   // 51
    1.021233380854902e+00,   // 52
    -7.225055866489238e+00,  // 53
    1.025511607201054e+01,   // 54
    -1.888277844494636e+01,  // 55
    4.593074666320076e+00,   // 56
    -5.020535640898480e-01,  // 57
    3.911657419453151e+00,   // 58
    3.309293152875913e+00,   // 59
    -4.212666617897496e+00,  // 60
    2.111363711389890e+02,   // 61
    8.722691691408075e-01,   // 62
    -2.180349351286207e-01,  // 63
    1.175361536237898e+00,   // 64
    5.860290998567241e-01,   // 65
    1.739248188249690e+00,   // 66
    -2.000282438634904e+00,  // 67
    -2.353346492574213e+00,  // 68
    9.625774535491269e-01,   // 69
    1.786328361452429e+01,   // 70
    4.033627053505576e+00,   // 71
    1.213684530735588e+00,   // 72
    -4.894153319027960e+02,  // 73
    1.642473440431330e-01,   // 74
    -4.259120564359865e-01,  // 75
    -1.552539473740763e+01,  // 76
    -8.582145395207064e+00,  // 77
    2.277734026050378e-01,   // 78
    -8.274747107221707e+01,  // 79
    -3.783245965514562e+01,  // 80
    -1.754860655719373e+01,  // 81
    -2.738284011783712e-01,  // 82
    8.978535369767620e+00,   // 83
    7.501036072934211e-01,   // 84
    -1.622203588189491e-01,  // 85
    -1.392945517822096e+00,  // 86
    -1.805402577227933e+02,  // 87
    1.103802095398265e+02,   // 88
    -2.726417014726193e-01,  // 89
    -5.308218786107749e+00,  // 90
    1.056609493913411e+01,   // 91
    -1.632643730369313e+00,  // 92
    2.759259255482202e+00,   // 93
    2.665573305059726e+01,   // 94
    -6.763562138326729e+00,  // 95
    7.770223029272005e+01,   // 96
    1.918304286356732e-01,   // 97
    5.144261823225600e+01,   // 98
    3.455320702797132e-01,   // 99
    -1.771538604656494e+02,  // 100
    6.957869909215631e+00,   // 101
    5.507495837585845e+01,   // 102
    -3.974962519646235e+01,  // 103
    6.344088131985568e+01,   // 104
    2.716534685106605e-01,   // 105
    -7.489631238230149e+01,  // 106
    -2.751521953248500e+00,  // 107
    -7.547568696931751e+00,  // 108
    -1.622664362078711e+01,  // 109
    -8.361068914663519e-02,  // 110
    -7.089665528236211e-01,  // 111
    5.852440236016893e+01,   // 112
    4.663639216071220e+00,   // 113
    -1.229128124490421e+02,  // 114
    -2.047419976100477e+00,  // 115
    -5.117518065513930e+01,  // 116
    -6.019542387638406e-01,  // 117
    -7.723968230177208e+01,  // 118
    -7.074762171814234e-01,  // 119
    1.498675041530755e+00,   // 120
    -4.597214344314243e-01,  // 121
    -1.023254862613206e+01,  // 122
    1.533178274577317e+02,   // 123
    6.728007298180304e-01,   // 124
    1.627924691074581e-01,   // 125
    1.937496501700873e+00,   // 126
    -5.408821221773594e+00,  // 127
    1.188324360107360e+00,   // 128
    -2.194597223722268e+00,  // 129
    1.237397244554251e+01,   // 130
    1.598093187154409e+00,   // 131
    1.117853473906935e+01,   // 132
    -3.178560856626202e+01,  // 133
    -1.524299543930228e+00,  // 134
    -1.423591003012400e+02,  // 135
    7.504916486777256e-02,   // 136
    -2.330697890022995e+01,  // 137
    -8.918972894948419e+00,  // 138
    1.804507106085390e+01,   // 139
    -9.378661800435363e+00,  // 140
    -9.666748415802519e+01,  // 141
    -2.184496632530373e+01,  // 142
    -9.518611876180785e-02,  // 143
    3.087619187794188e-01,   // 144
    4.205222987348804e+01,   // 145
    -1.181836421818782e+01,  // 146
    -2.330863522818624e-01,  // 147
    2.514640480512810e+00,   // 148
    -1.793719655794914e+00,  // 149
    1.516680267783703e+00,   // 150
    5.438385280777460e-01,   // 151
    3.083347838490701e+01,   // 152
    -1.005935449699854e+01,  // 153
    2.286593561720426e+00,   // 154
    -9.492326286252402e-02,  // 155
    1.476734389010896e+01,   // 156
    -9.215976204155556e+00,  // 157
    4.869535401832681e+00,   // 158
    1.133759192615345e+02,   // 159
    -2.148775640188943e-01,  // 160
    -1.401240995101314e+00,  // 161
    -5.110719104228154e-01,  // 162
    -1.844633416702736e+01,  // 163
    3.110265286460514e+01,   // 164
    6.410752443393297e+00,   // 165
    7.157049016545996e+00,   // 166
    -3.290989399400059e+01,  // 167
    3.844318083078153e+01,   // 168
    -8.565693991469944e+01,  // 169
    -1.218331075963810e+00,  // 170
    3.351058551940511e+01,   // 171
    1.702521052319496e+00,   // 172
    1.381096971763033e-01,   // 173
    2.289922338771239e+01,   // 174
    1.140845970568720e+02,   // 175
    1.587910776958094e+01,   // 176
    -9.216091068658055e-01,  // 177
    2.436637371459236e-01,   // 178
    -8.495103010308276e-01,  // 179
    9.096836526492669e+01,   // 180
    -4.681735162108578e-01,  // 181
    -5.555999390424734e+01,  // 182
    -3.712073186549133e+00,  // 183
    3.888426592077457e-01,   // 184
    -2.544550362621610e+00,  // 185
    5.311489677093989e+01,   // 186
    7.904842328391773e+00,   // 187
    -6.875100798647009e+00,  // 188
    3.858331726786371e-01,   // 189
    -1.233196819260199e+00,  // 190
    -6.002276278336611e+01,  // 191
    -1.592142895475853e+01,  // 192
    3.813572287643656e+00,   // 193
    4.634191164566599e+01,   // 194
    -2.691627606735205e+00,  // 195
    -2.131261435882293e+02,  // 196
    -3.266953579417035e+01,  // 197
    1.120910660307600e+02,   // 198
    3.420474570245723e+01,   // 199
    -1.537366223528300e+02,  // 200
    6.717025915608451e+02,   // 201
    -3.631391855144807e+00,  // 202
    2.685994516074155e-02,   // 203
    6.265485710883192e+01,   // 204
    -5.549446123382322e-01,  // 205
    2.960852462194497e+01,   // 206
    -1.994147706914930e+02,  // 207
    -4.081751339352151e+00,  // 208
    -6.792584631165363e+00,  // 209
    7.489409509162790e+01,   // 210
    -2.087848174416133e+01,  // 211
    8.296606397759871e+01,   // 212
    -5.902918469889365e+01,  // 213
    3.571721823934464e+02,   // 214
    5.133308008900522e-02,   // 215
    2.652683352745167e-05,   // 216
    2.658937930018420e-02,   // 217
    7.803934567375077e-01,   // 218
    1.030787393334626e-03,   // 219
    -8.877679379611735e-01,  // 220
    3.417621959305811e+00,   // 221
    -2.094175527340597e-02,  // 222
    3.534616198082318e-03,   // 223
    4.786554967010400e+01,   // 224
    4.820613600935067e-03,   // 225
    -3.245629700342000e-03,  // 226
    1.052503498989095e-01,   // 227
    -3.489374200116073e-03,  // 228
    2.222768234151123e-03,   // 229
    -1.263323219129145e-01,  // 230
    3.800192224012195e-02,   // 231
    3.952537557377575e-03,   // 232
    -1.568397252618155e+00,  // 233
    -5.771159451779162e-02,  // 234
    8.478306299942848e-03,   // 235
    -7.455492746090956e-01,  // 236
    4.002781274380017e+00,   // 237
    -3.109263652067190e+01,  // 238
    3.361319492849633e-03,   // 239
    -2.211380268688152e-01,  // 240
    -5.544091119169866e-03,  // 241
    -1.483964894128340e-02,  // 242
    3.670900331497255e-01,   // 243
    5.814514725417180e+00,   // 244
    -1.087131320262548e-02,  // 245
    -7.442650883455812e-01,  // 246
    -1.874393762930955e+00,  // 247
    3.937782459389709e-02,   // 248
    -2.710015362132304e+00,  // 249
    -8.852923558718409e-02,  // 250
    5.293147849321845e-03,   // 251
    3.682965280596182e-04,   // 252
    -1.202527297899182e-03,  // 253
    -1.796601299015969e+01,  // 254
    -7.794467876039174e+00,  // 255
    8.786940546828319e-03,   // 256
    -1.816743903665622e-01,  // 257
    -8.964665837603922e-04,  // 258
    1.418860643845550e-03,   // 259
    -3.569599278632730e-01,  // 260
    -3.001229701019655e-02,  // 261
    1.811323137424351e-01,   // 262
    -5.941404748293290e-01,  // 263
    -5.638896698244495e-04,  // 264
    1.997978167028790e-03,   // 265
    5.226467459964887e+00,   // 266
    -1.041965640072034e-01,  // 267
    -3.784831437555939e-03,  // 268
    -3.082116716276968e-04,  // 269
    -3.340452023283473e+00,  // 270
    -4.670434048726762e-03,  // 271
    -4.792362970578469e-02,  // 272
    -9.789729926703561e-02,  // 273
    2.660907842390989e-02,   // 274
    -4.544672382159782e-03,  // 275
    -4.187866866613026e-02,  // 276
    6.000317739769601e+00,   // 277
    -7.011983101446932e-03,  // 278
    -1.151536157362954e-01,  // 279
    2.539175076471459e+00,   // 280
    -1.772399318222060e-01,  // 281
    -4.135217103652104e-05,  // 282
    -8.136220484902113e-03,  // 283
    7.412063114815547e+00,   // 284
    -2.724932563478752e-02,  // 285
    2.396626453666635e-04,   // 286
    6.230531421384776e+00,   // 287
    1.017909591193059e-02,   // 288
    -6.140005771856310e-02,  // 289
    7.870075825570919e-02,   // 290
    1.192494205191334e-01,   // 291
    6.791992601576138e-01,   // 292
    6.786341914153099e-01,   // 293
    2.011075985388900e+00,   // 294
    2.290936615229433e+00,   // 295
    -1.538671670559566e+00,  // 296
    3.083760567400105e-02,   // 297
    1.427483183010443e-02,   // 298
    -1.940867175466954e-02,  // 299
    1.014554460505766e-03,   // 300
    -8.494847115733275e-04,  // 301
    -7.329606246554069e-01,  // 302
    1.699700070265220e-01,   // 303
    1.270273376355035e-02,   // 304
    -4.342285042961053e-02,  // 305
    -1.060296835226691e-02,  // 306
    -1.674179446100242e-03,  // 307
    2.500049694767205e+00,   // 308
    7.649201555727974e-03,   // 309
    -1.380134001071024e-02,  // 310
    -2.922101180993474e+01,  // 311
    5.170890241043023e-02,   // 312
    8.734843636833807e-02,   // 313
    5.445931047237135e-01,   // 314
    -8.165940276007126e-02,  // 315
    -1.746065801250673e-03,  // 316
    6.186355796118587e-02,   // 317
    1.182365730507747e-02,   // 318
    -5.776285537210974e-01,  // 319
    -5.814111876501702e-02,  // 320
    -7.882727525716804e-01,  // 321
    5.001015931849398e-04,   // 322
    5.864147754089364e-03,   // 323
    -1.220743485255082e-01,  // 324
    -7.677349014631721e+00,  // 325
    1.588335912720998e-03,   // 326
    3.506720033509213e+00,   // 327
    1.058307270417112e-03,   // 328
    2.444421810034157e-02,   // 329
    2.623700351184917e-01,   // 330
    3.083994322414911e-02,   // 331
    5.133850835015739e-02,   // 332
    -1.830337347855642e-01,  // 333
    1.118634814170023e-01,   // 334
    2.799308565625013e-02,   // 335
    3.272749995799451e+00,   // 336
    -3.122892533859183e-02,  // 337
    -3.905226497032339e-02,  // 338
    -1.323430911089954e-01,  // 339
    2.116948135761228e+01,   // 340
    2.064432791420956e-04,   // 341
    1.615194078901324e-03,   // 342
    1.980388999870532e-02,   // 343
    3.915916313713674e+00,   // 344
    8.645198282120942e+00,   // 345
    -2.330381496249324e-03,  // 346
    -1.100777810480990e+00,  // 347
    -2.442687615262339e-03,  // 348
    1.177395950260341e-01,   // 349
    1.147093336154691e+02,   // 350
    -1.247067650501886e+01,  // 351
    -9.996354171490145e-02,  // 352
    1.540526927769556e-02,   // 353
    1.753936220321124e-03,   // 354
    -2.228050394870721e-01,  // 355
    1.076752645549304e-02,   // 356
    4.558553728073014e-01,   // 357
    -3.361824809096642e-03,  // 358
    -1.436049088139224e+00,  // 359
    -5.403962563150141e-01,  // 360
    -3.324632003716473e-03,  // 361
    1.027271059721645e-02,   // 362
    -1.753424146111465e+00,  // 363
    -1.470422070666924e-01,  // 364
    3.061178846735091e-03,   // 365
    -2.992273340941334e-02,  // 366
    1.387333984712209e+00,   // 367
    2.382775366468545e-01,   // 368
    3.838964570755236e-02,   // 369
    1.872905840854447e+00,   // 370
    9.578112070082479e-03,   // 371
    -2.216238681424270e-02,  // 372
    -2.259718963559710e-01,  // 373
    -3.068848754970457e-03,  // 374
    2.635681007592174e-02,   // 375
    -4.189303187664893e-01,  // 376
    1.725885993656271e+01,   // 377
    -1.491288459411886e+00,  // 378
    -1.741991302762992e-01,  // 379
    1.932624985606446e-02,   // 380
    -6.074744411934375e-01,  // 381
    -1.031140893164147e-02,  // 382
    1.242504493553741e-02,   // 383
    -1.073293953720950e-02,  // 384
    1.171224934496619e+01,   // 385
    -3.464804988310392e-02,  // 386
    -4.675478793836237e-01,  // 387
    1.295843762238342e-02,   // 388
    5.803361581696150e-01,   // 389
    2.692140638327853e+00,   // 390
    -4.490855474267638e-03,  // 391
    -1.261442253472564e+00,  // 392
    2.372334900375904e-01,   // 393
    -2.737640152377385e-03,  // 394
    -8.562845489266363e-03,  // 395
    -3.524996572246576e-01,  // 396
    2.954679197970651e+01,   // 397
    -3.267975128991715e-02,  // 398
    3.945050585833640e-01,   // 399
    -1.191711402734527e-02,  // 400
    2.131911262962211e-01,   // 401
    3.102755114200252e-01,   // 402
    5.461772089151813e-03,   // 403
    -3.043522074702151e-01,  // 404
    2.779219825368406e+01,   // 405
    -8.185349073159251e-02,  // 406
    2.142448767808915e+01,   // 407
    -4.242337681788379e-02,  // 408
    -5.339370405815078e-01,  // 409
    3.190752878438325e+00,   // 410
    4.096013889597520e-03,   // 411
    7.254892108753187e-01,   // 412
    2.483532940171792e+00,   // 413
    1.492880565693585e+00,   // 414
    2.930360460142368e+01,   // 415
    5.251933434131837e-02,   // 416
    2.603131314816191e-04,   // 417
    4.614718854777198e-02,   // 418
    5.441088453290240e-01,   // 419
    -6.998066213259395e-03,  // 420
    1.053367896988218e-01,   // 421
    6.472039246652898e-01,   // 422
    3.514058493346884e-01,   // 423
    -5.414497445255300e+00,  // 424
    5.204023490229749e+00,   // 425
    -1.136688076087783e-02,  // 426
    -9.852900954586924e-02,  // 427
    -7.422752439330707e-03,  // 428
    1.170080796162793e+01,   // 429
    -5.076724312271764e+00,  // 430
    3.608445694453787e-01,   // 431
    5.250138165633719e-01,   // 432
    9.906539038667259e-01,   // 433
    -9.403300114534250e-01,  // 434
    -8.026069602486638e+00,  // 435
    -2.033567177207880e-02,  // 436
    -3.245292926312459e-03,  // 437
    -1.597249677394883e+00,  // 438
    2.824398121935303e-03,   // 439
    -2.800824939933018e+01,  // 440
    -1.995579057024330e-02,  // 441
    -2.239762477186968e-01,  // 442
    -5.138408222039806e-02,  // 443
    8.639210037593706e-03,   // 444
    -7.862182381694267e-03,  // 445
    3.930116569514527e-01,   // 446
    2.458184119806207e-03,   // 447
    -9.871156708689190e-03,  // 448
    -2.434963899607852e-03,  // 449
    -1.055039938909338e+01,  // 450
    -1.376430126899212e-02,  // 451
    4.198803950246857e-02,   // 452
    -5.278740054820918e+00,  // 453
    7.382212172403865e-02,   // 454
    -3.272383941148044e-02,  // 455
    -2.408616518614535e-03,  // 456
    6.487611086177364e-01,   // 457
    -1.568850333250843e-01,  // 458
    -1.173774216216808e-01,  // 459
    9.306973833838490e-02,   // 460
    -4.894389086815477e-02,  // 461
    1.010183913324022e-03,   // 462
    -2.133311812677051e-02,  // 463
    -2.337006216735706e-01,  // 464
    -6.478598799152459e-02,  // 465
    -2.477659010982667e+00,  // 466
    1.839465075061955e-03,   // 467
    6.593121023740296e-02,   // 468
    -1.246951075688066e+01,  // 469
    -1.245506131817022e-02,  // 470
    -6.957111273886731e-02,  // 471
    -2.565408863964179e+00,  // 472
    5.050970371224974e-01,   // 473
    -7.796049634300577e-04,  // 474
    4.758547237319551e-03,   // 475
    3.678529004111872e-01,   // 476
    -2.980048781677960e+00,  // 477
    2.564492608403556e-03,   // 478
    -4.220261467995212e+00,  // 479
    -5.151499214072393e-01,  // 480
    7.120432073153814e-02,   // 481
    -9.919656143675991e-01,  // 482
    1.434924245542416e-01,   // 483
    -3.606450193859170e-02,  // 484
    -1.954911820289601e+00,  // 485
    -5.229355429352110e-02,  // 486
    -9.811915727558745e-02,  // 487
    6.388621339666997e+00,   // 488
    4.715816591793746e-02,   // 489
    9.675036529459025e+00,   // 490
    4.220551889136098e-04,   // 491
    -1.323843184933878e-01,  // 492
    -2.676871969702662e+00,  // 493
    -1.578320497293936e-01,  // 494
    2.089656381992425e-02,   // 495
    -9.011018320367931e+00,  // 496
    -2.822173666228803e-03,  // 497
    8.329472372221387e-03,   // 498
    -8.622570084673084e+00,  // 499
    -1.178173085170413e-01,  // 500
    1.530351227874766e-01,   // 501
    4.999235962131203e-03,   // 502
    2.877156890760036e+01,   // 503
    5.342330652371230e+01,   // 504
    6.922781462817021e-01,   // 505
    2.836172131801216e+00,   // 506
    6.106930872017315e-01,   // 507
    -1.233163881941533e-02,  // 508
    3.991446191708811e-01,   // 509
    -1.153836574793918e-01,  // 510
    2.314094017836076e-02,   // 511
    1.505646321058291e-03,   // 512
    -7.205109782620691e-01,  // 513
    -1.020719208597748e-02,  // 514
    -1.805638021149984e-02,  // 515
    -5.085968382226197e-03,  // 516
    9.465616497804612e-01,   // 517
    -1.509595675584939e-02,  // 518
    -7.454126029967189e-01,  // 519
    7.117572419660592e-01,   // 520
    2.126148344998907e-01,   // 521
    -3.466094278451300e-01,  // 522
    6.763117623087330e-01,   // 523
    4.045318154885927e-02,   // 524
    -6.660456107052027e+00,  // 525
    1.141076099981283e-01,   // 526
    -3.394494545401501e-02,  // 527
    3.430764991061228e-03,   // 528
    6.147317688153234e+00,   // 529
    -1.959151292800480e-01,  // 530
    3.903797453021848e-03,   // 531
    3.777917015737976e-02,   // 532
    -8.407131955118456e-02,  // 533
    -1.215831441781701e-02,  // 534
    -2.767387985298763e+01,  // 535
    -8.555521382702604e-03,  // 536
    3.822214585974535e-02,   // 537
    3.211411772086818e-01,   // 538
    -6.639988589477107e-02,  // 539
    -2.283233361517897e+01,  // 540
    2.690293939693652e+01,   // 541
    -5.715741806709360e-02,  // 542
    6.472587781136225e-01,   // 543
    -1.256672716168632e-01,  // 544
    -2.598924527031452e+00,  // 545
    4.074318339169676e-02,   // 546
    -6.976572885161410e+00,  // 547
    -1.580355926374533e-03,  // 548
    -2.502435285833398e-02,  // 549
    3.084655568362317e-02,   // 550
    -7.798843391754906e-01,  // 551
    -7.420289795220082e-02,  // 552
    6.198829320447029e-03,   // 553
    1.045238896404645e-02,   // 554
    7.910505661305669e-01,   // 555
    -4.940853399531326e-03,  // 556
    -1.726161853888303e+00,  // 557
    -2.312034181317032e+01,  // 558
    1.283724655040418e-02,   // 559
    -2.857546338955981e+00,  // 560
    1.524326868335806e+00,   // 561
    1.327727541581163e-01,   // 562
    3.291614550728984e-02,   // 563
    3.655931810871264e-04,   // 564
    -1.200935233490773e+00,  // 565
    -1.006965255819922e-01,  // 566
    8.118126323017096e-01,   // 567
    -3.232013258446483e-02,  // 568
    -3.891410839595651e-03,  // 569
    -2.233559770890816e-02,  // 570
    7.826296683004475e+00,   // 571
    9.173011081219975e-03,   // 572
    1.884163780612995e-02,   // 573
    -1.867974581656789e+00,  // 574
    4.984187970486471e-02,   // 575
    -1.077261606189206e-01,  // 576
    -1.115959076639258e-02,  // 577
    -2.817972632405085e-02,  // 578
    4.701799444926386e-02,   // 579
    5.707988534023126e-02,   // 580
    1.400845473815132e+01,   // 581
    2.817080102461249e-01,   // 582
    5.214090713075231e-02,   // 583
    -6.596212913513438e-01,  // 584
    -4.373481912806742e-01,  // 585
    -8.977807909408600e-03,  // 586
    4.850548670252803e+01,   // 587
    -3.813292397922340e-02,  // 588
    -5.041556914822190e+01,  // 589
    -3.010651490667366e-01,  // 590
    9.708146414864689e-03,   // 591
    7.836736994216005e-03,   // 592
    5.857304566092262e-02,   // 593
    -3.473695975246328e+00,  // 594
    2.088814612295500e-01,   // 595
    -3.212227827690294e+00,  // 596
    -4.257134639891898e+00,  // 597
    3.967332222181145e-03,   // 598
    3.947103632020374e-03,   // 599
    -1.100517121053015e-02,  // 600
    -3.537054679987686e+01,  // 601
    2.377793535673771e-03,   // 602
    9.807391782288910e-01,   // 603
    5.162175770857211e-01,   // 604
    -1.410545054269260e+01,  // 605
    -2.681517725414551e-04,  // 606
    1.761358656394800e-01,   // 607
    3.323260119251726e-01,   // 608
    9.404277917596650e-01,   // 609
    9.888645202252144e-03,   // 610
    1.958970105992761e+00,   // 611
    8.519681969762151e-02,   // 612
    -1.089716769475379e-03,  // 613
    8.376871688076229e-03,   // 614
    -7.693079577131897e-04,  // 615
    -1.017676357823405e+00,  // 616
    -5.652509235744939e-02,  // 617
    -9.093176442590183e-03,  // 618
    -6.351255165854130e-03,  // 619
    -1.327950788751545e+01,  // 620
    1.840323610639356e-02,   // 621
    2.731474855608204e+01,   // 622
    -1.924015266991842e+01,  // 623
    -3.222957239756754e-03,  // 624
    2.209357139990350e-03,   // 625
    -8.812577598679345e+00,  // 626
    -9.519926289947279e-04,  // 627
    9.027369085848162e-03,   // 628
    7.860417928149238e-03,   // 629
    1.310746466261008e-01,   // 630
    -1.175109818844566e-01,  // 631
    2.212847418181007e+00,   // 632
    6.831671390711499e-03,   // 633
    -4.091709846290400e-04,  // 634
    -6.024137602958523e-04,  // 635
    3.882131876763623e+00,   // 636
    -4.575114058590114e-03,  // 637
    7.565346711945239e-02,   // 638
    -2.806361707359176e-03,  // 639
    1.798733038615828e+02,   // 640
    -8.033243290778775e-03,  // 641
    6.849990082142975e-02,   // 642
    -7.825306986981775e-04,  // 643
    -4.787731411551997e-04,  // 644
    -1.401657952099281e-02,  // 645
    -4.665963382345655e-03,  // 646
    4.094691187239955e-03,   // 647
    2.896215142785552e+00,   // 648
    -2.661225691643502e-03,  // 649
    8.020004385896143e-02,   // 650
    -1.345064420161292e+00,  // 651
    -1.038496518748241e+01,  // 652
    -4.794078586038616e+00,  // 653
    1.431377766044731e-03,   // 654
    -3.406479589013402e-02,  // 655
    -3.993748041541398e-01,  // 656
    1.478207045618181e+00,   // 657
    -6.750470330234515e-02,  // 658
    4.345810570187554e-01,   // 659
    -5.171661510743115e+00,  // 660
    8.435597057784796e-04,   // 661
    2.002426354440091e-02,   // 662
    -1.060277917589278e+00,  // 663
    5.931174587599368e+00,   // 664
    5.808997718573138e-01,   // 665
    1.216430982834748e+01,   // 666
    -4.339581330308650e-03,  // 667
    4.462373496876091e+00,   // 668
    -4.014243475772505e-04,  // 669
    5.341849274623510e+01,   // 670
    -2.830842131323120e-03,  // 671
    4.988572007128142e+00,   // 672
    1.906612488897250e-02,   // 673
    1.045316930674116e+00,   // 674
    7.495302800086721e-03,   // 675
    -3.671033826409527e+01,  // 676
    3.047525519436016e-03,   // 677
    2.028516903925590e+00,   // 678
    3.051901174799468e-03,   // 679
    4.204806931915363e+00,   // 680
    -9.775842722224158e-02,  // 681
    1.303247671491178e-02,   // 682
    -9.048222874572991e+00,  // 683
    1.866835641187513e-02,   // 684
    -2.535765908557723e-02,  // 685
    2.637239587255297e-02,   // 686
    7.637047914252713e-02,   // 687
    9.373261101544150e-02,   // 688
    2.517208681195202e-03,   // 689
    -2.105937972156083e-02,  // 690
    -5.440093592559280e+00,  // 691
    6.199097081147550e-03,   // 692
    1.849790565631803e-02,   // 693
    -4.935676951245974e-03,  // 694
    6.959726360520742e-02,   // 695
    7.679030383531246e-03,   // 696
    1.343035008838794e-01,   // 697
    4.484365945508977e-01,   // 698
    -1.420122772333366e-01,  // 699
    5.418760767328135e-01,   // 700
    -2.300146657072991e-03,  // 701
    8.453074050515565e-02,   // 702
    -4.604766262137260e-02,  // 703
    -1.929300797787945e-03,  // 704
    -1.716962823579036e-02,  // 705
    -2.368102778786970e-01,  // 706
    2.083914509428654e+00,   // 707
    -3.681575212322585e-03,  // 708
    -2.649356087520623e+01,  // 709
    2.647256296705505e-02,   // 710
    1.722942773058193e-01,   // 711
    3.361447832909950e+01,   // 712
    -3.754510596663498e-01,  // 713
    -1.365148521879600e-02,  // 714
    3.357339649396429e-03,   // 715
    6.490299427278759e-02,   // 716
    3.028577777810502e-02,   // 717
    1.006847396526122e+01,   // 718
    -4.960384665446082e-03,  // 719
    6.236025745127597e-02,   // 720
    -3.956758231078534e-01,  // 721
    1.632374428291740e-01,   // 722
    8.958656563724056e-03,   // 723
    -2.573176382593476e-01,  // 724
    1.650227403357042e-02,   // 725
    -5.422207681925119e-01,  // 726
    -2.688662615545741e+01,  // 727
    -6.732219649424038e+00,  // 728
    -4.888382044656479e-03,  // 729
    -8.979258319883472e-02,  // 730
    2.293368633692119e-02,   // 731
    -9.733527239545255e-02,  // 732
    -1.329895297705294e-01,  // 733
    -4.347795458688237e-01,  // 734
    3.881137928220593e-02,   // 735
    9.790613423573052e-02,   // 736
    1.489239684808634e+01,   // 737
    2.513997310785505e-02,   // 738
    7.545275403702525e+00,   // 739
    -1.234555744679412e+01,  // 740
    6.405382520619830e-01,   // 741
    -1.059985343080912e-03,  // 742
    1.521547271221699e-03,   // 743
    -3.521558727233768e-02,  // 744
    -2.574850731385575e-03,  // 745
    -2.188945120374154e-02,  // 746
    -1.183281920929490e-02,  // 747
    1.321103449437879e-02,   // 748
    1.119549393135453e-02,   // 749
    -1.315516080469739e-02,  // 750
    1.152722741879299e+00,   // 751
    1.514279381913393e+01,   // 752
    2.398412036082303e-03,   // 753
    1.388443458372435e+00,   // 754
    -7.173305804978180e-04,  // 755
    3.853140111182515e+01,   // 756
    9.112583048442952e-03,   // 757
    -1.673438931064463e+00,  // 758
    2.049768621321127e+00,   // 759
    -2.150412724634079e-02,  // 760
    1.798846043318953e-02,   // 761
    4.220630282627276e-04,   // 762
    1.581856927759950e+00,   // 763
    7.758246224703978e-04,   // 764
    1.265536356577537e-01,   // 765
    -8.072481031284290e-02,  // 766
    -5.599993681464030e+00,  // 767
    1.168102215848586e-02,   // 768
    -8.144722584333304e-02,  // 769
    7.187594523813430e-03,   // 770
    -2.120194950315693e-01,  // 771
    5.407226808300454e-02,   // 772
    -4.173507004128416e-02,  // 773
    5.984142015249169e-02,   // 774
    7.734259464893621e-02,   // 775
    2.164514879072918e-01,   // 776
    2.252993766411560e-02,   // 777
    -7.058038526961187e-03,  // 778
    1.798399527161585e-01,   // 779
    -2.319899533015398e-01,  // 780
    5.764528577810427e+00,   // 781
    1.348424904550029e-02,   // 782
    -2.255684483941352e-01,  // 783
    9.456653729492192e-01,   // 784
    -3.704288382997454e-01,  // 785
    2.853202261161585e-02,   // 786
    8.487251574313388e-02,   // 787
    -3.678866329465130e-02,  // 788
    7.661409589415701e-02,   // 789
    -1.673553926511387e+01,  // 790
    6.587331439080983e-03,   // 791
    -6.293696166967822e-03,  // 792
    -4.785247798817847e-02,  // 793
    4.507137819516456e-03,   // 794
    -6.566791418424229e+00,  // 795
    -3.267003993972608e-01,  // 796
    2.916174608614152e-01,   // 797
    4.984939939057065e-03,   // 798
    -1.951792672951769e+01,  // 799
    2.855688637173954e+00,   // 800
    -4.645694915123265e-03,  // 801
    3.035352054049378e-03,   // 802
    1.387740832659653e+01,   // 803
    -1.908615424825165e+01,  // 804
    -1.633775031521418e-03,  // 805
    -1.999146378124180e-01,  // 806
    4.778643858787871e-02,   // 807
    -4.773509787004335e-02,  // 808
    -4.441711535242246e-04,  // 809
    1.062517548243530e-03,   // 810
    3.467682399459772e-03,   // 811
    7.362771310477806e-03,   // 812
    -5.854575535933750e-01,  // 813
    -1.166202840072294e-03,  // 814
    -5.701033673929770e+01,  // 815
    6.713855780462873e-01,   // 816
    6.625515754382792e+00,   // 817
    -3.582639611136673e-04,  // 818
    1.239685021437553e+00,   // 819
    -6.066102035794940e-03,  // 820
    -7.965380529100143e-02,  // 821
    2.072436035702553e-01,   // 822
    4.946996215149450e-01,   // 823
    -2.428472245678764e-02,  // 824
    1.137372340508237e+00,   // 825
    -7.617899388715470e-03,  // 826
    3.614305506956612e-03,   // 827
    5.439885650745738e-03,   // 828
    1.568906093579821e-02,   // 829
    6.636895524777094e-03,   // 830
    1.332534508556042e-01,   // 831
    2.213574855260841e-03,   // 832
    9.767322560601809e+00,   // 833
    6.901590017438478e+00,   // 834
    -7.396925662950836e-01,  // 835
    -2.896844649229837e-03,  // 836
    3.510477854567302e+00,   // 837
    3.211070712140820e-02,   // 838
    2.164748713107003e-01,   // 839
    4.761173033123066e-02,   // 840
    1.648691568990321e-01,   // 841
    2.128359054432749e-02,   // 842
    1.128791181893081e-03,   // 843
    1.049171537863386e+00,   // 844
    -5.950166952403805e+00,  // 845
    -1.729696796822725e+00,  // 846
    -6.380502392298867e-03,  // 847
    -8.929102763744902e-02,  // 848
    2.218499336989645e+01,   // 849
    -3.569365188024445e-01,  // 850
    -1.024207746249313e+01,  // 851
    -9.080838509909751e-03,  // 852
    5.031992390277869e-05,   // 853
    -7.775843145060116e+00,  // 854
    -1.906367034052984e-02,  // 855
    1.780910014381157e-03,   // 856
    -2.016007968722374e-02,  // 857
    4.195866563067419e-01,   // 858
    -3.205499329815369e+00,  // 859
    -9.184449750327668e-01,  // 860
    -1.633702537003058e-02,  // 861
    -1.305552681687948e+00,  // 862
    1.936734185549517e-01,   // 863
    2.604540285747533e-02,   // 864
    -1.110304877532001e-02,  // 865
    1.265759070603906e+00,   // 866
    -1.900005461687467e+00,  // 867
    -9.137371883140869e-01,  // 868
    2.226772579293139e-03,   // 869
    6.672570561367534e-04,   // 870
    4.678511293967459e-02,   // 871
    5.465283080000596e+00,   // 872
    -3.316984117739912e-03,  // 873
    -4.183547025600476e-01,  // 874
    -6.517468466879650e-03,  // 875
    8.053667510458484e-02,   // 876
    9.802292489281403e-02,   // 877
    -3.784855703097363e-02,  // 878
    -1.598657131227242e-02,  // 879
    3.279857083429691e-01,   // 880
    -3.027650019182427e-02,  // 881
    -2.128875862421499e-03,  // 882
    -4.901746025664045e-03,  // 883
    -1.176693587807662e+00,  // 884
    3.212581186484663e+01,   // 885
    5.642684857412561e+00,   // 886
    -6.886051575914921e-03,  // 887
    -4.308676881333719e-03,  // 888
    6.961482182284355e+01,   // 889
    1.131837273318742e-01,   // 890
    -1.382406509265952e+00,  // 891
    -4.772293348627419e-03,  // 892
    -5.291127059189860e-02,  // 893
    -2.747108479694347e-03,  // 894
    3.923813986599176e+00,   // 895
    -3.283290719855846e-02,  // 896
    -2.271295503774833e-03,  // 897
    8.194813479275672e-03,   // 898
    -3.107610223944947e-01,  // 899
    1.438173141372485e-01,   // 900
    -1.533181754590502e-02,  // 901
    1.174614106094642e+00,   // 902
    9.750839053317847e-02,   // 903
    3.488941404374235e-01,   // 904
    2.470561624441177e-03,   // 905
    -1.984853758605432e-02,  // 906
    7.864397355903738e-01,   // 907
    -1.975160357806438e-01,  // 908
    7.851747593431241e-03,   // 909
    -1.337277203380824e-02,  // 910
    -5.386948755989135e+00,  // 911
    -1.009789222613320e-01,  // 912
    1.445147635603047e-03,   // 913
    1.387531219710128e-02,   // 914
    -1.185586577268993e-03,  // 915
    -4.722240791668424e-02,  // 916
    -4.678321280101585e-01,  // 917
    -1.206420971198487e+00,  // 918
    -6.373575296308464e-04,  // 919
    -2.505727444388436e-02,  // 920
    2.954873405411071e-03,   // 921
    3.902762339152746e-01,   // 922
    -1.066545803186842e-01,  // 923
    2.890115390887783e-01,   // 924
    4.244469529733346e-02,   // 925
    4.435194317435437e-03,   // 926
    -1.188522746876660e+01,  // 927
    1.344153257596424e-01,   // 928
    -2.353025010986859e-02,  // 929
    2.926768362027083e-01,   // 930
    -4.037477548345599e-03,  // 931
    4.077433366740460e-01,   // 932
    1.107385209940406e+00,   // 933
    -2.147370147194884e+01,  // 934
    3.455326888937749e-03,   // 935
    3.615831248106881e-02,   // 936
    -2.747934987500730e-02,  // 937
    -2.157849138186149e+00,  // 938
    -3.751110986890507e-01,  // 939
    3.568924796532647e-03,   // 940
    2.915778908363236e-02,   // 941
    8.224963099823666e-01,   // 942
    4.940270338905427e+00,   // 943
    1.957676898125186e+00,   // 944
    -1.628801948250545e+01,  // 945
    -2.162153301586336e-03,  // 946
    -8.062560875021226e-01,  // 947
    5.445293440842447e-02,   // 948
    -1.359437096665131e+00,  // 949
    6.986917526341320e-04,   // 950
    4.124253475678591e-01,   // 951
    -2.413981234056311e-02,  // 952
    1.569286907782045e-02,   // 953
    -2.784366579769844e-01,  // 954
    -4.559972017048364e-02,  // 955
    2.056700108787433e-03,   // 956
    -3.413778523224284e-02,  // 957
    -5.045267329169633e-01,  // 958
    -1.304184487617439e-02,  // 959
    -3.488419144799348e-02,  // 960
    1.432983661704427e-03,   // 961
    -7.615624021660241e-03,  // 962
    4.414195474790248e-03,   // 963
    1.930540094389761e-02,   // 964
    1.089000012207512e-01,   // 965
    8.338651570538671e-03,   // 966
    4.532024990759400e-05,   // 967
    3.976483375077324e+00,   // 968
    -1.139526747969113e-03,  // 969
    4.430923855101521e-02,   // 970
    -1.143162426511896e+02,  // 971
    3.703303098073098e-02,   // 972
    2.400240992777174e+01,   // 973
    -2.090464352008559e+01,  // 974
    3.581641486019253e-03,   // 975
    -2.430226976866477e-01,  // 976
    -8.369610256649906e+00,  // 977
    4.619277290994893e-01,   // 978
    -3.830026720862150e-02,  // 979
    -1.026561258375834e-02,  // 980
    -6.703228530753091e+01,  // 981
    1.673863692501776e-01,   // 982
    -3.250170488793043e+00,  // 983
    -4.940338851597019e+00,  // 984
    -3.879898815171673e-02,  // 985
    4.176427302793283e+00,   // 986
    8.777961189491668e-02,   // 987
    1.045685080922106e-03,   // 988
    -1.410612475917650e-01,  // 989
    -6.697944301370675e-01,  // 990
    -2.557752042348960e-03,  // 991
    1.070732054312337e-01,   // 992
    -6.381364819858689e-01,  // 993
    4.681845208916493e-02,   // 994
    -2.124179897992476e+00,  // 995
    4.290494267249705e-04,   // 996
    -2.803445370395707e+00,  // 997
    -1.762133286417721e-03,  // 998
    -1.471942945195998e-01,  // 999
    -4.541035442957606e-03,  // 1000
    -1.687504574713829e+01,  // 1001
    -3.679888303083125e-01,  // 1002
    3.035947995677273e+01,   // 1003
    -3.873346909046425e+00,  // 1004
    2.162465841217898e+01,   // 1005
    5.228555372967759e-02,   // 1006
    -1.063266692299904e-02,  // 1007
    -5.709751528982051e+00,  // 1008
    2.185271174252384e+00,   // 1009
    8.832527589243903e-02,   // 1010
    -7.498183467710566e-03,  // 1011
    -3.039307316626250e-03,  // 1012
    -8.794774930733650e-01,  // 1013
    -2.232913185451781e+00,  // 1014
    -7.336102814879147e-03,  // 1015
    2.879193553823908e-01,   // 1016
    -6.071731180734296e-03,  // 1017
    -1.468822874039240e+01,  // 1018
    1.027219479222846e-03,   // 1019
    9.015014830468896e-01,   // 1020
    -1.386822639743438e-01,  // 1021
    9.211490855899904e-01,   // 1022
    3.650844325310770e-01,   // 1023
    2.302785279593864e-01,   // 1024
    -1.688978167413312e-03,  // 1025
    1.379396442716590e-01,   // 1026
    2.737079810789822e+00,   // 1027
    2.704868503019619e-02,   // 1028
    4.339321263078177e-03,   // 1029
    1.198679979506811e-02,   // 1030
    8.516694146187630e+00,   // 1031
    6.908857742974023e-03,   // 1032
    4.529851430268395e-01,   // 1033
    7.205482405922729e+00,   // 1034
    5.815813769759073e-02,   // 1035
    5.954674832041270e-01,   // 1036
    -2.090275640011986e-02,  // 1037
    -3.980164150142960e+00,  // 1038
    1.382565432246343e+01,   // 1039
    1.003736296814399e-03,   // 1040
    2.185657260302784e-03,   // 1041
    1.196517832807288e-02,   // 1042
    2.609597119606745e-02,   // 1043
    -8.796850430213733e-02,  // 1044
    6.013091775491463e-03,   // 1045
    -1.171718909481054e-02,  // 1046
    1.321169077719419e-01,   // 1047
    -3.606705391058443e+00,  // 1048
    4.739232423878556e-03,   // 1049
    7.253162273581241e-01,   // 1050
    -2.822252249434357e-01,  // 1051
    -1.257000542384439e-03,  // 1052
    6.380186364015496e-04,   // 1053
    4.533487237588532e-02,   // 1054
    2.067255528140726e-02,   // 1055
    -1.552695725834706e-01,  // 1056
    1.635784261761977e-03,   // 1057
    4.652170600083434e-01,   // 1058
    7.136974315744429e-02,   // 1059
    5.601293341052792e-02,   // 1060
    -8.617037983254887e-01,  // 1061
    -1.998001373236299e-02,  // 1062
    8.548789411630450e-03,   // 1063
    -2.925942476002008e-03,  // 1064
    -2.531018057662784e-01,  // 1065
    4.407691268058347e-02,   // 1066
    -1.641838252045272e-01,  // 1067
    -2.033914605742067e+00,  // 1068
    -5.588893118666982e-02,  // 1069
    -2.544032751572282e-04,  // 1070
    4.011845095838008e-04,   // 1071
    -4.333074571810659e-03,  // 1072
    5.103752172780391e-04,   // 1073
    -3.118957411863647e-01,  // 1074
    -3.643460421250412e-01,  // 1075
    8.579851996318508e-01,   // 1076
    -1.708675179597726e-03,  // 1077
    -1.022461089417423e-01,  // 1078
    2.880003645759173e-03,   // 1079
    -1.080780594064200e+01,  // 1080
    6.291629961405575e-02,   // 1081
    -4.014006727397450e-03,  // 1082
    2.327597932961149e-01,   // 1083
    -3.040411309832680e+00,  // 1084
    3.870226914352206e+00,   // 1085
    -5.276921135618440e-03,  // 1086
    -1.562994254948196e-02,  // 1087
    -2.909498013576582e-01,  // 1088
    1.300854233008732e-02,   // 1089
    2.371505277627921e-03,   // 1090
    3.538927317228775e+01,   // 1091
    -1.738903691224657e+00,  // 1092
    3.272411088327627e-02,   // 1093
    -1.034110746303206e-02,  // 1094
    -6.268144048244390e+00,  // 1095
    -3.708223033621890e-01,  // 1096
    -4.605471853199991e-01,  // 1097
    3.003952247723897e-02,   // 1098
    7.535640958211427e-01,   // 1099
    1.449363785933556e-01,   // 1100
    -3.198145770234280e+01,  // 1101
    7.238165209193105e-02,   // 1102
    -4.571752980178633e-04,  // 1103
    -1.087014682264316e-04,  // 1104
    1.305781852076855e-01,   // 1105
    -5.913926857335191e+00,  // 1106
    -6.785233673584841e-02,  // 1107
    -1.839172720027920e-03,  // 1108
    -4.119805005748965e+01,  // 1109
    -5.140591182619549e-02,  // 1110
    -8.134083470873070e-02,  // 1111
    -7.484377077342487e-03,  // 1112
    -2.176358040681251e-01,  // 1113
    9.129184457183274e-01,   // 1114
    -3.069249442907114e-02,  // 1115
    2.411631446526680e+00,   // 1116
    -1.421660883713877e+00,  // 1117
    -1.716950906157263e+00,  // 1118
    3.143340384938381e-01,   // 1119
    5.538770814478473e-04,   // 1120
    -2.016853867816432e+01,  // 1121
    -1.757737372509279e+00,  // 1122
    -5.348030082661497e-01,  // 1123
    2.926205800948768e-01,   // 1124
    -4.008722186859864e-02,  // 1125
    1.762790540268823e-03,   // 1126
    -3.569272381208507e-02,  // 1127
    -4.540533012682946e-02,  // 1128
    -2.253792799654746e-01,  // 1129
    3.626906414966873e-02,   // 1130
    -6.229674296023692e-04,  // 1131
    -1.039922338586046e+01,  // 1132
    3.404260345512673e+00,   // 1133
    2.541627893469847e-01,   // 1134
    8.216258450166090e-01,   // 1135
    1.228147612352383e+01,   // 1136
    1.468913895213862e-01,   // 1137
    -4.201746238135714e-02,  // 1138
    -2.695598878170452e-02,  // 1139
    1.213358828438989e+00,   // 1140
    1.885490434473785e+01,   // 1141
    7.261673792803370e+00,   // 1142
    2.020624683994219e+00,   // 1143
    3.312089422497588e-02,   // 1144
    -1.826174212418197e+02,  // 1145
    -1.362597999083910e+00,  // 1146
    3.831449594734188e+01,   // 1147
    2.417338971383627e-01,   // 1148
    -5.690747688496415e-01,  // 1149
    6.980593751015782e-03,   // 1150
    1.360363364290176e+00,   // 1151
    1.016394669621179e-02,   // 1152
    -2.547886844839041e-02,  // 1153
    -3.492477951322843e-01,  // 1154
    -4.010188533304857e+00,  // 1155
    -2.190862296646716e-01,  // 1156
    -2.203233896851418e-04,  // 1157
    4.064831923475799e+00,   // 1158
    1.364651022815236e-01,   // 1159
    4.174978502119641e-01,   // 1160
    -7.272187354604932e-01,  // 1161
    -1.445894007158033e-02   // 1162
};


double System::Get3B(bool do_grads, bool use_ghost) {
    // 3B ENERGY
    double e3b_t = 0.0;

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
    step = num_threads;
#endif  // _OPENMP

    // Variables to be used for both serial and parallel implementation
    size_t first_grad = 0;
    size_t last_grad = 3 * numsites_;
    int rank = 0;
    std::vector<size_t> idxs;

    // Vector pools that allow compatibility between
    // serial and parallel implementation
    std::vector<double> e3b_pool(num_threads, 0.0);
    std::vector<std::vector<double>> grad_pool(num_threads);
    std::vector<std::vector<double>> virial_pool(num_threads);  // declare virial pool

    std::vector<std::vector<size_t>> all_trimers(num_threads);


    // find all the trimers and auto-assign them to threads
#ifdef _OPENMP
    #pragma omp parallel private(rank, idxs)
    {
        rank = omp_get_thread_num();
#endif
        
        for (size_t i = rank; i < nummon_; i += step) {
            idxs.push_back(i);
        }

        all_trimers[rank] = AddClustersParallel(3, cutoff3b_, idxs, use_ghost);

        grad_pool[rank] = std::vector<double>(3 * numsites_, 0.0);
        virial_pool[rank] = std::vector<double>(9, 0.0);

#ifdef _OPENMP  
    }
#endif

    std::vector<size_t> trimers_pool;

    for(size_t i = 0; i < num_threads; i++) {
        trimers_pool.insert(trimers_pool.end(), all_trimers[i].begin(), all_trimers[i].end());
        all_trimers[i].clear();
    }

    size_t trimers_pool_index = 0;

    // this variable is the maximum number of trimers that will be dispached to a thread at a time.
    // the number of trimers will be smaller near the end of the evaluaton when there are fewer trimers.
    // should probably be a multiple of 8 for compatibility with uncoming SIMD PIP evaluation.
    const size_t batch_size = 8;
    const size_t batch_size_factor = 8;

    const size_t max_profile_index = 200;

    std::vector<size_t> num_evals(num_threads, 0);
    std::vector<std::vector<size_t>> first_num_evals(max_profile_index, std::vector<size_t>(num_threads, 0));

    std::vector<double> total_poly_time(num_threads, 0.0);
    std::vector<std::vector<double>> first_poly_time(max_profile_index, std::vector<double>(num_threads, 0.0));

    // actually calculate the trimers
#ifdef _OPENMP
#pragma omp parallel private(rank, idxs) shared(trimers_pool)
    {
        rank = omp_get_thread_num();
#endif

        std::vector<size_t>& trimers = all_trimers[rank];

        size_t start_index;
        size_t this_batch_size;

        #pragma omp critical(trimers_pool_index)
        {

            start_index = trimers_pool_index;
            size_t truncated_batch_size = batch_size_factor*std::ceil(((trimers_pool.size() - start_index) / 3) / (batch_size_factor*num_threads)) + batch_size_factor;
            this_batch_size = std::min(truncated_batch_size, std::min(batch_size, (trimers_pool.size() - start_index) / 3));
            trimers_pool_index += this_batch_size * 3;
        }
        trimers.insert(trimers.end(), trimers_pool.begin() + start_index, trimers_pool.begin() + start_index + this_batch_size*3);

        // In order to continue, we need at least one dimer
        // If the size of the dimer vector is not at least 2, means
        // that we don't have any dimer
        bool skip = false;
        if (trimers.size() < 2) {
            skip = true;
            trimers.clear();
        }

        double bigmem[8283*8];

        void* pool = reinterpret_cast<void *>(bigmem);

        size_t space = 8283*8*8;

        double* t = reinterpret_cast<double *>(std::align(128, 8279*8*8, pool, space));

        double a[1163];

        std::copy(thefit, thefit + 1163, a);

        // The way the XYZ are set, they include the virtual site,
        // but we don't need the electrostatic virtual site for teh 2B
        // polynomials. Thus, we need to create a pair of vectors with the right
        // coordinates to pass to polynomials and dispersion
        std::vector<double> coord1;
        std::vector<double> coord2;
        std::vector<double> coord3;
        std::string m1;
        std::string m2;
        std::string m3;

        if (!skip) {
            m1 = monomers_[trimers[0]];
            m2 = monomers_[trimers[1]];
            m3 = monomers_[trimers[2]];
        }

        // Initialize the iteration variables
        size_t i = 0;
        size_t nt = 0;
        size_t nt_tot = 0;
        
        size_t cur_profile_index = 0;

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

                        double poly_time = 0.0;

                        size_t this_num_evals = 0;

                        e3b_pool[rank] += e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3, grad1, grad2, grad3, t, a, poly_time, this_num_evals, &virial);

                        num_evals[rank] += this_num_evals;
                        total_poly_time[rank] += poly_time;

                        if (cur_profile_index < max_profile_index) {
                            first_poly_time[cur_profile_index][rank] = poly_time;
                            first_num_evals[cur_profile_index][rank] = this_num_evals;
                        }

                        cur_profile_index += 1;

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
                            virial_pool[rank][j] += virial[j];
                        }

                    } else {
                        // POLYNOMIALS
                        e3b_pool[rank] += e3b::get_3b_energy(m1, m2, m3, nt, xyz1, xyz2, xyz3);
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

            if(trimers.size() - 3 * nt_tot == 0) {

                size_t start_index;
                size_t this_batch_size;

                #pragma omp critical(trimers_pool_index)
                {

                    start_index = trimers_pool_index;
                    size_t truncated_batch_size = batch_size_factor*std::ceil(((trimers_pool.size() - start_index) / 3) / (batch_size_factor*num_threads)) + batch_size_factor;
                    this_batch_size = std::min(truncated_batch_size, std::min(batch_size, (trimers_pool.size() - start_index) / 3));
                    trimers_pool_index += this_batch_size * 3;

                }
                trimers.insert(trimers.end(), trimers_pool.begin() + start_index, trimers_pool.begin() + start_index + this_batch_size*3);
            }
        }
#ifdef _OPENMP
    }  // parallel

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

    size_t total_evals = 0;
    for (size_t i: num_evals) {
        total_evals += i;
    }

    double final_poly_time = 0.0;
    for (double i: total_poly_time) {
        final_poly_time += i;
    }

    std::vector<size_t> total_first_evals(max_profile_index, 0);
    std::vector<double> final_first_poly_time(max_profile_index, 0.0);
    for (size_t i = 0; i < max_profile_index; i++) {
        for (size_t j: first_num_evals[i]) {
            total_first_evals[i] += j;
        }
        for (double j: first_poly_time[i]) {
            final_first_poly_time[i] += j;
        }
    }

    std::cout << "Total number of 3B evaluations: " << total_evals << " in " << final_poly_time << " (average=" << final_poly_time/total_evals << ")" << std::endl;

    for(size_t i = 0; i < max_profile_index; i++) {
        std::cout << "Eval period " << i << " evaluated " << total_first_evals[i] << " polys in " << final_first_poly_time[i] << " (average=" << final_first_poly_time[i]/total_first_evals[i] << ")" << std::endl;
    }

    return e3b_t;
}

double System::Get4B(bool do_grads, bool use_ghost) {
    // 4B ENERGY
    double e4b_t = 0.0;

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
    step = num_threads;
#endif  // _OPENMP

    // Variables to be used for both serial and parallel implementation
    size_t first_grad = 0;
    size_t last_grad = 3 * numsites_;
    int rank = 0;
    std::vector<size_t> idxs;

    // Vector pools that allow compatibility between
    // serial and parallel implementation
    std::vector<double> e4b_pool(num_threads, 0.0);
    std::vector<std::vector<double>> grad_pool(num_threads, std::vector<double>(3 * numsites_, 0.0));
    std::vector<std::vector<double>> virial_pool(num_threads, std::vector<double>(9, 0.0));  // declare virial pool

#ifdef _OPENMP
#pragma omp parallel private(rank, idxs)
    {
        rank = omp_get_thread_num();
#endif
        for (size_t i = rank; i < nummon_; i += step) {
            idxs.push_back(i);
        }

        std::vector<size_t> tetramers = AddClustersParallel(4, cutoff4b_, idxs, use_ghost);

        // Loop over all the tetramers
        for (size_t i = 0; i < tetramers.size(); i += 4) {
            // The way the XYZ are set, they include the virtual site,
            // but we don't need the electrostatic virtual site for the 4B
            // polynomials. Thus, we need to create a pair of vectors with the right
            // coordinates to pass to polynomials and dispersion
            std::vector<std::vector<double>> coords(4);
            std::vector<std::string> ms(4);
            std::vector<size_t> nats(4);

            // Set monomer names and push the coordinates
            for (size_t n = 0; n < 4; n++) {
                coords[n].clear();
                ms[n] = monomers_[tetramers[i + n]];
                nats[n] = nat_[tetramers[i + n]];

                for (size_t j = 0; j < 3 * nat_[tetramers[i + n]]; j++) {
                    coords[n].push_back(xyz_[3 * first_index_[tetramers[i + n]] + j]);
                }
            }

            // Fix tetramer positions if pbc
            if (use_pbc_) {
                systools::GetCloseNmerImage(box_, box_inverse_, 4, nats, 1, coords);
            }

            // Check if this pair needs to use MB-nrg
            bool use_poly = true;
            for (size_t i4b = 0; i4b < ignore_4b_poly_.size(); i4b++) {
                std::vector<std::string> v1 = ignore_4b_poly_[i4b];
                std::vector<std::string> v2 = ms;
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
                    std::vector<std::vector<double>> grad(4);
                    for (size_t n = 0; n < 4; n++) {
                        grad[n].resize(coords[n].size(), 0.0);
                    }
                    std::vector<double> virial(9, 0.0);  // declare virial tensor

                    // POLYNOMIALS
                    double e = e4b::get_4b_energy(ms[0], ms[1], ms[2], ms[3], 1, coords[0], coords[1], coords[2],
                                                  coords[3], grad[0], grad[1], grad[2], grad[3], &virial);

                    e4b_pool[rank] += e;

                    // Update gradients
                    for (size_t n = 0; n < 4; n++) {
                        for (size_t j = 0; j < 3 * nat_[tetramers[i + n]]; j++) {
                            grad_pool[rank][3 * first_index_[tetramers[i + n]] + j] += grad[n][j];
                        }
                    }
                    // Virial Tensor
                    for (size_t j = 0; j < 9; j++) {
                        virial_pool[rank][j] += virial[j];
                    }

                } else {
                    // POLYNOMIALS
                    double e =
                        e4b::get_4b_energy(ms[0], ms[1], ms[2], ms[3], 1, coords[0], coords[1], coords[2], coords[3]);
                    e4b_pool[rank] += e;
                }
            }
        }

#ifdef _OPENMP
    }  // parallel

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
        e4b_t += e4b_pool[i];
    }
    // Condensate virial
    for (int i = 0; i < num_threads; i++) {
        for (size_t j = 0; j < 9; j++) {
            virial_[j] += virial_pool[i][j];
        }
    }

    return e4b_t;
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

double System::GetNumDipoleHistory() { return electrostaticE_.GetNumDipoleHistory(); }

void System::SetNumDipoleHistory(size_t num_hist) { electrostaticE_.SetNumDipoleHistory(num_hist); }

std::vector<double> System::GetDipoleHistory(size_t indx) {
    return systools::ResetOrderReal3N(electrostaticE_.GetDipoleHistory(indx), initial_order_realSites_, numat_,
                                      first_index_, nat_);
}
void System::SetDipoleHistory(size_t indx, std::vector<double> mu_hist) {
    // need to reorder dipoles first; copied from SetRealXYZ()
    // Make sure that the xyz of input has the right size
    if (mu_hist.size() != 3 * numat_) {
        std::string text =
            "Sizes " + std::to_string(mu_hist.size()) + " and " + std::to_string(3 * numat_) + " don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    std::vector<double> mu_hist_ = std::vector<double>(3 * numsites_, 0.0);

    // Copy each coordinate in the apropriate place in the internal
    // xyz vector
    for (size_t i = 0; i < nat_.size(); i++) {
        size_t ini = 3 * initial_order_realSites_[i].second;
        size_t fin = ini + 3 * nat_[i];
        size_t ini_new = 3 * first_index_[i];
        std::copy(mu_hist.begin() + ini, mu_hist.begin() + fin, mu_hist_.begin() + ini_new);
    }

    electrostaticE_.SetDipoleHistory(indx, mu_hist_);
}

std::vector<size_t> System::GetInfoElectrostaticsCounts() { return electrostaticE_.GetInfoCounts(); }
std::vector<double> System::GetInfoElectrostaticsTimings() { return electrostaticE_.GetInfoTimings(); }

std::vector<size_t> System::GetInfoDispersionCounts() { return dispersionE_.GetInfoCounts(); }
std::vector<double> System::GetInfoDispersionTimings() { return dispersionE_.GetInfoTimings(); }

////////////////////////////////////////////////////////////////////////////////

}  // namespace bblock

////////////////////////////////////////////////////////////////////////////////

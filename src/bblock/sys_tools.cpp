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

#include "sys_tools.h"

/**
 * @file sys_tools.cpp
 * @brief Contains the implementation of all the functions defined in the header.
 */

namespace systools {

std::vector<std::pair<std::string, size_t>> OrderMonomers(
    std::vector<std::string> &mon, std::vector<size_t> sites, std::vector<size_t> nats,
    std::vector<size_t> &original2current_order, std::vector<std::pair<size_t, size_t>> &original_order,
    std::vector<std::pair<size_t, size_t>> &original_order_realSites) {
    // Make sure that mons, sites and nat have the same size and are
    // not empty
    if (mon.size() < 1) {
        std::string text = "Monomer vector cannot be empty.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (mon.size() != sites.size() || mon.size() != nats.size()) {
        std::string text = "Sizes of vectors mon(" + std::to_string(mon.size()) + "), sites(" +
                           std::to_string(sites.size()) + "), and nats(" + std::to_string(nats.size()) +
                           ") don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Create copy of the input monomers
    std::vector<std::string> monomers = mon;

    // Make sure that the output vectors are cleared
    original2current_order.clear();
    mon.clear();
    original_order.clear();
    original_order_realSites.clear();

    // Defining the vector with different monomer types
    std::vector<std::string> montypes;

    // Resize the input-internal relation vector
    original2current_order = std::vector<size_t>(monomers.size(), 0);

    // Look for number the different monomers
    for (size_t i = 0; i < monomers.size(); i++) {
        if (std::find(montypes.begin(), montypes.end(), monomers[i]) == montypes.end()) montypes.push_back(monomers[i]);
    }

    // For each type, count how many of it are there
    std::vector<std::pair<std::string, size_t>> mon_types_count;
    std::vector<std::pair<std::string, size_t>> montypetmp;
    for (size_t i = 0; i < montypes.size(); i++) {
        size_t nm = std::count(monomers.begin(), monomers.end(), montypes[i]);
        montypetmp.push_back(std::make_pair(montypes[i], nm));
    }

    // Add them from less mons to more mons
    while (montypetmp.size() > 0) {
        // Look which is the monomer type with less monomers
        std::pair<std::string, size_t> minmon = montypetmp[0];
        size_t min_ind = 0;
        for (size_t i = 1; i < montypetmp.size(); i++) {
            if (montypetmp[i].second < minmon.second) {
                minmon = montypetmp[i];
                min_ind = i;
            }
        }

        // Add the monomers of the minimum type
        mon_types_count.push_back(minmon);
        std::string monid = montypetmp[min_ind].first;
        size_t site_pos = 0;
        size_t nat_pos = 0;
        for (size_t i = 0; i < monomers.size(); i++) {
            if (monomers[i] == monid) {
                // Fill in order relation information
                original_order.push_back(std::make_pair(i, site_pos));
                original_order_realSites.push_back(std::make_pair(i, nat_pos));
                mon.push_back(monid);
                original2current_order[i] = mon.size() - 1;
            }
            // Update loop variables
            site_pos += sites[i];
            nat_pos += nats[i];
        }

        // Delete the monomer type we have just done
        montypetmp.erase(montypetmp.begin() + min_ind, montypetmp.begin() + min_ind + 1);
    }

    // Return the result
    return mon_types_count;
}

size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites, std::vector<size_t> &nat,
                     std::vector<size_t> &fi_at) {
    // Make sure that mons, sites and nat have the same size and are
    // not empty
    if (mon.size() < 1) {
        std::string text = "Monomer vector cannot be empty.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Copy vectors in case something goes wrong
    std::vector<size_t> sites_cp = sites;
    std::vector<size_t> nat_cp = nat;
    std::vector<size_t> fi_at_cp = fi_at;

    // Clearing Vectors
    sites.clear();
    nat.clear();
    fi_at.clear();

    size_t count = 0;
    size_t ats = 0;
    for (size_t i = 0; i < mon.size(); i++) {
        if (mon[i] == "h2o") {
            // Filling things for water.
            // Site Info
            // TODO Maybe we can read this from a database
            sites.push_back(4);
            nat.push_back(3);

            // =====>> SECTION SITES <<=====
            // ==> PASTE YOUR CODE BELOW <==
        } else if (mon[i] == "pf6-") {
            sites.push_back(7);
            nat.push_back(7);
        } else if (mon[i] == "ch4") {
            sites.push_back(5);
            nat.push_back(5);
        } else if (mon[i] == "nh4+") {
            sites.push_back(5);
            nat.push_back(5);
        } else if ( mon[i] == "co2") {
            sites.push_back(3);
            nat.push_back(3);
        } else if ( mon[i] == "dummy") {
            sites.push_back(1);
            nat.push_back(1);


            // Halides and alkali metal ions
        } else if (mon[i] == "f" || mon[i] == "cl" ||                                      // Halides
                   mon[i] == "br" || mon[i] == "i" || mon[i] == "li" || mon[i] == "na" ||  // Alkali metal ions
                   mon[i] == "k" || mon[i] == "rb" || mon[i] == "cs") {
            sites.push_back(1);
            nat.push_back(1);
        } else {
            // If monomer not found, throw exception
            std::string text = "No data in the dataset for monomer: " + mon[i];
            // Reset vectors
            sites = sites_cp;
            nat = nat_cp;
            fi_at = fi_at_cp;
            // Throw exception
            throw CUException(__func__, __FILE__, __LINE__, text);
        }

        fi_at.push_back(ats);
        ats += nat[i];
        count += sites[i];
    }

    // Setting total number of sites
    return count;
}

void FixMonomerCoordinates(std::vector<double> &xyz, std::vector<double> box, std::vector<size_t> nat,
                           std::vector<size_t> first_index) {
    // TODO assuming for now orthorombic box:
    // box = {a,0,0,0,b,0,0,0,c)

    // Check that the box has 9 components
    // Any other size is not acceptable
    if (box.size() != 9) {
        std::string text = "Box size of " + std::to_string(box.size()) + " is not acceptable.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Check that nat and first index have the same size
    if (nat.size() != first_index.size()) {
        std::string text = std::string("Atoms vector and first index vector don't ") +
                           std::string("have the same size: ") + std::to_string(nat.size()) + std::string(" vs. ") +
                           std::to_string(first_index.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Check that xyz has enough coordinates
    if (xyz.size() < 3 * first_index.back() + 3 * nat.back()) {
        std::string text = std::string("The xyz vector is too small ") + std::string("for the first indexes passed.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    size_t nmon = nat.size();

    std::vector<double> box2 = box;
    for (size_t i = 0; i < box.size(); i++) box2[i] *= 0.5;

    for (size_t i = 0; i < nmon; i++) {
        size_t shift = 3 * first_index[i];

        // Put central atom in main box
        double first_at[3];
        for (size_t j = 0; j < 3; j++) {
            double xyzi = xyz[shift + j];
            if (xyzi < -box2[3 * j + j]) {
                // here
                xyz[shift + j] += box[3 * j + j];
                // here
            } else if (xyzi > box2[3 * j + j]) {
                // here
                xyz[shift + j] -= box[3 * j + j];
            }
            first_at[j] = xyz[shift + j];
        }

        // Put rest of molecule atoms next to central atom
        for (size_t j = 1; j < nat[i]; j++) {
            size_t j3 = j * 3;
            for (size_t k = 0; k < 3; k++) {
                double di = xyz[shift + j3 + k] - first_at[k];
                // here
                if (di > box2[3 * k + k]) {
                    // here
                    xyz[shift + j3 + k] -= box[3 * k + k];
                    // here
                } else if (di <= -box2[3 * k + k]) {
                    // here
                    xyz[shift + j3 + k] += box[3 * k + k];
                }
            }
        }
    }
}

void GetCloseDimerImage(std::vector<double> box, size_t nat1, size_t nat2, size_t nd, double *xyz1, double *xyz2) {
    size_t shift1 = 0;
    size_t shift2 = 0;
    size_t coords1 = 3 * nat1;
    size_t coords2 = 3 * nat2;

    // Create a "box" with half of the sides
    std::vector<double> box2 = box;
    for (size_t i = 0; i < box.size(); i++) box2[i] *= 0.5;

    // Move every dimer to the right place
    for (size_t i = 0; i < nd; i++) {
        for (size_t k = 0; k < 3; k++) {
            double di = xyz2[shift2 + k] - xyz1[shift1 + k];
            // here
            if (di > box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz2[shift2 + 3 * j + k] -= box[3 * k + k];
                }
                // here
            } else if (di <= -box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz2[shift2 + j * 3 + k] += box[3 * k + k];
                }
            }
        }
        shift1 += coords1;
        shift2 += coords2;
    }
}

void GetCloseTrimerImage(std::vector<double> box, size_t nat1, size_t nat2, size_t nat3, size_t nt, double *xyz1,
                         double *xyz2, double *xyz3) {
    size_t shift1 = 0;
    size_t shift2 = 0;
    size_t shift3 = 0;
    size_t coords1 = 3 * nat1;
    size_t coords2 = 3 * nat2;
    size_t coords3 = 3 * nat3;

    // Create a "box" with half of the sides
    std::vector<double> box2 = box;
    for (size_t i = 0; i < box.size(); i++) box2[i] *= 0.5;

    for (size_t i = 0; i < nt; i++) {
        // Moving (if necessary) monomer in xyz2
        for (size_t k = 0; k < 3; k++) {
            double di = xyz2[shift2 + k] - xyz1[shift1 + k];
            // here
            if (di > box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz2[shift2 + j * 3 + k] -= box[3 * k + k];
                }
                // here
            } else if (di <= -box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz2[shift2 + j * 3 + k] += box[3 * k + k];
                }
            }
        }

        // Moving (if necessary) monomer in xyz3
        for (size_t k = 0; k < 3; k++) {
            double di = xyz3[shift3 + k] - xyz1[shift1 + k];
            // here
            if (di > box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz3[shift3 + j * 3 + k] -= box[3 * k + k];
                }
                // here
            } else if (di <= -box2[3 * k + k]) {
                // here
                for (size_t j = 0; j < nat2; j++) {
                    xyz3[shift3 + j * 3 + k] += box[3 * k + k];
                }
            }
        }
        shift1 += coords1;
        shift2 += coords2;
        shift3 += coords3;
    }
}

bool ComparePair(std::pair<size_t, double> a, std::pair<size_t, double> b) { return a.first < b.first; }

void GetCloseNeighbors(kdtutils::PointCloud<double> ptc, std::vector<double> reference, double cutoff,
                       std::vector<double> &xyz_out, std::vector<size_t> &indexes) {
    // Build the tree
    typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, kdtutils::PointCloud<double>>,
                                                kdtutils::PointCloud<double>, 3 /* dim */>
        my_kd_tree_t;
    my_kd_tree_t index(3 /*dim*/, ptc, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
    index.buildIndex();

    // Tree is built

    // Reset indexes and output coordinates
    indexes.clear();
    xyz_out.clear();

    // Perform the search
    std::vector<std::pair<size_t, double>> ret_matches;
    nanoflann::SearchParams params;
    const size_t nMatches = index.radiusSearch(reference.data(), cutoff * cutoff, ret_matches, params);

    std::sort(ret_matches.begin(), ret_matches.end(), ComparePair);

    // Resize xyz_out to be 3*nMatches
    xyz_out.resize(3 * nMatches);
    indexes.resize(nMatches);

    // Add the pairs that are not in the pairs vector
    for (size_t j = 0; j < nMatches; j++) {
        indexes[j] = ret_matches[j].first;
        size_t index = indexes[j];
        // Add coordinates in vectorized order
        xyz_out[j] = ptc.pts[index].x;
        xyz_out[j + nMatches] = ptc.pts[index].y;
        xyz_out[j + 2 * nMatches] = ptc.pts[index].z;
    }
}

void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend, size_t nmon, bool use_pbc,
                 std::vector<double> box, std::vector<double> xyz_orig, std::vector<size_t> first_index,
                 std::vector<size_t> &dimers, std::vector<size_t> &trimers) {
    // istart is the monomer position for which we will look all dimers and
    // trimers that contain it. iend is the last monomer position.
    // This means, if istart is 0 and iend is 2, we will look for all dimers
    // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!

    // nmon is the number of monomers in xyz
    // xyz_orig is a double vector with positions of all atoms
    // first_index is a size_t vector with the first index of the site 'i'
    // in in the monomer vector
    // dimers and trimers will be filled with the dimers and trimers found

    // Obtain xyz vector with the positions of first atom of each monomer
    std::vector<double> xyz;
    for (size_t i = istart; i < nmon; i++) {
        xyz.push_back(xyz_orig[3 * first_index[i]]);
        xyz.push_back(xyz_orig[3 * first_index[i] + 1]);
        xyz.push_back(xyz_orig[3 * first_index[i] + 2]);
    }

    size_t nmon2 = nmon - istart;

    // Obtain the data in the structure needed by the kd-tree
    kdtutils::PointCloud<double> ptc = kdtutils::XyzToCloud(xyz, use_pbc, box);

    // Build the tree
    typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, kdtutils::PointCloud<double>>,
                                                kdtutils::PointCloud<double>, 3 /* dim */>
        my_kd_tree_t;
    my_kd_tree_t index(3 /*dim*/, ptc, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
    index.buildIndex();

    // Perform a radial search within the cutoff
    dimers.clear();
    if (n_max > 2) trimers.clear();

    std::vector<size_t> idone;
    std::set<std::pair<size_t, size_t>> donej;
    for (size_t i = 0; i < iend - istart; i++) {
        // Define the query point
        double point[3];
        point[0] = ptc.pts[i].x;
        point[1] = ptc.pts[i].y;
        point[2] = ptc.pts[i].z;

        // Perform the search
        std::vector<std::pair<size_t, double>> ret_matches;
        nanoflann::SearchParams params;
        const size_t nMatches = index.radiusSearch(point, cutoff * cutoff, ret_matches, params);

        for (size_t j = 0; j < nMatches; j++) {
            size_t pos = ret_matches[j].first / nmon2;
            ret_matches[j].first -= nmon2 * pos;
        }

        std::sort(ret_matches.begin(), ret_matches.end(), ComparePair);
        std::set<std::pair<size_t, size_t>> donek;

        // Add the pairs that are not in the dimer vector
        for (size_t j = 0; j < nMatches; j++) {
            std::pair<std::set<std::pair<size_t, size_t>>::iterator, bool> retdim;
            if (ret_matches[j].first > i) {
                retdim = donej.insert(std::make_pair(i, ret_matches[j].first));
                if (retdim.second) {
                    dimers.push_back(i + istart);
                    dimers.push_back(ret_matches[j].first + istart);
                }

                // Add trimers if requested
                if (n_max > 2) {
                    std::pair<std::set<std::pair<size_t, size_t>>::iterator, bool> ret;
                    for (size_t k = 0; k < nMatches; k++) {
                        if (ret_matches[k].first > ret_matches[j].first) {
                            ret = donek.insert(std::make_pair(ret_matches[j].first, ret_matches[k].first));
                            if (ret.second) {
                                trimers.push_back(i + istart);
                                trimers.push_back(ret_matches[j].first + istart);
                                trimers.push_back(ret_matches[k].first + istart);
                            }
                        }
                    }
                    // Define query point, which is each of the points 'j' inside the
                    // radius of 'i'
                    double point2[3];
                    point2[0] = ptc.pts[ret_matches[j].first].x;
                    point2[1] = ptc.pts[ret_matches[j].first].y;
                    point2[2] = ptc.pts[ret_matches[j].first].z;
                    std::vector<std::pair<size_t, double>> ret_matches2;
                    nanoflann::SearchParams params2;
                    const size_t nMatches2 = index.radiusSearch(point2, cutoff * cutoff, ret_matches2, params2);

                    for (size_t k = 0; k < nMatches2; k++) {
                        size_t pos2 = ret_matches2[k].first / nmon2;
                        ret_matches2[k].first -= nmon2 * pos2;
                    }

                    std::sort(ret_matches2.begin(), ret_matches2.end(), ComparePair);

                    // Add the trimers that fulfil i > j > k, to avoid double counting
                    // We will add all trimers that fulfill the condition:
                    // At least 2 of the three distances must be smaller than the cutoff
                    for (size_t k = 0; k < nMatches2; k++) {
                        if (ret_matches2[k].first > i) {
                            size_t jel = ret_matches[j].first;
                            size_t kel = ret_matches2[k].first;
                            if (ret_matches[j].first > ret_matches2[k].first) {
                                jel = ret_matches2[k].first;
                                kel = ret_matches[j].first;
                            }
                            ret = donek.insert(std::make_pair(jel, kel));
                            if (ret.second && kel > jel) {
                                trimers.push_back(i + istart);
                                trimers.push_back(jel + istart);
                                trimers.push_back(kel + istart);
                            }
                        }
                    }
                }
            }
        }
    }
}

void GetExcluded(std::string mon, excluded_set_type &exc12, excluded_set_type &exc13, excluded_set_type &exc14) {
    // Clearing excluded pairs just in case
    exc12.clear();
    exc13.clear();
    exc14.clear();

    if (mon == "h2o") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc12.insert(std::make_pair(0, 3));
        // 13 distances
        exc13.insert(std::make_pair(1, 2));
        exc13.insert(std::make_pair(1, 3));
        exc13.insert(std::make_pair(2, 3));
    }

    // =====>> SECTION EXCLUDED <<=====
    // =====>> PASTE CODE BELOW <<=====

    if (mon == "ch4") {
        // 12 distances
        exc12.insert(std::make_pair(0,1));
        exc12.insert(std::make_pair(0,3));
        exc12.insert(std::make_pair(0,2));
        exc12.insert(std::make_pair(0,4));
        // 13 distances
        exc13.insert(std::make_pair(1,2));
        exc13.insert(std::make_pair(1,3));
        exc13.insert(std::make_pair(1,4));
        exc13.insert(std::make_pair(2,3));
        exc13.insert(std::make_pair(3,4));
        exc13.insert(std::make_pair(2,4));
        // 14 distances
    }
    if (mon == "nh4+") {
        // 12 distances
        exc12.insert(std::make_pair(0,1));
        exc12.insert(std::make_pair(0,3));
        exc12.insert(std::make_pair(0,2));
        exc12.insert(std::make_pair(0,4));
        // 13 distances
        exc13.insert(std::make_pair(1,2));
        exc13.insert(std::make_pair(1,3));
        exc13.insert(std::make_pair(1,4));
        exc13.insert(std::make_pair(2,3));
        exc13.insert(std::make_pair(3,4));
        exc13.insert(std::make_pair(2,4));
        // 14 distances
    }
    if (mon == "pf6-") {
        // 12 distances
        exc12.insert(std::make_pair(0,1));
        exc12.insert(std::make_pair(0,6));
        exc12.insert(std::make_pair(0,5));
        exc12.insert(std::make_pair(0,4));
        exc12.insert(std::make_pair(0,3));
        exc12.insert(std::make_pair(0,2));
        // 13 distances
        exc13.insert(std::make_pair(1,2));
        exc13.insert(std::make_pair(1,3));
        exc13.insert(std::make_pair(2,6));
        exc13.insert(std::make_pair(4,6));
        exc13.insert(std::make_pair(4,5));
        exc13.insert(std::make_pair(5,6));
        exc13.insert(std::make_pair(1,4));
        exc13.insert(std::make_pair(1,5));
        exc13.insert(std::make_pair(1,6));
        exc13.insert(std::make_pair(2,3));
        exc13.insert(std::make_pair(3,6));
        exc13.insert(std::make_pair(2,5));
        exc13.insert(std::make_pair(3,4));
        exc13.insert(std::make_pair(2,4));
        exc13.insert(std::make_pair(3,5));
        // 14 distances
    }
    if (mon == "co2") {
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc13.insert(std::make_pair(1, 2));
    }

}

bool IsExcluded(excluded_set_type exc, size_t a, size_t b) {
    return ((exc.find(std::make_pair(a, b)) != exc.end()) || (exc.find(std::make_pair(b, a)) != exc.end()));
}

double GetAdd(bool is12, bool is13, bool is14, std::string mon) {
    // Intermolecular aDD is always 0.055
    double aDD = 0.055;
    // For water
    if (mon == "h2o") {
        if (is12) {
            aDD = 0.626;
        } else {
            aDD = 0.055;
        }
        // Any other molecule (as for 01/10/2018)
    } else {
        if (is12 || is13) {
            aDD = 0.3;
        } else {
            aDD = 0.055;
        }
    }

    return aDD;
}

std::vector<double> ResetOrder3N(std::vector<double> coords, std::vector<std::pair<size_t, size_t>> original_order,
                                 std::vector<size_t> first_index, std::vector<size_t> sites) {
    // Define new vector
    std::vector<double> new_coords(coords.size());
    // Loop over monomers in the system, and fill in the right coordinates
    for (size_t i = 0; i < sites.size(); i++) {
        size_t ini = 3 * first_index[i];
        size_t fin = ini + 3 * sites[i];
        size_t ini_orig = 3 * original_order[i].second;
        std::copy(coords.begin() + ini, coords.begin() + fin, new_coords.begin() + ini_orig);
    }

    return new_coords;
}

std::vector<double> ResetOrderReal3N(std::vector<double> coords, std::vector<std::pair<size_t, size_t>> original_order,
                                     size_t numats, std::vector<size_t> first_index, std::vector<size_t> nats) {
    // Define new vector
    std::vector<double> new_coords(3 * numats);
    // Loop over monomers in the system, and fill in the right coordinates
    for (size_t i = 0; i < nats.size(); i++) {
        size_t ini = 3 * first_index[i];
        size_t fin = ini + 3 * nats[i];
        size_t ini_orig = 3 * original_order[i].second;
        std::copy(coords.begin() + ini, coords.begin() + fin, new_coords.begin() + ini_orig);
    }

    return new_coords;
}

void SetVSites(std::vector<double> &xyz, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind) {
    size_t fstind_3 = 3 * fst_ind;

    if (mon_id == "h2o") {
        // Some useful constants
        size_t nmns = n_mon * 3;
        size_t nmns2 = nmns * 2;
        size_t nmns3 = nmns * 3;

        // TODO Not vectorized
        // Reorganize data so is contigious
        std::vector<double> xyz2(nmns * nsites, 0.0);
        for (size_t nv = 0; nv < n_mon; nv++) {
            size_t nvns3 = nv * nsites * 3;
            for (size_t i = 0; i < nsites; i++) {
                size_t i3 = 3 * i;
                size_t i3nm = i3 * n_mon;
                for (size_t j = 0; j < 3; j++) {
                    xyz2[nv + j * n_mon + i3nm] = xyz[nvns3 + i3 + j + fstind_3];
                }
            }
        }

        // TODO Check why is multiversioning. Should not.
        // Obtain M-site coordinates from different sites for water
        for (size_t i = 0; i < 3; i++) {
            size_t inm = i * n_mon;
            for (size_t nv = 0; nv < n_mon; nv++) {
                xyz2[nmns3 + inm + nv] =
                    gamma1 * xyz2[0 + inm + nv] + gamma2 * (xyz2[nmns + inm + nv] + xyz2[nmns2 + inm + nv]);
            }
        }

        // TODO Not vectorized
        // Return the M-site coordinates to the original xyz vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            size_t nvns3 = nv * nsites * 3;
            for (size_t j = 0; j < 3; j++) {
                xyz[nvns3 + 9 + j + fstind_3] = xyz2[nv + j * n_mon + 9 * n_mon];
            }
        }
    }
}

void SetCharges(std::vector<double> xyz, std::vector<double> &charges, std::string mon_id, size_t n_mon, size_t nsites,
                size_t fst_ind, std::vector<double> &chg_der) {
    // Constant that calculates charge
    const double CHARGECON = 1.0;
    // const double CHARGECON = constants::CHARGECON;

    // Halide charges
    if (mon_id == "f" || mon_id == "cl" || mon_id == "br" || mon_id == "i") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = -1.0 * CHARGECON;
        }
    }

    // Alkali metal ions
    else if (mon_id == "li" || mon_id == "na" || mon_id == "k" || mon_id == "rb" || mon_id == "cs") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 1.0 * CHARGECON;
        }

        // =====>> SECTION CHARGES <<=====
        // =======>> PASTE BELOW <<=======
    } else if (mon_id == "pf6-") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv*nsites + 0] = 1.658833 * CHARGECON;
            charges[fst_ind + nv*nsites + 1] = -0.44313899999999995 * CHARGECON;
            charges[fst_ind + nv*nsites + 2] = -0.44313899999999995 * CHARGECON;
            charges[fst_ind + nv*nsites + 3] = -0.44313899999999995 * CHARGECON;
            charges[fst_ind + nv*nsites + 4] = -0.44313899999999995 * CHARGECON;
            charges[fst_ind + nv*nsites + 5] = -0.44313899999999995 * CHARGECON;
            charges[fst_ind + nv*nsites + 6] = -0.44313899999999995 * CHARGECON;

        }
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv*nsites + 0] = -0.538573 * CHARGECON;
            charges[fst_ind + nv*nsites + 1] = 0.13464325* CHARGECON;
            charges[fst_ind + nv*nsites + 2] = 0.13464325* CHARGECON;
            charges[fst_ind + nv*nsites + 3] = 0.13464325* CHARGECON;
            charges[fst_ind + nv*nsites + 4] = 0.13464325* CHARGECON;

        }
    } else if (mon_id == "nh4+") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv*nsites + 0] = -0.741125 * CHARGECON;
            charges[fst_ind + nv*nsites + 1] = 0.4352815 * CHARGECON;
            charges[fst_ind + nv*nsites + 2] = 0.4352815 * CHARGECON;
            charges[fst_ind + nv*nsites + 3] = 0.4352815 * CHARGECON;
            charges[fst_ind + nv*nsites + 4] = 0.4352815 * CHARGECON;

        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv*nsites + 0] = 0.706027 * CHARGECON;
            charges[fst_ind + nv*nsites + 1] = -0.3530135 * CHARGECON;
            charges[fst_ind + nv*nsites + 2] = -0.3530135 * CHARGECON;

        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 0.0;

        }


        // Note, for now, assuming only water has site dependant charges
    } else if (mon_id == "h2o") {
        // chgtmp = M, H1, H2 according to ttm4.cpp
        std::vector<double> chgtmp;
        size_t fstind_3 = 3 * fst_ind;

        chg_der = std::vector<double>(27 * n_mon, 0.0);

        // Calculate individual monomer's charges
        for (size_t nv = 0; nv < n_mon; nv++) {
            size_t ns3 = nsites * 3;
            size_t shift = 27 * nv;

            // Getting front and end of xyz vector of 1 monomer in system
            std::vector<double> atomcoords(xyz);
            std::vector<double> chgtmpnv((nsites - 1));

            // Calculating charge
            ps::dms_nasa(0.0, 0.0, 0.0, atomcoords.data() + (nv * ns3) + fstind_3, chgtmpnv.data(),
                         chg_der.data() + shift, false);
            // Inserting the found charges into chgtmp vector before calculating
            // new charge values
            chgtmp.insert(chgtmp.end(), chgtmpnv.begin(), chgtmpnv.end());
        }

        // Creating vector with contiguous data
        std::vector<double> chg2(n_mon * nsites, 0.0);

        // TODO Multiversioning
        // Reorganizing sites
        for (size_t nv = 0; nv < n_mon; nv++) {
            // looping over sites -- H1 and H2
            for (size_t i = 1; i < nsites - 1; i++) {
                chg2[nv + i * n_mon] = chgtmp[i + nv * (nsites - 1)];
            }

            // looping over M
            chg2[nv + 3 * n_mon] = chgtmp[nv * 3];
        }

        std::vector<double> chg2temp = chg2;

        // calculating charge
        for (size_t nv = 0; nv < n_mon; nv++) {
            size_t hy1 = n_mon + nv;
            size_t hy2 = 2 * n_mon + nv;
            size_t msite = 3 * n_mon + nv;

            // Hydrogen1
            chg2[hy1] = CHARGECON * (chg2temp[hy1] + gamma21 * (chg2temp[hy1] + chg2temp[hy2]));
            // Hydrogen2
            chg2[hy2] = CHARGECON * (chg2temp[hy2] + gamma21 * (chg2temp[hy1] + chg2temp[hy2]));
            // M
            chg2[msite] = CHARGECON * (chg2temp[msite] / (1.0 - gammaM));
        }

        // TODO multiversioning
        // Return all coordinates to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                charges[nv * nsites + j + fst_ind] = chg2[nv + n_mon * j];
            }
        }
    }
}

void SetPolfac(std::vector<double> &polfac, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind) {
    // Halides
    if (mon_id == "f") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 2.4669;
    } else if (mon_id == "cl") {  // Chloride
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 5.3602;
    } else if (mon_id == "br") {  // Bromide
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 7.1668;
    } else if (mon_id == "i") {  // Iodide
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 10.1184;
    }

    // Alkali metal ions
    if (mon_id == "li") {  // Lithium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.0285;
    } else if (mon_id == "na") {  // Sodium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.1476;
    } else if (mon_id == "k") {  // Potassium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.8184;
    } else if (mon_id == "rb") {  // Rubidium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 1.3614;
    } else if (mon_id == "cs") {  // Cesium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 2.3660;

        // =====>> SECTION POLFACS <<=====
        // =======>> PASTE BELOW <<=======
    } else if (mon_id == "pf6-") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv*nsites + 0] = 3.92467331297534;
            polfac[fst_ind + nv*nsites + 1] = 0.49711381987577635;
            polfac[fst_ind + nv*nsites + 2] = 0.49711381987577635;
            polfac[fst_ind + nv*nsites + 3] = 0.49711381987577635;
            polfac[fst_ind + nv*nsites + 4] = 0.49711381987577635;
            polfac[fst_ind + nv*nsites + 5] = 0.49711381987577635;
            polfac[fst_ind + nv*nsites + 6] = 0.49711381987577635;

        }
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv*nsites + 0] = 1.3932677 ;
            polfac[fst_ind + nv*nsites + 1] = 0.38978363;
            polfac[fst_ind + nv*nsites + 2] = 0.38978363;
            polfac[fst_ind + nv*nsites + 3] = 0.38978363;
            polfac[fst_ind + nv*nsites + 4] = 0.38978363;

        }
    } else if (mon_id == "nh4+") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv*nsites + 0] = 0.6918051284835324;
            polfac[fst_ind + nv*nsites + 1] = 0.2448826550598477;
            polfac[fst_ind + nv*nsites + 2] = 0.2448826550598477;
            polfac[fst_ind + nv*nsites + 3] = 0.2448826550598477;
            polfac[fst_ind + nv*nsites + 4] = 0.2448826550598477;

        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv*nsites + 0] = 1.471677;
            polfac[fst_ind + nv*nsites + 1] = 0.769790;
            polfac[fst_ind + nv*nsites + 2] = 0.769790;

        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv] = 0.0;

        }

    } else if (mon_id == "h2o") {
        // Creating vector with contiguous data
        std::vector<double> polfac2(n_mon * nsites, 0.0);
        for (size_t nv = 0; nv < n_mon; nv++) {
            // Oxygen
            polfac2[nv] = 1.310;

            // looping over sites -- H1 and H2
            for (size_t i = 1; i < nsites - 1; i++) {
                polfac2[nv + i * n_mon] = 0.294;
            }
            // M site
            polfac2[nv + n_mon * 3] = 1.310;
        }

        // TODO Multiversioning
        // Return all polfacs to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                polfac[nv * nsites + j + fst_ind] = polfac2[nv + n_mon * j];
            }
        }
    }
}

void SetPol(std::vector<double> &pol, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind) {
    // Halides
    if (mon_id == "f") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 2.4669;
    } else if (mon_id == "cl") {  // Chloride
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 5.3602;
    } else if (mon_id == "br") {  // Bromide
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 7.1668;
    } else if (mon_id == "i") {  // Iodide
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 10.1184;
    }

    // Alkali metal ions
    if (mon_id == "li") {  // Lithium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.0285;
    } else if (mon_id == "na") {  // Sodium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.1476;
    } else if (mon_id == "k") {  // Potassium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.8184;
    } else if (mon_id == "rb") {  // Rubidium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 1.3614;
    } else if (mon_id == "cs") {  // Cesium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 2.3660;

        // =====>> SECTION POLS <<=====
        // =====>> PASTE  BELOW <<=====

    } else if (mon_id == "pf6-") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv*nsites + 0] = 3.92467331297534;
            pol[fst_ind + nv*nsites + 1] = 0.49711381987577635;
            pol[fst_ind + nv*nsites + 2] = 0.49711381987577635;
            pol[fst_ind + nv*nsites + 3] = 0.49711381987577635;
            pol[fst_ind + nv*nsites + 4] = 0.49711381987577635;
            pol[fst_ind + nv*nsites + 5] = 0.49711381987577635;
            pol[fst_ind + nv*nsites + 6] = 0.49711381987577635;

        }
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv*nsites + 0] = 1.3932677 ;
            pol[fst_ind + nv*nsites + 1] = 0.38978363;
            pol[fst_ind + nv*nsites + 2] = 0.38978363;
            pol[fst_ind + nv*nsites + 3] = 0.38978363;
            pol[fst_ind + nv*nsites + 4] = 0.38978363;

        }
    } else if (mon_id == "nh4+") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv*nsites + 0] = 0.6918051284835324;
            pol[fst_ind + nv*nsites + 1] = 0.2448826550598477;
            pol[fst_ind + nv*nsites + 2] = 0.2448826550598477;
            pol[fst_ind + nv*nsites + 3] = 0.2448826550598477;
            pol[fst_ind + nv*nsites + 4] = 0.2448826550598477;

        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv*nsites + 0] = 1.471677;
            pol[fst_ind + nv*nsites + 1] = 0.769790;
            pol[fst_ind + nv*nsites + 2] = 0.769790;

        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv] = 0.0;

        }

    } else if (mon_id == "h2o") {
        // Creating vector with contiguous data
        std::vector<double> pol2(n_mon * nsites, 0.0);
        for (size_t nv = 0; nv < n_mon; nv++) {
            // Oxygen
            pol2[nv] = 1.310;

            // looping over sites -- H1 and H2
            for (size_t i = 1; i < nsites - 1; i++) {
                pol2[nv + i * n_mon] = 0.294;
            }
        }

        // TODO Multiversioning
        // Return all pols to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                pol[nv * nsites + j + fst_ind] = pol2[nv + n_mon * j];
            }
        }
    }
}

void SetC6LongRange(std::vector<double> &c6_lr, std::string mon_id, size_t n_mon, size_t natoms, size_t fst_ind) {
    // All these C6 come from Qchem/avtz. We put two molecules at 50 A and get the c6 of the atoms.
    if (mon_id == "f") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 25.56412750183350184739;
    } else if (mon_id == "cl") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 57.88297168036554772821;
    } else if (mon_id == "br") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 0;
    } else if (mon_id == "i") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 0;
    } else if (mon_id == "li") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 3.24887148714749872914;
    } else if (mon_id == "na") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 16.02787872333703428437;
    } else if (mon_id == "k") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 0;
    } else if (mon_id == "rb") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 0;
    } else if (mon_id == "cs") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 0;
        // Water is the only monomer which C6 does not come from qchem.
        // It comes from MB-pol (C6O = sqrt(C6OO))
    } else if (mon_id == "pf6-") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 36.64845818186718069999; // P
            c6_lr[nv * natoms + fst_ind + 1] = 11.31594587280531048051; // F
            c6_lr[nv * natoms + fst_ind + 2] = 11.31594587280531048051; // F
            c6_lr[nv * natoms + fst_ind + 3] = 11.31594587280531048051; // F
            c6_lr[nv * natoms + fst_ind + 4] = 11.31594587280531048051; // F
            c6_lr[nv * natoms + fst_ind + 5] = 11.31594587280531048051; // F
            c6_lr[nv * natoms + fst_ind + 6] = 11.31594587280531048051; // F
        }
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.41398863;  // N
            c6_lr[nv * natoms + fst_ind + 1] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 2] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 3] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 4] = 6.064748037;  // H
        }

    } else if (mon_id == "nh4+") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 11.00107139850448566299;  // N
            c6_lr[nv * natoms + fst_ind + 1] = 3.89676610697743571923;  // H
            c6_lr[nv * natoms + fst_ind + 2] = 3.89676610697743571923;  // H
            c6_lr[nv * natoms + fst_ind + 3] = 3.89676610697743571923;  // H
            c6_lr[nv * natoms + fst_ind + 4] = 3.89676610697743571923;  // H
        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.91673320223304547491;  // C
            c6_lr[nv * natoms + fst_ind + 1] = 13.04205731316957524126;  // O
            c6_lr[nv * natoms + fst_ind + 2] = 13.04205731316957524126;  // O
        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[fst_ind + nv] = 0.0;

        }
    } else if (mon_id == "h2o") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 15.40523357222455098728;     // O
            c6_lr[nv * natoms + fst_ind + 1] = 4.48258697649551357815;  // H
            c6_lr[nv * natoms + fst_ind + 2] = 4.48258697649551357815;  // H
        }
    }
}

// Assuming for now xyzxyzxyz...
void RedistributeVirtGrads2Real(const std::string mon, const size_t nmon, const size_t fi_crd,
                                std::vector<double> &grad) {
    if (mon == "h2o") {
        for (size_t i = 0; i < nmon; i++) {
            const size_t shift = fi_crd + i * 4 * 3;
            for (size_t k = 0; k < 3; ++k) {
                grad[shift + k] += gamma1 * grad[shift + 9 + k];      // O
                grad[shift + 3 + k] += gamma2 * grad[shift + 9 + k];  // H
                grad[shift + 6 + k] += gamma2 * grad[shift + 9 + k];  // H
                grad[shift + 9 + k] = 0.0;                            // M
            }
        }
    }
}

void ChargeDerivativeForce(const std::string mon, const size_t nmon, const size_t fi_crd, const size_t fi_sites,
                           const std::vector<double> phi, std::vector<double> &grad,
                           const std::vector<double> chg_grad) {
    // If water, extracted from patridge-schwneke paper
    if (mon == "h2o") {
        for (size_t mm = 0; mm < nmon; mm++) {
            const size_t shift = fi_crd + 12 * mm;
            const size_t sphi = fi_sites + 4 * mm;
            double gradq[27];
            std::fill(gradq, gradq + 27, 0.0);
            double chgdev[27];
            std::copy(chg_grad.begin() + 27 * mm, chg_grad.begin() + 27 * (mm + 1), chgdev);

#define DQ3(l, m, k) chgdev[k + 3 * (m + 3 * l)]
#define GRADQ(l, m, k) gradq[k + 3 * (m + 3 * l)]

            for (size_t k = 0; k < 3; ++k) {
                GRADQ(0, 0, k) = DQ3(0, 0, k) + gamma21 * (DQ3(0, 0, k) + DQ3(0, 1, k));
                GRADQ(1, 0, k) = DQ3(1, 0, k) + gamma21 * (DQ3(1, 0, k) + DQ3(1, 1, k));
                GRADQ(2, 0, k) = DQ3(2, 0, k) + gamma21 * (DQ3(2, 0, k) + DQ3(2, 1, k));

                GRADQ(0, 1, k) = DQ3(0, 1, k) + gamma21 * (DQ3(0, 1, k) + DQ3(0, 0, k));
                GRADQ(1, 1, k) = DQ3(1, 1, k) + gamma21 * (DQ3(1, 1, k) + DQ3(1, 0, k));
                GRADQ(2, 1, k) = DQ3(2, 1, k) + gamma21 * (DQ3(2, 1, k) + DQ3(2, 0, k));

                GRADQ(0, 2, k) = DQ3(0, 2, k) - 2 * gamma21 * (DQ3(0, 0, k) + DQ3(0, 1, k));
                GRADQ(1, 2, k) = DQ3(1, 2, k) - 2 * gamma21 * (DQ3(1, 0, k) + DQ3(1, 1, k));
                GRADQ(2, 2, k) = DQ3(2, 2, k) - 2 * gamma21 * (DQ3(2, 0, k) + DQ3(2, 1, k));
            }

            for (size_t i = 0; i < 27; ++i) gradq[i] *= constants::COULOMB;

            const size_t io = shift;
            const size_t ih1 = shift + 3;
            const size_t ih2 = shift + 6;

            for (size_t k = 0; k < 3; ++k) {
                grad[ih1 + k] += GRADQ(0, 0, k) * phi[sphi + 1]     // phi(h1)
                                 + GRADQ(0, 1, k) * phi[sphi + 2]   // phi(h2)
                                 + GRADQ(0, 2, k) * phi[sphi + 3];  // phi(M)

                grad[ih2 + k] += GRADQ(1, 0, k) * phi[sphi + 1]     // phi(h1)
                                 + GRADQ(1, 1, k) * phi[sphi + 2]   // phi(h2)
                                 + GRADQ(1, 2, k) * phi[sphi + 3];  // phi(M)

                grad[io + k] += GRADQ(2, 0, k) * phi[sphi + 1]     // phi(h1)
                                + GRADQ(2, 1, k) * phi[sphi + 2]   // phi(h2)
                                + GRADQ(2, 2, k) * phi[sphi + 3];  // phi(M)
            }
        }
    }
}

}  // namespace systools

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
    std::vector<std::string> &mon, std::vector<size_t> &islocal, std::vector<size_t> sites, std::vector<size_t> nats,
    std::vector<size_t> &original2current_order, std::vector<std::pair<size_t, size_t>> &original_order,
    std::vector<std::pair<size_t, size_t>> &original_order_realSites) {
    // Make sure that mons, sites and nat have the same size and are
    // not empty
    if (mon.size() < 1) {
        std::string text = "Monomer vector cannot be empty.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (mon.size() != sites.size() || mon.size() != nats.size() || mon.size() != islocal.size()) {
        std::string text = "Sizes of vectors mon(" + std::to_string(mon.size()) + "), sites(" +
                           std::to_string(sites.size()) + "), and nats(" + std::to_string(nats.size()) +
                           "), and islocal(" + std::to_string(islocal.size()) + ") don't match.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Create copy of the input monomers
    std::vector<std::string> monomers = mon;

    // Create copy of the input local/ghost descriptors
    std::vector<size_t> _is_local = islocal;

    // Make sure that the output vectors are cleared
    original2current_order.clear();
    mon.clear();
    islocal.clear();
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
                islocal.push_back(_is_local[i]);
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
                     std::vector<size_t> &fi_at, std::vector<size_t> &fi_sites, nlohmann::json mon_j) {
    // Make sure that mon is not empty
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
    fi_sites.clear();

    size_t count = 0;
    size_t ats = 0;
    for (size_t i = 0; i < mon.size(); i++) {
        bool is_in_json = false;
        if(mon_j.contains(mon[i])) {
            try {
                size_t ns = mon_j[mon[i]]["sites"];
                size_t na = mon_j[mon[i]]["nat"];
                sites.push_back(ns);
                nat.push_back(na);
                is_in_json = true;
            } catch (...) {
                is_in_json = false;
            }
        }

        if (!is_in_json) {
            if (mon[i] == "h2o") {
                // Filling things for water.
                // Site Info
                // TODO Maybe we can read this from a database
                sites.push_back(4);
                nat.push_back(3);

            } else if (mon[i] == "dp2") {
                sites.push_back(2);
                nat.push_back(2);

            } else if (mon[i] == "dp1") {
                sites.push_back(1);
                nat.push_back(1);

            } else if (mon[i] == "dp1p") {
                sites.push_back(1);
                nat.push_back(1);

            } else if (mon[i] == "ch4") {
                sites.push_back(5);
                nat.push_back(5);
            } else if (mon[i] == "co2_archive") {
                sites.push_back(3);
                nat.push_back(3);
            } else if (mon[i] == "nh3" || mon[i] == "nh3pbe0d3bj") {
                sites.push_back(4);
                nat.push_back(4);
            } else if (mon[i] == "h4_dummy") {
                sites.push_back(4);
                nat.push_back(4);
            } else if (mon[i] == "co2") {
                sites.push_back(3);
                nat.push_back(3);
            } else if (mon[i] == "co2cm5100" || mon[i] == "co2cm595" || mon[i] == "co2cm590" || mon[i] == "co2cm5875" ||
                       mon[i] == "co2cm585" || mon[i] == "co2cm580") {
                sites.push_back(3);
                nat.push_back(3);
            } else if (mon[i] == "n2o5") {
                sites.push_back(7);
                nat.push_back(7);
            } else if (mon[i] == "ar") {
                sites.push_back(1);
                nat.push_back(1);
            } else if (mon[i] == "he") {
                sites.push_back(1);
                nat.push_back(1);
            } else if (mon[i] == "dummy") {
                sites.push_back(1);
                nat.push_back(1);

                // Halides and alkali metal ions
            } else if (mon[i] == "f-" || mon[i] == "cl-" ||                                        // Halides
                       mon[i] == "br-" || mon[i] == "i-" || mon[i] == "li+" || mon[i] == "na+" ||  // Alkali metal ions
                       mon[i] == "k+" || mon[i] == "rb+" || mon[i] == "cs+") {
                sites.push_back(1);
                nat.push_back(1);
            } else if (mon[i] == "h2") {
                sites.push_back(2);
                nat.push_back(2);
            } else if (mon[i] == "mbpbe") {
                sites.push_back(4);
                nat.push_back(3);
                // =====>> BEGIN SECTION SITES <<=====
                // ==> PASTE YOUR CODE BELOW <==
                // END SECTION SITES
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
        }

        fi_at.push_back(ats);
        fi_sites.push_back(count);
        ats += nat[i];
        count += sites[i];
    }

    // Setting total number of sites
    return count;
}

void FixMonomerCoordinates(std::vector<double> &xyz, std::vector<double> box, std::vector<double> box_inv,
                           std::vector<size_t> nat, std::vector<size_t> first_index) {
    // Check that the box has 9 components
    // Any other size is not acceptable
    if (box.size() != 9) {
        std::string text = "Box size of " + std::to_string(box.size()) + " is not acceptable.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (box_inv.size() != 9) {
        std::string text = "Box inverse size of " + std::to_string(box_inv.size()) + " is not acceptable.";
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

    for (size_t i = 0; i < nmon; i++) {
        // Move first atom of monomer into main box
        size_t shift3 = 3 * first_index[i];

        double x_rec = box_inv[0] * xyz[shift3] + box_inv[3] * xyz[shift3 + 1] + box_inv[6] * xyz[shift3 + 2];
        double y_rec = box_inv[1] * xyz[shift3] + box_inv[4] * xyz[shift3 + 1] + box_inv[7] * xyz[shift3 + 2];
        double z_rec = box_inv[2] * xyz[shift3] + box_inv[5] * xyz[shift3 + 1] + box_inv[8] * xyz[shift3 + 2];

        x_rec -= std::floor(x_rec + 0.5);
        y_rec -= std::floor(y_rec + 0.5);
        z_rec -= std::floor(z_rec + 0.5);

        xyz[shift3 + 0] = box[0] * x_rec + box[3] * y_rec + box[6] * z_rec;
        xyz[shift3 + 1] = box[1] * x_rec + box[4] * y_rec + box[7] * z_rec;
        xyz[shift3 + 2] = box[2] * x_rec + box[5] * y_rec + box[8] * z_rec;

        for (size_t j = 1; j < nat[i]; j++) {
            double xr = box_inv[0] * xyz[shift3 + 3 * j] + box_inv[3] * xyz[shift3 + 3 * j + 1] +
                        box_inv[6] * xyz[shift3 + 3 * j + 2];
            double yr = box_inv[1] * xyz[shift3 + 3 * j] + box_inv[4] * xyz[shift3 + 3 * j + 1] +
                        box_inv[7] * xyz[shift3 + 3 * j + 2];
            double zr = box_inv[2] * xyz[shift3 + 3 * j] + box_inv[5] * xyz[shift3 + 3 * j + 1] +
                        box_inv[8] * xyz[shift3 + 3 * j + 2];

            xr -= std::floor(xr - x_rec + 0.5);
            yr -= std::floor(yr - y_rec + 0.5);
            zr -= std::floor(zr - z_rec + 0.5);

            xyz[shift3 + 3 * j + 0] = box[0] * xr + box[3] * yr + box[6] * zr;
            xyz[shift3 + 3 * j + 1] = box[1] * xr + box[4] * yr + box[7] * zr;
            xyz[shift3 + 3 * j + 2] = box[2] * xr + box[5] * yr + box[8] * zr;
        }
    }
}

void GetCloseDimerImage(std::vector<double> box, std::vector<double> box_inv, size_t nat1, size_t nat2, size_t nd,
                        double *xyz1, double *xyz2) {
    size_t shift1 = 0;
    size_t shift2 = 0;
    size_t coords1 = 3 * nat1;
    size_t coords2 = 3 * nat2;

    // Move every dimer to the right place
    for (size_t i = 0; i < nd; i++) {
        // Move first atom of monomer into main box

        double x_rec = box_inv[0] * xyz1[shift1] + box_inv[3] * xyz1[shift1 + 1] + box_inv[6] * xyz1[shift1 + 2];
        double y_rec = box_inv[1] * xyz1[shift1] + box_inv[4] * xyz1[shift1 + 1] + box_inv[7] * xyz1[shift1 + 2];
        double z_rec = box_inv[2] * xyz1[shift1] + box_inv[5] * xyz1[shift1 + 1] + box_inv[8] * xyz1[shift1 + 2];

        double xr0 = box_inv[0] * xyz2[shift2] + box_inv[3] * xyz2[shift2 + 1] + box_inv[6] * xyz2[shift2 + 2];
        double yr0 = box_inv[1] * xyz2[shift2] + box_inv[4] * xyz2[shift2 + 1] + box_inv[7] * xyz2[shift2 + 2];
        double zr0 = box_inv[2] * xyz2[shift2] + box_inv[5] * xyz2[shift2 + 1] + box_inv[8] * xyz2[shift2 + 2];

        double dx0 = std::floor(xr0 - x_rec + 0.5);
        double dy0 = std::floor(yr0 - y_rec + 0.5);
        double dz0 = std::floor(zr0 - z_rec + 0.5);

        for (size_t j = 0; j < nat2; j++) {
            double xr = box_inv[0] * xyz2[shift2 + 3 * j] + box_inv[3] * xyz2[shift2 + 3 * j + 1] +
                        box_inv[6] * xyz2[shift2 + 3 * j + 2];
            double yr = box_inv[1] * xyz2[shift2 + 3 * j] + box_inv[4] * xyz2[shift2 + 3 * j + 1] +
                        box_inv[7] * xyz2[shift2 + 3 * j + 2];
            double zr = box_inv[2] * xyz2[shift2 + 3 * j] + box_inv[5] * xyz2[shift2 + 3 * j + 1] +
                        box_inv[8] * xyz2[shift2 + 3 * j + 2];

            xr -= dx0;
            yr -= dy0;
            zr -= dz0;

            xyz2[shift2 + 3 * j + 0] = box[0] * xr + box[3] * yr + box[6] * zr;
            xyz2[shift2 + 3 * j + 1] = box[1] * xr + box[4] * yr + box[7] * zr;
            xyz2[shift2 + 3 * j + 2] = box[2] * xr + box[5] * yr + box[8] * zr;
        }

        shift1 += coords1;
        shift2 += coords2;
    }
}

// void GetCloseTrimerImage(std::vector<double> box, std::vector<double> box_inv, size_t nat1, size_t nat2, size_t nat3,
//                         size_t nt, std::vector<double> &xyz1, std::vector<double> &xyz2, std::vector<double> &xyz3) {
//    size_t shift1 = 0;
//    size_t shift2 = 0;
//    size_t shift3 = 0;
//    size_t coords1 = 3 * nat1;
//    size_t coords2 = 3 * nat2;
//    size_t coords3 = 3 * nat3;
//
//    for (size_t i = 0; i < nt; i++) {
//        double x_rec = box_inv[0] * xyz1[shift1] + box_inv[3] * xyz1[shift1 + 1] + box_inv[6] * xyz1[shift1 + 2];
//        double y_rec = box_inv[1] * xyz1[shift1] + box_inv[4] * xyz1[shift1 + 1] + box_inv[7] * xyz1[shift1 + 2];
//        double z_rec = box_inv[2] * xyz1[shift1] + box_inv[5] * xyz1[shift1 + 1] + box_inv[8] * xyz1[shift1 + 2];
//
//        double xr0 = box_inv[0] * xyz2[shift2] + box_inv[3] * xyz2[shift2 + 1] + box_inv[6] * xyz2[shift2 + 2];
//        double yr0 = box_inv[1] * xyz2[shift2] + box_inv[4] * xyz2[shift2 + 1] + box_inv[7] * xyz2[shift2 + 2];
//        double zr0 = box_inv[2] * xyz2[shift2] + box_inv[5] * xyz2[shift2 + 1] + box_inv[8] * xyz2[shift2 + 2];
//
//        double dx0 = std::floor(xr0 - x_rec + 0.5);
//        double dy0 = std::floor(yr0 - y_rec + 0.5);
//        double dz0 = std::floor(zr0 - z_rec + 0.5);
//
//        // Move monomer 2 to be the closest image to mon1 if needed
//        for (size_t j = 0; j < nat2; j++) {
//            double xr = box_inv[0] * xyz2[shift2 + 3 * j] + box_inv[3] * xyz2[shift2 + 3 * j + 1] +
//                        box_inv[6] * xyz2[shift2 + 3 * j + 2];
//            double yr = box_inv[1] * xyz2[shift2 + 3 * j] + box_inv[4] * xyz2[shift2 + 3 * j + 1] +
//                        box_inv[7] * xyz2[shift2 + 3 * j + 2];
//            double zr = box_inv[2] * xyz2[shift2 + 3 * j] + box_inv[5] * xyz2[shift2 + 3 * j + 1] +
//                        box_inv[8] * xyz2[shift2 + 3 * j + 2];
//
//            xr -= dx0;
//            yr -= dy0;
//            zr -= dz0;
//
//            xyz2[shift2 + 3 * j + 0] = box[0] * xr + box[3] * yr + box[6] * zr;
//            xyz2[shift2 + 3 * j + 1] = box[1] * xr + box[4] * yr + box[7] * zr;
//            xyz2[shift2 + 3 * j + 2] = box[2] * xr + box[5] * yr + box[8] * zr;
//        }
//
//        // Move monomer 3 to be the closest image to mon1 if needed
//        xr0 = box_inv[0] * xyz3[shift3] + box_inv[3] * xyz3[shift3 + 1] + box_inv[6] * xyz3[shift3 + 2];
//        yr0 = box_inv[1] * xyz3[shift3] + box_inv[4] * xyz3[shift3 + 1] + box_inv[7] * xyz3[shift3 + 2];
//        zr0 = box_inv[2] * xyz3[shift3] + box_inv[5] * xyz3[shift3 + 1] + box_inv[8] * xyz3[shift3 + 2];
//
//        dx0 = std::floor(xr0 - x_rec + 0.5);
//        dy0 = std::floor(yr0 - y_rec + 0.5);
//        dz0 = std::floor(zr0 - z_rec + 0.5);
//
//        for (size_t j = 0; j < nat3; j++) {
//            double xr = box_inv[0] * xyz3[shift3 + 3 * j] + box_inv[3] * xyz3[shift3 + 3 * j + 1] +
//                        box_inv[6] * xyz3[shift3 + 3 * j + 2];
//            double yr = box_inv[1] * xyz3[shift3 + 3 * j] + box_inv[4] * xyz3[shift3 + 3 * j + 1] +
//                        box_inv[7] * xyz3[shift3 + 3 * j + 2];
//            double zr = box_inv[2] * xyz3[shift3 + 3 * j] + box_inv[5] * xyz3[shift3 + 3 * j + 1] +
//                        box_inv[8] * xyz3[shift3 + 3 * j + 2];
//
//            xr -= dx0;
//            yr -= dy0;
//            zr -= dz0;
//
//            xyz3[shift3 + 3 * j + 0] = box[0] * xr + box[3] * yr + box[6] * zr;
//            xyz3[shift3 + 3 * j + 1] = box[1] * xr + box[4] * yr + box[7] * zr;
//            xyz3[shift3 + 3 * j + 2] = box[2] * xr + box[5] * yr + box[8] * zr;
//        }
//        shift1 += coords1;
//        shift2 += coords2;
//        shift3 += coords3;
//    }
//}

void GetCloseTrimerImage(std::vector<double> box, std::vector<double> box_inv, size_t nat1, size_t nat2, size_t nat3,
                         size_t nt, std::vector<double> &xyz1, std::vector<double> &xyz2, std::vector<double> &xyz3) {
    GetCloseDimerImage(box, box_inv, nat1, nat2, nt, xyz1.data(), xyz2.data());
    GetCloseDimerImage(box, box_inv, nat1, nat3, nt, xyz1.data(), xyz3.data());
}

// void GetCloseTetramerImage(std::vector<double> box, std::vector<double> box_inv,
//                            size_t nat1, size_t nat2, size_t nat3, size_t nat4, size_t nt,
//                            std::vector<double> &xyz1, std::vector<double> &xyz2,
//                            std::vector<double> &xyz3, std::vector<double> &xyz4) {
//    GetCloseDimerImage(box, box_inv, nat1, nat2, nt, xyz1.data(), xyz2.data());
//    GetCloseDimerImage(box, box_inv, nat1, nat3, nt, xyz1.data(), xyz3.data());
//    GetCloseDimerImage(box, box_inv, nat1, nat4, nt, xyz1.data(), xyz4.data());
//}

void GetCloseNmerImage(std::vector<double> box, std::vector<double> box_inv, size_t oc, std::vector<size_t> nat,
                       size_t nc, std::vector<std::vector<double>> &xyz) {
    if (oc == 2) {
        GetCloseDimerImage(box, box_inv, nat[0], nat[1], nc, xyz[0].data(), xyz[1].data());
    } else if (oc == 3) {
        GetCloseDimerImage(box, box_inv, nat[0], nat[1], nc, xyz[0].data(), xyz[1].data());
        GetCloseDimerImage(box, box_inv, nat[0], nat[2], nc, xyz[0].data(), xyz[2].data());
    } else if (oc == 4) {
        GetCloseDimerImage(box, box_inv, nat[0], nat[1], nc, xyz[0].data(), xyz[1].data());
        GetCloseDimerImage(box, box_inv, nat[0], nat[2], nc, xyz[0].data(), xyz[2].data());
        GetCloseDimerImage(box, box_inv, nat[0], nat[3], nc, xyz[0].data(), xyz[3].data());
    } else {
        for (size_t i = 1; i < oc; i++)
            GetCloseDimerImage(box, box_inv, nat[0], nat[i], nc, xyz[0].data(), xyz[i].data());
    }
}

bool ComparePair(std::pair<size_t, double> a, std::pair<size_t, double> b) { return a.first < b.first; }

void AddClusters(size_t n_max, double cutoff, std::vector<size_t> idxs, size_t nmon, bool use_pbc,
                 std::vector<double> box, std::vector<double> box_inverse, std::vector<double> xyz_orig,
                 std::vector<size_t> first_index, std::vector<size_t> is_local, std::vector<int> tag,
                 std::vector<size_t> &nmers, bool use_ghost) {
    // istart is the monomer position for which we will look all dimers and
    // trimers that contain it. iend is the last monomer position.
    // This means, if istart is 0 and iend is 2, we will look for all dimers
    // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!

    // nmon is the number of monomers in xyz
    // xyz_orig is a double vector with positions of all atoms
    // first_index is a size_t vector with the first index of the site 'i'
    // in in the monomer vector
    // is_local is local/ghost descriptor for monomers; interactions involving
    //  all ghost monomers are ignored
    // use_ghost controls whether or not to include ghost monomers in clusters; default is no.
    // nmers will be filled with the dimers or trimers found

    // if use_ghost == true,
    //       include local+ghost monomers in xyz, but only include local-ghost interactions
    // if use_ghost == false,
    //       include only local monomers in xyz and do nothing special

    // Perform a radial search within the cutoff
    nmers.clear();

    // if first monomer is ghost and we're not computing local-ghost interactions, then skip
    // if (!use_ghost && !is_local[istart]) return;

    // Obtain xyz vector with the positions of first atom of each monomer
    std::vector<double> xyz;
    // std::vector<double> xyz;
    size_t nmon2 = 0;

    std::vector<size_t> mon_index;
    std::vector<size_t> tag_index;
    for (size_t i = 0; i < nmon; i++) {
        if (use_ghost || (!use_ghost && is_local[i])) {
            xyz.push_back(xyz_orig[3 * first_index[i]]);
            xyz.push_back(xyz_orig[3 * first_index[i] + 1]);
            xyz.push_back(xyz_orig[3 * first_index[i] + 2]);
            mon_index.push_back(i);
            tag_index.push_back(tag[first_index[i]]);
            nmon2++;
        }
    }

    if (nmon2 < 2) return;
    if (n_max > 2 && nmon2 < 3) return;
    if (n_max > 3 && nmon2 < 4) return;

    // Obtain the data in the structure needed by the kd-tree
    kdtutils::PointCloud<double> ptc = kdtutils::XyzToCloud(xyz, use_pbc, box, box_inverse);

    // Build the tree
    typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, kdtutils::PointCloud<double>>,
                                                kdtutils::PointCloud<double>, 3 /* dim */>
        my_kd_tree_t;
    my_kd_tree_t index(3 /*dim*/, ptc, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
    index.buildIndex();

    for (size_t ii = 0; ii < idxs.size(); ii++) {
        size_t i = idxs[ii];

        // if idx only contains local atom, then this line has no effect
        if (!is_local[i]) continue;

        // Define the query point
        double point[3];
        point[0] = ptc.pts[i].x;
        point[1] = ptc.pts[i].y;
        point[2] = ptc.pts[i].z;

        // Perform the search
        std::vector<std::pair<size_t, double>> ret_matches;
        nanoflann::SearchParams params;
        const size_t nMatches = index.radiusSearch(point, cutoff * cutoff, ret_matches, params);

        // can sort but is uneccessary
        // std::sort(ret_matches.begin(), ret_matches.end(), ComparePair);

        //  follwing code assumes cutoff*2 < box size, so for an atom,
        //  at most one pbc replica exists within cutoff around a point,
        //  to ensure no trimer contains monomer interacting with its image

        // Add the pairs that are not in the dimer vector
        if (n_max == 2) {
            for (size_t j = 0; j < nMatches; j++) {
                size_t id_j = ret_matches[j].first % nmon2;
                // add dimer (i, id_j) if i has the lower label
                if (tag_index[i] < tag_index[id_j]) {
                    nmers.push_back(mon_index[i]);
                    nmers.push_back(mon_index[id_j]);
                }
            }
        }

        // Add trimers if requested
        if (n_max == 3) {
            for (size_t j = 0; j < nMatches; j++) {
                for (size_t k = j + 1; k < nMatches; k++) {
                    double djk[3];
                    djk[0] = ptc.pts[ret_matches[j].first].x - ptc.pts[ret_matches[k].first].x;
                    djk[1] = ptc.pts[ret_matches[j].first].y - ptc.pts[ret_matches[k].first].y;
                    djk[2] = ptc.pts[ret_matches[j].first].z - ptc.pts[ret_matches[k].first].z;
                    size_t id_j = ret_matches[j].first % nmon2;
                    size_t id_k = ret_matches[k].first % nmon2;

                    // sanity check: no self-image interaction. shouldn't happen if box > 2*cutoff
                    if (id_j == i || id_k == i || id_j == id_k) continue;

                    // add trimer (i, id_j, id_k) if trimer is not a triangle or if i has the lowest label
                    if (djk[0] * djk[0] + djk[1] * djk[1] + djk[2] * djk[2] > cutoff * cutoff ||
                        (tag_index[i] < tag_index[id_j] && tag_index[i] < tag_index[id_k])) {
                        nmers.push_back(mon_index[i]);
                        nmers.push_back(mon_index[id_j]);
                        nmers.push_back(mon_index[id_k]);
                    }
                }
            }
        }

        // Add tetramers if requested
        if (n_max == 4) {
            for (size_t j = 0; j < nMatches; j++) {
                for (size_t k = j + 1; k < nMatches; k++) {
                    for (size_t l = k + 1; l < nMatches; l++) {
                        double djk[3];
                        djk[0] = ptc.pts[ret_matches[j].first].x - ptc.pts[ret_matches[k].first].x;
                        djk[1] = ptc.pts[ret_matches[j].first].y - ptc.pts[ret_matches[k].first].y;
                        djk[2] = ptc.pts[ret_matches[j].first].z - ptc.pts[ret_matches[k].first].z;
                        double d2jk = djk[0] * djk[0] + djk[1] * djk[1] + djk[2] * djk[2];

                        double djl[3];
                        djl[0] = ptc.pts[ret_matches[j].first].x - ptc.pts[ret_matches[l].first].x;
                        djl[1] = ptc.pts[ret_matches[j].first].y - ptc.pts[ret_matches[l].first].y;
                        djl[2] = ptc.pts[ret_matches[j].first].z - ptc.pts[ret_matches[l].first].z;
                        double d2jl = djl[0] * djl[0] + djl[1] * djl[1] + djl[2] * djl[2];

                        double dkl[3];
                        dkl[0] = ptc.pts[ret_matches[k].first].x - ptc.pts[ret_matches[l].first].x;
                        dkl[1] = ptc.pts[ret_matches[k].first].y - ptc.pts[ret_matches[l].first].y;
                        dkl[2] = ptc.pts[ret_matches[k].first].z - ptc.pts[ret_matches[l].first].z;
                        double d2kl = dkl[0] * dkl[0] + dkl[1] * dkl[1] + dkl[2] * dkl[2];

                        size_t id_j = ret_matches[j].first % nmon2;
                        size_t id_k = ret_matches[k].first % nmon2;
                        size_t id_l = ret_matches[l].first % nmon2;

                        //  Use this condition for cutoff-by-largest-distance scheme
                        //  if (tag_index[i] < tag_index[id_j] && tag_index[i] < tag_index[id_k] && tag_index[i] <
                        //  tag_index[id_l] && d2jk < cutoff * cutoff && d2jl < cutoff * cutoff && d2kl < cutoff *
                        //  cutoff) {

                        //  Use this condition for center-3-neighbor scheme
                        if (!((tag_index[i] >= tag_index[id_j] && d2jk < cutoff * cutoff && d2jl < cutoff * cutoff) ||
                              (tag_index[i] >= tag_index[id_k] && d2jk < cutoff * cutoff && d2kl < cutoff * cutoff) ||
                              (tag_index[i] >= tag_index[id_l] && d2kl < cutoff * cutoff && d2jl < cutoff * cutoff))) {
                            // add tetramers here
                            nmers.push_back(mon_index[i]);
                            nmers.push_back(mon_index[id_j]);
                            nmers.push_back(mon_index[id_k]);
                            nmers.push_back(mon_index[id_l]);
                        }
                    }
                }
            }
        }
    }
}

void GetExcluded(std::string mon, nlohmann::json mon_j, excluded_set_type &exc12, excluded_set_type &exc13,
                 excluded_set_type &exc14) {
    // Clearing excluded pairs just in case
    exc12.clear();
    exc13.clear();
    exc14.clear();

    bool is_in_json = false;
    if(mon_j.contains(mon)) {
        try {
            excluded_set_type e12 = mon_j[mon]["exc12"];
            exc12 = e12;
            is_in_json = true;
        } catch (...) {
            exc12.clear();
        }

        try {
            excluded_set_type e13 = mon_j[mon]["exc13"];
            exc13 = e13;
            is_in_json = true;
        } catch (...) {
            exc13.clear();
        }

        try {
            excluded_set_type e14 = mon_j[mon]["exc14"];
            exc14 = e14;
            is_in_json = true;
        } catch (...) {
            exc14.clear();
        }
    }

    if (is_in_json) return;

    // MBX v0.1.0a
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
    // MBX v0.2.3a
    if (mon == "co2_archive") {
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc13.insert(std::make_pair(1, 2));
    }
    // MBX v0.2.4a
    if (mon == "ch4") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 3));
        exc12.insert(std::make_pair(0, 2));
        exc12.insert(std::make_pair(0, 4));
        // 13 distances
        exc13.insert(std::make_pair(1, 2));
        exc13.insert(std::make_pair(1, 3));
        exc13.insert(std::make_pair(1, 4));
        exc13.insert(std::make_pair(2, 3));
        exc13.insert(std::make_pair(3, 4));
        exc13.insert(std::make_pair(2, 4));
    }

    if (mon == "co2") {
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc13.insert(std::make_pair(1, 2));
    }

    if (mon == "co2cm5100" || mon == "co2cm595" || mon == "co2cm590" || mon == "co2cm5875" || mon == "co2cm585" ||
        mon == "co2cm580") {
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc13.insert(std::make_pair(1, 2));
    }

    if (mon == "n2o5") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(1, 3));
        exc12.insert(std::make_pair(2, 6));
        exc12.insert(std::make_pair(1, 4));
        exc12.insert(std::make_pair(2, 5));
        exc12.insert(std::make_pair(0, 2));
        // 13 distances
        exc13.insert(std::make_pair(1, 2));
        exc13.insert(std::make_pair(5, 6));
        exc13.insert(std::make_pair(0, 6));
        exc13.insert(std::make_pair(0, 5));
        exc13.insert(std::make_pair(0, 4));
        exc13.insert(std::make_pair(0, 3));
        exc13.insert(std::make_pair(3, 4));
        // 14 distances
        exc14.insert(std::make_pair(1, 5));
        exc14.insert(std::make_pair(1, 6));
        exc14.insert(std::make_pair(2, 3));
        exc14.insert(std::make_pair(2, 4));
    }

    if (mon == "nh3" || mon == "nh3pbe0d3bj") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 3));
        exc12.insert(std::make_pair(0, 2));
        // 13 distances
        exc13.insert(std::make_pair(1, 2));
        exc13.insert(std::make_pair(1, 3));
        exc13.insert(std::make_pair(2, 3));
        // 14 distances
    }

    if (mon == "dp2") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        // 13 distances
        // 14 distances
    }

    if (mon == "h4_dummy") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        exc12.insert(std::make_pair(0, 2));
        exc12.insert(std::make_pair(0, 3));
        // 13 distances
        exc13.insert(std::make_pair(1, 2));
        exc13.insert(std::make_pair(1, 3));
        exc13.insert(std::make_pair(2, 3));
        // 14 distances
    }

    if (mon == "h2") {
        // 12 distances
        exc12.insert(std::make_pair(0, 1));
        // 13 distances
        // 14 distances
    }

    if (mon == "mbpbe") {
        exc13.insert(std::make_pair(2, 3));
        exc13.insert(std::make_pair(1, 3));
        exc13.insert(std::make_pair(1, 2));
        exc12.insert(std::make_pair(0, 3));
        exc12.insert(std::make_pair(0, 2));
        exc12.insert(std::make_pair(0, 1));
    }

    // =====>> BEGIN SECTION EXCLUDED <<=====
    // =====>> PASTE CODE BELOW <<=====
    // =====>> END SECTION EXCLUDED <<=====
}

bool IsExcluded(excluded_set_type exc, size_t a, size_t b) {
    return ((exc.find(std::make_pair(a, b)) != exc.end()) || (exc.find(std::make_pair(b, a)) != exc.end()));
}

double GetAdd(bool is12, bool is13, bool is14, std::string mon) {
    // Intermolecular aDD is always 0.055
    double aDD = 0.055;
    // For water
    if (mon == "h2o" || mon == "mbpbe") {
        if (is12) {
            aDD = 0.626;
        } else {
            aDD = 0.055;
        }
        // Any other molecule (as for 01/10/2018)
    } else if (mon == "dp1") {
        aDD = 1.0E24;
    } else if (mon == "dp2") {
        aDD = 1.0E24;
    } else {
        if (is12 || is13) {
            aDD = 0.3;
        } else {
            aDD = 0.055;
        }
    }

    return aDD;
}

double GetAcc(std::string mon) {
    double aCC = 0.4;
    // For water
    if (mon == "dp1") {
        aCC = 1.0E24;
    }

    if (mon == "dp2") {
        aCC = 1.0E24;
    }

    return aCC;
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

    if (mon_id == "h2o" || mon_id == "mbpbe") {
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
                size_t fst_ind, std::vector<double> &chg_der, nlohmann::json mon_j) {
    // Constant that calculates charge
    const double CHARGECON = 1.0;
    // const double CHARGECON = constants::CHARGECON;

    bool is_in_json = false;
    try {
        std::vector<double> chg = mon_j[mon_id]["charges"];
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                charges[fst_ind + nv * nsites + j] = chg[j] * CHARGECON;
            }
        }
        is_in_json = true;
    } catch (...) {
    }

    if (is_in_json) return;
    // Halide charges
    if (mon_id == "f-" || mon_id == "cl-" || mon_id == "br-" || mon_id == "i-") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = -1.0 * CHARGECON;
        }
    }

    // Alkali metal ions
    else if (mon_id == "li+" || mon_id == "na+" || mon_id == "k+" || mon_id == "rb+" || mon_id == "cs+") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 1.0 * CHARGECON;
        }

    } else if (mon_id == "nh3" || mon_id == "nh3pbe0d3bj") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = -0.8205 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = 0.2735 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = 0.2735 * CHARGECON;
            charges[fst_ind + nv * nsites + 3] = 0.2735 * CHARGECON;
        }

    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = -0.538573 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = 0.13464325 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = 0.13464325 * CHARGECON;
            charges[fst_ind + nv * nsites + 3] = 0.13464325 * CHARGECON;
            charges[fst_ind + nv * nsites + 4] = 0.13464325 * CHARGECON;
        }
    } else if (mon_id == "dp1p") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites] = 1.0 * CHARGECON;
        }
    } else if (mon_id == "dp2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 1.0 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = 1.0 * CHARGECON;
        }
    } else if (mon_id == "dp1") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites] = 1.0 * CHARGECON;
        }
    } else if (mon_id == "co2_archive") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 0.706027 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = -0.3530135 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = -0.3530135 * CHARGECON;
        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 0.502742 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = -0.251371 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = -0.251371 * CHARGECON;
        }
    } else if (mon_id == "co2cm5100" || mon_id == "co2cm595" || mon_id == "co2cm590" || mon_id == "co2cm5875" ||
               mon_id == "co2cm585" || mon_id == "co2cm580") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 0.502742 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = -0.251371 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = -0.251371 * CHARGECON;
        }
    } else if (mon_id == "h4_dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 0.00000;
            charges[fst_ind + nv * nsites + 1] = 0.00000;
            charges[fst_ind + nv * nsites + 2] = 0.00000;
            charges[fst_ind + nv * nsites + 3] = 0.00000;
        }
    } else if (mon_id == "n2o5") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = -0.316592 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = 0.703783 * CHARGECON;
            charges[fst_ind + nv * nsites + 2] = 0.703783 * CHARGECON;
            charges[fst_ind + nv * nsites + 3] = -0.27274349999999997 * CHARGECON;
            charges[fst_ind + nv * nsites + 4] = -0.27274349999999997 * CHARGECON;
            charges[fst_ind + nv * nsites + 5] = -0.27274349999999997 * CHARGECON;
            charges[fst_ind + nv * nsites + 6] = -0.27274349999999997 * CHARGECON;
        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 0.0;
        }
    } else if (mon_id == "he") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 0.0;
        }
    } else if (mon_id == "ar") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv] = 0.0;
        }

    } else if (mon_id == "h2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            charges[fst_ind + nv * nsites + 0] = 0.0 * CHARGECON;
            charges[fst_ind + nv * nsites + 1] = 0.0 * CHARGECON;
        }
        // =====>> BEGIN SECTION CHARGES <<=====
        // =======>> PASTE BELOW <<=======

        // END SECTION CHARGES

    } else if (mon_id == "mbpbe") {
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
                         chg_der.data() + shift);
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
            chg2[hy1] = CHARGECON * (0.306258 + gamma21 * (0.306258 + 0.306258));
            // Hydrogen2
            chg2[hy2] = CHARGECON * (0.306258 + gamma21 * (0.306258 + 0.306258));
            // M
            chg2[msite] = CHARGECON * (-0.612537 / (1.0 - gammaM));
        }

        // TODO multiversioning
        // Return all coordinates to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                charges[nv * nsites + j + fst_ind] = chg2[nv + n_mon * j];
            }
        }
        // Note, for now, assuming only water has site dependant charges
    } else if (mon_id == "h2o") {


    size_t nthreads = 1;
    #ifdef _OPENMP
    #pragma omp parallel  // omp_get_num_threads() needs to be inside
                      // parallel region to get number of threads
        {
            if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
        }
    #endif

        // chgtmp = M, H1, H2 according to ttm4.cpp
        std::vector<double> chgtmp(n_mon * (nsites - 1), 0.0);
        size_t fstind_3 = 3 * fst_ind;

        chg_der = std::vector<double>(27 * n_mon, 0.0);

        std::vector<std::vector<double>> chgtmpnvpool;
        for (size_t i = 0; i < nthreads; i++) {
            chgtmpnvpool.push_back(std::vector<double>((nsites - 1)));
        }

        // Calculate individual monomer's charges
        #pragma omp parallel for
        for (size_t nv = 0; nv < n_mon; nv++) {

                int rank = 0;
                #ifdef _OPENMP
                    rank = omp_get_thread_num();
                #endif

            size_t ns3 = nsites * 3;
            size_t shift = 27 * nv;

            std::vector<double>& chgtmpnv = chgtmpnvpool[rank];

            // Calculating the partridge schwenke geometry dependent charges
            ps::dms_nasa(0.0, 0.0, 0.0, xyz.data() + (nv * ns3) + fstind_3, chgtmpnv.data(),
                         chg_der.data() + shift);

            double h1_nasa_charge = chgtmpnv[1];
            double h2_nasa_charge = chgtmpnv[2];
            double o_nasa_charge = chgtmpnv[0];

            // Redistribute Charges according over the hydrogens and M-site according to TTM4 model.

            // Oxygen
            charges[nv * nsites + 0 + fst_ind] = 0.0;

            // Hydrogen1
            charges[nv * nsites + 1 + fst_ind] = CHARGECON * (h1_nasa_charge + gamma21 * (h1_nasa_charge + h2_nasa_charge));
            // Hydrogen2
            charges[nv * nsites + 2 + fst_ind] = CHARGECON * (h2_nasa_charge + gamma21 * (h1_nasa_charge + h2_nasa_charge));
            // M
            charges[nv * nsites + 3 + fst_ind] = CHARGECON * (o_nasa_charge / (1.0 - gammaM));
        }
    }
}

void SetPolfac(std::vector<double> &polfac, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind,
               nlohmann::json mon_j) {
    bool is_in_json = false;
    try {
        std::vector<double> pf = mon_j[mon_id]["polfac"];
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                polfac[fst_ind + nv * nsites + j] = pf[j];
            }
        }
        is_in_json = true;
    } catch (...) {
    }

    if (is_in_json) return;

    // Halides
    if (mon_id == "f-") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.8079;
    } else if (mon_id == "cl-") {  // Chloride
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 5.3602;
    } else if (mon_id == "br-") {  // Bromide
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 3.7819;
    } else if (mon_id == "i-") {  // Iodide
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 5.9563;
    }

    // Alkali metal ions
    if (mon_id == "li+") {  // Lithium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.0285;
    } else if (mon_id == "na+") {  // Sodium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.1476;
    } else if (mon_id == "k+") {  // Potassium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 0.8184;
    } else if (mon_id == "rb+") {  // Rubidium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 1.3614;
    } else if (mon_id == "cs+") {  // Cesium
        for (size_t nv = 0; nv < n_mon; nv++) polfac[fst_ind + nv] = 2.3660;

        // =====>> BEGIN SECTION POLFACS <<=====
        // =======>> PASTE BELOW <<=======
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 1.3932677;
            polfac[fst_ind + nv * nsites + 1] = 0.38978363;
            polfac[fst_ind + nv * nsites + 2] = 0.38978363;
            polfac[fst_ind + nv * nsites + 3] = 0.38978363;
            polfac[fst_ind + nv * nsites + 4] = 0.38978363;
        }
    } else if (mon_id == "dp1p") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites] = 0.0;
        }
    } else if (mon_id == "dp2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 0.0;
            polfac[fst_ind + nv * nsites + 1] = 0.0;
        }
    } else if (mon_id == "dp1") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites] = 0.0;
        }
    } else if (mon_id == "co2_archive") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 1.471677;
            polfac[fst_ind + nv * nsites + 1] = 0.769790;
            polfac[fst_ind + nv * nsites + 2] = 0.769790;
        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 1.2046;
            polfac[fst_ind + nv * nsites + 1] = 0.6722;
            polfac[fst_ind + nv * nsites + 2] = 0.6722;
        }
    } else if (mon_id == "co2cm5100" || mon_id == "co2cm595" || mon_id == "co2cm590" || mon_id == "co2cm5875" ||
               mon_id == "co2cm585" || mon_id == "co2cm580") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 1.2046;
            polfac[fst_ind + nv * nsites + 1] = 0.6722;
            polfac[fst_ind + nv * nsites + 2] = 0.6722;
        }
    } else if (mon_id == "nh3" || mon_id == "nh3pbe0d3bj") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 0.9556;
            polfac[fst_ind + nv * nsites + 1] = 0.3624;
            polfac[fst_ind + nv * nsites + 2] = 0.3624;
            polfac[fst_ind + nv * nsites + 3] = 0.3624;
        }
    } else if (mon_id == "h4_dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 0.00000;
            polfac[fst_ind + nv * nsites + 1] = 0.00000;
            polfac[fst_ind + nv * nsites + 2] = 0.00000;
            polfac[fst_ind + nv * nsites + 3] = 0.00000;
        }

    } else if (mon_id == "n2o5") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 0.7292804719246812;
            polfac[fst_ind + nv * nsites + 1] = 0.9556760793256731;
            polfac[fst_ind + nv * nsites + 2] = 0.9556760793256731;
            polfac[fst_ind + nv * nsites + 3] = 0.7251064765496543;
            polfac[fst_ind + nv * nsites + 4] = 0.7251064765496543;
            polfac[fst_ind + nv * nsites + 5] = 0.7251064765496543;
            polfac[fst_ind + nv * nsites + 6] = 0.7251064765496543;
        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv] = 0.0;
        }
    } else if (mon_id == "ar") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv] = 1.645;
        }
    } else if (mon_id == "he") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv] = 0.20493754;
        }

    } else if (mon_id == "h2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            polfac[fst_ind + nv * nsites + 0] = 0.3198;
            polfac[fst_ind + nv * nsites + 1] = 0.3198;
        }
        // =====>> END SECTION POLFACS <<=====
    } else if (mon_id == "mbpbe") {
        // Creating vector with contiguous data
        std::vector<double> polfac2(n_mon * nsites, 0.0);
        for (size_t nv = 0; nv < n_mon; nv++) {
            // Oxygen
            polfac2[nv] = 0.7705056046539932;

            // looping over sites -- H1 and H2
            for (size_t i = 1; i < nsites - 1; i++) {
                polfac2[nv + i * n_mon] = 0.4364107405988837;
            }
            // M site
            polfac2[nv + n_mon * 3] = 0.7705056046539932;
        }

        // TODO Multiversioning
        // Return all polfacs to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                polfac[nv * nsites + j + fst_ind] = polfac2[nv + n_mon * j];
            }
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

void SetPol(std::vector<double> &pol, std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind,
            nlohmann::json mon_j) {
    bool is_in_json = false;
    try {
        std::vector<double> p = mon_j[mon_id]["pol"];
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                pol[fst_ind + nv * nsites + j] = p[j];
            }
        }
        is_in_json = true;
    } catch (...) {
    }

    if (is_in_json) return;

    // Halides
    if (mon_id == "f-") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.8079;
    } else if (mon_id == "cl-") {  // Chloride
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 5.3602;
    } else if (mon_id == "br-") {  // Bromide
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 3.7819;
    } else if (mon_id == "i-") {  // Iodide
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 5.9563;
    }

    // Alkali metal ions
    if (mon_id == "li+") {  // Lithium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.0285;
    } else if (mon_id == "na+") {  // Sodium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.1476;
    } else if (mon_id == "k+") {  // Potassium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 0.8184;
    } else if (mon_id == "rb+") {  // Rubidium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 1.3614;
    } else if (mon_id == "cs+") {  // Cesium
        for (size_t nv = 0; nv < n_mon; nv++) pol[fst_ind + nv] = 2.3660;
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 1.3932677;
            pol[fst_ind + nv * nsites + 1] = 0.38978363;
            pol[fst_ind + nv * nsites + 2] = 0.38978363;
            pol[fst_ind + nv * nsites + 3] = 0.38978363;
            pol[fst_ind + nv * nsites + 4] = 0.38978363;
        }
    } else if (mon_id == "dp1p") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites] = 0.0;
        }
    } else if (mon_id == "dp2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 0.0;
            pol[fst_ind + nv * nsites + 1] = 0.0;
        }
    } else if (mon_id == "dp1") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites] = 0.0;
        }
    } else if (mon_id == "nh3" || mon_id == "nh3pbe0d3bj") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 0.9556;
            pol[fst_ind + nv * nsites + 1] = 0.3624;
            pol[fst_ind + nv * nsites + 2] = 0.3624;
            pol[fst_ind + nv * nsites + 3] = 0.3624;
        }
    } else if (mon_id == "co2_archive") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 1.471677;
            pol[fst_ind + nv * nsites + 1] = 0.769790;
            pol[fst_ind + nv * nsites + 2] = 0.769790;
        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 1.2046;
            pol[fst_ind + nv * nsites + 1] = 0.6722;
            pol[fst_ind + nv * nsites + 2] = 0.6722;
        }
    } else if (mon_id == "co2cm5100" || mon_id == "co2cm595" || mon_id == "co2cm590" || mon_id == "co2cm5875" ||
               mon_id == "co2cm585" || mon_id == "co2cm580") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 1.2046;
            pol[fst_ind + nv * nsites + 1] = 0.6722;
            pol[fst_ind + nv * nsites + 2] = 0.6722;
        }
    } else if (mon_id == "h4_dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 0.00000;
            pol[fst_ind + nv * nsites + 1] = 0.00000;
            pol[fst_ind + nv * nsites + 2] = 0.00000;
            pol[fst_ind + nv * nsites + 3] = 0.00000;
        }
    } else if (mon_id == "n2o5") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 0.7292804719246812;
            pol[fst_ind + nv * nsites + 1] = 0.9556760793256731;
            pol[fst_ind + nv * nsites + 2] = 0.9556760793256731;
            pol[fst_ind + nv * nsites + 3] = 0.7251064765496543;
            pol[fst_ind + nv * nsites + 4] = 0.7251064765496543;
            pol[fst_ind + nv * nsites + 5] = 0.7251064765496543;
            pol[fst_ind + nv * nsites + 6] = 0.7251064765496543;
        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv] = 0.0;
        }
    } else if (mon_id == "ar") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv] = 1.645;
        }
    } else if (mon_id == "he") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv] = 0.20493754;
        }

    } else if (mon_id == "h2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            pol[fst_ind + nv * nsites + 0] = 0.3198;
            pol[fst_ind + nv * nsites + 1] = 0.3198;
        }

        // =====>> BEGIN SECTION POLS <<=====
        // =====>> PASTE  BELOW <<=====

        // =====>> END SECTION POLS <<=====

    } else if (mon_id == "mbpbe") {
        // Creating vector with contiguous data
        std::vector<double> pol2(n_mon * nsites, 0.0);
        for (size_t nv = 0; nv < n_mon; nv++) {
            // Oxygen
            pol2[nv] = 0.7705056046539932;

            // looping over sites -- H1 and H2
            for (size_t i = 1; i < nsites - 1; i++) {
                pol2[nv + i * n_mon] = 0.4364107405988837;
            }
        }

        // TODO Multiversioning
        // Return all pols to the original vector
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < nsites; j++) {
                pol[nv * nsites + j + fst_ind] = pol2[nv + n_mon * j];
            }
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

void SetLJLongRange(std::vector<double> &lj_lr, std::string mon_id, size_t n_mon, size_t natoms, size_t fst_ind,
                    std::vector<std::pair<std::string, std::string>> use_lj, nlohmann::json repdisp_j) {
    std::vector<double> ljlr_i(natoms, 0.0);
    for (size_t i = 0; i < natoms; i++) {
        double eps, sigma;
        lj::GetLjParams(mon_id, mon_id, i, i, eps, sigma, use_lj, repdisp_j);
        ljlr_i[i] = 2 * std::sqrt(eps) * sigma * sigma * sigma;
    }

    for (size_t nv = 0; nv < n_mon; nv++) {
        for (size_t i = 0; i < natoms; i++) {
            lj_lr[fst_ind + nv * natoms + i] = ljlr_i[i];
        }
    }
}

void SetC6LongRange(std::vector<double> &c6_lr, std::string mon_id, size_t n_mon, size_t natoms, size_t fst_ind,
                    nlohmann::json mon_j) {
    bool is_in_json = false;
    try {
        std::vector<double> c6lr = mon_j[mon_id]["c6lr"];
        for (size_t nv = 0; nv < n_mon; nv++) {
            for (size_t j = 0; j < natoms; j++) {
                c6_lr[fst_ind + nv * natoms + j] = c6lr[j];
            }
        }
        is_in_json = true;
    } catch (...) {
    }

    if (is_in_json) return;

    // All these C6 come from Qchem/avtz. We put two molecules at 50 A and get the c6 of the atoms.
    if (mon_id == "f-") {  // Fluoride
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 25.56412750183350184739;
    } else if (mon_id == "cl-") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 57.88297168036554772821;
    } else if (mon_id == "br-") {
        // FIXME This value will be set from C6 Br-O and Br-H. Qchem does not allow
        // C6 calculations for only pseudopotential atoms, i.e. 2 bromide, iodide...)
        // It will be calculated as:
        // (C6(Br--O)/C6_lr(O) + C6(Br--H)/C6_lr(H)) / 2
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 74.56169774397084024344;
    } else if (mon_id == "i-") {
        // FIXME This value will be set from C6 I-O and I-H. Qchem does not allow
        // C6 calculations for only pseudopotential atoms, i.e. 2 bromide, iodide...)
        // It will be calculated as:
        // (C6(I--O)/C6_lr(O) + C6(I--H)/C6_lr(H)) / 2
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 105.39445721563933883337;
    } else if (mon_id == "li+") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 3.24887148714749872914;
    } else if (mon_id == "na+") {
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 16.02787872333703428437;
    } else if (mon_id == "k+") {
        // FIXME This value will be set from C6 K-O and K-H. Qchem does not allow
        // C6 calculations for only pseudopotential atoms, i.e. 2 bromide, iodide...)
        // It will be calculated as:
        // (C6(K--O)/C6_lr(O) + C6(K--H)/C6_lr(H)) / 2
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 37.63136349992751547203;
    } else if (mon_id == "rb+") {
        // FIXME This value will be set from C6 Rb-O and Rb-H. Qchem does not allow
        // C6 calculations for only pseudopotential atoms, i.e. 2 bromide, iodide...)
        // It will be calculated as:
        // (C6(Rb--O)/C6_lr(O) + C6(Rb--H)/C6_lr(H)) / 2
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 49.17633137941422098718;
    } else if (mon_id == "cs+") {
        // FIXME This value will be set from C6 Cs-O and Cs-H. Qchem does not allow
        // C6 calculations for only pseudopotential atoms, i.e. 2 bromide, iodide...)
        // It will be calculated as:
        // (C6(Cs--O)/C6_lr(O) + C6(Cs--H)/C6_lr(H)) / 2
        for (size_t nv = 0; nv < n_mon; nv++) c6_lr[fst_ind + nv] = 65.76255818916154320248;
        // BEGIN SECTION C6_LONG_RANGE
        // ==> PASTE YOUR CODE BELOW <==
    } else if (mon_id == "nh3" || mon_id == "nh3pbe0d3bj") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 15.618415412582673;     // A
            c6_lr[nv * natoms + fst_ind + 1] = 6.328530635147467;  // B
            c6_lr[nv * natoms + fst_ind + 2] = 6.328530635147467;  // B
            c6_lr[nv * natoms + fst_ind + 3] = 6.328530635147467;  // B
        }
    } else if (mon_id == "ch4") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.41398863;      // N
            c6_lr[nv * natoms + fst_ind + 1] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 2] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 3] = 6.064748037;  // H
            c6_lr[nv * natoms + fst_ind + 4] = 6.064748037;  // H
        }

    } else if (mon_id == "co2_archive") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.91673320223304547491;      // C
            c6_lr[nv * natoms + fst_ind + 1] = 13.04205731316957524126;  // O
            c6_lr[nv * natoms + fst_ind + 2] = 13.04205731316957524126;  // O
        }
    } else if (mon_id == "dp1p") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 0.0;
        }
    } else if (mon_id == "dp2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind + 0] = 0.0;
            c6_lr[nv * natoms + fst_ind + 1] = 0.0;
        }
    } else if (mon_id == "dp1") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 0.0;
        }
    } else if (mon_id == "co2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.91673320223304547491;      // C
            c6_lr[nv * natoms + fst_ind + 1] = 13.04205731316957524126;  // O
            c6_lr[nv * natoms + fst_ind + 2] = 13.04205731316957524126;  // O
        }
    } else if (mon_id == "co2cm5100" || mon_id == "co2cm595" || mon_id == "co2cm590" || mon_id == "co2cm5875" ||
               mon_id == "co2cm585" || mon_id == "co2cm580") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 17.91673320223304547491;      // C
            c6_lr[nv * natoms + fst_ind + 1] = 13.04205731316957524126;  // O
            c6_lr[nv * natoms + fst_ind + 2] = 13.04205731316957524126;  // O
        }
    } else if (mon_id == "h4_dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 0.00000;
            c6_lr[nv * natoms + fst_ind + 1] = 0.00000;
            c6_lr[nv * natoms + fst_ind + 2] = 0.00000;
            c6_lr[nv * natoms + fst_ind + 3] = 0.00000;
        }
    } else if (mon_id == "dummy") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[fst_ind + nv] = 0.0;
        }
    } else if (mon_id == "ar") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[fst_ind + nv] = 27.83030183091804;
        }
    } else if (mon_id == "he") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[fst_ind + nv] = 4.93437037524;
        }
    } else if (mon_id == "h2") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 6.740200293759822;      // A
            c6_lr[nv * natoms + fst_ind + 1] = 6.740200293759822;  // A
        }
    } else if (mon_id == "n2o5") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[fst_ind + nv] = 13.020241929607836;               // O
            c6_lr[nv * natoms + fst_ind + 1] = 13.09042957671318;   // N
            c6_lr[nv * natoms + fst_ind + 2] = 13.09042957671318;   // N
            c6_lr[nv * natoms + fst_ind + 3] = 13.402239942963767;  // O
            c6_lr[nv * natoms + fst_ind + 4] = 13.402239942963767;  // O
            c6_lr[nv * natoms + fst_ind + 5] = 13.402239942963767;  // O
            c6_lr[nv * natoms + fst_ind + 6] = 13.402239942963767;  // O
        }
        // END SECTION C6_LONG_RANGE
    } else if (mon_id == "mbpbe") {
        for (size_t nv = 0; nv < n_mon; nv++) {
            c6_lr[nv * natoms + fst_ind] = 13.637419110667532;  // A
            c6_lr[nv * natoms + fst_ind + 1] = 6.290794862336555;   // B
            c6_lr[nv * natoms + fst_ind + 2] = 6.290794862336555;   // B
        }
        // Water is the only monomer which C6 does not come from qchem.
        // It comes from MB-pol (C6O = sqrt(C6OO))
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
    if (mon == "h2o" || mon == "mbpbe") {
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
                           const std::vector<double> phi, std::vector<double> &grad, const std::vector<double> chg_grad,
                           double *crd, std::vector<double> *qdvirial) {
    // Note: XYZ is in the internal electorstatics order: Ox1Ox2Ox3...Oy1Oy2Oy3.. Oz1...Hx1Hx2..
    // If water, extracted from patridge-schwneke paper
    if (mon == "h2o") {
        for (size_t mm = 0; mm < nmon; mm++) {
            // Declaring shfts for coordinates and fields
            const size_t shift = fi_crd + 12 * mm;
            const size_t sphi = fi_sites + 4 * mm;
            // Size of gradq is 27: derivative of charge in each site (3) with respect of the position of each site
            // (3) in each of the xyz components (3); 3x3x3 OHH reign
            double gradq[27];
            std::fill(gradq, gradq + 27, 0.0);
            // Derivatives of the charges in HHM reign
            double chgdev[27];
            std::copy(chg_grad.begin() + 27 * mm, chg_grad.begin() + 27 * (mm + 1), chgdev);

// Fast way to access the derivatives
#define DQ3(l, m, k) chgdev[k + 3 * (m + 3 * l)]
#define GRADQ(l, m, k) gradq[k + 3 * (m + 3 * l)]

            // Convert charge derivatives from HHM to OHH
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

            // Convert units to match kcal/mol when multiplied by phi
            for (size_t i = 0; i < 27; ++i) gradq[i] *= constants::COULOMB;

            const size_t io = shift;
            const size_t ih1 = shift + 3;
            const size_t ih2 = shift + 6;

            // See J. Chem. Phys. 116, 5115 (2002); https://doi.org/10.1063/1.1447904, Eqs. A8 and A9
            // Retrieve the actual gradients in kcal/mol
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

            if (qdvirial != 0) {
                // OHH coordinates of current water molecule
                std::vector<double> temp_pos = {crd[mm * 1],        crd[mm + nmon],     crd[mm + 2 * nmon],
                                                crd[mm + 3 * nmon], crd[mm + 4 * nmon], crd[mm + 5 * nmon],
                                                crd[mm + 6 * nmon], crd[mm + 7 * nmon], crd[mm + 8 * nmon]};
                std::vector<double> chgtmpnv_test((3));
                std::vector<double> chgder_test((27));

                std::vector<double> aux_data(6, 0.0);  // declare array to store auxillary output from dms_nasa_vir

                ps::dms_nasa(0.0, 0.0, 0.0, temp_pos.data(), chgtmpnv_test.data(), chgder_test.data(),
                             &aux_data);  // get them aux data (charge derivateves with respect to internal coords)

                // get the charge derivatives in internal coordinates ( r12 = rOH1, r13=rOH2, cos = cos(theta))
                // p1 is H1 charge, p2 is H2 charge, p0 would be the O = -H1 - H2
                double dp1dr12 = aux_data[0];  // pass data onto variables
                double dp1dr13 = aux_data[1];
                double dp2dr12 = aux_data[2];
                double dp2dr13 = aux_data[3];
                double dp1dcos = aux_data[4];
                double dp2dcos = aux_data[5];

                std::vector<double> dqdr12(4, 0.0);
                std::vector<double> dqdr13(4, 0.0);
                std::vector<double> dqdcos(4, 0.0);

                double gamma = gammaM;

                double tmp = gamma / 2.0 / (1.0 - gamma);

                // adding M-site contribution to the derivatives -> converting from p (3 point charge fropm PS) to q
                // (4 point charge)
                dqdr12[1] = dp1dr12 + (dp1dr12 + dp2dr12) * tmp;  // h1
                dqdr13[1] = dp1dr13 + (dp1dr13 + dp2dr13) * tmp;
                dqdcos[1] = dp1dcos + (dp1dcos + dp2dcos) * tmp;

                dqdr12[2] = dp2dr12 + (dp1dr12 + dp2dr12) * tmp;  // h2
                dqdr13[2] = dp2dr13 + (dp1dr13 + dp2dr13) * tmp;
                dqdcos[2] = dp2dcos + (dp1dcos + dp2dcos) * tmp;

                dqdr12[3] = -(dp1dr12 + dp2dr12) / (1.0 - gamma);  // M
                dqdr13[3] = -(dp1dr13 + dp2dr13) / (1.0 - gamma);
                dqdcos[3] = -(dp1dcos + dp2dcos) / (1.0 - gamma);

                // probably start at ii =1
                for (int ii = 0; ii < 4; ii++) {  // loop over all sites

                    // get the electrostatic potential on that site
                    double vtmp = 0.0;

                    if (ii == 1) {
                        vtmp = phi[sphi + 1];  // h1
                    }

                    if (ii == 2) {
                        vtmp = phi[sphi + 2];  // h2
                    }

                    if (ii == 3) {
                        vtmp = phi[sphi + 3];  // M
                    }

                    // We could remove l loop, and set m=1 and m=2, or delete the m part.

                    for (int l = 0; l < 3; l++) {  // double loop for charge derivatives with respect to each
                                                   // internal bond coordinate (ie r12 and r13) for each atom.

                        for (int m = l + 1; m < 4; m++) {
                            double rx;
                            double ry;
                            double rz;
                            double dqdr_tmp;
                            double prefac;

                            // get the distances and charge derivatives with respect to r12 and r13 (the OH bonds)
                            if ((l == 0) && (m == 1)) {
                                dqdr_tmp = dqdr12[ii];

                                rx = temp_pos[0] - temp_pos[3];
                                ry = temp_pos[1] - temp_pos[4];
                                rz = temp_pos[2] - temp_pos[5];
                                prefac = 1.0;

                            } else if ((l == 0) && (m == 2)) {
                                dqdr_tmp = dqdr13[ii];

                                rx = temp_pos[0] - temp_pos[6];
                                ry = temp_pos[1] - temp_pos[7];
                                rz = temp_pos[2] - temp_pos[8];
                                prefac = 1.0;

                            } else {
                                rx = EPSILON;
                                ry = EPSILON;
                                rz = EPSILON;
                                dqdr_tmp = EPSILON;
                                prefac = 0.0;
                            }

                            double rjk = std::sqrt(rx * rx + ry * ry + rz * rz);

                            // add to virial
                            (*qdvirial)[0] -= vtmp * dqdr_tmp * rx * rx / rjk * prefac * constants::COULOMB;
                            (*qdvirial)[1] -= vtmp * dqdr_tmp * rx * ry / rjk * prefac * constants::COULOMB;
                            (*qdvirial)[2] -= vtmp * dqdr_tmp * rx * rz / rjk * prefac * constants::COULOMB;

                            (*qdvirial)[4] -= vtmp * dqdr_tmp * ry * ry / rjk * prefac * constants::COULOMB;
                            (*qdvirial)[5] -= vtmp * dqdr_tmp * ry * rz / rjk * prefac * constants::COULOMB;

                            (*qdvirial)[8] -= vtmp * dqdr_tmp * rz * rz / rjk * prefac * constants::COULOMB;

                        }  // m
                    }      // l

                    // angular terms
                    double rx1 = temp_pos[0] - temp_pos[3];
                    double ry1 = temp_pos[1] - temp_pos[4];
                    double rz1 = temp_pos[2] - temp_pos[5];

                    double rx2 = temp_pos[0] - temp_pos[6];
                    double ry2 = temp_pos[1] - temp_pos[7];
                    double rz2 = temp_pos[2] - temp_pos[8];

                    double rx3 = temp_pos[3] - temp_pos[6];
                    double ry3 = temp_pos[4] - temp_pos[7];
                    double rz3 = temp_pos[5] - temp_pos[8];

                    double r1 = std::sqrt(rx1 * rx1 + ry1 * ry1 + rz1 * rz1);
                    double r2 = std::sqrt(rx2 * rx2 + ry2 * ry2 + rz2 * rz2);
                    double r3 = std::sqrt(rx3 * rx3 + ry3 * ry3 + rz3 * rz3);

                    double rdot = rx1 * rx2 + ry1 * ry2 + rz1 * rz2;
                    double cost = rdot / (r1 * r2);

                    double dcosdr12 = (r1 - r2 * cost) / r1 / r2;
                    double dcosdr13 = (r2 - r1 * cost) / r1 / r2;
                    double dcosdr23 = -r3 / r1 / r2;

                    // chain rule the cosine to bond coords
                    double dcostmp1 = dcosdr12 * rx1 * rx1 / r1 + dcosdr13 * rx2 * rx2 / r2 + dcosdr23 * rx3 * rx3 / r3;
                    double dcostmp2 = dcosdr12 * rx1 * ry1 / r1 + dcosdr13 * rx2 * ry2 / r2 + dcosdr23 * rx3 * ry3 / r3;
                    double dcostmp3 = dcosdr12 * rx1 * rz1 / r1 + dcosdr13 * rx2 * rz2 / r2 + dcosdr23 * rx3 * rz3 / r3;

                    double dcostmp5 = dcosdr12 * ry1 * ry1 / r1 + dcosdr13 * ry2 * ry2 / r2 + dcosdr23 * ry3 * ry3 / r3;
                    double dcostmp6 = dcosdr12 * ry1 * rz1 / r1 + dcosdr13 * ry2 * rz2 / r2 + dcosdr23 * ry3 * rz3 / r3;

                    double dcostmp9 = dcosdr12 * rz1 * rz1 / r1 + dcosdr13 * rz2 * rz2 / r2 + dcosdr23 * rz3 * rz3 / r3;

                    // add to virial
                    (*qdvirial)[0] -= dqdcos[ii] * dcostmp1 * vtmp * constants::COULOMB;
                    (*qdvirial)[1] -= dqdcos[ii] * dcostmp2 * vtmp * constants::COULOMB;
                    (*qdvirial)[2] -= dqdcos[ii] * dcostmp3 * vtmp * constants::COULOMB;
                    (*qdvirial)[4] -= dqdcos[ii] * dcostmp5 * vtmp * constants::COULOMB;
                    (*qdvirial)[5] -= dqdcos[ii] * dcostmp6 * vtmp * constants::COULOMB;
                    (*qdvirial)[8] -= dqdcos[ii] * dcostmp9 * vtmp * constants::COULOMB;

                    (*qdvirial)[3] = (*qdvirial)[1];
                    (*qdvirial)[6] = (*qdvirial)[2];
                    (*qdvirial)[7] = (*qdvirial)[5];

                }  // ii

            }  // end virial
        }
    }
}

}  // namespace systools

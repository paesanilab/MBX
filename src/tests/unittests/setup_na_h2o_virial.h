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

#ifndef UNITTESTS_SETUP_Na_H2O_VIRIAL_H
#define UNITTESTS_SETUP_Na_H2O_VIRIAL_H

#define SETUP_Na_H2O_VIRIAL                                                                          \
    const size_t n_monomers = 2;                                                                                    \
    std::vector<std::string> atom_names{"Na", "O", "H", "H"};                                                           \
    std::vector<std::string> monomer_names = {"na","h2o"};                                                          \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"na", "h2o"}}; \
    std::vector<size_t> n_atoms_vector = {1,3};                                                                       \
    std::vector<size_t> n_sites_vector = {1,4};                                                                       \
    std::vector<size_t> first_index = {0,1};                                                                          \
    std::vector<size_t> first_index_realSites = {0,1};                                                                \
    const size_t n_atoms = 4;                                                                                          \
    const size_t n_sites = 5;         \
    std::vector<double> box = {15.0, 0.0, 0.0, 0.0, 15.0, 0.0, 0.0, 0.0,15.0};                           \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"na", 1},{"h2o", 1}};                \
    std::vector<double> virial_1b = {   \
        -15.5885 , 5.34557 , 5.68179 , 5.34557 , -1.94539 , -2.55637 , 5.68179 , -2.55637 , -5.36263}; \
    std::vector<double> virial_elec = { \
        11.1457 , -3.97076 , 1.79961 , -3.97076 , 2.08396 , 1.0376 , 1.79961 , 1.0376 , 5.64599}; \
    std::vector<double> virial_buck = {8.14904 , -3.9217 , -3.47649 , -3.9217 , 1.89835 , 1.69604 , -3.47649 , 1.69604 , 1.537}; \
    std::vector<double> virial_disp = { \
        -2.63753 , 1.24327 , 1.02596 , 1.24327 , -0.592478 , -0.501568 , 1.02596 , -0.501568 , -0.453768}; \
    std::vector<double> real_coords = std::vector<double>{                                          \
   0.0000029477,        0.0000014926,       -0.0000005021,\
    -2.7779819073,        1.2138365139,        0.9901681838,\
   -3.1030647065,        1.1366277718,        0.0870932044, \
   -1.8474796079,        0.9364284622,        0.8766593013 }; 
#endif

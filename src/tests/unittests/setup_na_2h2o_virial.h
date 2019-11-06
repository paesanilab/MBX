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

#ifndef UNITTESTS_SETUP_Na_2H2O_VIRIAL_H
#define UNITTESTS_SETUP_Na_2H2O_VIRIAL_H

#define SETUP_Na_2H2O_VIRIAL                                                                          \
    const size_t n_monomers = 3;                                                                                    \
    std::vector<std::string> atom_names{"Na", "O", "H", "H", "O", "H", "H"};                                                           \
    std::vector<std::string> monomer_names = {"na","h2o","h2o"};                                                          \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"na", "h2o"}}; \
    std::vector<size_t> n_atoms_vector = {1,3,3};                                                                       \
    std::vector<size_t> n_sites_vector = {1,4,4};                                                                       \
    std::vector<size_t> first_index = {0,1,4};                                                                          \
    std::vector<size_t> first_index_realSites = {0,1,5};                                                                \
    const size_t n_atoms = 7;                                                                                          \
    const size_t n_sites = 9;         \
    std::vector<double> box = {15.0, 0.0, 0.0, 0.0, 15.0, 0.0, 0.0, 0.0,15.0};                           \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"na", 1},{"h2o", 2}};                \
    std::vector<double> virial_1b = {   \
        -2.48093 , 0.0643855 , 0.368728 , 0.0643855 , -3.11611 , 0.0370003 , 0.368728 , 0.0370003 , -2.91067}; \
    std::vector<double> virial_2b = { 117.723 , 12.0134 , 68.7994 , 12.0134 , -0.791657 , 6.90374 , 68.7994 , 6.90374 , 37.5397 }; \
    std::vector<double> virial_3b = {-3.22932 , -0.340712 , -1.95121 , -0.340712 , 0.131853 , -0.195796 , -1.95121 , -0.195796 , -0.955259}; \
    std::vector<double> virial_elec = { \
        -106.018 , -11.1615 , -63.9204 , -11.1615 , 4.09211 , -6.41416 , -63.9204 , -6.41416 , -31.5209}; \
    std::vector<double> virial_buck = {124.518 , 12.4852 , 71.501 , 12.4852 , 1.35017 , 7.17484 , 71.501 , 7.17484 , 41.1867}; \
    std::vector<double> virial_disp = { \
        -9.22417 , -0.916337 , -5.24774 , -0.916337 , -0.184352 , -0.526591 , -5.24774 , -0.526591 , -3.10812}; \
    std::vector<double> real_coords = std::vector<double>{                                          \
     -9.099416005e-07,   2.108440460e-07,   1.544110895e-06,\
      1.949776179e+00,   1.956526819e-01,   1.120478161e+00,\
      2.077694229e+00,   2.439613467e-01,   2.070272125e+00,\
      2.834431181e+00,   2.489517449e-01,   7.525786264e-01,\
     -1.949778003e+00,  -1.956522593e-01,  -1.120475066e+00,\
     -2.514409080e+00,   5.104488051e-01,  -1.442062323e+00,\
     -2.397719981e+00,  -1.003361051e+00,  -1.380782235e+00 };
#endif

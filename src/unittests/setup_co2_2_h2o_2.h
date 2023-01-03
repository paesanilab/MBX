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

#ifndef UNITTESTS_SETUP_CO2_2_H2O_2_H
#define UNITTESTS_SETUP_CO2_2_H2O_2_H

#define SETUP_CO2_2_H2O_2                                                                                             \
    double one_body_energy = 7.8419587769e-01;                                                                        \
    double two_body_energy = 1.0787028559e+01;                                                                        \
    double three_body_energy = 0.0000000000e+00;                                                                      \
    double dispersion_energy = -8.4380632817e+00;                                                                     \
    double electrostatic_energy = -1.7175209596e+01;                                                                  \
    double total_energy = -1.4042048440e+01;                                                                          \
    const int n_monomers = 4;                                                                                         \
    std::vector<double> coords{-3.4761788810e-01, 2.2373133660e-01,  -9.9154063000e-03, 7.0932878000e-02,             \
                               -6.5197216420e-01, 6.4717641080e-01,  -7.7861447400e-01, 1.0711451166e+00,             \
                               -6.8794039240e-01, 2.0135383728e+00,  2.4563814200e+00,  -9.0849269180e-01,            \
                               2.4173378894e+00,  1.3610844076e+00,  -7.8399837350e-01, 1.6310136520e+00,             \
                               3.5458287814e+00,  -1.0710246849e+00, 2.7444571499e+00,  -2.8895397500e-02,            \
                               2.0146795423e+00,  2.0559286005e+00,  -6.6905971320e-01, 1.7989680726e+00,             \
                               3.1607063310e+00,  1.4497453300e-01,  1.1623613238e+00,  2.6863654098e+00,             \
                               -1.2838090910e-01, 1.7868117332e+00,  9.9259685460e-01,  2.1439525102e+00,             \
                               1.6639051316e+00,  8.0501784870e-01,  2.7574021582e+00,  2.3791378598e+00,             \
                               1.6203025410e+00,  1.4960194683e+00,  2.0291893372e+00,  1.0864993964e+00,             \
                               2.1365953595e+00,  1.8944371375e+00};                                                  \
    std::vector<double> real_coords{-3.4761788810e-01, 2.2373133660e-01,  -9.9154063000e-03, 7.0932878000e-02,        \
                                    -6.5197216420e-01, 6.4717641080e-01,  -7.7861447400e-01, 1.0711451166e+00,        \
                                    -6.8794039240e-01, 2.0135383728e+00,  2.4563814200e+00,  -9.0849269180e-01,       \
                                    2.4173378894e+00,  1.3610844076e+00,  -7.8399837350e-01, 1.6310136520e+00,        \
                                    3.5458287814e+00,  -1.0710246849e+00, 2.7444571499e+00,  -2.8895397500e-02,       \
                                    2.0146795423e+00,  2.0559286005e+00,  -6.6905971320e-01, 1.7989680726e+00,        \
                                    3.1607063310e+00,  1.4497453300e-01,  1.1623613238e+00,  9.9259685460e-01,        \
                                    2.1439525102e+00,  1.6639051316e+00,  8.0501784870e-01,  2.7574021582e+00,        \
                                    2.3791378598e+00,  1.6203025410e+00,  1.4960194683e+00,  2.0291893372e+00};       \
    std::vector<std::string> atom_names{"C", "O", "O", "C", "O", "O", "O", "H", "H", "O", "H", "H"};                  \
    std::vector<std::string> monomer_names = {"co2_archive", "co2_archive", "h2o", "h2o"};                            \
    std::vector<size_t> islocal = {1, 1, 1, 1};                                                                       \
    std::vector<size_t> n_atoms_vector = {3, 3, 3, 3};                                                                \
    std::vector<size_t> n_sites_vector = {3, 3, 4, 4};                                                                \
    std::vector<size_t> first_index = {0, 3, 6, 10};                                                                  \
    std::vector<size_t> first_index_realSites = {0, 3, 6, 9};                                                         \
    const size_t n_atoms = 12;                                                                                        \
    const size_t n_sites = 14;                                                                                        \
    std::vector<double> polfac{                                                                                       \
        1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01, 1.4716770000e+00, 7.6979000000e-01,                     \
        7.6979000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,                     \
        1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};                                      \
    std::vector<double> pol{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01, 1.4716770000e+00, 7.6979000000e-01, \
                            7.6979000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, \
                            1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00};                  \
    std::vector<double> charges{7.0602700000e-01,  -3.5301350000e-01, -3.5301350000e-01, 7.0602700000e-01,            \
                                -3.5301350000e-01, -3.5301350000e-01, 0.0000000000e+00,  5.7199974482e-01,            \
                                5.7203329069e-01,  -1.1440330355e+00, 0.0000000000e+00,  5.7636979828e-01,            \
                                5.7479892955e-01,  -1.1511687278e+00};                                                \
    std::vector<double> real_polfac{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01, 1.4716770000e+00,           \
                                    7.6979000000e-01, 7.6979000000e-01, 1.3100000000e+00, 2.9400000000e-01,           \
                                    2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};          \
    std::vector<double> real_pol{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01, 1.4716770000e+00,              \
                                 7.6979000000e-01, 7.6979000000e-01, 1.3100000000e+00, 2.9400000000e-01,              \
                                 2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};             \
    std::vector<double> real_charges{7.0602700000e-01,  -3.5301350000e-01, -3.5301350000e-01, 7.0602700000e-01,       \
                                     -3.5301350000e-01, -3.5301350000e-01, 0.0000000000e+00,  5.7199974482e-01,       \
                                     5.7203329069e-01,  0.0000000000e+00,  5.7636979828e-01,  5.7479892955e-01};      \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"co2_archive", 2}, {"h2o", 2}};              \
    std::vector<size_t> internal_original_to_current_order{0, 1, 2, 3};                                               \
    std::vector<std::pair<size_t, size_t>> internal_original_order{{0, 0}, {1, 3}, {2, 6}, {3, 10}};                  \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 3}, {2, 6}, {3, 9}};         \
    std::vector<double> C6_long_range{1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01,         \
                                      1.3042057313e+01, 1.3042057313e+01, 1.5405233572e+01, 4.4825869765e+00,         \
                                      4.4825869765e+00, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00};        \
    std::vector<std::string> internal_monomer_names{"co2_archive", "co2_archive", "h2o", "h2o"};                      \
    std::vector<size_t> mon_idxs_at;                                                                                  \
    std::vector<std::string> mon_id_at;
#endif

/* NRG file
SYSTEM NRG
MOLECULE
MONOMER co2
C -0.3476178881 0.2237313366 -0.0099154063
O 0.070932878 -0.6519721642 0.6471764108
O -0.778614474 1.0711451166 -0.6879403924
ENDMON
ENDMOL
MOLECULE
MONOMER co2
C 2.0135383728 2.45638142 -0.9084926918
O 2.4173378894 1.3610844076 -0.7839983735
O 1.631013652 3.5458287814 -1.0710246849
ENDMON
ENDMOL
MOLECULE
MONOMER h2o
O 2.7444571499 -0.0288953975 2.0146795423
H 2.0559286005 -0.6690597132 1.7989680726
H 3.160706331 0.144974533 1.1623613238
ENDMON
ENDMOL
MOLECULE
MONOMER h2o
O 0.9925968546 2.1439525102 1.6639051316
H 0.8050178487 2.7574021582 2.3791378598
H 1.620302541 1.4960194683 2.0291893372
ENDMON
ENDMOL
ENDSYS
*/

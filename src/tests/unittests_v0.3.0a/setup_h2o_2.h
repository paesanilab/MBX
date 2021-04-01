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

#ifndef UNITTESTS_SETUP_H2O_2_H
#define UNITTESTS_SETUP_H2O_2_H

#define SETUP_H2O_2                                                                                 \
    const int n_monomers = 2;                                                                       \
    const int n_atoms = 8;                                                                          \
    std::vector<double> chg_grad(27 * n_monomers, 0.0);                                             \
    std::vector<double> polfac(n_atoms, 1e-7);                                                      \
    std::vector<double> pol(n_atoms, 1e-7);                                                         \
    std::vector<std::string> monomer_names(n_monomers, "h2o");                                      \
    std::vector<size_t> islocal(n_monomers, 1);                                                     \
    std::vector<int> sys_at_tag(n_atoms, 0);                                                        \
    std::vector<double> charges(n_atoms, 0);                                                        \
    for (int n = 0; n < n_monomers; ++n) {                                                          \
        charges[4 * n + 0] = qO;                                                                    \
        charges[4 * n + 1] = qH;                                                                    \
        charges[4 * n + 2] = qH;                                                                    \
        charges[4 * n + 3] = qM;                                                                    \
        polfac[4 * n + 0] = pol[4 * n + 0] = polfacO;                                               \
        polfac[4 * n + 1] = pol[4 * n + 1] = polfacH;                                               \
        polfac[4 * n + 2] = pol[4 * n + 2] = polfacH;                                               \
        polfac[4 * n + 3] = pol[4 * n + 3] = polfacM;                                               \
    }                                                                                               \
    std::vector<size_t> first_ind(n_monomers, 0);                                                   \
    for (int n = 1; n < n_monomers; ++n) {                                                          \
        first_ind[n] = 4 * n;                                                                       \
    }                                                                                               \
    std::vector<size_t> sites(n_monomers, 4);                                                       \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                \
    std::vector<double> coords = std::vector<double>{                                               \
        -11.5109336618, 1.0563960610,  6.6033301068,  -11.7795660388, 0.8781403410,  5.7020569050,  \
        -12.2938264730, 0.8826090481,  7.1259281936,  -11.6417034996, 1.0063049360,  6.5256614324,  \
        -11.5501833807, -1.5847863244, -1.5602148182, -11.1133062356, -0.8169061174, -1.9286305994, \
        -11.2385781621, -2.3166732236, -2.0926390799, -11.4605158991, -1.5472825089, -1.6602317315};
#endif

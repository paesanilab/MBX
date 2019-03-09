#ifndef UNITTESTS_SETUP_XE_2_H
#define UNITTESTS_SETUP_XE_2_H

#define SETUP_XE_2                                                                             \
    const size_t n_monomers = 2;                                                                              \
    const size_t n_atoms = 2;                                                                                 \
    const size_t n_sites = 2;                                                                                \
    std::vector<double> chg_grad(n_sites * 3,0.0);                                                                          \
    std::vector<double> polfac(n_sites, 0.0);                                                              \
    std::vector<double> pol(n_sites, 0.0582251502186);                                                                 \
    std::vector<std::string> monomer_names(n_monomers, "xe");                                             \
    std::vector<double> charges(n_sites, 0.0); \
    std::vector<size_t> first_ind(n_monomers, 0);                                                          \
    for (size_t n = 1; n < n_monomers; ++n) {                                                                 \
        first_ind[n] = n;                                                                              \
    }                                                                                                      \
    std::vector<size_t> sites(n_monomers, 1);                                                              \
    std::vector<size_t> nats(n_monomers, 1);                                                               \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"xe", n_monomers}};                       \
    std::vector<double> coords{ \
0.653513132613, 0.818856259835, 0.174820633501 ,    \
0.0157243094599 , 1.42425312123 , 0.816185274262};
#endif

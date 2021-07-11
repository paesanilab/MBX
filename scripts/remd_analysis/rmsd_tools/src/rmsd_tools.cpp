#include "rmsd_tools.h"

namespace rmsd_tools {

std::vector<double> GetMasses(std::vector<std::string> atoms) {
    mass_map_type m;

    m.insert(std::make_pair("H", 1.00782503207));
    m.insert(std::make_pair("O", 15.99491461956));
    m.insert(std::make_pair("F", 18.998403162));
    m.insert(std::make_pair("Cl", 34.968852682));
    m.insert(std::make_pair("Br", 78.9183376));
    m.insert(std::make_pair("I", 126.9044719));
    m.insert(std::make_pair("Li", 7.01600343));
    m.insert(std::make_pair("Na", 22.9897692));
    m.insert(std::make_pair("K", 38.96370648));
    m.insert(std::make_pair("Rb", 84.91149772));
    m.insert(std::make_pair("Cs", 132.905429));
    m.insert(std::make_pair("C", 12.00000000));
    m.insert(std::make_pair("N", 14.00307400443));

    std::vector<double> masses(atoms.size(), 1.0);
    for (size_t i = 0; i < atoms.size(); i++) {
        mass_map_type::const_iterator it = m.find(atoms[i]);
        if (it == m.end()) {
            masses[i] = 50.0;
            std::cerr << "**WARNING** Mass for atom " << atoms[i]
                      << " is not in the database. Please add it. Useing default of 50.0 for now.\n";
        } else {
            masses[i] = it->second;
        }
    }

    return masses;
}

std::vector<std::vector<size_t> > ReadPermutations(size_t num_ats, std::string perm_file) {
    std::vector<std::vector<size_t> > perms;

    std::ifstream iff(perm_file.c_str());
    if (!iff.good()) {
        std::vector<size_t> p(num_ats);
        for (size_t i = 0; i < num_ats; i++) p[i] = i;
        perms.push_back(p);
        return perms;
    }

    while (!iff.eof()) {
        std::vector<size_t> p(num_ats);
        std::string line;
        std::getline(iff, line);
        if (iff.eof()) break;
        std::stringstream ss(line);
        for (size_t i = 0; i < num_ats; i++) {
            ss >> p[i];
        }

        perms.push_back(p);
    }

    return perms;
}

void GetRelevantCoordinates(std::vector<size_t> ignored_indexes, std::vector<double> xyz,
                            std::vector<double> &new_xyz) {
    size_t num_ats = xyz.size() / 3;
    new_xyz = std::vector<double>(3 * (num_ats - ignored_indexes.size()));

    // Get the relevant coordinates
    size_t ic = 0;
    for (size_t i = 0; i < num_ats; i++) {
        if (std::find(ignored_indexes.begin(), ignored_indexes.end(), i) == ignored_indexes.end()) {
            for (size_t j = 0; j < 3; j++) {
                new_xyz[3 * ic + j] = xyz[3 * i + j];
            }
            ic++;
        }
    }
}

void GetRelevantMassesAndAtoms(std::vector<size_t> ignored_indexes, std::vector<double> masses,
                               std::vector<std::string> atoms, std::vector<double> &new_masses,
                               std::vector<std::string> &new_atoms) {
    size_t num_ats = atoms.size();
    new_atoms = std::vector<std::string>(num_ats - ignored_indexes.size());
    new_masses = std::vector<double>(num_ats - ignored_indexes.size());

    double total_mass = 0.0;

    size_t ic = 0;
    for (size_t i = 0; i < num_ats; i++) {
        if (std::find(ignored_indexes.begin(), ignored_indexes.end(), i) == ignored_indexes.end()) {
            new_masses[ic] = masses[i];
            total_mass += masses[i];
            new_atoms[ic] = atoms[i];
            ic++;
        }
    }

    for (size_t i = 0; i < ic; i++) {
        new_masses[i] /= total_mass;
    }
}

double CalculateRmsd(std::vector<double> xyz1, std::vector<double> xyz2, std::vector<std::vector<size_t> > permutations,
                     std::vector<double> mass, bool use_inversion, double threshold) {
    // Get the CoM
    std::vector<double> com1(3, 0.0);
    std::vector<double> com2(3, 0.0);

    double total_mass = 0.0;

    size_t nat = mass.size();
    for (size_t i = 0; i < nat; i++) {
        total_mass += mass[i];
        for (size_t j = 0; j < 3; j++) {
            com1[j] += mass[i] * xyz1[3 * i + j];
            com2[j] += mass[i] * xyz2[3 * i + j];
        }
    }

    // Masses are already scaled. No need to divide again
    // for (size_t i = 0; i < 3; i++) {
    //    com1[i] /= total_mass;
    //    com2[i] /= total_mass;
    //}

    // CoM won't be affected by permutations
    std::vector<double> y1(3 * nat, 0.0);
    std::vector<double> y2(3 * nat, 0.0);
    double y1n = 0.0;
    double y2n = 0.0;

    for (size_t i = 0; i < nat; i++) {
        for (size_t j = 0; j < 3; j++) {
            const size_t k = 3 * i + j;

            y1[k] = xyz1[k] - com1[j];
            y1n += mass[i] * y1[k] * y1[k];
            y2[k] = xyz2[k] - com2[j];
            y2n += mass[i] * y2[k] * y2[k];
        }
    }

    double result = std::numeric_limits<double>::infinity();

    // Go over the permutations
    for (size_t i = 0; i < permutations.size(); i++) {
        std::vector<double> z2(3 * nat);
        for (size_t j = 0; j < nat; j++) {
            size_t s = 3 * permutations[i][j];
            std::copy(y2.begin() + s, y2.begin() + s + 3, z2.begin() + 3 * j);
        }

        // Compute rms y1 -- z2
        double lambda(0.0), q[4];
        rmsd_q(nat, mass.data(), y1.data(), z2.data(), lambda, q);

        // Masses were scaled (m/mtot) so no need to divide by total_mass
        double rmsd = std::sqrt(std::max(0.0, (y1n + y2n) - 2 * lambda));
        // double rmsd = std::sqrt(std::max(0.0, ((y1n + y2n) - 2 * lambda)) / total_mass);

        result = std::min(rmsd, result);
        if (result < threshold) return result;

        // If inversion is activated, also do it
        if (use_inversion) {
            for (size_t j = 0; j < 3 * nat; j++) {
                z2[j] = -z2[j];
            }

            double lambda2, q2[4];
            rmsd_q(nat, mass.data(), y1.data(), z2.data(), lambda2, q2);

            double rmsd2 = std::sqrt(std::max(0.0, (y1n + y2n) - 2 * lambda2));

            result = std::min(rmsd2, result);
        }

        // If rmsd is lower than trheshold, no need to proceed with permutations
        if (result < threshold) return result;
    }

    return result;
}
}  // namespace rmsd_tools

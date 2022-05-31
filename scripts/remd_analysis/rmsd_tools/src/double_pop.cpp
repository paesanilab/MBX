
#include <iostream>

#include "bblock/system.h"
#include "io_tools/read_nrg.h"
#include "io_tools/read_xyz.h"

#include "json/json.h"
#include "json_tools.h"

#include "rmsd_tools.h"

int main(int argc, char** argv) {
    if (argc < 7) {
        std::cout << "Usage: " << argv[0]
                  << " <config_all.nrg> <configs_all.xyz> <configs_ref_big.xyz> <config_ref.nrg> <configs_ref.xyz> "
                     "<output_prefix> [<remd.json>]\n";
        return 0;
    }

    // Declare the system to be read from nrg
    std::vector<bblock::System> s, sr;

    // Declare atoms, coords and boxes to be read from xyz
    std::vector<std::vector<std::string> > ats, atsref, atsref2;
    std::vector<std::vector<double> > coords, coordsref, coordsref2;
    std::vector<std::vector<double> > boxes, boxesref, boxesref2;

    nlohmann::json j, j_default;

    try {
        // Read the NRG file
        tools::ReadNrg(argv[1], s);
        tools::ReadNrg(argv[4], sr);

        // Read the trakectory
        tools::ReadXYZ(argv[2], ats, coords, boxes);
        tools::ReadXYZ(argv[3], atsref2, coordsref2, boxesref2);
        tools::ReadXYZ(argv[5], atsref, coordsref, boxesref);

        // If json file is passed, use json settings. Use defaults otherwise.
        if (argc > 7) {
            j = rmsd_tools::SetUpFromJson(argv[7]);
        } else {
            j = rmsd_tools::SetUpFromJson();
        }
        j_default = rmsd_tools::SetUpRscFromJson();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Define the settings
    // Atoms to ignore will be defaulted to an empty list
    std::vector<size_t> indexes_to_ignore;
    try {
        std::vector<size_t> indexes_to_ignore2 = j["indexes_to_ignore"];
        indexes_to_ignore = indexes_to_ignore2;
    } catch (...) {
        std::cerr << "**WARNING** \"indexes_to_ignore\" is not defined in json file. Using empty list.\n";
        std::vector<size_t> indexes_to_ignore2 = j_default["indexes_to_ignore"];
        indexes_to_ignore = indexes_to_ignore2;
    }

    // Atoms to ignore will be defaulted to an empty list
    std::vector<size_t> indexes_to_ignore_ref;
    try {
        std::vector<size_t> indexes_to_ignore2 = j["indexes_to_ignore_ref"];
        indexes_to_ignore_ref = indexes_to_ignore2;
    } catch (...) {
        std::cerr << "**WARNING** \"indexes_to_ignore\" is not defined in json file. Using empty list.\n";
        std::vector<size_t> indexes_to_ignore2 = j_default["indexes_to_ignore_ref"];
        indexes_to_ignore_ref = indexes_to_ignore2;
    }

    // Threshold will be defaulted to 0.01
    double threshold;
    try {
        threshold = j["threshold"];
    } catch (...) {
        std::cerr << "**WARNING** \"threshold\" is not defined in json file. Using 0.01 Angstrom.\n";
        threshold = j_default["threshold"];
    }

    std::string perm;
    try {
        perm = j["permutation_file"];
    } catch (...) {
        std::cerr << "**WARNING** \"permutation_file\" is not defined in json file. Using empty file.\n";
        perm = j_default["permutation_file"];
    }

    bool use_inversion;
    try {
        use_inversion = j["use_inversion"];
    } catch (...) {
        std::cerr << "**WARNING** \"use_inversion\" is not defined in json file. Using true.\n";
        use_inversion = j_default["use_inversion"];
    }

    std::string out_pref(argv[6]);

    // Set up the masses. Since all the frames are assumed to be the same system,
    // look only at the first one.
    std::vector<double> all_masses = rmsd_tools::GetMasses(ats[0]);
    std::vector<double> all_massesref = rmsd_tools::GetMasses(atsref[0]);
    std::vector<double> all_massesref2 = rmsd_tools::GetMasses(atsref2[0]);

    // Find numer of atoms
    size_t num_ats = ats[0].size();
    size_t num_ats_ref = atsref[0].size();
    size_t num_ats_ref2 = atsref2[0].size();

    // Get the relevant masses and atoms (i.e. the ones not ignored)
    // Masses will be scaled (mass(i)/Mass(total)
    std::vector<double> masses;
    std::vector<double> massesref;
    std::vector<double> massesref2;
    std::vector<std::string> atoms;
    std::vector<std::string> atomsref;
    std::vector<std::string> atomsref2;
    rmsd_tools::GetRelevantMassesAndAtoms(indexes_to_ignore, all_masses, ats[0], masses, atoms);
    rmsd_tools::GetRelevantMassesAndAtoms(indexes_to_ignore_ref, all_massesref, atsref[0], massesref, atomsref);
    rmsd_tools::GetRelevantMassesAndAtoms(indexes_to_ignore, all_massesref2, atsref2[0], massesref2, atomsref2);

    // Check that ref and all configs have the same number of atoms. Otherwise something is wrong:
    if (atoms.size() != atomsref.size()) {
        std::cerr << "Something is wrong. Reference and All number of atoms do not match (" << atomsref.size() << " vs "
                  << atoms.size() << ")" << std::endl;
        return 1;
    }

    if (atoms.size() != atomsref2.size()) {
        std::cerr << "Something is wrong. Reference2 and All number of atoms do not match (" << atomsref2.size()
                  << " vs " << atoms.size() << ")" << std::endl;
        return 1;
    }

    // Check that the atoms are the same in the same order:
    for (size_t i = 0; i < atoms.size(); i++) {
        if (atoms[i] != atomsref[i]) {
            std::cerr << "Something is wrong. Atom order between ref and all does not match for index " << i << ": ("
                      << atomsref[i] << " vs " << atoms[i] << ")" << std::endl;
            return 1;
        }
    }

    for (size_t i = 0; i < atoms.size(); i++) {
        if (atoms[i] != atomsref2[i]) {
            std::cerr << "Something is wrong. Atom order between ref2 and all does not match for index " << i << ": ("
                      << atomsref2[i] << " vs " << atoms[i] << ")" << std::endl;
            return 1;
        }
    }

    // Get the permutations from the file specified in json
    std::vector<std::vector<size_t> > perms = rmsd_tools::ReadPermutations(ats[0].size(), perm);

    // Get the configuration coordinates that we don't ignore
    std::vector<std::vector<double> > xyz(coords.size(), std::vector<double>(3 * atoms.size()));
    for (size_t i = 0; i < coords.size(); i++) {
        rmsd_tools::GetRelevantCoordinates(indexes_to_ignore, coords[i], xyz[i]);
    }

    std::vector<std::vector<double> > xyzref2(coordsref2.size(), std::vector<double>(3 * atomsref2.size()));
    for (size_t i = 0; i < coordsref2.size(); i++) {
        rmsd_tools::GetRelevantCoordinates(indexes_to_ignore, coordsref2[i], xyzref2[i]);
    }

    std::vector<std::vector<double> > xyzref(coordsref.size(), std::vector<double>(3 * atomsref.size()));
    for (size_t i = 0; i < coordsref.size(); i++) {
        rmsd_tools::GetRelevantCoordinates(indexes_to_ignore_ref, coordsref[i], xyzref[i]);
    }

    size_t nref = xyzref.size();

    // At this point, the unique configurations from the main traj are in xyzref2
    // Get the number of unique configurations:
    size_t nunique = xyzref2.size();

    // Create a vector that will contain which traj unique configs are equivalent to the references
    std::vector<std::vector<size_t> > ref2unique(nref);

    // Fill that vector. Each element is a vector with indexes of which configs of traj are eq to ref
    for (size_t i = 0; i < xyzref2.size(); i++) {
        double min_rmsd = 1.0E10;
        size_t min_index = 0;
        for (size_t j = 0; j < xyzref.size(); j++) {
            double rmsd = rmsd_tools::CalculateRmsd(xyzref2[i], xyzref[j], perms, masses, use_inversion, threshold);
            if (rmsd < min_rmsd) {
                min_rmsd = rmsd;
                min_index = j;
            }
        }
        ref2unique[min_index].push_back(i);
    }

    // Now the same but for the traj and unique traj configs
    std::vector<std::vector<size_t> > unique2traj(nunique);

    for (size_t i = 0; i < xyz.size(); i++) {
        double min_rmsd = 1.0E10;
        size_t min_index = 0;
        for (size_t j = 0; j < xyzref2.size(); j++) {
            double rmsd = rmsd_tools::CalculateRmsd(xyz[i], xyzref2[j], perms, masses, use_inversion, threshold);
            if (rmsd < min_rmsd) {
                min_rmsd = rmsd;
                min_index = j;
            }
        }
        unique2traj[min_index].push_back(i);
    }

    // Now find the populations of the traj with respect to the unique isomers
    std::vector<double> pops_unique(nunique, 0);

    for (size_t i = 0; i < unique2traj.size(); i++) {
        pops_unique[i] = unique2traj[i].size();
    }

    // And now, the populations of the reference
    std::vector<double> pops(nref, 0);
    size_t tot_samples = xyz.size();
    for (size_t i = 0; i < nref; i++) {
        for (size_t j = 0; j < ref2unique[i].size(); j++) {
            pops[i] += pops_unique[ref2unique[i][j]];
        }
    }

    // Make pops relative to 1
    for (size_t i = 0; i < pops_unique.size(); i++) pops_unique[i] /= double(tot_samples);
    for (size_t i = 0; i < pops.size(); i++) pops[i] /= double(tot_samples);

    // Print Output files

    // First, the relations between isomers
    std::string fname = out_pref + std::string("_isomer_equivalence.dat");
    std::ofstream off(fname);
    for (size_t i = 0; i < ref2unique.size(); i++) {
        for (size_t j = 0; j < ref2unique[i].size(); j++) {
            off << std::setw(5) << ref2unique[i][j];
        }
        off << std::endl;
    }
    off.close();
    off.clear();

    // Write pops with respect to the reference and the traj reference
    fname = out_pref + std::string("_pops_ref.dat");
    off.open(fname);
    for (size_t i = 0; i < pops.size(); i++) {
        off << std::setw(15) << std::scientific << std::setprecision(6) << pops[i];
    }
    off << std::endl;
    off.close();
    off.clear();

    fname = out_pref + std::string("_pops_ref2.dat");
    off.open(fname);
    for (size_t i = 0; i < pops_unique.size(); i++) {
        off << std::setw(15) << std::scientific << std::setprecision(6) << pops_unique[i];
    }
    off << std::endl;
    off.close();
    off.clear();

    // Write xyz files with the isomer equivalences
    for (size_t i = 0; i < ref2unique.size(); i++) {
        std::stringstream ss;
        ss << out_pref << "_isomers_equal_to_ref_isomer_" << i << +".xyz";
        fname = ss.str();
        off.open(fname);
        for (size_t jj = 0; jj < ref2unique[i].size(); jj++) {
            size_t j = ref2unique[i][jj];
            off << atsref2[0].size() << std::endl;
            off << j << std::endl;
            for (size_t k = 0; k < atsref2[0].size(); k++) {
                off << std::scientific << std::setprecision(8) << std::setw(10) << atsref2[0][k] << std::setw(20)
                    << coordsref2[j][3 * k + 0] << std::setw(20) << coordsref2[j][3 * k + 1] << std::setw(20)
                    << coordsref2[j][3 * k + 2] << std::endl;
            }
        }
        off.close();
        off.clear();
    }

    return 0;
}


#include <iostream>

#include "bblock/system.h"
#include "io_tools/read_nrg.h"
#include "io_tools/read_xyz.h"

#include "json/json.h"
#include "json_tools.h"

#include "rmsd_tools.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <config.nrg> <configs.xyz> [<rsc.json>]\n";
        return 0;
    }

    // Declare the system to be read from nrg
    std::vector<bblock::System> s;

    // Declare atoms, coords and boxes to be read from xyz
    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;

    nlohmann::json j, j_default;

    try {
        // Read the NRG file
        tools::ReadNrg(argv[1], s);

        // Read the trakectory
        tools::ReadXYZ(argv[2], ats, coords, boxes);

        // If json file is passed, use json settings. Use defaults otherwise.
        if (argc > 3) {
            j = rmsd_tools::SetUpRscFromJson(argv[3]);
        } else {
            j = rmsd_tools::SetUpRscFromJson();
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

    std::string out_file;
    try {
        out_file = j["output_file"];
    } catch (...) {
        std::cerr << "**WARNING** \"output_file\" is not defined in json file. Using rsc.xyz.\n";
        out_file = j_default["output_file"];
    }

    // Set up the masses. Since all the frames are assumed to be the same system,
    // look only at the first one.
    std::vector<double> all_masses = rmsd_tools::GetMasses(ats[0]);

    // Find numer of atoms
    size_t num_ats = ats[0].size();

    // Get the relevant masses and atoms (i.e. the ones not ignored)
    // Masses will be scaled (mass(i)/Mass(total)
    std::vector<double> masses;
    std::vector<std::string> atoms;
    rmsd_tools::GetRelevantMassesAndAtoms(indexes_to_ignore, all_masses, ats[0], masses, atoms);

    // Get the permutations from the file specified in json
    std::vector<std::vector<size_t> > perms = rmsd_tools::ReadPermutations(ats[0].size(), perm);

    // Get the configuration coordinates that we don't ignore
    std::vector<std::vector<double> > xyz(coords.size(), std::vector<double>(3 * atoms.size()));
    for (size_t i = 0; i < coords.size(); i++) {
        rmsd_tools::GetRelevantCoordinates(indexes_to_ignore, coords[i], xyz[i]);
    }

    // Loop over the configurations and delete the duplicates
    for (size_t i = 0; i < xyz.size() - 1; i++) {
        if (xyz[i].size() < 1) continue;
        for (size_t j = i + 1; j < xyz.size(); j++) {
            if (xyz[j].size() < 1) continue;
            double rmsd = rmsd_tools::CalculateRmsd(xyz[i], xyz[j], perms, masses, use_inversion, threshold);
            if (rmsd < threshold) {
                xyz[j].clear();
                std::cout << "Removed configuration " << j << ", equivalent to " << i << "; rms = " << rmsd
                          << std::endl;
            }
        }
    }

    std::ofstream off(out_file.c_str());
    size_t count = 0;
    for (size_t i = 0; i < xyz.size(); i++) {
        if (xyz[i].size() > 0) {
            off << ats[0].size() << std::endl;
            off << count << std::endl;
            for (size_t j = 0; j < num_ats; j++) {
                off << std::scientific << std::setprecision(8) << std::setw(10) << ats[0][j] << std::setw(20)
                    << coords[i][3 * j + 0] << std::setw(20) << coords[i][3 * j + 1] << std::setw(20)
                    << coords[i][3 * j + 2] << std::endl;
            }
            count++;
        }
    }

    off.close();

    return 0;
}

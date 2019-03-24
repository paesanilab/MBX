#include "read_xyz.h"

void ReadXyz(std::ifstream &ifs, std::vector<double> &xyz, std::vector<std::string> &atoms) {
    std::string line;
    std::stringstream ss;

    // Read number of atoms
    std::getline(ifs,line);

    // Return if file is empty
    if (ifs.eof()) return;
    ss.str(line);
    size_t nat;
    ss >> nat;
    std::getline(ifs,line);

    // Allocate the vectors
    xyz = std::vector<double>(3*nat,0.0);
    atoms = std::vector<std::string>(nat);

    // Read coordinates and atoms
    for (size_t i = 0; i < nat; i++) {
        std::getline(ifs,line);
        ss.clear();
        ss.str(line);
        ss >> atoms[i] >> xyz[3*i] >> xyz[3*i + 1] >> xyz[3*i + 2];
    }
}

void ReadXyzTrajectory(char* filename, std::vector<std::vector<double> > &xyz, std::vector<std::vector<std::string> > &atoms) {
    // Reset vectors
    xyz.clear();
    atoms.clear();
    // open file
    std::ifstream ifs(filename);
    while (true) {
        std::vector<double> frame_xyz;
        std::vector<std::string> frame_ats;
        // Read single frame
        ReadXyz(ifs,frame_xyz,frame_ats);
        if (ifs.eof()) break;

        // Update vectors
        xyz.push_back(frame_xyz);
        atoms.push_back(frame_ats);

    }

    ifs.close();
}

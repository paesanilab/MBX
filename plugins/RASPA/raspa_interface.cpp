
#include "raspa_interface.h"

MBXMonomerInfo raspa_interface::getMBXMonomerInfo(std::string first_atom_type) {
    if(first_atom_type == "Ow") {
        return MBXMonomerInfo(3, "h2o");
    }
}

float raspa_interface::energy(
            const SimulationBox &simulationBox,
            std::span<const Atom> moleculeAtoms,
            std::span<const Atom> frameworkAtoms
) {

    // This should get passed to MBX from RASPA, but for now we hardcode it.
    std::string json_path = "path/to/mbx.json";

    /***
     * SECTION 1: Setup MBX System object.
    ***/

    bblock::System *mbx = new bblock::System();

    int cumulativeTagIndex = 0;

    for(int i = 0; i < moleculeAtoms.size(); i++) {

        MBXMonomerInfo monomerInfo = getMBXMonomerInfo(moleculeAtoms[i].type);

        int numAtoms = monomerInfo.numAtoms;

        std::vector<double> atomCoordinates(numAtoms);
        std::vector<std::string> atomNames(numAtoms);
        std::string molName = monomerInfo.monomerName;

        for(int j = 0; j < numAtoms; j++) {
            atomCoordinates[j * 3] = moleculeAtoms[i + j].position.x;
            atomCoordinates[j * 3 + 1] = moleculeAtoms[i + j].position.y;
            atomCoordinates[j * 3 + 2] = moleculeAtoms[i + j].position.z;

            atomNames[j] = moleculeAtoms[i + j].type;
        }

        // This decides weather this guest molecule is a real molecule or a ghost molecule
        // (such as an image of a molecule from a neighboring subdomain)
        // Since we use only one subdomain, we set this to true
        bool islocal = true;

        bool tagIndex = cumulativeTagIndex;

        mbx->AddMonomer(atomCoordinates, atomNames, molName, islocal, tagIndex);

        cumulativeTagIndex += numAtoms;

        i += numAtoms - 1;
    }

    std::vector<double> frameworkCoords(frameworkAtoms.size() * 3);
    std::vector<double> frameworkCharges(frameworkAtoms.size());
    std::vector<size_t> frameworkIsLocals(frameworkAtoms.size(), true);
    std::vector<int> frameworkTags(frameworkAtoms.size());

    for (int i = 0; i < frameworkMolecules.size(); i++) {
        const Atom &atom = frameworkAtoms[i];

        frameworkCoords[i * 3] = atom.position.x;
        frameworkCoords[i * 3 + 1] = atom.position.y;
        frameworkCoords[i * 3 + 2] = atom.position.z;

        frameworkCharges[i] = atom.charge;
        frameworkTags[i] = cumulativeTagIndex;

        cumulativeTagIndex += 1;
    }
    
    mbx->SetExternalChargesAndPositions(chg_ext, xyz_ext, islocal_ext, tag_ext);

    std::vector<double> box(9, 0.0);

    // Box x vector
    box[0] = simulationBox.cell.m11;
    box[1] = simulationBox.cell.m12;
    box[2] = simulationBox.cell.m13;

    // Box y vector
    box[3] = simulationBox.cell.m21;
    box[4] = simulationBox.cell.m22;
    box[5] = simulationBox.cell.m23;

    // Box z vector
    box[6] = simulationBox.cell.m31;
    box[7] = simulationBox.cell.m32;
    box[8] = simulationBox.cell.m33;
    
    mbx->SetPBC(box);

    mbx->SetUpFromJson(json_path);

    /***
     * SECTION 2: Calculate MBX energy.
    ***/

    double energy =  mbx->Energy(false); // false means don't calculate forces

    /***
     * SECTION 3: Destroy MBX System object.
    ***/

    delete mbx;

    return energy;
}
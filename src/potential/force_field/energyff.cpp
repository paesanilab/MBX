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

#include "energyff.h"

// #define DEBUG

namespace eff {

double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, bool &good, int nat,
                     std::vector<double> box, std::vector<double> box_inv) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Coords:\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "nmon: " << nm << std::endl;
    std::cerr << "good: " << good << std::endl;
    std::cerr << "nat: " << nat << std::endl;
#endif

    std::vector<eff::Bond> bonds = connectivity.GetBondVec();
    std::vector<eff::Angles> angles = connectivity.GetAnglesVec();
    std::vector<eff::Dihedral> dihedrals = connectivity.GetDihedralVec();
    std::vector<eff::Inversion> inversions = connectivity.GetInversionVec();
    double bondEnergy = 0;
    double angleEnergy = 0;
    double dihedralEnergy = 0;
    double inversionEnergy = 0;

    bool use_pbc = box.size();

    for (int mon_num = 0; mon_num < nm; mon_num++) {
        std::vector<double> xyz(3 * nat, 0.0);
        std::copy(xyz1.begin() + 3 * nat * mon_num, xyz1.begin() + 3 * nat * (mon_num + 1), xyz.begin());
        double mon_E = 0.0;
        for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
            // obtain the indexes for the specific bond we are looking at
            std::vector<size_t> indexes = bond->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
            }

            double distance = CalculateDistance(coor1, coor2);

            bondEnergy += bond->GetEnergy(distance);
            mon_E += bond->GetEnergy(distance);
        }

        for (auto angle = angles.begin(); angle != angles.end(); angle++) {
            std::vector<size_t> indexes = angle->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
                tools::GetClosePoint(coor1, coor3, box, box_inv);
            }

            // calculate the angle between three coordinates in degrees
            double theta = CalculateAngle(coor1, coor2, coor3);

            angleEnergy += angle->GetEnergy(theta);
            mon_E += angle->GetEnergy(theta);
        }

        for (auto dihedral = dihedrals.begin(); dihedral != dihedrals.end(); dihedral++) {
            std::vector<size_t> indexes = dihedral->GetIndexes();
            // fill coordinates with the same method as bonds (see above)
            // connectivity file specifies the order to fill the coordinates here.
            // the connectivity file MUST have the central atom inbetween the two
            // outer atoms

            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
            std::vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
                tools::GetClosePoint(coor1, coor3, box, box_inv);
                tools::GetClosePoint(coor1, coor4, box, box_inv);
            }

            double phi = CalculateDihedralAngle(coor1, coor2, coor3, coor4);

            dihedralEnergy += dihedral->GetEnergy(phi);
            mon_E += dihedral->GetEnergy(phi);
        }

        for (auto inversion = inversions.begin(); inversion != inversions.end(); inversion++) {
            std::vector<size_t> indexes = inversion->GetIndexes();

            // fill coordinate vectors
            std::vector<double> centralCoor(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
            std::vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(centralCoor, coor2, box, box_inv);
                tools::GetClosePoint(centralCoor, coor3, box, box_inv);
                tools::GetClosePoint(centralCoor, coor4, box, box_inv);
            }

            // calculate the set of inversion angles between 4 coordinates
            std::vector<double> phis = CalculateInversionAngle(centralCoor, coor2, coor3, coor4);

            double currInversionEnergy = 0.0;

            // Accumulate inversion energy over all 3 inversion angles
            for (size_t i = 0; i < 3; i++) {
                currInversionEnergy += inversion->GetEnergy(phis[i]);
            }  // end for loop

            // Line written on 12/12/2019. This line is written to make the inversion
            // energy match dlpoly. It is currently unknown why dlpoly chooses to
            // average its inversion potential energy.
            currInversionEnergy /= 3.0;
            inversionEnergy += currInversionEnergy;
            mon_E += currInversionEnergy;
        }

        if (mon_E > EMAX1B) {
            good = false;
        }
    }

    double tot_E = bondEnergy + angleEnergy + dihedralEnergy + inversionEnergy;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "bond: " << bondEnergy << std::endl;
    std::cerr << "angle: " << angleEnergy << std::endl;
    std::cerr << "dihedral: " << dihedralEnergy << std::endl;
    std::cerr << "inversion: " << inversionEnergy << std::endl;
    std::cerr << "total: " << tot_E << std::endl;
#endif

    return (tot_E);
}

double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     bool &good, int nat, std::vector<double> box, std::vector<double> box_inv,
                     std::vector<double> *virial) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Coords:\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;

    std::cerr << "Grads:\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "Virial:\n";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }

    std::cerr << "nmon: " << nm << std::endl;
    std::cerr << "good: " << good << std::endl;
    std::cerr << "nat: " << nat << std::endl;
#endif

    std::vector<eff::Bond> bonds = connectivity.GetBondVec();
    std::vector<eff::Angles> angles = connectivity.GetAnglesVec();
    std::vector<eff::Dihedral> dihedrals = connectivity.GetDihedralVec();
    std::vector<eff::Inversion> inversions = connectivity.GetInversionVec();
    double bondEnergy = 0;
    double angleEnergy = 0;
    double dihedralEnergy = 0;
    double inversionEnergy = 0;

    bool use_pbc = box.size();

    for (int mon_num = 0; mon_num < nm; mon_num++) {
        std::vector<double> xyz(3 * nat, 0.0);
        std::copy(xyz1.begin() + 3 * nat * mon_num, xyz1.begin() + 3 * nat * (mon_num + 1), xyz.begin());
        double mon_E = 0.0;
        for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
            // obtain the indexes for the specific bond we are looking at
            std::vector<size_t> indexes = bond->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
            }

            double distance = CalculateDistance(coor1, coor2);

            bondEnergy += bond->GetEnergy(distance);
            mon_E += bond->GetEnergy(distance);

            // Get topology gradient. Each bond will contribute a gradient to
            // each of the two atoms.
            double delta_grad = bond->GetTopologyGradient(distance);

            // Get distance gradient to propagate gradient back to the x,y,z
            // dimension for each atom in the topology
            double delta_dist_x;
            double delta_dist_y;
            double delta_dist_z;
            for (int i = 0; i < indexes.size(); i++) {
                // First index
                if (i == 0) {
                    delta_dist_x = (coor1[0] - coor2[0]);
                    delta_dist_y = (coor1[1] - coor2[1]);
                    delta_dist_z = (coor1[2] - coor2[2]);
                }
                // Second index
                else {
                    delta_dist_x = -(coor1[0] - coor2[0]);
                    delta_dist_y = -(coor1[1] - coor2[1]);
                    delta_dist_z = -(coor1[2] - coor2[2]);
                }

                grad1[(indexes[i] - 1) * 3 + (mon_num * nat * 3)] += (delta_dist_x * delta_grad) / distance;
                grad1[(indexes[i] - 1) * 3 + 1 + (mon_num * nat * 3)] += (delta_dist_y * delta_grad) / distance;
                grad1[(indexes[i] - 1) * 3 + 2 + (mon_num * nat * 3)] += (delta_dist_z * delta_grad) / distance;
            }

#ifdef DEBUG
            std::vector<double> bndvirtest(9);

            bndvirtest[0] = -((delta_dist_x * delta_grad) / distance) * delta_dist_x;
            bndvirtest[1] = -((delta_dist_x * delta_grad) / distance) * delta_dist_y;
            bndvirtest[2] = -((delta_dist_x * delta_grad) / distance) * delta_dist_z;

            bndvirtest[4] = -((delta_dist_y * delta_grad) / distance) * delta_dist_y;
            bndvirtest[5] = -((delta_dist_y * delta_grad) / distance) * delta_dist_z;

            bndvirtest[8] = -((delta_dist_z * delta_grad) / distance) * delta_dist_z;

            bndvirtest[3] = bndvirtest[1];
            bndvirtest[6] = bndvirtest[2];
            bndvirtest[7] = bndvirtest[5];

            std::cout << "###########test bnd vir###########" << std::endl;
            for (int teri = 0; teri < 9; teri++) {
                std::cout << bndvirtest[teri] * 418.4 << std::endl;
            }
#endif

            (*virial)[0] -= ((delta_dist_x * delta_grad) / distance) * delta_dist_x;
            (*virial)[1] -= ((delta_dist_x * delta_grad) / distance) * delta_dist_y;
            (*virial)[2] -= ((delta_dist_x * delta_grad) / distance) * delta_dist_z;

            (*virial)[4] -= ((delta_dist_y * delta_grad) / distance) * delta_dist_y;
            (*virial)[5] -= ((delta_dist_y * delta_grad) / distance) * delta_dist_z;

            (*virial)[8] -= ((delta_dist_z * delta_grad) / distance) * delta_dist_z;

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }

        for (auto angle = angles.begin(); angle != angles.end(); angle++) {
            std::vector<size_t> indexes = angle->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
                tools::GetClosePoint(coor1, coor3, box, box_inv);
            }

            // calculate the angle between three coordinates in degrees
            double theta = CalculateAngle(coor1, coor2, coor3);

            angleEnergy += angle->GetEnergy(theta);
            mon_E += angle->GetEnergy(theta);

            // Get topology gradient. Each angle will contribute a gradient to
            // each of the three atoms.
            double delta_grad = angle->GetTopologyGradient(theta);

            double first_term = 1 / std::max(1.0e-8, sin(theta));
            double second_term = delta_grad;

            // Compute the vectors
            // NOTE: IN DLPOLY, atom i is the middle atom. In the documentation of
            // this code, I use j as the middle atom
            std::vector<double> r_ij = {coor1[0] - coor2[0], coor1[1] - coor2[1], coor1[2] - coor2[2]};
            std::vector<double> r_ik = {coor3[0] - coor2[0], coor3[1] - coor2[1], coor3[2] - coor2[2]};
            double dist_r_ij = CalculateDistance(r_ij);
            double dist_r_ik = CalculateDistance(r_ik);

            // Used to store the force for this angle to be used in
            // virial calculation
            std::vector<double> forces(9);
            // Loop through each atom
            for (int index = 0; index < indexes.size(); index++) {
                int delta_lj;
                int delta_li;
                int delta_lk;

                // idx = 0 is atom on the end. Dlpoly defines this to be atom j. This is
                // why here it is defined as lj = 1. The idx loop is looping from end_atom->mid_atom->end_atom
                if (index == 0) {
                    delta_lj = 1;
                    delta_li = 0;
                    delta_lk = 0;
                } else if (index == 1) {
                    delta_lj = 0;
                    delta_li = 1;
                    delta_lk = 0;
                } else {
                    delta_lj = 0;
                    delta_li = 0;
                    delta_lk = 1;
                }

                // Loop through each coordinate within an atom
                for (int dim = 0; dim < 3; dim++) {
                    double third_term = 0;

                    // First term
                    third_term += (delta_lj - delta_li) * (r_ik[dim] / (dist_r_ij * dist_r_ik));

                    // Second term
                    third_term += (delta_lk - delta_li) * (r_ij[dim] / (dist_r_ij * dist_r_ik));

                    // Third term
                    third_term -= cos(theta) * ((delta_lj - delta_li) * (r_ij[dim] / (dist_r_ij * dist_r_ij)) +
                                                (delta_lk - delta_li) * (r_ik[dim] / (dist_r_ik * dist_r_ik)));

                    // Take the negative because we want the gradient not the force (refer
                    // to dl_poly manual)
                    grad1[(indexes[index] - 1) * 3 + dim + (mon_num * nat * 3)] +=
                        -(first_term * second_term * third_term);
                    forces[index * 3 + dim] = (first_term * second_term * third_term);
                }
            }

#ifdef DEBUG
            std::vector<double> anglevirtest(9);

            anglevirtest[0] = r_ij[0] * forces[0] + r_ik[0] * forces[6];
            anglevirtest[1] = r_ij[0] * forces[1] + r_ik[0] * forces[7];
            anglevirtest[2] = r_ij[0] * forces[2] + r_ik[0] * forces[8];

            anglevirtest[4] = r_ij[1] * forces[1] + r_ik[1] * forces[7];
            anglevirtest[5] = r_ij[1] * forces[2] + r_ik[1] * forces[8];

            anglevirtest[8] = r_ij[2] * forces[2] + r_ik[2] * forces[8];

            anglevirtest[3] = anglevirtest[1];
            anglevirtest[6] = anglevirtest[2];
            anglevirtest[7] = anglevirtest[5];

            std::cout << "###########test angle vir###########" << std::endl;
            for (int teri = 0; teri < 9; teri++) {
                std::cout << anglevirtest[teri] * 418.4 << std::endl;
            }
#endif

            (*virial)[0] += r_ij[0] * forces[0] + r_ik[0] * forces[6];
            (*virial)[1] += r_ij[0] * forces[1] + r_ik[0] * forces[7];
            (*virial)[2] += r_ij[0] * forces[2] + r_ik[0] * forces[8];

            (*virial)[4] += r_ij[1] * forces[1] + r_ik[1] * forces[7];
            (*virial)[5] += r_ij[1] * forces[2] + r_ik[1] * forces[8];

            (*virial)[8] += r_ij[2] * forces[2] + r_ik[2] * forces[8];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }

        for (auto dihedral = dihedrals.begin(); dihedral != dihedrals.end(); dihedral++) {
            std::vector<size_t> indexes = dihedral->GetIndexes();
            // fill coordinates with the same method as bonds (see above)
            // connectivity file specifies the order to fill the coordinates here.
            // the connectivity file MUST have the central atom inbetween the two
            // outer atoms

            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
            std::vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(coor1, coor2, box, box_inv);
                tools::GetClosePoint(coor1, coor3, box, box_inv);
                tools::GetClosePoint(coor1, coor4, box, box_inv);
            }

            double phi = CalculateDihedralAngle(coor1, coor2, coor3, coor4);

            dihedralEnergy += dihedral->GetEnergy(phi);
            mon_E += dihedral->GetEnergy(phi);

            // Get topology gradient. Each dihedral angle will contribute a gradient to
            // each of the four atoms.
            double delta_grad = dihedral->GetTopologyGradient(phi);
            double cummu_grad = 0;

            // Multiply by minus 1 because we want gradient not force. Check Dlpoly
            // manual equation 2.42 to see why
            cummu_grad += (-1.0 / sin(phi)) * delta_grad;

            // Used to store the current force for this dihedral to be used in
            // virial calculation
            std::vector<double> curr_force(12);

            // Compute gradient of B for each atom and component and appends the
            // result to the gradient vector
            CalculateGradB(coor1, coor2, coor3, coor4, indexes, cummu_grad, phi, grad1, mon_num, nat, curr_force);

            // set up all of the vectors from the four points
            std::vector<double> fvector = {coor1[0] - coor2[0], coor1[1] - coor2[1],
                                           coor1[2] - coor2[2]};  // This is vector rij
            std::vector<double> svector = {coor2[0] - coor3[0], coor2[1] - coor3[1],
                                           coor2[2] - coor3[2]};  // This is vector rjk
            std::vector<double> tvector = {coor3[0] - coor4[0], coor3[1] - coor4[1],
                                           coor3[2] - coor4[2]};  // This is vector rkn
#ifdef DEBUG
            std::vector<double> dihedralvirtest(9);

            dihedralvirtest[0] =
                fvector[0] * curr_force[0] + svector[0] * (curr_force[3] - curr_force[6]) - tvector[0] * curr_force[9];
            dihedralvirtest[1] =
                fvector[1] * curr_force[0] + svector[1] * (curr_force[3] - curr_force[6]) - tvector[1] * curr_force[9];
            dihedralvirtest[2] =
                fvector[2] * curr_force[0] + svector[2] * (curr_force[3] - curr_force[6]) - tvector[2] * curr_force[9];

            dihedralvirtest[4] =
                fvector[1] * curr_force[1] + svector[1] * (curr_force[4] - curr_force[7]) - tvector[1] * curr_force[10];
            dihedralvirtest[5] =
                fvector[1] * curr_force[2] + svector[1] * (curr_force[5] - curr_force[8]) - tvector[1] * curr_force[11];

            dihedralvirtest[8] =
                fvector[2] * curr_force[2] + svector[2] * (curr_force[5] - curr_force[8]) - tvector[2] * curr_force[11];

            dihedralvirtest[3] = dihedralvirtest[1];
            dihedralvirtest[6] = dihedralvirtest[2];
            dihedralvirtest[7] = dihedralvirtest[5];

            std::cout << "###########test dihedral vir###########" << std::endl;
            for (int teri = 0; teri < 9; teri++) {
                std::cout << dihedralvirtest[teri] * 418.4 << std::endl;
            }
#endif

            (*virial)[0] +=
                fvector[0] * curr_force[0] + svector[0] * (curr_force[3] - curr_force[6]) - tvector[0] * curr_force[9];
            (*virial)[1] +=
                fvector[1] * curr_force[0] + svector[1] * (curr_force[3] - curr_force[6]) - tvector[1] * curr_force[9];
            (*virial)[2] +=
                fvector[2] * curr_force[0] + svector[2] * (curr_force[3] - curr_force[6]) - tvector[2] * curr_force[9];

            (*virial)[4] +=
                fvector[1] * curr_force[1] + svector[1] * (curr_force[4] - curr_force[7]) - tvector[1] * curr_force[10];
            (*virial)[5] +=
                fvector[1] * curr_force[2] + svector[1] * (curr_force[5] - curr_force[8]) - tvector[1] * curr_force[11];

            (*virial)[8] +=
                fvector[2] * curr_force[2] + svector[2] * (curr_force[5] - curr_force[8]) - tvector[2] * curr_force[11];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }

        for (auto inversion = inversions.begin(); inversion != inversions.end(); inversion++) {
            std::vector<size_t> indexes = inversion->GetIndexes();

            // fill coordinate vectors
            std::vector<double> centralCoor(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
            std::vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

            // Put all coordinates in the same region of space
            if (use_pbc) {
                tools::GetClosePoint(centralCoor, coor2, box, box_inv);
                tools::GetClosePoint(centralCoor, coor3, box, box_inv);
                tools::GetClosePoint(centralCoor, coor4, box, box_inv);
            }

            // calculate the set of inversion angles between 4 coordinates
            std::vector<double> phis = CalculateInversionAngle(centralCoor, coor2, coor3, coor4);

            double currInversionEnergy = 0.0;

            // Accumulate inversion energy over all 3 inversion angles
            for (size_t i = 0; i < 3; i++) {
                currInversionEnergy += inversion->GetEnergy(phis[i]);
            }

            // Line written on 12/12/2019. This line is written to make the inversion
            // energy match dlpoly. It is currently unknown why dlpoly chooses to
            // average it inversion potential energy.
            currInversionEnergy /= 3.0;

            inversionEnergy += currInversionEnergy;
            mon_E += currInversionEnergy;

            std::vector<double> temp_shift_vector = phis;

            // Shift the phis vector because the inversion angles are assigned
            // in the incorrect order from dlpoly
            for (int i = 0; i < 3; i++) {
                phis[i] = temp_shift_vector[(i + 1) % 3];
            }

            std::vector<double> cummu_grad = {0.0, 0.0, 0.0};

            // Compute derivative of functional form for each contributing
            // inversion angle
            for (int i = 0; i < 3; i++) {
                double delta_grad = (1.0 / (3.0 * sin(phis[i]))) * inversion->GetTopologyGradient(phis[i]);
                cummu_grad[i] += -1 * delta_grad;
            }

            // Used to store the force for this current inversion angle to be used in
            // virial calculation
            std::vector<double> curr_force(12);

            // Compute remainder of gradient
            CalculateInversionGrad(centralCoor, coor2, coor3, coor4, indexes, cummu_grad, phis, grad1, mon_num, nat,
                                   curr_force);

            // Coordinates used to compute virial
            std::vector<double> rij = {coor2[0] - centralCoor[0], coor2[1] - centralCoor[1], coor2[2] - centralCoor[2]};
            std::vector<double> rik = {coor3[0] - centralCoor[0], coor3[1] - centralCoor[1], coor3[2] - centralCoor[2]};
            std::vector<double> rin = {coor4[0] - centralCoor[0], coor4[1] - centralCoor[1], coor4[2] - centralCoor[2]};

#ifdef DEBUG
            std::vector<double> inversionvirtest(9);
            inversionvirtest[0] = rij[0] * curr_force[3] + rik[0] * curr_force[6] + rin[0] * curr_force[9];
            inversionvirtest[1] = rij[1] * curr_force[3] + rik[1] * curr_force[6] + rin[1] * curr_force[9];
            inversionvirtest[2] = rij[2] * curr_force[3] + rik[2] * curr_force[6] + rin[2] * curr_force[9];
            inversionvirtest[4] = rij[1] * curr_force[4] + rik[1] * curr_force[7] + rin[1] * curr_force[10];
            inversionvirtest[5] = rij[1] * curr_force[5] + rik[1] * curr_force[8] + rin[1] * curr_force[11];
            inversionvirtest[8] = rij[2] * curr_force[5] + rik[2] * curr_force[8] + rin[2] * curr_force[11];
            inversionvirtest[3] = inversionvirtest[1];
            inversionvirtest[6] = inversionvirtest[2];
            inversionvirtest[7] = inversionvirtest[5];

            std::cout << "###########test inversion vir###########" << std::endl;
            for (int teri = 0; teri < 9; teri++) {
                std::cout << inversionvirtest[teri] * 418.4 << std::endl;
            }
#endif

            (*virial)[0] += rij[0] * curr_force[3] + rik[0] * curr_force[6] + rin[0] * curr_force[9];
            (*virial)[1] += rij[1] * curr_force[3] + rik[1] * curr_force[6] + rin[1] * curr_force[9];
            (*virial)[2] += rij[2] * curr_force[3] + rik[2] * curr_force[6] + rin[2] * curr_force[9];
            (*virial)[4] += rij[1] * curr_force[4] + rik[1] * curr_force[7] + rin[1] * curr_force[10];
            (*virial)[5] += rij[1] * curr_force[5] + rik[1] * curr_force[8] + rin[1] * curr_force[11];
            (*virial)[8] += rij[2] * curr_force[5] + rik[2] * curr_force[8] + rin[2] * curr_force[11];
            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }

        if (mon_E > EMAX1B) {
            good = false;
        }
    }

    double tot_E = bondEnergy + angleEnergy + dihedralEnergy + inversionEnergy;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;

    std::cerr << "bond: " << bondEnergy << std::endl;
    std::cerr << "angle: " << angleEnergy << std::endl;
    std::cerr << "dihedral: " << dihedralEnergy << std::endl;
    std::cerr << "inversion: " << inversionEnergy << std::endl;
    std::cerr << "total: " << tot_E << std::endl;

    std::cerr << "Grads:\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;

    if (virial != 0) {
        std::cerr << "Virial:\n";
        for (size_t i = 0; i < (*virial).size(); i++) {
            std::cerr << (*virial)[i] << " , ";
        }
        std::cerr << std::endl;
    }
#endif

    return (tot_E);
}
}  // namespace eff

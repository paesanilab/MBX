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

namespace eff {
// vector<Xyz_frame> frames;
// vector<Bond> bonds;
// vector<Angles> angles;
// vector<Dihedral> dihedrals;
// vector<Inversion> inversions;
// vector<double> charge;
// vector<double> pol;
// vector<double> pol_fac;
// vector<size_t> atom_type;
// size_t total_num_atom_types = 0;
// vector<double> c6;
// vector<double> d6;
// vector<double> a;
// excluded_set_type ex12;
// excluded_set_type ex13;
// excluded_set_type ex14;
// vector<vector<size_t> > non_excluded;
// double bondEnergy = 0;
// double angleEnergy = 0;
// double dihedralEnergy = 0;
// double inversionEnergy = 0;
// vector<double> gradients;

double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, bool &good, int nat) {
    std::vector<eff::Bond> bonds = connectivity.GetBondVec();
    std::vector<eff::Angles> angles = connectivity.GetAnglesVec();
    std::vector<eff::Dihedral> dihedrals = connectivity.GetDihedralVec();
    std::vector<eff::Inversion> inversions = connectivity.GetInversionVec();
    double bondEnergy = 0;
    double angleEnergy = 0;
    double dihedralEnergy = 0;
    double inversionEnergy = 0;

    // for (auto frame = frames.begin(); frame != frames.end(); frame++) {
    for (int mon_num = 0; mon_num < nm; mon_num++) {
        std::vector<double> xyz(3 * nat, 0.0);
        std::copy(xyz1.begin() + 3 * nat * mon_num, xyz1.begin() + 3 * nat * (mon_num + 1), xyz.begin());
        // std::copy() = frame->GetCoor();
        double mon_E = 0.0;
        for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
            // obtain the indexes for the specific bond we are looking at
            std::vector<size_t> indexes = bond->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);

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

            // calculate the set of inversion angles between 4 coordinates
            std::vector<double> phis = CalculateInversionAngle(centralCoor, coor2, coor3, coor4);

            double currInversionEnergy = 0.0;

            // Accumulate inversion energy over all 3 inversion angles
            for (size_t i = 0; i < 3; i++) {
                currInversionEnergy += inversion->GetEnergy(phis[i]);
            }  // end for loop

            // Line written on 12/12/2019. This line is written to make the inversion
            // energy match dlpoly. It is currently unknown why dlpoly chooses to
            // average it inversion potential energy.
            currInversionEnergy /= 3.0;
            inversionEnergy += currInversionEnergy;
            mon_E += currInversionEnergy;
        }

        if (mon_E > EMAX1B) {
            good = false;
        }
    }

    double tot_E = bondEnergy + angleEnergy + dihedralEnergy + inversionEnergy;

    // std::cout << "total energy is: " << tot_E << std::endl;
    // std::cout << " bond energy is : " << bondEnergy << std::endl;
    // std::cout << " angle energy is : " << angleEnergy << std::endl;
    // std::cout << " dihedral energy is : " << dihedralEnergy << std::endl;
    // std::cout << " inversion energy is : " << inversionEnergy << std::endl;
    return (tot_E);
}

double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     bool &good, int nat, std::vector<double> *virial) {
    std::vector<eff::Bond> bonds = connectivity.GetBondVec();
    std::vector<eff::Angles> angles = connectivity.GetAnglesVec();
    std::vector<eff::Dihedral> dihedrals = connectivity.GetDihedralVec();
    std::vector<eff::Inversion> inversions = connectivity.GetInversionVec();
    double bondEnergy = 0;
    double angleEnergy = 0;
    double dihedralEnergy = 0;
    double inversionEnergy = 0;

    // for (auto frame = frames.begin(); frame != frames.end(); frame++) {
    for (int mon_num = 0; mon_num < nm; mon_num++) {
        std::vector<double> xyz(3 * nat, 0.0);
        std::copy(xyz1.begin() + 3 * nat * mon_num, xyz1.begin() + 3 * nat * (mon_num + 1), xyz.begin());
        // std::copy() = frame->GetCoor();
        double mon_E = 0.0;
        for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
            // obtain the indexes for the specific bond we are looking at
            std::vector<size_t> indexes = bond->GetIndexes();

            // fill coordinate vectors
            std::vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);

            double distance = CalculateDistance(coor1, coor2);

            bondEnergy += bond->GetEnergy(distance);
            mon_E += bond->GetEnergy(distance);

            // std::cout << "distance is: " << distance << std::endl;

            // Get topology gradient. Each bond will contribute a gradient to
            // each of the two atoms.
            double delta_grad = bond->GetTopologyGradient(distance);

            // std::cout << "bond delta_grad is: " << delta_grad << std::endl;

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

            // std::cout << "###########test bnd vir###########" << std::endl;
            // for (int teri = 0; teri < 9; teri++) {
            //     std::cout << bndvirtest[teri] * 418.4 << std::endl;
            // }

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

            // calculate the angle between three coordinates in degrees
            double theta = CalculateAngle(coor1, coor2, coor3);

            angleEnergy += angle->GetEnergy(theta);
            mon_E += angle->GetEnergy(theta);

            // std::cout << "theta is: " << theta << std::endl;

            // Get topology gradient. Each angle will contribute a gradient to
            // each of the three atoms.
            double delta_grad = angle->GetTopologyGradient(theta);

            // std::cout << "Angle delta_grad is: " << delta_grad << std::endl;

            double first_term = 1 / std::max(1.0e-8, sin(theta));
            double second_term = delta_grad;

            // Compute the vectors
            // NOTE: IN DLPOLY, atom i is the middle atom. In the documentation of
            // this code, I use j as the middle atom
            std::vector<double> r_ij = {coor1[0] - coor2[0], coor1[1] - coor2[1], coor1[2] - coor2[2]};
            std::vector<double> r_ik = {coor3[0] - coor2[0], coor3[1] - coor2[1], coor3[2] - coor2[2]};
            double dist_r_ij = CalculateDistance(r_ij);
            double dist_r_ik = CalculateDistance(r_ik);

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

            std::vector<double> bndvirtest(9);

            bndvirtest[0] = r_ij[0] * forces[0] + r_ik[0] * forces[6];
            bndvirtest[1] = r_ij[0] * forces[1] + r_ik[0] * forces[7];
            bndvirtest[2] = r_ij[0] * forces[2] + r_ik[0] * forces[8];

            bndvirtest[4] = r_ij[1] * forces[1] + r_ik[1] * forces[7];
            bndvirtest[5] = r_ij[1] * forces[2] + r_ik[1] * forces[8];

            bndvirtest[8] = r_ij[2] * forces[2] + r_ik[2] * forces[8];

            bndvirtest[3] = bndvirtest[1];
            bndvirtest[6] = bndvirtest[2];
            bndvirtest[7] = bndvirtest[5];

            // std::cout << "###########test angle vir###########" << std::endl;
            // for (int teri = 0; teri < 9; teri++) {
            //     std::cout << bndvirtest[teri] * 418.4 << std::endl;
            // }

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
            double phi = CalculateDihedralAngle(coor1, coor2, coor3, coor4);

            dihedralEnergy += dihedral->GetEnergy(phi);
            mon_E += dihedral->GetEnergy(phi);

            // std::cout << "phi is: " << phi << std::endl;

            // Begin gradient calculation

            // Get topology gradient. Each dihedral angle will contribute a gradient to
            // each of the four atoms.
            double delta_grad = dihedral->GetTopologyGradient(phi);
            // std::cout << "dihedral delta_grad is: " << delta_grad << std::endl;

            double cummu_grad = 0;  // Gradient accumulator

            // Multiply by minus 1 because we want gradient not force. Check Dlpoly
            // manual equation 2.42 to see why

            std::string func_form = dihedral->GetFunctionalForm();

            // if (func_form == "cos" || func_form == "harm" || func_form == "cos3") {
            cummu_grad += (-1) * (1.0 / sin(phi)) * delta_grad;
            // } else if (func_form == "hcos") {
            // cummu_grad += (-1) * delta_grad;
            // }

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

            std::vector<double> bndvirtest(9);

            bndvirtest[0] =
                fvector[0] * curr_force[0] + svector[0] * (curr_force[3] - curr_force[6]) - tvector[0] * curr_force[9];
            bndvirtest[1] =
                fvector[1] * curr_force[0] + svector[1] * (curr_force[3] - curr_force[6]) - tvector[1] * curr_force[9];
            bndvirtest[2] =
                fvector[2] * curr_force[0] + svector[2] * (curr_force[3] - curr_force[6]) - tvector[2] * curr_force[9];

            bndvirtest[4] =
                fvector[1] * curr_force[1] + svector[1] * (curr_force[4] - curr_force[7]) - tvector[1] * curr_force[10];
            bndvirtest[5] =
                fvector[1] * curr_force[2] + svector[1] * (curr_force[5] - curr_force[8]) - tvector[1] * curr_force[11];

            bndvirtest[8] =
                fvector[2] * curr_force[2] + svector[2] * (curr_force[5] - curr_force[8]) - tvector[2] * curr_force[11];

            bndvirtest[3] = bndvirtest[1];
            bndvirtest[6] = bndvirtest[2];
            bndvirtest[7] = bndvirtest[5];

            // std::cout << "###########test dihedral vir###########" << std::endl;
            // for (int teri = 0; teri < 9; teri++) {
            //     std::cout << bndvirtest[teri] * 418.4 << std::endl;
            // }

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
            // string func_form = inversion -> GetFunctionalForm();

            // The non linear value can not be directly calculated. We need to put a
            // 0 as a place holder because we are going to add all of the potentials
            // from all of the inversion angles to this 0'd value.
            // vector<double> non_linear_value;
            // if ( func_form == "harm" || func_form == "none"){
            //   non_linear_value.push_back(0);
            // }

            // size_t inversion_type = inversion->GetTopologyType();
            // size_t counter = 0;
            // bool linear_flag = inversion->GetLinearFlag();

            // fill coordinate vectors
            std::vector<double> centralCoor(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
            std::vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
            std::vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
            std::vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

            // calculate the set of inversion angles between 4 coordinates
            std::vector<double> phis = CalculateInversionAngle(centralCoor, coor2, coor3, coor4);

            double currInversionEnergy = 0.0;

            // Accumulate inversion energy over all 3 inversion angles
            for (size_t i = 0; i < 3; i++) {
                currInversionEnergy += inversion->GetEnergy(phis[i]);
                // std::cout << "inversion energy is: " << inversion->GetEnergy(phis[i]);
                // std::cout << "phi is: " << phis[i] << std::endl;
            }  // end for loop

            // Line written on 12/12/2019. This line is written to make the inversion
            // energy match dlpoly. It is currently unknown why dlpoly chooses to
            // average it inversion potential energy.
            currInversionEnergy /= 3.0;

            inversionEnergy += currInversionEnergy;
            mon_E += currInversionEnergy;
            // Begin calculation of gradient

            std::vector<double> tmp = phis;

            // Shift the phis vector because the inversion angles are assigned
            // in the incorreect order. This is not an issue for energy computation. Hence, this is
            // a quick fix for the gradients
            for (int i = 0; i < 3; i++) {
                phis[i] = tmp[(i + 1) % 3];
            }

            // Compute the gradient of the functional form for each inversion angle.
            // Note: It is currently unknown why sin(phis[i]) is multiplied by 3.
            std::vector<double> cummu_grad = {0.0, 0.0, 0.0};

            // Compute derivative of functional form for each contributing angle
            for (int i = 0; i < 3; i++) {
                double delta_grad = inversion->GetTopologyGradient(phis[i]);

                cummu_grad[i] += -1 * delta_grad;
                // std::cout << "Inversion delta_grad is: " << delta_grad << std::endl;
            }

            std::vector<double> curr_force(12);

            // Compute remainder of gradient
            CalculateInversionGrad(centralCoor, coor2, coor3, coor4, indexes, cummu_grad, phis, grad1, mon_num, nat,
                                   curr_force);

            std::vector<double> rij = {coor2[0] - centralCoor[0], coor2[1] - centralCoor[1], coor2[2] - centralCoor[2]};
            std::vector<double> rik = {coor3[0] - centralCoor[0], coor3[1] - centralCoor[1], coor3[2] - centralCoor[2]};
            std::vector<double> rin = {coor4[0] - centralCoor[0], coor4[1] - centralCoor[1], coor4[2] - centralCoor[2]};

            std::vector<double> bndvirtest(9);

            bndvirtest[0] = rij[0] * curr_force[3] + rik[0] * curr_force[6] + rin[0] * curr_force[9];
            bndvirtest[1] = rij[1] * curr_force[3] + rik[1] * curr_force[6] + rin[1] * curr_force[9];
            bndvirtest[2] = rij[2] * curr_force[3] + rik[2] * curr_force[6] + rin[2] * curr_force[9];
            bndvirtest[4] = rij[1] * curr_force[4] + rik[1] * curr_force[7] + rin[1] * curr_force[10];
            bndvirtest[5] = rij[1] * curr_force[5] + rik[1] * curr_force[8] + rin[1] * curr_force[11];
            bndvirtest[8] = rij[2] * curr_force[5] + rik[2] * curr_force[8] + rin[2] * curr_force[11];
            bndvirtest[3] = bndvirtest[1];
            bndvirtest[6] = bndvirtest[2];
            bndvirtest[7] = bndvirtest[5];

            // std::cout << "###########test inversion vir###########" << std::endl;
            // for (int teri = 0; teri < 9; teri++) {
            //     std::cout << bndvirtest[teri] * 418.4 << std::endl;
            // }

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

    // std::cout << "total energy is: " << tot_E << std::endl;
    // std::cout << " bond energy is : " << bondEnergy << std::endl;
    // std::cout << " angle energy is : " << angleEnergy << std::endl;
    // std::cout << " dihedral energy is : " << dihedralEnergy << std::endl;
    // std::cout << " inversion energy is : " << inversionEnergy << std::endl;

    // int count = 0;

    // std::cout << std::endl;
    // std::cout << "gradients in kcal/mol are: " << std::endl;
    // for (auto it = grad1.begin(); it != grad1.end(); it++) {
    //     if (count != 0 && count % 3 == 0) {
    //         std::cout << std::endl;
    //     }
    //     count++;
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;

    // count = 0;

    // std::cout << "gradients in dlpoly units are: " << std::endl;
    // for (auto it = grad1.begin(); it != grad1.end(); it++) {
    //     if (count != 0 && count % 3 == 0) {
    //         std::cout << std::endl;
    //     }
    //     count++;
    //     std::cout << (*it * 100 * 4.184) << " ";
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;

    // count = 0;

    // std::cout << "Forces in dlpoly units are: " << std::endl;
    // for (auto it = grad1.begin(); it != grad1.end(); it++) {
    //     if (count != 0 && count % 3 == 0) {
    //         std::cout << std::endl;
    //     }
    //     count++;
    //     std::cout << (*it * 100 * 4.184 * -1) << " ";
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;

    return (tot_E);
}
}  // namespace eff

// using namespace std;

// /**
//  * Main is the driver of the eval code and will handle all of the piece of
//  * information and piece them together to generate the final energy value.
//  * @param  argc number of command line arguments
//  * @param  argv pointer to the command line arguments
//  * @return      0
//  */
// int main(int argc, char** argv) {
//     // Checking for the correct usage
//     if (argc < 5) {
//         std::cerr << "usage: ./eval-1b-ttm "
//                   << "<frame> "
//                   << "<parameter.dat> "
//                   << "<charge_polarizability_file> "
//                   << "<C6-D6_file> " << std::endl;
//         return 0;
//     }

//     // Read the frame like the training set
//     frames = Read_xyz(argv[1]);

//     // create ifstream object to pass into read_connectivity
//     ifstream parameters(argv[2]);

//     // read the connectivity file. bonds, angles, and dihedral vectors are now
//     // filled and contains all the information necessary. Also reads the parmeters
//     // from the parameters.dat file
//     Read_connectivity(parameters, bonds, angles, dihedrals, inversions, ex12, ex13, ex14, true);

//     // create ifstream object to pass into read_charge_pol
//     ifstream charge_pol(argv[3]);

//     // ensure that all of the vectors are appropriately sized.
//     charge.resize(frames[0].GetNumAtoms(), 0.0);
//     pol.resize(frames[0].GetNumAtoms(), 0.0);
//     pol_fac.resize(frames[0].GetNumAtoms(), 0.0);
//     atom_type.resize(frames[0].GetNumAtoms(), 0.0);

//     // read the charge and polarizability file.
//     Read_charge_pol(charge_pol, charge, pol, pol_fac, atom_type);

//     // create ifstream object to pass into Read_C6_D6 to read the C6 and D6 values
//     ifstream C6_D6(argv[4]);

//     // Store the number of atom types in the molecule
//     total_num_atom_types = *max_element(atom_type.begin(), atom_type.end());

//     // Ensure that all of the vectors are appropriately sized
//     // Size is square of total_num_atom_types because that is the maximum number
//     // of possible values for the c6, d6, and a that we can have. Default value is 0,
//     // unless it is given in the c6 d6 a file.
//     c6.resize(total_num_atom_types * total_num_atom_types, 0.0);
//     d6.resize(total_num_atom_types * total_num_atom_types, 0.0);
//     a.resize(total_num_atom_types * total_num_atom_types, 0.0);
//     // read in the c6, d6, and a values into their respective vectors.
//     Read_C6_D6(C6_D6, c6, d6, a);

//     // Obtain permutations of all atom indexes. NOT FRAME SPECIFIC
//     size_t num_atoms = frames[0].GetNumAtoms();

//     // Resize gradients for each atom. Atom order in the vector will follow the
//     // order in the xyz file
//     gradients.resize(3 * num_atoms);

//     // This will give you every single permutation of all the atoms types in
//     // every single frame
//     for (size_t i = 1; i <= num_atoms; i++) {
//         for (size_t j = i + 1; j <= num_atoms; j++) {
//             // Create a length 4 vector to store the indexes and atom types.
//             // The order will always be: index1 index2 atom_type1 atom_type2
//             vector<size_t> excluded_type_and_index(4, 0);

//             // If we can not find the current i,j index pair in the excluded pair
//             // sets, then they must be non_excluded.
//             // append the pair's ATOM TYPES to the non_excluded set. We need the atom
//             // types because dispersion and repulsion are calculated based on atom
//             // types
//             if (!isExcluded(ex12, ex13, ex14, i, j)) {
//                 excluded_type_and_index[0] = i;
//                 excluded_type_and_index[1] = j;
//                 excluded_type_and_index[2] = atom_type[i - 1];
//                 excluded_type_and_index[3] = atom_type[j - 1];
//                 non_excluded.push_back(excluded_type_and_index);
//             }
//         }
//     }

//     size_t frameNumber = 0;

//     // Set up each of the vectors to store the 3 types of energies for all
//     // frames in the xyz
//     vector<double> electrostatic_energies(frames.size(), 0.0);
//     vector<double> dispersion_energies(frames.size(), 0.0);
//     vector<double> repulsion_energies(frames.size(), 0.0);

//     // Check if we need to account for dispersion. If not, then we don't need to
//     // adjust at all
//     if (!non_excluded.empty()) {
//         // Conduct dispersion adjustment for every single frame
//         for (auto frame = frames.begin(); frame != frames.end(); frame++) {
//             // For every single frame, conduct the dispersion adjustment, by calculating the
//             // INDIVIDUAL dispersion energy for each thing that is in non_excluded set

//             vector<double> xyz = frame->GetCoor();

//             for (auto non_ex = non_excluded.begin(); non_ex != non_excluded.end(); non_ex++) {
//                 size_t index_1 = (*non_ex)[0];  // these need to be atom types
//                 size_t index_2 = (*non_ex)[1];  // these need to be atom types

//                 size_t atom_type1 = (*non_ex)[2];
//                 size_t atom_type2 = (*non_ex)[3];

//                 vector<double> coor1(xyz.begin() + (index_1 - 1) * 3, xyz.begin() + (index_1)*3);
//                 vector<double> coor2(xyz.begin() + (index_2 - 1) * 3, xyz.begin() + (index_2)*3);
//                 double distance = CalculateDistance(coor1, coor2);
//                 double c6ij = c6[(atom_type1 - 1) * total_num_atom_types + (atom_type2 - 1)];
//                 double d6ij = d6[(atom_type1 - 1) * total_num_atom_types + (atom_type2 - 1)];
//                 double aij = a[(atom_type1 - 1) * total_num_atom_types + (atom_type2 - 1)];

//                 double dispersion_value = x2o::dispersion(distance, d6ij, c6ij);
//                 double repulsion_value = x2o::buckingham(d6ij, aij, distance);

//                 dispersion_energies[frameNumber] += dispersion_value;
//                 repulsion_energies[frameNumber] += repulsion_value;
//             }  // End non excluded set loop
//             frameNumber++;
//         }  // End frames loop
//     }      // End if statement

//     // size_t counter = 0;

//     // Loop over all of the entries in the training set
//     for (size_t k = 0; k < frames.size(); ++k) {
//         vector<double> coor_vec = frames[k].GetCoor();
//         sys::system my_system;
//         // Molecule
//         // n_at is the number of atom types, and pols is the polarizability vector
//         int n_at = pol.size();
//         molecule mol;
//         mol.allocate(n_at);

//         mol.set_coords(coor_vec.data());

//         mol.set_charges(charge.data());  // pass pointer to vector holding charges here

//         mol.set_pol(pol.data());         // pass pointer to vector holding pol here
//         mol.set_polfac(pol_fac.data());  // pass pointer to vector holding pol fac here

//         // Add the excluded 12, 13, and 14
//         std::shared_ptr<molecule> tmp = std::make_shared<molecule>(mol);
//         std::pair<size_t, size_t> zeroBasePair;

//         // excluded_set_type ex12_filtered = ex12;
//         // for ( auto it = ex12_filtered.begin() ; it != ex12_filtered.end() ; it++){
//         //   if ( it->first < it->second){
//         //     ex12_filtered.erase(it);
//         //   }
//         // }
//         //
//         // for (auto excluded = ex12_filtered.begin(); excluded != ex12_filtered.end(); excluded++) {
//         //   zeroBasePair.first = excluded->first - 1;
//         //   zeroBasePair.second = excluded->second - 1;
//         //   tmp->excluded12.insert(zeroBasePair);
//         //
//         // }

//         // excluded_set_type ex12_filtered = ex12;
//         for (auto it = ex12.begin(); it != ex12.end(); it++) {
//             if (it->first < it->second) {
//                 zeroBasePair.first = it->first - 1;
//                 zeroBasePair.second = it->second - 1;
//                 tmp->excluded12.insert(zeroBasePair);
//             }
//         }

//         // excluded_set_type ex13_filtered = ex13;
//         // for ( auto it = ex13_filtered.begin() ; it != ex13_filtered.end() ; it++){
//         //   if ( it->first < it->second){
//         //     ex13_filtered.erase(it);
//         //   }
//         // }
//         // for (auto excluded = ex13_filtered.begin(); excluded != ex13_filtered.end(); excluded++) {
//         //   zeroBasePair.first = excluded->first - 1;
//         //   zeroBasePair.second = excluded->second - 1;
//         //   tmp->excluded13.insert(zeroBasePair);
//         // }

//         // excluded_set_type ex13_filtered;
//         for (auto it = ex13.begin(); it != ex13.end(); it++) {
//             if (it->first < it->second) {
//                 zeroBasePair.first = it->first - 1;
//                 zeroBasePair.second = it->second - 1;
//                 tmp->excluded13.insert(zeroBasePair);
//             }
//         }

//         // excluded_set_type ex14_filtered = ex14;
//         // for ( auto it = ex14_filtered.begin() ; it != ex14_filtered.end() ; it++){
//         //   if ( it->first < it->second){
//         //     ex14_filtered.erase(it);
//         //   }
//         // }

//         // excluded_set_type ex14_filtered;
//         for (auto it = ex14.begin(); it != ex14.end(); it++) {
//             if (it->first < it->second) {
//                 zeroBasePair.first = it->first - 1;
//                 zeroBasePair.second = it->second - 1;
//                 tmp->excluded14.insert(zeroBasePair);
//                 // ex14_filtered.insert(zeroBasePair);
//             }
//         }

//         // for (auto excluded = ex14_filtered.begin(); excluded != ex14_filtered.end(); excluded++) {
//         //   zeroBasePair.first = excluded->first - 1;
//         //   zeroBasePair.second = excluded->second - 1;
//         //   tmp->excluded14.insert(zeroBasePair);
//         // }

//         /////////////// U S E D    O N L Y   F O R   U R E A /////////////

//         // zeroBasePair.first = 4;
//         // zeroBasePair.second = 6;
//         // tmp->excluded14.insert(zeroBasePair);
//         //
//         // zeroBasePair.first = 4;
//         // zeroBasePair.second = 7;
//         // tmp->excluded14.insert(zeroBasePair);
//         //
//         // zeroBasePair.first = 5;
//         // zeroBasePair.second = 6;
//         // tmp->excluded14.insert(zeroBasePair);
//         //
//         // zeroBasePair.first = 5;
//         // zeroBasePair.second = 7;
//         // tmp->excluded14.insert(zeroBasePair);

//         /////////////// U S E D    O N L Y   F O R   U R E A /////////////

//         tmp->id = "n";
//         my_system.add_molecule(tmp);

//         ttm::electrostatics elect;
//         double* grad = 0;
//         ttm::smear_ttm4x smr;
//         smr.m_aDD_inter = 0.055;
//         smr.m_aDD_intra_12 = 0.300;
//         smr.m_aDD_intra_13 = 0.300;
//         smr.m_aDD_intra_14 = 0.055;
//         double energy = elect(my_system.nsites, my_system.charges, my_system.polfac, my_system.atmpolar,
//                               my_system.coords, my_system.excluded12, my_system.excluded13, my_system.excluded14,
//                               my_system.first_index_of_molec, my_system.molec, smr, grad);
//         electrostatic_energies[k] = energy;
//     }

// for (auto frame = frames.begin(); frame != frames.end(); frame++) {
//     vector<double> xyz = frame->GetCoor();

//     for (auto bond = bonds.begin(); bond != bonds.end(); bond++) {
//         // obtain the indexes for the specific bond we are looking at
//         vector<size_t> indexes = bond->GetIndexes();

//         // fill coordinate vectors
//         vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
//         vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);

//         double distance = CalculateDistance(coor1, coor2);

//         bondEnergy += bond->GetEnergy(distance);

//         // Get topology gradient. Each bond will contribute a gradient to
//         // each of the two atoms.
//         double delta_grad = bond->GetTopologyGradient(distance);

//         // cout << "delta_grad is: " << delta_grad;

//         // Get distance gradient to propagate gradient back to the x,y,z
//         // dimension for each atom in the topology
//         for (int i = 0; i < indexes.size(); i++) {
//             double delta_dist_x;
//             double delta_dist_y;
//             double delta_dist_z;

//             // First index
//             if (i == 0) {
//                 delta_dist_x = (coor1[0] - coor2[0]);
//                 delta_dist_y = (coor1[1] - coor2[1]);
//                 delta_dist_z = (coor1[2] - coor2[2]);
//             }
//             // Second index
//             else {
//                 delta_dist_x = -(coor1[0] - coor2[0]);
//                 delta_dist_y = -(coor1[1] - coor2[1]);
//                 delta_dist_z = -(coor1[2] - coor2[2]);
//             }

//             gradients[(indexes[i] - 1) * 3] += (delta_dist_x * delta_grad) / distance;
//             gradients[(indexes[i] - 1) * 3 + 1] += (delta_dist_y * delta_grad) / distance;
//             gradients[(indexes[i] - 1) * 3 + 2] += (delta_dist_z * delta_grad) / distance;
//         }
//     }

//     for (auto angle = angles.begin(); angle != angles.end(); angle++) {
//         vector<size_t> indexes = angle->GetIndexes();

//         // fill coordinate vectors
//         vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
//         vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
//         vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);

//         // calculate the angle between three coordinates in degrees
//         double theta = CalculateAngle(coor1, coor2, coor3);

//         angleEnergy += angle->GetEnergy(theta);

//         // Get topology gradient. Each angle will contribute a gradient to
//         // each of the three atoms.
//         double delta_grad = angle->GetTopologyGradient(theta);

//         // Begin Dlpoly solution
//         double first_term = 1 / sin(theta);
//         double second_term = delta_grad;

//         // Compute the vectors
//         // NOTE: IN DLPOLY, atom i is the middle atom. In the documentation of
//         // this code, I use j as the middle atom
//         vector<double> r_ij = {coor1[0] - coor2[0], coor1[1] - coor2[1], coor1[2] - coor2[2]};
//         vector<double> r_ik = {coor3[0] - coor2[0], coor3[1] - coor2[1], coor3[2] - coor2[2]};
//         double dist_r_ij = CalculateDistance(r_ij);
//         double dist_r_ik = CalculateDistance(r_ik);

//         // Loop through each atom
//         for (int index = 0; index < indexes.size(); index++) {
//             int delta_lj;
//             int delta_li;
//             int delta_lk;

//             // idx = 0 is atom on the end. Dlpoly defines this to be atom j. This is
//             // why here it is defined as lj = 1. The idx loop is looping from end_atom->mid_atom->end_atom
//             if (index == 0) {
//                 delta_lj = 1;
//                 delta_li = 0;
//                 delta_lk = 0;
//             } else if (index == 1) {
//                 delta_lj = 0;
//                 delta_li = 1;
//                 delta_lk = 0;
//             } else {
//                 delta_lj = 0;
//                 delta_li = 0;
//                 delta_lk = 1;
//             }

//             // Loop through each coordinate within an atom
//             for (int dim = 0; dim < 3; dim++) {
//                 double third_term = 0;

//                 // First term
//                 third_term += (delta_lj - delta_li) * (r_ik[dim] / (dist_r_ij * dist_r_ik));

//                 // Second term
//                 third_term += (delta_lk - delta_li) * (r_ij[dim] / (dist_r_ij * dist_r_ik));

//                 // Third term
//                 third_term -= cos(theta) *
//                               ((delta_lj - delta_li) * (r_ij[dim] / (dist_r_ij * dist_r_ij) +
//                                                         (delta_lk - delta_li) * (r_ik[dim] / (dist_r_ik *
//                                                         dist_r_ik))));

//                 // Take the negative because we want the gradient not the force (refer
//                 // to dl_poly manual)
//                 gradients[(indexes[index] - 1) * 3 + dim] += -(first_term * second_term * third_term);
//             }
//         }

//         // End Dlpoly solution

//         /*
//         Begin personal solution

//         // Symbols correspond with the cosine formula.
//         // (a^2 + b^2 - c^2) / 2ab =cos(C)
//         double a = CalculateDistance(coor1, coor2);
//         double b = CalculateDistance(coor2, coor3);
//         double c = CalculateDistance(coor1, coor3);

//         // The left side of the cosine formula above
//         double angle_form = cos(theta);

//         // Derivative of theta wrt a
//         double delta_t_a = (-1) * (1 / sqrt(1 - pow(angle_form, 2))) * (1 / (2 * b) - b / (2 * pow(a, 2)) +
//         pow(c, 2) / (2 * pow(a, 2) * b)); double delta_t_b = (-1) * (1 / sqrt(1 - pow(angle_form, 2))) * (-a / (2
//         * pow(b, 2)) + 1 / (2 * a) + pow(c, 2) / (2 * a * pow(b, 2))); double delta_t_c = (-1) * (1 / sqrt(1 -
//         pow(angle_form, 2))) * (-2 * c / (2 * a * b));

//         // Get distance gradient to propagate gradient back to the x,y,z
//         // dimension for each atom in the topology
//         for (int i = 0; i < indexes.size(); i++) {
//           double delta_a_x;
//           double delta_a_y;
//           double delta_a_z;
//           double delta_b_x;
//           double delta_b_y;
//           double delta_b_z;
//           double delta_c_x;
//           double delta_c_y;
//           double delta_c_z;

//           // First index
//           if (i == 0) {
//             // Derivative of a and c wrt x. Same for the rest
//             delta_a_x = ((coor1[0] - coor2[0]) / a);
//             delta_a_y = ((coor1[1] - coor2[1]) / a);
//             delta_a_z = ((coor1[2] - coor2[2]) / a);
//             delta_b_x = 0.0;
//             delta_b_y = 0.0;
//             delta_b_z = 0.0;
//             delta_c_x = ((coor1[0] - coor3[0]) / c);
//             delta_c_y = ((coor1[1] - coor3[1]) / c);
//             delta_c_z = ((coor1[2] - coor3[2]) / c);

//           }
//           // Second index
//           else if (i == 1) {
//             delta_a_x = ((coor2[0] - coor1[0]) / a);
//             delta_a_y = ((coor2[1] - coor1[1]) / a);
//             delta_a_z = ((coor2[2] - coor1[2]) / a);
//             delta_b_x = ((coor2[0] - coor3[0]) / b);
//             delta_b_y = ((coor2[1] - coor3[1]) / b);
//             delta_b_z = ((coor2[2] - coor3[2]) / b);
//             delta_c_x = 0.0;
//             delta_c_y = 0.0;
//             delta_c_z = 0.0;

//           }
//           // Third index
//           else {
//             delta_a_x = 0.0;
//             delta_a_y = 0.0;
//             delta_a_z = 0.0;
//             delta_b_x = ((coor3[0] - coor2[0]) / b);
//             delta_b_y = ((coor3[1] - coor2[1]) / b);
//             delta_b_z = ((coor3[2] - coor2[2]) / b);
//             delta_c_x = ((coor3[0] - coor1[0]) / c);
//             delta_c_y = ((coor3[1] - coor1[1]) / c);
//             delta_c_z = ((coor3[2] - coor1[2]) / c);
//           }

//           gradients[(indexes[i] - 1) * 3] += delta_grad * (delta_t_a * delta_a_x + delta_t_b * delta_b_x +
//           delta_t_c
//         * delta_c_x); gradients[(indexes[i] - 1) * 3 + 1] += delta_grad * (delta_t_a * delta_a_y + delta_t_b *
//         delta_b_y + delta_t_c * delta_c_y); gradients[(indexes[i] - 1) * 3 + 2] += delta_grad * (delta_t_a *
//         delta_a_z + delta_t_b * delta_b_z + delta_t_c * delta_c_z);
//         }

//           End Personal solution
//         */
//     }

//     for (auto dihedral = dihedrals.begin(); dihedral != dihedrals.end(); dihedral++) {
//         vector<size_t> indexes = dihedral->GetIndexes();
//         // fill coordinates with the same method as bonds (see above)
//         // connectivity file specifies the order to fill the coordinates here.
//         // the connectivity file MUST have the central atom inbetween the two
//         // outer atoms

//         vector<double> coor1(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
//         vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
//         vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
//         vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);
//         double phi = CalculateDihedralAngle(coor1, coor2, coor3, coor4);

//         dihedralEnergy += dihedral->GetEnergy(phi);

//         // Begin gradient calculation

//         // Get topology gradient. Each dihedral angle will contribute a gradient to
//         // each of the four atoms.
//         double delta_grad = dihedral->GetTopologyGradient(phi);

//         double cummu_grad = 0;  // Gradient accumulator

//         // Multiply by minus 1 because we want gradient not force. Check Dlpoly
//         // manual equation 2.42 to see why
//         cummu_grad += (-1) * (1 / sin(phi)) * delta_grad;

//         // Compute gradient of B for each atom and component and appends the
//         // result to the gradient vector
//         CalculateGradB(coor1, coor2, coor3, coor4, indexes, cummu_grad, phi, gradients);
//     }

//     for (auto inversion = inversions.begin(); inversion != inversions.end(); inversion++) {
//         vector<size_t> indexes = inversion->GetIndexes();
//         // string func_form = inversion -> GetFunctionalForm();

//         // The non linear value can not be directly calculated. We need to put a
//         // 0 as a place holder because we are going to add all of the potentials
//         // from all of the inversion angles to this 0'd value.
//         // vector<double> non_linear_value;
//         // if ( func_form == "harm" || func_form == "none"){
//         //   non_linear_value.push_back(0);
//         // }

//         // size_t inversion_type = inversion->GetTopologyType();
//         // size_t counter = 0;
//         // bool linear_flag = inversion->GetLinearFlag();

//         // fill coordinate vectors
//         vector<double> centralCoor(xyz.begin() + (indexes[0] - 1) * 3, xyz.begin() + (indexes[0]) * 3);
//         vector<double> coor2(xyz.begin() + (indexes[1] - 1) * 3, xyz.begin() + (indexes[1]) * 3);
//         vector<double> coor3(xyz.begin() + (indexes[2] - 1) * 3, xyz.begin() + (indexes[2]) * 3);
//         vector<double> coor4(xyz.begin() + (indexes[3] - 1) * 3, xyz.begin() + (indexes[3]) * 3);

//         // calculate the set of inversion angles between 4 coordinates
//         vector<double> phis = CalculateInversionAngle(centralCoor, coor2, coor3, coor4);

//         // Accumulate inversion energy over all 3 inversion angles
//         for (size_t i = 0; i < 3; i++) {
//             inversionEnergy += inversion->GetEnergy(phis[i]);
//         }  // end for loop

//         // Line written on 12/12/2019. This line is written to make the inversion
//         // energy match dlpoly. It is currently unknown why dlpoly chooses to
//         // average it inversion potential energy.
//         inversionEnergy /= 3.0;

//         // Begin calculation of gradient

//         vector<double> tmp = phis;

//         // Shift the phis vector because the inversion angles are assigned
//         // in the incorreect order. This is not an issue for energy computation. Hence, this is
//         // a quick fix for the gradients
//         for (int i = 0; i < 3; i++) {
//             phis[i] = tmp[(i + 1) % 3];
//         }

//         // Compute the gradient of the functional form for each inversion angle.
//         // Note: It is currently unknown why sin(phis[i]) is multiplied by 3.
//         vector<double> cummu_grad = {0.0, 0.0, 0.0};

//         // Compute derivative of functional form for each contributing angle
//         for (int i = 0; i < 3; i++) {
//             // cout << "bbullshit " << phis[i] << endl;
//             cummu_grad[i] += -1 * inversion->GetTopologyGradient(phis[i]);
//             // cout << "cummugrad is:" << cummu_grad[i] << endl;
//         }

//         // Compute remainder of gradient
//         CalculateInversionGrad(centralCoor, coor2, coor3, coor4, indexes, cummu_grad, phis, gradients);
//     }
// }
//     cout << "total energy is: "
//          << bondEnergy + angleEnergy + dihedralEnergy + inversionEnergy + electrostatic_energies[0] +
//                 dispersion_energies[0] + repulsion_energies[0]
//          << endl;
//     cout << " bond energy is : " << bondEnergy << endl;
//     cout << " angle energy is : " << angleEnergy << endl;
//     cout << " dihedral energy is : " << dihedralEnergy << endl;
//     cout << " inversion energy is : " << inversionEnergy << endl;
//     cout << " electro energy is : " << electrostatic_energies[0] << endl;
//     cout << " dispersion energy is : " << dispersion_energies[0] << endl;
//     cout << " repulsion energy is : " << repulsion_energies[0] << endl;

//     int count = 0;

//     cout << endl;
//     cout << "gradients in kcal/mol are: " << endl;
//     for (auto it = gradients.begin(); it != gradients.end(); it++) {
//         if (count != 0 && count % 3 == 0) {
//             cout << endl;
//         }
//         count++;
//         cout << *it << " ";
//     }
//     cout << endl;
//     cout << endl;

//     count = 0;

//     cout << "gradients in dlpoly units are: " << endl;
//     for (auto it = gradients.begin(); it != gradients.end(); it++) {
//         if (count != 0 && count % 3 == 0) {
//             cout << endl;
//         }
//         count++;
//         cout << (*it * 100 * 4.184) << " ";
//     }
//     cout << endl;
//     cout << endl;

//     count = 0;

//     cout << "Forces in dlpoly units are: " << endl;
//     for (auto it = gradients.begin(); it != gradients.end(); it++) {
//         if (count != 0 && count % 3 == 0) {
//             cout << endl;
//         }
//         count++;
//         cout << (*it * 100 * 4.184 * -1) << " ";
//     }
//     cout << endl;
//     cout << endl;
// }

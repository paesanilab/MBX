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

#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <string>
#include <vector>
#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/topology.h"

/**
 * @file connectivity.h
 * @brief Contains field variables and methods need to store and use
 *        a connectivity file
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @class Conn
 * @brief Conn is the class representing the connectivity. Uses the
 *        connectivity file to generate objects for use with the
 *        classical force field potential
 */
class Conn {
   public:
    /**
     * constructor to set field variables
     * @param[in] mon_id The monomer id
     * @param[in] bond_vec Vector of bonds
     * @param[in] angle_vec Vector of angles
     * @param[in] dihedral_vec Vector of dihedrals
     * @param[in] inversion_vec Vector of inversions
     */
    Conn(std::string mon_id, std::vector<Bond> bond_vec, std::vector<Angles> angle_vec,
         std::vector<Dihedral> dihedral_vec, std::vector<Inversion> inversion_vec);
    /**
     * Default destructor for a connectivity object
     */
    ~Conn();

    /////////////////////////////////////////////////////////////////////////////
    // Getters //////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the monomer id
     * @return string representing the monomer id
     */
    std::string GetMonId();

    /**
     * Gets the vector of bonds
     * @return vector of bond
     */
    std::vector<Bond> GetBondVec();

    /**
     * Gets the vector of angles
     * @return vector of angles
     */
    std::vector<Angles> GetAnglesVec();

    /**
     * Gets the vector of dihedrals
     * @return vector of dihedral
     */
    std::vector<Dihedral> GetDihedralVec();

    /**
     * Gets the vector of inversions
     * @return vector of inversion
     */
    std::vector<Inversion> GetInversionVec();

    /**
     * @brief Checks if two connectivity are the same by inspecting
     * all of the field variables.
     * @param[in] connectivity Is the other connectivity object that we
     * are comparing against
     * @return True if the connectivity objects are the same. False otherwise
     */
    bool operator==(Conn const &connectivity) const;

   private:
    /**
     * Represents the monomer id
     */
    std::string mon_id_;

    /**
     * Holds a vector of all the bonds for the given monomer id
     */
    std::vector<Bond> bond_vec_;
    /**
     * Holds a vector of all the angles for the given monomer id
     */
    std::vector<Angles> angle_vec_;
    /**
     * Holds a vector of all the dihedrals for the given monomer id
     */
    std::vector<Dihedral> dihedral_vec_;
    /**
     * Holds a vector of all the inversions for the given monomer id
     */
    std::vector<Inversion> inversion_vec_;
};

}  // namespace eff

#endif

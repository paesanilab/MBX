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

#include "potential/force_field/connectivity.h"

namespace eff {

Conn::Conn(std::string mon_id, std::vector<Bond> bond_vec, std::vector<Angles> angle_vec,
           std::vector<Dihedral> dihedral_vec, std::vector<Inversion> inversion_vec) {
    mon_id_ = mon_id;
    bond_vec_ = bond_vec;
    angle_vec_ = angle_vec;
    dihedral_vec_ = dihedral_vec;
    inversion_vec_ = inversion_vec;
}

Conn::~Conn() {}
std::string Conn::GetMonId() { return mon_id_; }
std::vector<Bond> Conn::GetBondVec() { return bond_vec_; }
std::vector<Angles> Conn::GetAnglesVec() { return angle_vec_; }
std::vector<Dihedral> Conn::GetDihedralVec() { return dihedral_vec_; }
std::vector<Inversion> Conn::GetInversionVec() { return inversion_vec_; }

bool Conn::operator==(Conn const &connectivity) const {
    // Check field variables

    if (connectivity.mon_id_ != mon_id_ || connectivity.bond_vec_ != bond_vec_ ||
        connectivity.angle_vec_ != angle_vec_ || connectivity.dihedral_vec_ != dihedral_vec_ ||
        connectivity.inversion_vec_ != inversion_vec_) {
        return false;
    }

    return true;
}

}  // namespace eff

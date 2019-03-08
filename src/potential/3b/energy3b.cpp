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

#include "energy3b.h"

namespace e3b {

double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3) {
    // Order the three monomer names and corresponding xyz
    if (m2 < m1 && m2 < m3) {
        std::swap(m1, m2);
        std::swap(xyz1, xyz2);
        if (m3 < m2) {
            std::swap(m2, m3);
            std::swap(xyz2, xyz3);
        }
    } else if (m3 < m1 && m3 < m2) {
        std::swap(m1, m3);
        std::swap(xyz1, xyz3);
        if (m3 < m2) {
            std::swap(m2, m3);
            std::swap(xyz2, xyz3);
        }
    } else if (m3 < m2) {
        std::swap(m2, m3);
        std::swap(xyz2, xyz3);
    }

    if (m1 == "h2o" and m2 == "h2o" and m3 == "h2o") {
        x2o::x3b_v2x pot;
        return pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), nm);
    } else {
        return 0.0;
    }
}

double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3, std::vector<double> &grd1,
                     std::vector<double> &grd2, std::vector<double> &grd3) {
    size_t index1(1), index2(2), index3(3);

    // Order the three monomer names and corresponding xyz
    if (m2 < m1 && m2 < m3) {
        std::swap(m1, m2);
        std::swap(xyz1, xyz2);
        std::swap(grd1, grd2);
        std::swap(index1, index2);
        //    std::string tmp = m1;
        //    m1 = m2;
        //    m2 = tmp;
        //    std::vector<double> tmp2 = std::move(xyz1);
        //    xyz1 = std::move(xyz2);
        //    xyz2 = std::move(tmp2);
        //    tmp2 = std::move(grd1);
        //    grd1 = std::move(grd2);
        //    grd2 = std::move(tmp2);
        if (m3 < m2) {
            std::swap(m2, m3);
            std::swap(xyz2, xyz3);
            std::swap(grd2, grd3);
            std::swap(index2, index3);
            //      std::string tmp3 = m2;
            //      m2 = m3;
            //      m3 = tmp3;
            //      std::vector<double> tmp4 = std::move(xyz2);
            //      xyz2 = std::move(xyz3);
            //      xyz3 = std::move(tmp4);
            //      tmp4 = std::move(grd2);
            //      grd2 = std::move(grd3);
            //      grd3 = std::move(tmp4);
        }
    } else if (m3 < m1 && m3 < m2) {
        std::swap(m1, m3);
        std::swap(xyz1, xyz3);
        std::swap(grd1, grd3);
        std::swap(index1, index3);
        //    std::string tmp = m1;
        //    m1 = m3;
        //    m3 = tmp;
        //    std::vector<double> tmp2 = std::move(xyz1);
        //    xyz1 = std::move(xyz3);
        //    xyz3 = std::move(tmp2);
        //    tmp2 = std::move(grd1);
        //    grd1 = std::move(grd3);
        //    grd2 = std::move(tmp2);
        if (m3 < m2) {
            std::swap(m2, m3);
            std::swap(xyz2, xyz3);
            std::swap(grd2, grd3);
            std::swap(index2, index3);
            //      std::string tmp3 = m2;
            //      m2 = m3;
            //      m3 = tmp3;
            //      std::vector<double> tmp4 = std::move(xyz2);
            //      xyz2 = std::move(xyz3);
            //      xyz3 = std::move(tmp4);
            //    tmp2 = std::move(grd1);
            //    grd1 = std::move(grd2);
            //    grd2 = std::move(tmp2);
        }
    } else if (m3 < m2) {
        std::swap(m2, m3);
        std::swap(xyz2, xyz3);
        std::swap(grd2, grd3);
        std::swap(index2, index3);
        //    std::string tmp3 = m2;
        //    m2 = m3;
        //    m3 = tmp3;
        //    std::vector<double> tmp4 = std::move(xyz2);
        //    xyz2 = std::move(xyz3);
        //    xyz3 = std::move(tmp4);
        //    tmp2 = std::move(grd1);
        //    grd1 = std::move(grd2);
        //    grd2 = std::move(tmp2);
    }

    double energy = 0.0;
    // Note: in the conditional, m2 >= m1 ALWAYS
    if (m1 == "h2o" and m2 == "h2o" and m3 == "h2o") {
        x2o::x3b_v2x pot;
        energy = pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), grd1.data(), grd2.data(), grd3.data(), nm);
    } else {
        energy = 0.0;
    }

    if (index1 != 1) {
        if (index2 == 1) {
            std::swap(index1, index2);
            std::swap(grd1, grd2);
        } else if (index3 == 1) {
            std::swap(index1, index3);
            std::swap(grd1, grd3);
        }  // else { error
    }

    if (index2 != 2) {
        std::swap(index2, index3);
        std::swap(grd2, grd3);
    }

    return energy;
}

}  // namespace e3b

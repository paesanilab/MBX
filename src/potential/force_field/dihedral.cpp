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

#include "dihedral.h"

/**
 * @file dihedral.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

namespace eff {

Dihedral::Dihedral(){};

Dihedral::Dihedral(std::string topology, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = topology;
    functional_form_ = functional_form;
    std::transform(topology_.begin(), topology_.end(), topology_.begin(), ::tolower);
    std::transform(functional_form_.begin(), functional_form_.end(), functional_form_.begin(), ::tolower);
    indexes_ = indexes;

    if (functional_form_ == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
    } else if (functional_form_ == "cos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 2;
    } else if (functional_form_ == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form_ == "hcos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form_ == "cos3") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 0;
    } else {
        std::string text = "Undefined or missing functional form for dihedral: " + functional_form_;
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(num_linear_params_, 0.0);
    nonlinear_parameters_ = std::vector<double>(num_nonlinear_params_, 0.0);
}

Dihedral::~Dihedral(){};

void Dihedral::SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters) {
    if (functional_form_ == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
    } else if (functional_form_ == "cos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 2;
    } else if (functional_form_ == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form_ == "hcos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form_ == "cos3") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 0;
    } else {
        std::string text = "Undefined or missing functional form for dihedral: " + functional_form_;
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (num_linear_params_ != linear_parameters.size()) {
        std::string text = "Expected number of linear parameters for functional form " + functional_form_ + " is " +
                           std::to_string(num_linear_params_) + " while the number of parameters passed is " +
                           std::to_string(linear_parameters.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (num_nonlinear_params_ != nonlinear_parameters.size()) {
        std::string text = "Expected number of nonlinear parameters for functional form " + functional_form_ + " is " +
                           std::to_string(num_nonlinear_params_) + " while the number of parameters passed is " +
                           std::to_string(nonlinear_parameters.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = linear_parameters;
    nonlinear_parameters_ = nonlinear_parameters;
}

double Dihedral::GetEnergy(double x) {
    double energy = 0.0;

    if (functional_form_ == "none") {
    } else if (functional_form_ == "cos") {
        energy = linear_parameters_[0] * (1 + cos(nonlinear_parameters_[0] * x - nonlinear_parameters_[1]));
    } else if (functional_form_ == "harm") {
        // x needs to be rescaled as in dlpoly with the following two operations
        x -= nonlinear_parameters_[0];
        x -= (round(x * recip_two_pi)) * two_pi;
        energy = 0.5 * linear_parameters_[0] * x * x;
    } else if (functional_form_ == "hcos") {
        energy = 0.5 * linear_parameters_[0] * (cos(x) - cos(nonlinear_parameters_[0])) *
                 (cos(x) - cos(nonlinear_parameters_[0]));
    } else if (functional_form_ == "cos3") {
        energy = 0.5 * linear_parameters_[0] * (1 + cos(x)) + 0.5 * linear_parameters_[1] * (1 - cos(2 * x)) +
                 0.5 * linear_parameters_[2] * (1 + cos(3 * x));
    }

    return energy;
}

double Dihedral::GetTopologyGradient(double x) {
    double val = 0.0;
    if (functional_form_ == "none") {
        val = 0.0;
    } else if (functional_form_ == "cos") {
        val = (-1) * (linear_parameters_[0] * nonlinear_parameters_[0]) *
              sin(nonlinear_parameters_[0] * x - nonlinear_parameters_[1]);
    } else if (functional_form_ == "harm") {
        x -= nonlinear_parameters_[0];
        x -= (round(x * recip_two_pi)) * two_pi;
        val = linear_parameters_[0] * x;
    } else if (functional_form_ == "hcos") {
        val = -linear_parameters_[0] * sin(x) * (cos(x) - cos(nonlinear_parameters_[0]));
    } else if (functional_form_ == "cos3") {
        val = -0.5 * linear_parameters_[0] * sin(x) + 0.5 * 2 * linear_parameters_[1] * sin(2 * x) -
              0.5 * 3 * linear_parameters_[2] * sin(3 * x);
    }
    return val;
}

bool Dihedral::operator==(Dihedral const &dihedral) const {
    // Check field variables
    if (dihedral.topology_ != topology_ || dihedral.functional_form_ != functional_form_ ||
        dihedral.indexes_ != indexes_ || dihedral.num_linear_params_ != num_linear_params_ ||
        dihedral.num_nonlinear_params_ != num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are the
    // same
    for (int i = 0; i < dihedral.num_nonlinear_params_; i++) {
        if (fabs(dihedral.nonlinear_parameters_[i] - nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are the same
    for (int i = 0; i < dihedral.num_linear_params_; i++) {
        if (fabs(dihedral.linear_parameters_[i] - linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}
}  // namespace eff

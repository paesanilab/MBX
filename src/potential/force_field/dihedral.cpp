#include "dihedral.h"

/**
 * @file dihedral.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Dihedral::Dihedral(){};

Dihedral::Dihedral(std::string topology, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = topology;
    functional_form_ = functional_form;
    indexes_ = indexes;

    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
        std::cerr << "Dihedral has none functional form. Its energy and gradients are not being evaluated" << std::endl;
    } else if (functional_form == "cos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 2;
    } else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form == "hcos") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form == "cos3") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 0;
    } else {
        std::string text = "Undefined or missing functional form for dihedral";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(num_linear_params_, 0.0);
    nonlinear_parameters_ = std::vector<double>(num_nonlinear_params_, 0.0);
}

Dihedral::~Dihedral(){};

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
    if (dihedral.topology_ != this->topology_ || dihedral.functional_form_ != this->functional_form_ ||
        dihedral.indexes_ != this->indexes_ || dihedral.num_linear_params_ != this->num_linear_params_ ||
        dihedral.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are the
    // same
    for (int i = 0; i < dihedral.num_nonlinear_params_; i++) {
        if (fabs(dihedral.nonlinear_parameters_[i] - this->nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are the same
    for (int i = 0; i < dihedral.num_linear_params_; i++) {
        if (fabs(dihedral.linear_parameters_[i] - this->linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

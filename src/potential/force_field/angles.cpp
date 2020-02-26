#include "angles.h"

/**
 * @file angles.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Angles::Angles(){};

Angles::Angles(std::string topology, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = topology;
    functional_form_ = functional_form;
    indexes_ = indexes;

    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
        std::cerr << "Angle has none functional form. Its energy and gradients are not being evaluated" << std::endl;
    } else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form == "quartic") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 1;
    } else {
        std::string text = "Undefined or missing functional form for angles";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(num_linear_params_, 0.0);
    nonlinear_parameters_ = std::vector<double>(num_nonlinear_params_, 0.0);
}

Angles::~Angles(){};

double Angles::GetEnergy(double x) {
    double energy = 0.0;

    if (functional_form_ == "none") {
    } else if (functional_form_ == "harm") {
        energy = linear_parameters_[0] * 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    } else if (functional_form_ == "quartic") {
        energy = 0.5 * linear_parameters_[0] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) +
                 (1.0 / 3.0) * linear_parameters_[1] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                     (x - nonlinear_parameters_[0]) +
                 0.25 * linear_parameters_[2] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                     (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    }

    return energy;
}

double Angles::GetTopologyGradient(double x) {
    double val = 0.0;

    if (functional_form_ == "none") {
        val = 0.0;
    } else if (functional_form_ == "harm") {
        val = linear_parameters_[0] * (x - nonlinear_parameters_[0]);
    } else if (functional_form_ == "quartic") {
        double term1 = linear_parameters_[0] * (x - nonlinear_parameters_[0]);
        double term2 = linear_parameters_[1] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        double term3 = linear_parameters_[2] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                       (x - nonlinear_parameters_[0]);
        val = term1 + term2 + term3;
    }
    return val;
}

bool Angles::operator==(Angles const &angle) const {
    // Check field variables
    if (angle.topology_ != this->topology_ || angle.functional_form_ != this->functional_form_ ||
        angle.indexes_ != this->indexes_ || angle.num_linear_params_ != this->num_linear_params_ ||
        angle.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are the
    // same
    for (int i = 0; i < angle.num_nonlinear_params_; i++) {
        if (fabs(angle.nonlinear_parameters_[i] - this->nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are the same
    for (int i = 0; i < angle.num_linear_params_; i++) {
        if (fabs(angle.linear_parameters_[i] - this->linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    return true;
}
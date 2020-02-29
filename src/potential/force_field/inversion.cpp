#include "inversion.h"

/**
 * @file inversion.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Inversion::Inversion(){};

Inversion::Inversion(std::string topology, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = topology;
    functional_form_ = functional_form;
    indexes_ = indexes;

    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
        std::cerr << "Inversion has none functional form. Its energy and gradients are not being evaluated"
                  << std::endl;
    }

    else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else {
        std::string text = "Undefined or missing functional form for inversion";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(num_linear_params_, 0.0);
    nonlinear_parameters_ = std::vector<double>(num_nonlinear_params_, 0.0);
}

Inversion::~Inversion(){};

double Inversion::GetEnergy(double x) {
    double energy = 0.0;

    if (functional_form_ == "none") {
    } else if (functional_form_ == "harm") {
        energy = linear_parameters_[0] * 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    }
    return energy;
}

double Inversion::GetTopologyGradient(double x) {
    double val = 0.0;

    if (functional_form_ == "none") {
        val = 0.0;
    } else if (functional_form_ == "harm") {
        if (fabs(x) > 1.0e-12) {
            val = linear_parameters_[0] * (x - nonlinear_parameters_[0]);
            val = val * (1.0 / (3.0 * sin(x)));
        } else {
            val = 0.0;
        }
    }

    return val;
}

bool Inversion::operator==(Inversion const &inversion) const {
    // Check field variables
    if (inversion.topology_ != this->topology_ || inversion.functional_form_ != this->functional_form_ ||
        inversion.indexes_ != this->indexes_ || inversion.num_linear_params_ != this->num_linear_params_ ||
        inversion.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are the
    // same
    for (int i = 0; i < inversion.num_nonlinear_params_; i++) {
        if (fabs(inversion.nonlinear_parameters_[i] - this->nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are the same
    for (int i = 0; i < inversion.num_linear_params_; i++) {
        if (fabs(inversion.linear_parameters_[i] - this->linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    return true;
}
#include "angles.h"

/**
 * @file angles.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Angles::Angles(){};

Angles::Angles(std::string connectivity, size_t angle_type, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = connectivity;
    functional_form_ = functional_form;
    indexes_ = indexes;
    topology_type_ = angle_type;

    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
    } else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form == "quartic") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 1;
    } else {
        std::string text = "Undefined functional form for angles";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(GetNumLinear(), 0.0);
    nonlinear_parameters_ = std::vector<double>(GetNumNonLinear(), 0.0);
}

Angles::~Angles(){};

double Angles::GetEnergy(double x) {
    double energy = 0.0;

    // Skip fitting this angle if "none" found for functional_form
    if (functional_form_ == "none") {
        // use each functional form to calculate the potential energy
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

std::vector<double> Angles::GetNonLinearValue(double x) {
    double val = 0.0;
    std::vector<double> nonLinear;

    // Skip because this angle is not fitted
    if (functional_form_ == "none") {
        nonLinear.push_back(val);
    } else if (functional_form_ == "harm") {
        val = 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
    } else if (functional_form_ == "quartic") {
        val = 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
        val = (1.0 / 3.0) * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
              (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
        val = 0.25 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
              (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
    }
    return nonLinear;
}

double Angles::GetTopologyGradient(double x) {
    double val = 0.0;

    // Gradient is 0 for an unfitted bond
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
    if (angle.topology_ != this->topology_ || angle.topology_type_ != this->topology_type_ ||
        angle.functional_form_ != this->functional_form_ || angle.indexes_ != this->indexes_ ||
        angle.linear_parameters_ != this->linear_parameters_ ||
        angle.nonlinear_parameters_ != this->nonlinear_parameters_ || angle.linear_ != this->linear_ ||
        angle.num_linear_params_ != this->num_linear_params_ ||
        angle.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    return true;
}
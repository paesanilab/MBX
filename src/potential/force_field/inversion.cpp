#include "inversion.h"

/**
 * @file inversion.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Inversion::Inversion(){};

Inversion::Inversion(std::string connectivity, size_t inversion_type, std::vector<size_t> indexes,
                     std::string functional_form) {
    topology_ = connectivity;
    functional_form_ = functional_form;
    indexes_ = indexes;
    topology_type_ = inversion_type;

    // Skip fitting this inversion angle if "none" found for functional_form
    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
    }

    else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else {
        std::string text = "Undefined functional form for inversion";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(GetNumLinear(), 0.0);
    nonlinear_parameters_ = std::vector<double>(GetNumNonLinear(), 0.0);
}

Inversion::~Inversion(){};

double Inversion::GetEnergy(double x) {
    double energy = 0.0;

    // Skip fitting this inversion angle if "none" found for functional_form
    if (functional_form_ == "none") {
    }

    else if (functional_form_ == "harm") {
        energy = linear_parameters_[0] * 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    }
    return energy;
}

std::vector<double> Inversion::GetNonLinearValue(double x) {
    double val = 0.0;
    std::vector<double> nonLinear;

    // Skip because this inversion angle is not fitted
    if (functional_form_ == "none") {
        nonLinear.push_back(val);
    } else if (functional_form_ == "harm") {
        val = 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
    }
    return nonLinear;
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

#include "bond.h"

/**
 * @file bond.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Bond::Bond(){};

Bond::Bond(std::string connectivity, size_t bond_type, std::vector<size_t> indexes, std::string functional_form) {
    topology_ = connectivity;
    functional_form_ = functional_form;
    indexes_ = indexes;
    topology_type_ = bond_type;

    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
    } else if (functional_form == "harm") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 1;
    } else if (functional_form == "morse") {
        num_linear_params_ = 1;
        num_nonlinear_params_ = 2;
    } else if (functional_form == "quartic") {
        num_linear_params_ = 3;
        num_nonlinear_params_ = 1;
    } else {
        std::string text = "Undefined functional form for bond";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(GetNumLinear(), 0.0);
    nonlinear_parameters_ = std::vector<double>(GetNumNonLinear(), 0.0);
}

Bond::~Bond(){};

double Bond::GetEnergy(double x) {
    double energy = 0.0;
    // Skip because the bond is not fitted
    if (functional_form_ == "none") {
    }
    // use each functional form to calculate the potential energy
    else if (functional_form_ == "harm") {
        energy = linear_parameters_[0] * 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    } else if (functional_form_ == "morse") {
        double exp1 = (1 - std::exp(-nonlinear_parameters_[0] * (x - nonlinear_parameters_[1])));
        energy = linear_parameters_[0] * exp1 * exp1;
    } else if (functional_form_ == "quartic") {
        energy = 0.5 * linear_parameters_[0] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) +
                 (1.0 / 3.0) * linear_parameters_[1] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                     (x - nonlinear_parameters_[0]) +
                 0.25 * linear_parameters_[2] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                     (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
    }

    return energy;
}

std::vector<double> Bond::GetNonLinearValue(double x) {
    double val = 0.0;
    std::vector<double> nonLinear;

    // Skip because bond is not fitted
    if (functional_form_ == "none") {
        nonLinear.push_back(val);
    } else if (functional_form_ == "harm") {
        val = 0.5 * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        nonLinear.push_back(val);
    } else if (functional_form_ == "morse") {
        val = (1 - std::exp(-nonlinear_parameters_[0] * (x - nonlinear_parameters_[1])));
        val = val * val;
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

double Bond::GetTopologyGradient(double x) {
    double val = 0.0;

    // Gradient is 0 for an unfitted bond
    if (functional_form_ == "none") {
        val = 0.0;
    } else if (functional_form_ == "harm") {
        val = linear_parameters_[0] * (x - nonlinear_parameters_[0]);
    } else if (functional_form_ == "morse") {
        // Needs to be checked
        // Formula is: 2*k*E0*(1-e^-k(rij-r0))(e^-k(rij-r0))
        double coef = 2 * nonlinear_parameters_[0] * linear_parameters_[0];
        double middle = 1 - std::exp(-nonlinear_parameters_[0] * (x - nonlinear_parameters_[1]));
        double right = std::exp(-nonlinear_parameters_[0] * (x - nonlinear_parameters_[1]));
        val = coef * middle * right;
    } else if (functional_form_ == "quartic") {
        double term1 = linear_parameters_[0] * (x - nonlinear_parameters_[0]);
        double term2 = linear_parameters_[1] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]);
        double term3 = linear_parameters_[2] * (x - nonlinear_parameters_[0]) * (x - nonlinear_parameters_[0]) *
                       (x - nonlinear_parameters_[0]);
        val = term1 + term2 + term3;
    }
    return val;
}

bool Bond::operator==(Bond const &bond) const {
    // Check field variables
    // std::cout << "called bond compare" << std::endl;
    if (bond.topology_ != this->topology_ || bond.topology_type_ != this->topology_type_ ||
        bond.functional_form_ != this->functional_form_ || bond.indexes_ != this->indexes_ ||
        bond.linear_ != this->linear_ || bond.num_linear_params_ != this->num_linear_params_ ||
        bond.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are correct
    for (int i = 0; i < bond.num_nonlinear_params_; i++) {
        // If the difference between a single entry in the parameters is greater
        // than constant epsilon, then return false. the two parameters are
        // different
        if (fabs(bond.nonlinear_parameters_[i] - this->nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are correct
    for (int i = 0; i < bond.num_linear_params_; i++) {
        // If the difference between a single entry in the parameters is greater
        // than constant epsilon, then return false. the two parameters are
        // different
        if (fabs(bond.linear_parameters_[i] - this->linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    return true;
}
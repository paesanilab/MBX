#include "dihedral.h"

/**
 * @file dihedral.cpp
 * @brief Contains the implentation of all the functions defined in the header
 */

Dihedral::Dihedral(){};

Dihedral::Dihedral(std::string connectivity, size_t dihedral_type, std::vector<size_t> indexes,
                   std::string functional_form) {
    topology_ = connectivity;
    functional_form_ = functional_form;
    indexes_ = indexes;
    topology_type_ = dihedral_type;

    // Skip fitting this dihedral if "none" found for functional_form
    if (functional_form == "none") {
        num_linear_params_ = 0;
        num_nonlinear_params_ = 0;
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
        std::string text = "Undefined functional form for dihedral";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    linear_parameters_ = std::vector<double>(GetNumLinear(), 0.0);
    nonlinear_parameters_ = std::vector<double>(GetNumNonLinear(), 0.0);
}

Dihedral::~Dihedral(){};

double Dihedral::GetEnergy(double x) {
    double energy = 0.0;

    // Skip because the dihedral is not fitted
    if (functional_form_ == "none") {
    }
    // use each functional form to calculate the potential energy
    else if (functional_form_ == "cos") {
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

std::vector<double> Dihedral::GetNonLinearValue(double x) {
    double val = 0.0;
    std::vector<double> nonLinear;

    // Skip because dihedral is not fitted
    if (functional_form_ == "none") {
        nonLinear.push_back(val);
    } else if (functional_form_ == "cos") {
        val = 1 + cos(nonlinear_parameters_[0] * x - nonlinear_parameters_[1]);
        nonLinear.push_back(val);
    } else if (functional_form_ == "harm") {
        x -= nonlinear_parameters_[0];
        x -= (round(x * recip_two_pi)) * two_pi;
        val = 0.5 * x * x;
        nonLinear.push_back(val);
    } else if (functional_form_ == "hcos") {
        val = 0.5 * (cos(x) - cos(nonlinear_parameters_[0])) * (cos(x) - cos(nonlinear_parameters_[0]));
        nonLinear.push_back(val);
    } else if (functional_form_ == "cos3") {
        val = 0.5 * (1 + cos(x));
        nonLinear.push_back(val);
        val = 0.5 * (1 - cos(2 * x));
        nonLinear.push_back(val);
        val = 0.5 * (1 + cos(3 * x));
        nonLinear.push_back(val);
    }
    return nonLinear;
}

void Dihedral::SetIncludeExclude(std::string includeExclude) { includeExclude_ = includeExclude; }

std::string Dihedral::GetIncludeExclude() { return includeExclude_; }

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
    if (dihedral.topology_ != this->topology_ || dihedral.topology_type_ != this->topology_type_ ||
        dihedral.functional_form_ != this->functional_form_ || dihedral.indexes_ != this->indexes_ ||
        dihedral.linear_ != this->linear_ || dihedral.num_linear_params_ != this->num_linear_params_ ||
        dihedral.num_nonlinear_params_ != this->num_nonlinear_params_) {
        return false;
    }

    // Iterate through each of the non linear parameters and check they are correct
    for (int i = 0; i < dihedral.num_nonlinear_params_; i++) {
        // If the difference between a single entry in the parameters is greater
        // than constant epsilon, then return false. the two parameters are
        // different
        if (fabs(dihedral.nonlinear_parameters_[i] - this->nonlinear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // Iterate through each of the linear parameters and check they are correct
    for (int i = 0; i < dihedral.num_linear_params_; i++) {
        // If the difference between a single entry in the parameters is greater
        // than constant epsilon, then return false. the two parameters are
        // different
        if (fabs(dihedral.linear_parameters_[i] - this->linear_parameters_[i]) > EPSILON) {
            return false;
        }
    }

    // if (dihedral.topology_ != this->topology_) {
    //     std::cout << "dihedral.topology_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "dihedral.topology_ (correct)" << std::endl;
    // }

    // if (dihedral.topology_type_ != this->topology_type_) {
    //     std::cout << "topology_type_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "topology_type_ same (correct)" << std::endl;
    // }

    // if (dihedral.functional_form_ != this->functional_form_) {
    //     std::cout << "functional_form_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "functional_form_ same (correct)" << std::endl;
    // }

    // if (dihedral.indexes_ != this->indexes_) {
    //     std::cout << "indexes_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "indexes_ same (correct)" << std::endl;
    // }

    // if (dihedral.linear_parameters_ != this->linear_parameters_) {
    //     std::cout << "linear_parameters_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "linear_parameters_ same (correct)" << std::endl;
    // }

    // if (dihedral.nonlinear_parameters_ != this->nonlinear_parameters_) {
    //     std::cout << "nonlinear_parameters_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "nonlinear_parameters_ same (correct)" << std::endl;
    // }

    // if (dihedral.linear_ != this->linear_) {
    //     std::cout << "linear_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "linear_ same (correct)" << std::endl;
    // }

    // if (dihedral.num_linear_params_ != this->num_linear_params_) {
    //     std::cout << "num_linear_params_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "num_linear_params_ same (correct)" << std::endl;
    // }

    // if (dihedral.num_nonlinear_params_ != this->num_nonlinear_params_) {
    //     std::cout << "num_nonlinear_params_ not same (incorrect)" << std::endl;
    // } else {
    //     std::cout << "num_nonlinear_params_ same (correct)" << std::endl;
    // }

    return true;
}

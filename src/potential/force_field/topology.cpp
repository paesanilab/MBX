#include "topology.h"

/**
 * @file topology.cpp
 * @brief Implementing all of the methods in topology.h
 */

Topology::Topology() {}
Topology::~Topology() {}

void Topology::GetParameters(std::vector<double> &linear_parameters, std::vector<double> &nonlinear_parameters) {
    linear_parameters = linear_parameters_;
    nonlinear_parameters = nonlinear_parameters_;
}

size_t Topology::GetNumNonLinear() {
    if (linear_) {
        // the case where we want to fit the linear terms as normal linear terms
        return num_nonlinear_params_;
    }
    // the case where we want to fit the linear terms as nonlinear terms
    return num_nonlinear_params_ + num_linear_params_;
}

size_t Topology::GetNumLinear() {
    if (linear_) {
        // the case where we want to fit the linear terms as normal linear terms
        return num_linear_params_;
    }
    // the case where we want to fit the linear terms as nonlinear terms
    return 0;
}

void Topology::SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters) {
    linear_parameters_ = linear_parameters;
    nonlinear_parameters_ = nonlinear_parameters;
}

void Topology::SetParameters(std::vector<double> parameters) {
    // std::cout << "1 yes It worked" << std::endl;
    // std::cout << "num lin is: "<<num_linear_params_ << std::endl;
    linear_parameters_.resize(num_linear_params_);
    nonlinear_parameters_.resize(num_nonlinear_params_);
    // std::cout << "size: "<<linear_parameters_.size() << std::endl;
    std::copy(parameters.begin(), parameters.begin() + num_linear_params_, linear_parameters_.begin());
    // std::cout << "2 yes it did work" << std::endl;
    // std::cout << "num nonlin is: "<<num_nonlinear_params_ << std::endl;
    std::copy(parameters.begin() + num_linear_params_, parameters.end(), nonlinear_parameters_.begin());
}

void Topology::SetIndexes(std::vector<size_t> indexes) { indexes_ = indexes; }

std::vector<size_t> Topology::GetIndexes() { return indexes_; }

void Topology::SetTopologyType(size_t topology_type) { topology_type_ = topology_type; }

size_t Topology::GetTopologyType() { return topology_type_; }

void Topology::SetFunctionalForm(std::string functional_form) { functional_form_ = functional_form; }

std::string Topology::GetFunctionalForm() { return functional_form_; }

void Topology::SetLinearFlag(bool linear) { linear_ = linear; }

bool Topology::GetLinearFlag() { return linear_; }

void Topology::SetTopology(std::string topology) { topology_ = topology; }

std::string Topology::GetTopology() { return topology_; }

void Topology::PrintParameters() {
    std::cout << "Type " << topology_type_ << " - " << functional_form_ << std::endl;
    if (functional_form_ == "harm") {
        std::cout << std::fixed << std::setw(10) << std::right << "k = " << std::setw(18) << std::right
                  << linear_parameters_[0] << std::setw(18) << std::right << "[kcal/(molA^2)]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "x = " << std::setw(18) << std::right
                  << nonlinear_parameters_[0] << std::setw(18) << std::right << "[A or Rad]" << std::endl;
    } else if (functional_form_ == "morse") {
        std::cout << std::fixed << std::setw(10) << std::right << "E0 = " << std::setw(18) << std::right
                  << linear_parameters_[0] << std::setw(18) << std::right << "[kcal/mol]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "k = " << std::setw(18) << std::right
                  << nonlinear_parameters_[0] << std::setw(18) << std::right << "[1/A]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "r0 = " << std::setw(18) << std::right
                  << nonlinear_parameters_[1] << std::setw(18) << std::right << "[A]" << std::endl;
    } else if (functional_form_ == "quartic") {
        std::cout << std::fixed << std::setw(10) << std::right << "k1 = " << std::setw(18) << std::right
                  << linear_parameters_[0] << std::setw(18) << std::right << "[kcal/molA^2]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "k2 = " << std::setw(18) << std::right
                  << linear_parameters_[1] << std::setw(18) << std::right << "[kcal/molA^3]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "k3 = " << std::setw(18) << std::right
                  << linear_parameters_[2] << std::setw(18) << std::right << "[kcal/molA^4]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "x = " << std::setw(18) << std::right
                  << nonlinear_parameters_[0] << std::setw(18) << std::right << "[A]" << std::endl;
    } else if (functional_form_ == "cos") {
        std::cout << std::fixed << std::setw(10) << std::right << "A = " << std::setw(18) << std::right
                  << linear_parameters_[0] << std::setw(18) << std::right << "[kcal/mol]" << std::endl;
        std::cout << std::fixed << std::setw(10) << std::right << "m = " << std::setw(18) << std::right
                  << nonlinear_parameters_[0] << std::setw(18) << std::right << "[1/rad]" << std::endl;

        std::cout << std::fixed << std::setw(10) << std::right << "delta = " << std::setw(18) << std::right
                  << nonlinear_parameters_[1] << std::setw(18) << std::right << "[unitless]" << std::endl;
    }

    else {
        // additional functional forms to be printed need to be put here
    }
}

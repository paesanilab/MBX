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

size_t Topology::GetNumNonLinear() { return num_nonlinear_params_; }

size_t Topology::GetNumLinear() { return num_linear_params_; }

void Topology::SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters) {
    linear_parameters_ = linear_parameters;
    nonlinear_parameters_ = nonlinear_parameters;
}

void Topology::SetParameters(std::vector<double> parameters) {
    linear_parameters_.resize(num_linear_params_);
    nonlinear_parameters_.resize(num_nonlinear_params_);
    std::copy(parameters.begin(), parameters.begin() + num_linear_params_, linear_parameters_.begin());
    std::copy(parameters.begin() + num_linear_params_, parameters.end(), nonlinear_parameters_.begin());
}

void Topology::SetIndexes(std::vector<size_t> indexes) { indexes_ = indexes; }

std::vector<size_t> Topology::GetIndexes() { return indexes_; }

void Topology::SetTopologyType(size_t topology_type) { topology_type_ = topology_type; }

size_t Topology::GetTopologyType() { return topology_type_; }

void Topology::SetFunctionalForm(std::string functional_form) { functional_form_ = functional_form; }

std::string Topology::GetFunctionalForm() { return functional_form_; }

void Topology::SetTopology(std::string topology) { topology_ = topology; }

std::string Topology::GetTopology() { return topology_; }

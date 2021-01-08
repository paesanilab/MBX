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

#include "topology.h"

/**
 * @file topology.cpp
 * @brief Implementing all of the methods in topology.h
 */

namespace eff {

Topology::Topology() {}
Topology::~Topology() {}

void Topology::GetParameters(std::vector<double> &linear_parameters, std::vector<double> &nonlinear_parameters) {
    linear_parameters = linear_parameters_;
    nonlinear_parameters = nonlinear_parameters_;
}

size_t Topology::GetNumNonLinear() { return num_nonlinear_params_; }

size_t Topology::GetNumLinear() { return num_linear_params_; }

void Topology::SetNumNonLinear(size_t num_nonlinear) { num_nonlinear_params_ = num_nonlinear; }

void Topology::SetNumLinear(size_t num_linear) { num_linear_params_ = num_linear; }

void Topology::SetIndexes(std::vector<size_t> indexes) { indexes_ = indexes; }

std::vector<size_t> Topology::GetIndexes() { return indexes_; }

void Topology::SetFunctionalForm(std::string functional_form) {
    functional_form_ = functional_form;
    std::transform(functional_form_.begin(), functional_form_.end(), functional_form_.begin(), ::tolower);
}

std::string Topology::GetFunctionalForm() { return functional_form_; }

void Topology::SetTopology(std::string topology) {
    topology_ = topology;
    std::transform(topology_.begin(), topology_.end(), topology_.begin(), ::tolower);
}

std::string Topology::GetTopology() { return topology_; }
}  // namespace eff

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

#ifndef UNITTESTS_SETUP_DIM_JSON_H
#define UNITTESTS_SETUP_DIM_JSON_H

#define SETUP_DIM_JSON                                                                                              \
    nlohmann::json user_j = {                                                                                       \
        {"pairs", nlohmann::json::array(                                                                            \
                      {nlohmann::json::array({"mymon1", "mymon2"}), nlohmann::json::array({"mymon1", "mymon3"})})}, \
        {"types1", nlohmann::json::array({nlohmann::json::array({"a", "b"}), nlohmann::json::array({"a", "b"})})},  \
        {"types2", nlohmann::json::array({nlohmann::json::array({"c", "d"}), nlohmann::json::array({"e", "f"})})},  \
        {"c6",                                                                                                      \
         {nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "c"}), 100.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "d"}), 200.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "c"}), 300.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "d"}), 400.00})}),              \
          nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "e"}), 500.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "f"}), 600.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "e"}), 700.00}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "f"}), 800.00})})}},            \
        {"d6",                                                                                                      \
         {nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "c"}), 1.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "d"}), 2.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "c"}), 3.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "d"}), 4.0000})}),              \
          nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "e"}), 5.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "f"}), 6.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "e"}), 7.0000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "f"}), 8.0000})})}},            \
        {"a",                                                                                                       \
         {nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "c"}), 1000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "d"}), 2000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "c"}), 3000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "d"}), 4000.0})}),              \
          nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "e"}), 5000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "f"}), 6000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "e"}), 7000.0}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "f"}), 8000.0})})}},            \
        {"eps",                                                                                                     \
         {nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "c"}), 10.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "d"}), 20.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "c"}), 30.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "d"}), 40.000})}),              \
          nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "e"}), 50.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "f"}), 60.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "e"}), 70.000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "f"}), 80.000})})}},            \
        {"sigma",                                                                                                   \
         {nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "c"}), 0.1000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "d"}), 0.2000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "c"}), 0.3000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "d"}), 0.4000})}),              \
          nlohmann::json::array({nlohmann::json::array({nlohmann::json::array({"a", "e"}), 0.5000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"a", "f"}), 0.6000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "e"}), 0.7000}),                \
                                 nlohmann::json::array({nlohmann::json::array({"b", "f"}), 0.8000})})}}};
#endif

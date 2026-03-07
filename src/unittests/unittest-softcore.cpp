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

#include "tools/testutils.h"

#include "bblock/system.h"
#include "json/json.hpp"

#include <string>
#include <vector>

namespace {

void CreateSystem(bblock::System &s, const std::vector<std::string> &mon_ids, const std::vector<size_t> &nats,
                  const std::vector<size_t> &first_index, const std::vector<double> &coords,
                  const std::vector<std::string> &atom_names, const nlohmann::json &j) {
    for (size_t i = 0; i < mon_ids.size(); i++) {
        size_t numat = nats[i];
        size_t fi = first_index[i];
        std::vector<double> xyz(coords.begin() + 3 * fi, coords.begin() + 3 * (fi + numat));
        std::vector<std::string> atoms(atom_names.begin() + fi, atom_names.begin() + fi + numat);
        s.AddMonomer(xyz, atoms, mon_ids[i]);
    }

    s.Initialize();
    s.SetUpFromJson(j);
}

struct IonSoftcoreEval {
    double energy;
    std::vector<double> grads;
};

struct ExpectedLambdaCase {
    std::string ion_id;
    std::string ion_atom_name;
    std::string component;
    double lambda;
    double expected_energy;
    std::vector<double> expected_grads;
};

bblock::System BuildIonSoftcoreSystem(const std::string &ion_id, const std::string &ion_atom_name, double elec_lambda,
                                      const std::string &selector, double two_b_lambda, double three_b_lambda) {
    std::vector<std::string> mon_ids = {ion_id, "h2o", "h2o"};
    std::vector<size_t> nats = {1, 3, 3};
    std::vector<size_t> first_index = {0, 1, 4};

    // 10th frame from TEST_softcore/ion_2waters.lammpstrj
    std::vector<double> coords = {
        27.87168, 25.00000, 25.00000,  // ion
        25.00000, 26.00000, 25.00000,  // water 1 O
        24.24094, 26.58773, 25.00000,  // water 1 H
        25.75906, 26.58773, 25.00000,  // water 1 H
        25.00000, 24.00000, 25.00000,  // water 2 O
        25.75906, 23.41227, 25.00000,  // water 2 H
        24.24094, 23.41227, 25.00000   // water 2 H
    };
    std::vector<std::string> atom_names = {ion_atom_name, "O", "H", "H", "O", "H", "H"};

    nlohmann::json j = {
        {"Note", "Electrostatics softcore unit test"},
        {"MBX",
         {{"twobody_cutoff", 9.0},
          {"threebody_cutoff", 7.0},
          {"dipole_tolerance", 1E-8},
          {"dipole_max_it", 100},
          {"dipole_method", "cg"},
          {"alpha_ewald_elec", 0.60},
          {"grid_density_elec", 2.5},
          {"spline_order_elec", 6},
          {"alpha_ewald_disp", 0.60},
          {"grid_density_disp", 2.5},
          {"spline_order_disp", 6},
          {"ignore_2b_poly", nlohmann::json::array()},
          {"ignore_3b_poly", nlohmann::json::array()},
          {"elec_lambda", elec_lambda},
          {"elec_lambda_monomer", selector},
          {"two_b_lambda", two_b_lambda},
          {"three_b_lambda", three_b_lambda}}}
    };

    bblock::System system;
    CreateSystem(system, mon_ids, nats, first_index, coords, atom_names, j);
    return system;
}

IonSoftcoreEval EvaluateIonComponent(const ExpectedLambdaCase &test_case) {
    bblock::System system;
    if (test_case.component == "elec") {
        system =
            BuildIonSoftcoreSystem(test_case.ion_id, test_case.ion_atom_name, test_case.lambda, test_case.ion_id, 1.0, 1.0);
        return {system.Electrostatics(true), system.GetRealGrads()};
    }
    if (test_case.component == "2b") {
        system = BuildIonSoftcoreSystem(test_case.ion_id, test_case.ion_atom_name, 1.0, "", test_case.lambda, 1.0);
        return {system.TwoBodyEnergy(true), system.GetRealGrads()};
    }

    system = BuildIonSoftcoreSystem(test_case.ion_id, test_case.ion_atom_name, 1.0, "", 1.0, test_case.lambda);
    return {system.ThreeBodyEnergy(true), system.GetRealGrads()};
}

void CheckIonSoftcoreReferenceCases(const std::vector<ExpectedLambdaCase> &test_cases) {
    const double energy_tol = 1.0e-10;
    const double grad_tol = 1.0e-8;

    for (const auto &test_case : test_cases) {
        DYNAMIC_SECTION("Reference check for " << test_case.ion_id << " " << test_case.component << " lambda "
                                               << test_case.lambda) {
            const IonSoftcoreEval actual = EvaluateIonComponent(test_case);
            REQUIRE(actual.energy == Approx(test_case.expected_energy).margin(energy_tol));
            REQUIRE(actual.grads.size() == test_case.expected_grads.size());
            for (size_t i = 0; i < actual.grads.size(); ++i) {
                REQUIRE(actual.grads[i] == Approx(test_case.expected_grads[i]).margin(grad_tol));
            }
        }
    }
}

}  // namespace

TEST_CASE("Softcore electrostatics reference values for li and cl ions") {
    const std::vector<ExpectedLambdaCase> test_cases = {
        {
            "li+", "Li", "elec", 1.0, -1.915711830750543e+00,
            {7.726871735064572e+00, -2.174208644357697e-14, 0.000000000000000e+00, -1.586861557094438e+01, 8.836006503866180e+00, 0.000000000000000e+00, 7.883970043368276e-01, -2.354349259120170e-01, 0.000000000000000e+00, 1.121678269907528e+01, -1.707617735150889e+01, 0.000000000000000e+00, -1.586861557094445e+01, -8.836006503866194e+00, 0.000000000000000e+00, 1.121678269907529e+01, 1.707617735150890e+01, 0.000000000000000e+00, 7.883970043368507e-01, 2.354349259120414e-01, 0.000000000000000e+00}
        },
        {
            "li+", "Li", "elec", 5.000000000000000e-01, 2.996904981439922e+00,
            {3.256717888960621e+00, -9.287077376937208e-15, 0.000000000000000e+00, -5.335027546746884e+00, 2.635214615855676e+00, 0.000000000000000e+00, 9.343925603795267e-03, -4.368175995407313e-01, 0.000000000000000e+00, 3.697324676662780e+00, -5.644973235207490e+00, 0.000000000000000e+00, -5.335027546746907e+00, -2.635214615855713e+00, 0.000000000000000e+00, 3.697324676662777e+00, 5.644973235207514e+00, 0.000000000000000e+00, 9.343925603809478e-03, 4.368175995407571e-01, 0.000000000000000e+00}
        },
        {
            "li+", "Li", "elec", 0.000000000000000e+00, 5.043588830662299e+00,
            {-2.750626807849214e-01, -1.296342019377001e-15, 0.000000000000000e+00, 3.500152423349390e-01, 7.234544725182506e-01, 0.000000000000000e+00, -4.558617032782393e-01, -1.016006516748384e+00, 0.000000000000000e+00, 2.433778013357619e-01, -8.498981613546892e-01, 0.000000000000000e+00, 3.500152423349361e-01, -7.234544725183154e-01, 0.000000000000000e+00, 2.433778013357493e-01, 8.498981613547247e-01, 0.000000000000000e+00, -4.558617032782251e-01, 1.016006516748416e+00, 0.000000000000000e+00}
        },
        {
            "cl-", "Cl", "elec", 1.000000000000000e+00, -8.948432690648591e-01,
            {-1.281510423621867e+00, 2.340157298972320e-14, 0.000000000000000e+00, 1.195130340992856e+01, -1.800754762224278e+00, 0.000000000000000e+00, 1.570588628629473e+00, -4.271599070713263e+00, 0.000000000000000e+00, -1.288113682674712e+01, 1.362517642350818e+01, 0.000000000000000e+00, 1.195130340992863e+01, 1.800754762224150e+00, 0.000000000000000e+00, -1.288113682674717e+01, -1.362517642350812e+01, 0.000000000000000e+00, 1.570588628629485e+00, 4.271599070713320e+00, 0.000000000000000e+00}
        },
        {
            "cl-", "Cl", "elec", 5.000000000000000e-01, 5.426319746397567e+00,
            {-1.234272600675531e+00, 1.231282763459282e-14, 0.000000000000000e+00, 3.938017979000587e+00, -2.593698624402130e-02, 0.000000000000000e+00, -4.858906874186824e-01, -1.940881626844426e+00, 0.000000000000000e+00, -2.834990991244145e+00, 3.148892159229261e+00, 0.000000000000000e+00, 3.938017979000613e+00, 2.593698624392893e-02, 0.000000000000000e+00, -2.834990991244173e+00, -3.148892159229221e+00, 0.000000000000000e+00, -4.858906874186704e-01, 1.940881626844461e+00, 0.000000000000000e+00}
        },
        {
            "cl-", "Cl", "elec", 0.000000000000000e+00, 5.036106603817563e+00,
            {2.936238439668171e-01, 1.449097404363629e-15, 0.000000000000000e+00, -4.465530787007281e-01, 7.963890147487174e-01, 0.000000000000000e+00, -4.821925466487516e-01, -1.004589372095241e+00, 0.000000000000000e+00, 7.819337033660707e-01, -1.305471112384156e+00, 0.000000000000000e+00, -4.465530787007258e-01, -7.963890147487778e-01, 0.000000000000000e+00, 7.819337033660552e-01, 1.305471112384187e+00, 0.000000000000000e+00, -4.821925466487378e-01, 1.004589372095273e+00, 0.000000000000000e+00}
        }
    };

    CheckIonSoftcoreReferenceCases(test_cases);
}

TEST_CASE("Softcore 2b reference values for li and cl ions") {
    const std::vector<ExpectedLambdaCase> test_cases = {
        {
            "li+", "Li", "2b", 1.000000000000000e+00, 3.075020991060432e+01,
            {-2.558129504117193e+00, -1.527666881884215e-13, 2.686739719592879e-14, 1.599383981710652e+00, -1.217653811383563e+02, 9.376499435306538e-12, 1.142821225127017e+00, 3.833339154369920e+00, -3.221382149702614e-12, -1.463140454780353e+00, 4.256740670966019e+00, -3.120211625488596e-12, 1.599383981708024e+00, 1.217653811383760e+02, -7.087676407808838e-12, -1.463140454766285e+00, -4.256740670979137e+00, 1.912288392898414e-12, 1.142821225118147e+00, -3.833339154376194e+00, 2.115391314438568e-12}
        },
        {
            "li+", "Li", "2b", 5.000000000000000e-01, 3.022730288918967e+01,
            {-1.279064752058597e+00, -7.638334409421077e-14, 1.343369859796439e-14, 7.996919908576710e-01, -1.217433477482264e+02, 9.399765739298905e-12, 8.034710311066748e-01, 3.995155961229461e+00, -3.245079470147103e-12, -9.636306459362334e-01, 4.206856719523445e+00, -3.111176380595630e-12, 7.996919908548721e-01, 1.217433477482460e+02, -7.074667001310104e-12, -9.636306459222075e-01, -4.206856719536558e+00, 1.917256493122878e-12, 8.034710310978241e-01, -3.995155961235723e+00, 2.102243277872490e-12}
        },
        {
            "li+", "Li", "2b", 0.000000000000000e+00, 2.970439586777502e+01,
            {0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 4.689804100621586e-12, -1.217213143580965e+02, 9.423032043291273e-12, 4.641208370863330e-01, 4.156972768089002e+00, -3.268776790591592e-12, -4.641208370921133e-01, 4.156972768080870e+00, -3.102141135702664e-12, 1.720401598959143e-12, 1.217213143581160e+02, -7.061657594811371e-12, -4.641208370781298e-01, -4.156972768093979e+00, 1.922224593347341e-12, 4.641208370775010e-01, -4.156972768095251e+00, 2.089095241306412e-12}
        },
        {
            "cl-", "Cl", "2b", 1.000000000000000e+00, 3.523456340593054e+01,
            {-1.997083126796588e+01, 4.263256414560601e-14, -5.329070518200751e-15, 5.241243464860389e+00, -1.209806645889499e+02, 9.427108106368300e-12, 4.188944048279093e+00, 2.680577176200642e+00, -3.273220486763231e-12, 5.552281208424548e-01, -3.588457609090838e-01, -3.099997145768651e-12, 5.241243464857306e+00, 1.209806645889695e+02, -7.053505468657317e-12, 5.552281208564134e-01, 3.588457608959059e-01, 1.926512573215366e-12, 4.188944048270250e+00, -2.680577176206896e+00, 2.080207848963134e-12}
        },
        {
            "cl-", "Cl", "2b", 5.000000000000000e-01, 3.246947963685278e+01,
            {-9.985415633982939e+00, 2.131628207280301e-14, -2.664535259100376e-15, 2.620621732432539e+00, -1.213509894735232e+02, 9.425070074829787e-12, 2.326532442682713e+00, 3.418774972144822e+00, -3.270998638677411e-12, 4.555364187517075e-02, 1.899063503585893e+00, -3.101069140735658e-12, 2.620621732429513e+00, 1.213509894735428e+02, -7.057581531734344e-12, 4.555364188914179e-02, -1.899063503599037e+00, 1.924368583281354e-12, 2.326532442673876e+00, -3.418774972151073e+00, 2.084651545134773e-12}
        },
        {
            "cl-", "Cl", "2b", 0.000000000000000e+00, 2.970439586777502e+01,
            {0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 4.689804100621586e-12, -1.217213143580965e+02, 9.423032043291273e-12, 4.641208370863330e-01, 4.156972768089002e+00, -3.268776790591592e-12, -4.641208370921133e-01, 4.156972768080870e+00, -3.102141135702664e-12, 1.720401598959143e-12, 1.217213143581160e+02, -7.061657594811371e-12, -4.641208370781298e-01, -4.156972768093979e+00, 1.922224593347341e-12, 4.641208370775010e-01, -4.156972768095251e+00, 2.089095241306412e-12}
        }
    };

    CheckIonSoftcoreReferenceCases(test_cases);
}

TEST_CASE("Softcore 3b reference values for li and cl ions") {
    const std::vector<ExpectedLambdaCase> test_cases = {
        {
            "li+", "Li", "3b", 1.000000000000000e+00, -4.333300402887006e+00,
            {4.468865956159273e+00, 7.047695760320494e-13, 0.000000000000000e+00, 1.758551732237270e+01, 1.582366038035899e+01, 0.000000000000000e+00, 6.286616041888844e+00, 2.590893059174300e+00, 0.000000000000000e+00, -2.610656634234235e+01, -7.542853799858815e+00, 0.000000000000000e+00, 1.758551732237328e+01, -1.582366038036405e+01, 0.000000000000000e+00, -2.610656634233933e+01, 7.542853799857415e+00, 0.000000000000000e+00, 6.286616041887591e+00, -2.590893059168543e+00, 0.000000000000000e+00}
        },
        {
            "li+", "Li", "3b", 5.000000000000000e-01, -2.166650201443503e+00,
            {2.234432978079636e+00, 3.523847880160247e-13, 0.000000000000000e+00, 8.792758661186348e+00, 7.911830190179494e+00, 0.000000000000000e+00, 3.143308020944422e+00, 1.295446529587150e+00, 0.000000000000000e+00, -1.305328317117118e+01, -3.771426899929407e+00, 0.000000000000000e+00, 8.792758661186642e+00, -7.911830190182025e+00, 0.000000000000000e+00, -1.305328317116966e+01, 3.771426899928707e+00, 0.000000000000000e+00, 3.143308020943795e+00, -1.295446529584272e+00, 0.000000000000000e+00}
        },
        {
            "li+", "Li", "3b", 0.000000000000000e+00, 0.000000000000000e+00,
            {0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00}
        },
        {
            "cl-", "Cl", "3b", 1.000000000000000e+00, -3.175705463326803e+00,
            {3.625086252854212e+00, -2.300243329145246e-14, 3.686287386450715e-17, -2.884650424691095e+00, 7.647016250742796e+00, -2.393845218179795e-14, -2.897253556212001e+00, -1.366425855399296e+00, 1.048660442177844e-14, 3.969360854475952e+00, 1.426707647313394e+00, 9.677089476293979e-15, -2.884650424691017e+00, -7.647016250742739e+00, -2.848409508834043e-15, 3.969360854475942e+00, -1.426707647313421e+00, 3.540601410065798e-15, -2.897253556211991e+00, 1.366425855399278e+00, 2.971544080031262e-15}
        },
        {
            "cl-", "Cl", "3b", 5.000000000000000e-01, -1.587852731663402e+00,
            {1.812543126427106e+00, -1.150121664572623e-14, 1.843143693225358e-17, -1.442325212345547e+00, 3.823508125371398e+00, -1.196922609089898e-14, -1.448626778106000e+00, -6.832129276996479e-01, 5.243302210889220e-15, 1.984680427237976e+00, 7.133538236566972e-01, 4.838544738146989e-15, -1.442325212345508e+00, -3.823508125371370e+00, -1.424204754417021e-15, 1.984680427237971e+00, -7.133538236567107e-01, 1.770300705032899e-15, -1.448626778105995e+00, 6.832129276996388e-01, 1.485772040015631e-15}
        },
        {
            "cl-", "Cl", "3b", 0.000000000000000e+00, 0.000000000000000e+00,
            {0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00, 0.000000000000000e+00}
        }
    };

    CheckIonSoftcoreReferenceCases(test_cases);
}

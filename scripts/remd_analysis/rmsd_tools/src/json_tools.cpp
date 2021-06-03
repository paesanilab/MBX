#include "json_tools.h"

namespace rmsd_tools {
nlohmann::json SetUpRscFromJson(char *json_file) {
    nlohmann::json j_default = {{"threshold", 0.01},
                                {"indexes_to_ignore", nlohmann::json::array()},
                                {"permutation_file", ""},
                                {"use_inversion", true},
                                {"output_file", "rsc.xyz"}};

    if (json_file == 0) {
        return j_default;
    } else {
        std::ifstream iff;
        iff.open(json_file);
        nlohmann::json j = nlohmann::json::parse(iff);
        return j;
    }
}

}  // namespace rmsd_tools

#ifndef JSON_TOOLS_RSC_H
#define JSON_TOOLS_RSC_H

#include "json/json.h"
#include <fstream>

namespace rmsd_tools {
nlohmann::json SetUpRscFromJson(char *json_file = 0);
nlohmann::json SetUpPopsFromJson(char *json_file = 0);

}  // namespace rmsd_tools

#endif

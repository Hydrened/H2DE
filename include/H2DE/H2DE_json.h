#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool H2DE_CreateJsonFile(const std::filesystem::path& path, bool override, bool encode);
bool H2DE_CreateJsonFile(const std::filesystem::path& path, const json& data, bool override, bool encode);
json H2DE_ReadJsonFile(const std::filesystem::path& path);
bool H2DE_WriteJsonFile(const std::filesystem::path& path, const json& data, bool encode);

#endif

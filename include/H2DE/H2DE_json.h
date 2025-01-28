#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <exception>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

json H2DE_ReadJson(std::filesystem::path file);
void H2DE_WriteJson(std::filesystem::path file, json data, bool encode = true);

#endif

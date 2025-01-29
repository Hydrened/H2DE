#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <exception>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * Reads a JSON file
 * \param file path to the file
 * \returns the JSON data
 * \since H2DE-1.3.16
 */
json H2DE_ReadJson(std::filesystem::path file);
/**
 * Writes data to a JSON file
 * \param file path to the file
 * \param data the JSON data to write
 * \param encode whether the data should be encoded (true by default)
 * \since H2DE-1.3.16
 */
void H2DE_WriteJson(std::filesystem::path file, json data, bool encode = true);

#endif

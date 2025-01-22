#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <exception>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * Reads a json file
 * \param file the name of the file
 * \return the json data
 * \since H2DE-1.3.16
 */
json H2DE_ReadJson(std::string file);
/**
 * Write data in a json file
 * \param file the name of the file
 * \param json a pointer to the json data
 * \return true on sucess and false otherwise
 * \since H2DE-1.3.16
 */
bool H2DE_WriteJson(std::string file, json* json);

#endif

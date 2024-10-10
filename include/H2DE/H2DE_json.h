#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <exception>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * Contains the methods required by the engine to open and write json files
 * \since H2DE-1.0.0
 */
class H2DE_Json {
public:
    /**
     * Reads a json file
     * \param file the name of the file
     * \return the json data
     * \since H2DE-1.0.0
     */
    static json* read(std::string file);
    /**
     * Write data in a json file
     * \param file the name of the file
     * \param j a pointer to the json data
     * \param dump number of space for indentation
     * \return true on sucess and false otherwise
     * \since H2DE-1.0.0
     */
    static bool write(std::string file, json* j, unsigned int dump);
};

#endif

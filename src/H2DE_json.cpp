#include "H2DE/H2DE_json.h"

json H2DE_ReadJson(std::filesystem::path file) {
    json res = "";
    std::ifstream inputFile(file);

    if (inputFile.is_open()) {
        std::string fileContent;
        inputFile >> fileContent;
        inputFile.close();

        try {
            res = json::parse(base64_decode(fileContent));
        } catch (const std::exception& e) {
            res = json::parse(fileContent);
        }

    } else throw std::runtime_error("H2DE-201 => Error opening the file");
    return res;
}

void H2DE_WriteJson(std::filesystem::path file, json data, bool encode) {
    std::ofstream outputFile(file);
    if (!outputFile.is_open()) throw std::runtime_error("H2DE-202 => Error opening the file");

    if (encode) outputFile << base64_encode(data.dump());
    else outputFile << data.dump();

    if (outputFile.fail()) throw std::runtime_error("H2DE-301 => Error writing the file");
    outputFile.close();
}

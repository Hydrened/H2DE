#include <H2DE/H2DE_json.h>

json H2DE_ReadJson(std::string file) {
    json res = "";
    std::string encodedRes;
    std::ifstream inputFile(file);
    if (inputFile.is_open()) {
        inputFile >> encodedRes;
        inputFile.close();
        res = json::parse(base64_decode(encodedRes));
    } else throw std::runtime_error("ENGINE => Error opening the file");
    return res;
}

bool H2DE_WriteJson(std::string file, json* json) {
    std::ofstream outputFile(file);
    if (!outputFile.is_open()) return false;
    outputFile << base64_encode(json->dump());
    if (outputFile.fail()) return false;
    outputFile.close();
    return true;
}

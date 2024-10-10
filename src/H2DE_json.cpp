#include <H2DE/H2DE_json.h>

json* H2DE_Json::read(std::string file) {
    json* res = new json;
    std::ifstream inputFile(file);
    if (inputFile.is_open()) {
        inputFile >> *res;
        inputFile.close();
    } else throw std::runtime_error("ENGINE => Error opening the file");
    return res;
}

bool H2DE_Json::write(std::string file, json* j, unsigned int dump) {
    std::ofstream outputFile(file);
    if (!outputFile.is_open()) return false;
    outputFile << j->dump(dump);
    if (outputFile.fail()) return false;
    outputFile.close();
    return true;
}

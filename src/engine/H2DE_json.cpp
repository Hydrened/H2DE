#include "H2DE/H2DE_json.h"

// CREATE
bool H2DE_CreateJsonFile(const std::filesystem::path& path, bool override, bool encode) {
    if (!override && std::filesystem::exists(path)) {
        return false;
    }

    return H2DE_WriteJsonFile(path, json{}, encode);
}

bool H2DE_CreateJsonFile(const std::filesystem::path& path, const json& data, bool override, bool encode) {
    if (!override && std::filesystem::exists(path)) {
        return false;
    }

    return H2DE_WriteJsonFile(path, data, encode);
}

// READ
json H2DE_ReadJsonFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: File not found: " << path << std::endl;
        return json{};
    }

    std::ifstream file(path);

    if (!file) {
        std::cerr <<  "H2DE => \033[31mERROR\033[0m: Failed to open file: " << path << std::endl;
        return json{};
    }

    std::string fileContent;
    file >> fileContent;
    file.close();

    try {
        json data;

        try {
            data = json::parse(fileContent);
        } catch (const std::exception& encodeError) {
            data = json::parse(base64_decode(fileContent));
        }

        if (data.is_discarded()) {
            std::cerr << "H2DE => \033[31mERROR\033[0m: Invalid JSON format in file: " << path << std::endl;
            return json{};
        }

        return data;

    } catch (const std::exception& parseError) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: Failed to parse JSON: " << parseError.what() << std::endl;
        return json{};
    }
}

// WRITE
bool H2DE_WriteJsonFile(const std::filesystem::path& path, const json& data, bool encode) {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: Failed to open file for writing: " << path << std::endl;
        return false;
    }

    if (encode) {
        file << base64_encode(data.dump());
    } else {
        file << data;
    }

    if (!file.good()) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: Failed to write JSON to file: " << path << std::endl;
        return false;
    }

    return true;
}

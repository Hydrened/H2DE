#include "H2DE/H2DE_json.h"
#include "H2DE/H2DE_error.h"

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
        H2DE_Error::logError("File not found:" + path.string());
        return json{};
    }

    std::ifstream file(path);

    if (!file) {
        H2DE_Error::logError("Failed to open file: " + path.string());
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
            H2DE_Error::logError("Invalid JSON format in file: " + path.string());
            return json{};
        }

        return data;

    } catch (const std::exception& parseError) {
        H2DE_Error::logError("Failed to parse JSON: " + std::string(parseError.what()));
        return json{};
    }
}

// WRITE
bool H2DE_WriteJsonFile(const std::filesystem::path& path, const json& data, bool encode) {
    std::ofstream file(path);
    if (!file) {
        H2DE_Error::logError("Failed to open file for writing: " + path.string());
        return false;
    }

    if (encode) {
        file << base64_encode(data.dump());
    } else {
        file << data;
    }

    if (!file.good()) {
        H2DE_Error::logError("Failed to write JSON to file: " + path.string());
        return false;
    }

    return true;
}

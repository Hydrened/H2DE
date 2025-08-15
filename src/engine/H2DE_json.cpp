#include "H2DE/engine/H2DE_json.h"
#include "H2DE/engine/H2DE_codec.h"
#include "H2DE/engine/H2DE_error.h"

// CREATE
bool H2DE::Json::create(const std::filesystem::path& path, bool override, bool encode) {
    if (!override && std::filesystem::exists(path)) {
        return false;
    }

    return H2DE::Json::write(path, json{}, encode);
}

bool H2DE::Json::create(const std::filesystem::path& path, const json& data, bool override, bool encode) {
    if (!override && std::filesystem::exists(path)) {
        return false;
    }

    return H2DE::Json::write(path, data, encode);
}

// READ
json H2DE::Json::read(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        H2DE_Error::logError("File not found:" + path.string());
        return json{};
    }

    std::ifstream file(path, std::ios::binary);

    if (!file) {
        H2DE_Error::logError("Failed to open file: " + path.string());
        return json{};
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    try {
        json data;

        const std::string encodedSignature = "H2DEENC";
        bool isEncoded = (fileContent.size() >= encodedSignature.length())
            ? (fileContent.substr(0, encodedSignature.length()) == encodedSignature)
            : false;

        if (isEncoded) {
            data = json::parse(H2DE_Codec::decode(fileContent.substr(encodedSignature.length())));
        } else {
            data = json::parse(fileContent);
        }

        return data;

    } catch (const std::exception& parseError) {
        H2DE_Error::logError("Failed to parse JSON: " + std::string(parseError.what()));
        return json{};
    }
}

// WRITE
bool H2DE::Json::write(const std::filesystem::path& path, const json& data, bool encode) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        H2DE_Error::logError("Failed to open file for writing: " + path.string());
        return false;
    }

    const std::string encodedSignature = "H2DEENC";

    if (encode) {
        file << encodedSignature;
        file << H2DE_Codec::encode(data.dump());

    } else {
        file << data.dump();
    }

    if (!file.good()) {
        H2DE_Error::logError("Failed to write JSON to file: " + path.string());
        return false;
    }

    return true;
}

#include "H2DE/H2DE_settings.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Settings::H2DE_Settings(H2DE_Engine* e) : engine(e) {
    initFile();
    refreshValues();
}

void H2DE_Settings::initFile() {
    if (!std::filesystem::exists(path)) {
        std::ofstream file(path);
    }
}

void H2DE_Settings::refreshValues() {
    std::ifstream file(path);
    if (!file) {
        H2DE_Error::throwError("Error reading settings.ini");
    }

    values.clear();
    std::string line;
    std::optional<std::string> currentSection = std::nullopt;

    while (std::getline(file, line)) {

        if (isSection(line)) {
            currentSection = line.substr(1, line.length() - 2);
            values[currentSection.value()] = {};
            continue;
        }

        if (!currentSection.has_value()) {
            continue;
        }

        if (!isKey(line)) {
            continue;
        }

        const std::pair<std::string, std::string> value = getKeyAndValue(line);
        values[currentSection.value()][value.first] = value.second;
    }
}

void H2DE_Settings::refreshFile() {
    std::ofstream file(path, std::ios::trunc);

    if (!file.is_open()) {
        H2DE_Error::throwError("Error reading settings.ini");
        return;
    }

    bool isFirstLine = true;
    for (const auto& [section, keys] : values) {

        if (!isFirstLine) {
            file << "\n\n";
        } else {
            isFirstLine = false;
        }

        file << "[" + section + "]";

        for (const auto& [key, value] : keys) {
            file << "\n" + key + "=" + value;
        }
    }

    file.close();
    refreshValues();
}

// CLEANUP
H2DE_Settings::~H2DE_Settings() {

}

// ACTIONS
// -- add
bool H2DE_Settings::addSection(const std::string& name) {
    if (hasSection(name)) {
        return false;
    }

    std::ofstream file(path, std::ios::app);

    if (file.is_open()) {
        file << "\n\n";
        file << "[" + name + "]";
        file.close();

        refreshValues();
        return true;

    } else {
        H2DE_Error::throwError("Error reading settings.ini");
        return false;
    }
}

bool H2DE_Settings::addKey(const std::string& section, const std::string& key, const std::string& value) {
    if (hasKey(section, key)) {
        return false;
    }

    values[section][key] = value;
    refreshFile();

    return true;
}

// GETTER
bool H2DE_Settings::hasSection(const std::string& section) const {
    auto it = values.find(section);
    return (it != values.end());
}

bool H2DE_Settings::hasKey(const std::string& section, const std::string& key) const {
    if (!hasSection(section)) {
        return false;
    }

    auto it = values.at(section).find(key);
    return (it != values.at(section).end());
}

bool H2DE_Settings::isSection(const std::string& line) const {
    if (line.length() == 0) {
        return false;
    }

    bool isNameValid = (line.find('=') == std::string::npos);
    bool isLineASection = (line.substr(0, 1) == "[" && line.substr(line.length() - 1) == "]");

    return (isNameValid && isLineASection);
}

bool H2DE_Settings::isKey(const std::string& line) const {
    if (line.length() == 0) {
        return false;
    }

    const size_t equalPos = line.find('=');
    if (equalPos == std::string::npos) {
        return false;
    }

    bool hasKey = (equalPos != 0);
    return hasKey;
}

const std::pair<std::string, std::string> H2DE_Settings::getKeyAndValue(const std::string& line) const {
    const size_t equalPos = line.find('=');
    return { line.substr(0, equalPos), line.substr(equalPos + 1) };
}

std::string H2DE_Settings::getKeyString(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    if (!hasKey(section, key)) {
        return defaultValue;
    }

    return values.at(section).at(key);
}

int H2DE_Settings::getKeyInteger(const std::string& section, const std::string& key, int defaultValue) const {
    if (!hasKey(section, key)) {
        return defaultValue;
    }
    
    try {
        int value = std::stoi(values.at(section).at(key));
        return value;

    } catch (const std::exception& e) {
        H2DE_Error::throwError("Error converting to int: " + std::string(e.what()));
        return defaultValue;
    }
}

bool H2DE_Settings::getKeyBoolean(const std::string& section, const std::string& key, bool defaultValue) const {
    if (!hasKey(section, key)) {
        return defaultValue;
    }

    return (values.at(section).at(key) == "true");
}

// SETTER
bool H2DE_Settings::setKeyValue(const std::string& section, const std::string& key, const std::string& value) {
    if (!hasKey(section, key)) {
        return false;
    }

    values[section][key] = value;
    refreshFile();

    return true;
}

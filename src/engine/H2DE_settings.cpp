#include "H2DE/engine/H2DE_settings.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Settings::H2DE_Settings() {
    _initFile();
    _refreshValues();
}

void H2DE_Settings::_initFile() {
    if (!std::filesystem::exists(_path)) {
        std::ofstream file(_path);
    }
}

void H2DE_Settings::_refreshValues() {
    std::ifstream file(_path);
    if (!file) {
        H2DE_Error::throwError("Error reading settings.ini");
    }

    _values.clear();
    std::string line;
    std::optional<std::string> currentSection = std::nullopt;

    while (std::getline(file, line)) {

        if (_isSection(line)) {
            currentSection = line.substr(1, line.length() - 2);
            _values[currentSection.value()] = {};
            continue;
        }

        if (!currentSection.has_value()) {
            continue;
        }

        if (!_isKey(line)) {
            continue;
        }

        const std::pair<std::string, std::string> value = _getKeyAndValue(line);
        _values[currentSection.value()][value.first] = value.second;
    }
}

void H2DE_Settings::_refreshFile() {
    std::ofstream file(_path, std::ios::trunc);

    if (!file.is_open()) {
        H2DE_Error::throwError("Error reading settings.ini");
        return;
    }

    bool isFirstLine = true;
    for (const auto& [section, keys] : _values) {

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
    _refreshValues();
}

// ACTIONS
bool H2DE_Settings::addSection(const std::string& name) {
    if (_hasSection(name)) {
        return false;
    }

    std::ofstream file(_path, std::ios::app);

    if (file.is_open()) {
        file << "\n\n";
        file << "[" + name + "]";
        file.close();

        _refreshValues();
        return true;

    } else {
        H2DE_Error::throwError("Error reading settings.ini");
        return false;
    }
}

bool H2DE_Settings::addKey(const std::string& section, const std::string& key, const std::string& value) {
    if (_hasKey(section, key)) {
        return false;
    }

    _values[section][key] = value;
    _refreshFile();

    return true;
}

// GETTER
bool H2DE_Settings::_hasSection(const std::string& section) const {
    auto it = _values.find(section);
    return (it != _values.end());
}

bool H2DE_Settings::_hasKey(const std::string& section, const std::string& key) const {
    if (!_hasSection(section)) {
        return false;
    }

    auto it = _values.at(section).find(key);
    return (it != _values.at(section).end());
}

bool H2DE_Settings::_isSection(const std::string& line) const {
    if (line.length() == 0) {
        return false;
    }

    bool isNameValid = (line.find('=') == std::string::npos);
    bool isLineASection = (line.substr(0, 1) == "[" && line.substr(line.length() - 1) == "]");

    return (isNameValid && isLineASection);
}

bool H2DE_Settings::_isKey(const std::string& line) const {
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

const std::pair<std::string, std::string> H2DE_Settings::_getKeyAndValue(const std::string& line) const {
    const size_t equalPos = line.find('=');
    return { line.substr(0, equalPos), line.substr(equalPos + 1) };
}

std::string H2DE_Settings::getKeyString(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    if (!_hasKey(section, key)) {
        return defaultValue;
    }

    return _values.at(section).at(key);
}

int H2DE_Settings::getKeyInteger(const std::string& section, const std::string& key, int defaultValue) const {
    if (!_hasKey(section, key)) {
        return defaultValue;
    }

    try {
        int value = std::stoi(_values.at(section).at(key));
        return value;

    } catch (const std::exception& e) {
        H2DE_Error::throwError("Error converting to int: " + std::string(e.what()));
        return defaultValue;
    }
}

bool H2DE_Settings::getKeyBoolean(const std::string& section, const std::string& key, bool defaultValue) const {
    if (!_hasKey(section, key)) {
        return defaultValue;
    }

    return (_values.at(section).at(key) == "true");
}

// SETTER
bool H2DE_Settings::setKeyValue(const std::string& section, const std::string& key, const std::string& value) {
    if (!_hasKey(section, key)) {
        return false;
    }

    _values[section][key] = value;
    _refreshFile();

    return true;
}

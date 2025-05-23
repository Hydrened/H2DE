#include "H2DE/H2DE_settings.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Engine::H2DE_Settings::H2DE_Settings() {
    initFile();

    updateReader();

    if (reader->ParseError() != 0) {
        H2DE_Error::throwError("Error reading settings.ini");
    }
}

void H2DE_Engine::H2DE_Settings::initFile() {
    if (!std::filesystem::exists(path)) {
        std::ofstream file(path);
    }
}

// CLEANUP
H2DE_Engine::H2DE_Settings::~H2DE_Settings() {
    if (reader) {
        delete reader;
    }
}

// EVENTS
void H2DE_Engine::H2DE_Settings::updateReader() {
    reader = new INIReader(path.string());
}

// ADD
bool H2DE_Engine::H2DE_Settings::addLineAt(const std::string& newLine, size_t position) {
    std::ifstream file(path);
    if (!file) {
        H2DE_Error::throwError("Error reading settings.ini");
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    if (position > lines.size()) {
        position = lines.size();
    }
    lines.insert(lines.begin() + position, newLine);

    std::ofstream outputFile(path);
    if (!outputFile) {
        H2DE_Error::throwError("Error reading settings.ini");
        return false;
    }

    for (const std::string& line : lines) {
        outputFile << line << "\n";
    }

    updateReader();
    return true;
}

bool H2DE_SettingsAddSection(const H2DE_Engine* engine, const std::string& section) {
    H2DE_Error::checkEngine(engine);

    if (engine->settings->hasSection(section)) {
        return false;
    }

    engine->settings->addLineAt("[" + section + "]", -1);
    engine->settings->updateReader();

    return true;
}

bool H2DE_SettingsAddKey(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value) {
    H2DE_Error::checkEngine(engine);

    if (!engine->settings->hasSection(section)) {
        H2DE_SettingsAddSection(engine, section);
    }

    if (engine->settings->hasKey(section, key)) {
        return false;
    }

    std::string newLine = key + '=' + value;
    size_t position = engine->settings->getLastSectionPosition(section);
    return engine->settings->addLineAt(newLine, position + 1);
}

// GETTER
std::vector<std::string> H2DE_Engine::H2DE_Settings::getLines() const {
    std::ifstream file(path);
    if (!file) {
        H2DE_Error::throwError("Error reading settings.ini");
    }

    std::vector<std::string> res;
    std::string line;

    while (std::getline(file, line)) {
        res.push_back(line);
    }

    return res;
}

bool H2DE_Engine::H2DE_Settings::hasSection(const std::string& section) const {
    std::vector<std::string> lines = getLines();
    std::string line = '[' + section + ']';
    return std::find(lines.begin(), lines.end(), line) != lines.end();
}

bool H2DE_Engine::H2DE_Settings::hasKey(const std::string& section, const std::string& key) const {
    std::vector<std::string> lines = getLines();
    bool inGoodSection = false;

    for (int i = 0; i <= getLastSectionPosition(section); i++) {
        if (lines[i] == '[' + section + ']') {
            inGoodSection = true;

        } else if (inGoodSection) {
            size_t separator = lines[i].find('=');
            std::string k = lines[i].substr(0, separator);
            if (k == key) return true;
        }
    }

    return false;
}

size_t H2DE_Engine::H2DE_Settings::getLastSectionPosition(const std::string& section) const {
    std::vector<std::string> lines = getLines();
    bool eneteredSection = false;

    for (size_t i = 0; i < lines.size(); i++) {
        std::string line = lines[i];

        if (line.length() == 0) {
            continue;
        }

        if (line[0] != '[') {
            continue;
        }

        if (eneteredSection) {
            return i - 1;
        }

        if (line.substr(1, line.length() - 2) == section) {
            eneteredSection = true;
        }
    }

    return lines.size() - 1;
}

int H2DE_SettingsGetKeyInteger(const H2DE_Engine* engine, const std::string& section, const std::string& key, int defaultValue) {
    H2DE_Error::checkEngine(engine);
    return engine->settings->reader->GetInteger(section, key, defaultValue);
}

std::string H2DE_SettingsGetKeyString(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& defaultValue) {
    H2DE_Error::checkEngine(engine);
    return engine->settings->reader->GetString(section, key, defaultValue);
}

bool H2DE_SettingsGetKeyBool(const H2DE_Engine* engine, const std::string& section, const std::string& key, bool defaultValue) {
    H2DE_Error::checkEngine(engine);
    return engine->settings->reader->GetBoolean(section, key, defaultValue);
}

// SETTER
bool H2DE_SettingsSetKeyValue(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value) {
    H2DE_Error::checkEngine(engine);

    if (!engine->settings->hasKey(section, key)) {
        return H2DE_SettingsAddKey(engine, section, key, value);
    }

    std::vector<std::string> lines = engine->settings->getLines();
    
    for (int i = engine->settings->getLastSectionPosition(section); i >= 0; i--) {
        if (lines[i] == '[' + section + ']') {
            return false;
        }

        size_t separator = lines[i].find('=');
        std::string k = lines[i].substr(0, separator);

        if (k == key) {
            std::string newLine = key + '=' + value;
            lines[i] = newLine;
            break;
        }
    }

    std::ofstream outputFile(engine->settings->path);
    if (!outputFile) {
        H2DE_Error::throwError("Error reading settings.ini");
        return false;
    }

    for (const std::string& line : lines) {
        outputFile << line << "\n";
    }

    engine->settings->updateReader();
    return true;
}

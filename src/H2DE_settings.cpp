#include "H2DE/H2DE_settings.h"

// INIT
H2DE_Engine::H2DE_Settings::H2DE_Settings() {
    initFile();

    updateReader();

    if (reader->ParseError() != 0) {
        throw std::runtime_error("H2DE-201: Error reading settings.ini");
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
        throw std::runtime_error("H2DE-203: Error reading settings.ini");
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
        throw std::runtime_error("H2DE-204: Error reading settings.ini");
        return false;
    }

    for (const std::string& line : lines) {
        outputFile << line << "\n";
    }

    updateReader();
    return true;
}

bool H2DE_Engine::H2DE_Settings::addSection(const std::string& section) {
    if (hasSection(section)) {
        return false;
    }

    addLineAt("[" + section + "]", -1);
    updateReader();

    return true;
}

bool H2DE_Engine::H2DE_Settings::addKey(const std::string& section, const std::string& key, const std::string& value) {
    if (!hasSection(section)) {
        return false;
    }

    if (hasKey(section, key)) {
        return false;
    }

    std::string newLine = key + '=' + value;
    size_t position = getLastSectionPosition(section);
    return addLineAt(newLine, position + 1);
}

// GETTER
INIReader* H2DE_Engine::H2DE_Settings::getReader() const {
    return reader;
}

std::vector<std::string> H2DE_Engine::H2DE_Settings::getLines() const {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("H2DE-202 => Error reading settings.ini");
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

        if (line == "") {
            continue;
        }

        if (line[0] == '[') {
            if (!eneteredSection) {
                if (line.substr(1, line.length() - 2) == section) {
                    eneteredSection = true;
                }
            } else return i - 1;
        }
    }

    return lines.size() - 1;
}

// SETTER
bool H2DE_Engine::H2DE_Settings::setKeyValue(const std::string& section, const std::string& key, const std::string& value) {
    if (!hasSection(section)) {
        return false;
    }

    if (!hasKey(section, key)) {
        return false;
    }

    std::vector<std::string> lines = getLines();
    
    for (int i = getLastSectionPosition(section) - 1; i >= 0; i--) {
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

    std::ofstream outputFile(path);
    if (!outputFile) {
        throw std::runtime_error("H2DE-205: Error reading settings.ini");
        return false;
    }

    for (const std::string& line : lines) {
        outputFile << line << "\n";
    }

    updateReader();
    return true;
}

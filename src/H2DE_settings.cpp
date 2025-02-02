#include "H2DE_settings.h"

// INIT
H2DE_Settings::H2DE_Settings() {
    static bool once = false;
    if (once) throw std::runtime_error("H2DE-111: Can't create more than one settings file");
    once = true;

    reader = new INIReader(path.string());
}

void H2DE_InitSettings(H2DE_Settings* settings, const std::map<std::string, std::vector<std::string>>& data) {
    if (settings->settingsFileExists()) return;

    std::ofstream file(settings->path);
    if (!file.is_open()) throw std::runtime_error("H2DE-204 => Error creating settings.ini");

    for (const auto& [section, lines] : data) {
        file << "[" + section + "]" + '\n';
        for (const std::string& line : lines) file << line + '\n';
        file << '\n';
    }
}

// CLEANUP
H2DE_Settings::~H2DE_Settings() {
    delete reader;
}

// GETTER
bool H2DE_Settings::settingsFileExists() {
    return std::filesystem::exists(path);
}

INIReader* H2DE_GetSettingReader(H2DE_Settings* settings) {
    return settings->reader;
}

// SETTER
void H2DE_SetSettingValue(H2DE_Settings* settings, std::string section, std::string key, std::string value) {
    if (!settings->settingsFileExists()) return;

    std::ifstream readingFile(settings->path);
    if (!readingFile) throw std::runtime_error("H2DE-205 => Error reading settings.ini");

    std::vector<std::string> lines;
    std::string line;
    bool foundSection = false;
    bool passedSection = false;

    while (std::getline(readingFile, line)) {
        if (foundSection) {
            if (!passedSection) {
                std::string currentKey = line.substr(0, line.find('='));
                if (currentKey == key) line = key + '=' + value;
            }
            if (line[0] == '[') passedSection = true;

        } else if (line == '[' + section + ']' && !passedSection) foundSection = true;

        lines.push_back(line);
    }
    readingFile.close();

    std::ofstream writingFile(settings->path, std::ios::trunc);
    if (!writingFile.is_open()) throw std::runtime_error("H2DE-206 => Error creating settings.ini");

    for (const auto& l : lines) writingFile << l << '\n';
    writingFile.close();
}

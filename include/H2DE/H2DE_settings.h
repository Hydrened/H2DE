#ifndef H2DE_SETTINGS_H
#define H2DE_SETTINGS_H

#include <fstream>
#include <inih/INIReader.h>
#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Settings {
private:
    std::filesystem::path path = "settings.ini";
    INIReader* reader = nullptr;

    void initFile();

    void updateReader();

    bool addLineAt(const std::string& line, size_t position);

    std::vector<std::string> getLines() const;
    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    size_t getLastSectionPosition(const std::string& section) const;

public:
    H2DE_Settings();
    ~H2DE_Settings();

    friend bool H2DE_SettingsAddSection(H2DE_Engine* engine, const std::string& section);
    friend bool H2DE_SettingsAddKey(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend bool H2DE_SettingsSetKeyValue(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend int H2DE_SettingsGetKeyInteger(H2DE_Engine* engine, const std::string& section, const std::string& key, const int& defaultValue);
    friend std::string H2DE_SettingsGetKeyString(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& defaultValue);
    friend bool H2DE_SettingsGetKeyBool(H2DE_Engine* engine, const std::string& section, const std::string& key, const bool& defaultValue);
};

#endif

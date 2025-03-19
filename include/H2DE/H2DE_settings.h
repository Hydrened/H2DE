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

    bool addSection(const std::string& section);
    bool addKey(const std::string& section, const std::string& key, const std::string& value);
    
    INIReader* getReader() const;

    bool setKeyValue(const std::string& section, const std::string& key, const std::string& value);
};

#endif

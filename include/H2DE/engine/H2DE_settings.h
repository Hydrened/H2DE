#ifndef H2DE_SETTINGS_H
#define H2DE_SETTINGS_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Settings {
public:
    bool addSection(const std::string& name);
    bool addKey(const std::string& section, const std::string& key, const std::string& value);

    std::string getKeyString(const std::string& section, const std::string& key, const std::string& defaultValue) const;
    int getKeyInteger(const std::string& section, const std::string& key, int defaultValue) const;
    bool getKeyBoolean(const std::string& section, const std::string& key, bool defaultValue) const;

    bool setKeyValue(const std::string& section, const std::string& key, const std::string& value);

    friend class H2DE_Engine;

private:
    H2DE_Engine* engine;

    const std::filesystem::path path = "settings.ini";
    std::map<std::string, std::map<std::string, std::string>> values = {};

    H2DE_Settings(H2DE_Engine* engine);
    ~H2DE_Settings() = default;

    void initFile();
    void refreshValues();
    void refreshFile();

    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    bool isSection(const std::string& line) const;
    bool isKey(const std::string& line) const;
    const std::pair<std::string, std::string> getKeyAndValue(const std::string& line) const;
};

#endif

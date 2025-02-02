#ifndef H2DE_SETTINGS_H
#define H2DE_SETTINGS_H

#include <H2DE_engine.h>
#include <inih/INIReader.h>

/**
 * Type used to identify the settings
 * \since H2DE-2.0.8
 */
class H2DE_Settings {
private:
    std::filesystem::path path = "settings.ini";
    INIReader* reader = nullptr;

    bool settingsFileExists();

public:
    H2DE_Settings();
    ~H2DE_Settings();

    /**
     * Initializes the settings file
     * \param settings pointer to the settings
     * \param data each section and lines for the settings file
     * \since H2DE-2.0.8
     */
    friend void H2DE_InitSettings(H2DE_Settings* settings, const std::map<std::string, std::vector<std::string>>& data);

    /**
     * Gets the settings reader
     * \param settings pointer to the settings
     * \returns a pointer to the reader
     * \since H2DE-2.0.8
     */
    friend INIReader* H2DE_GetSettingReader(H2DE_Settings* settings);

    /**
     * Sets the value of a setting
     * \param settings pointer to the settings
     * \param section section where the setting is
     * \param key name of the setting
     * \param value new value if the setting
     * \since H2DE-2.0.8
     */
    friend void H2DE_SetSettingValue(H2DE_Settings* settings, std::string section, std::string key, std::string value);
};

#endif

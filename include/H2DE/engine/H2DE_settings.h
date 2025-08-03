#pragma once

/**
 * @file H2DE_settings.h
 * @brief Manage settings stored in an INI-like file for the H2DE engine.
 * 
 * This class handles reading, writing, and manipulating configuration
 * stored in a plain text INI-style file (sections with keys and values).
 * It caches values in memory for efficient access and synchronizes
 * with the file when updates happen.
 * 
 * It supports:
 * - Adding/removing sections and keys
 * - Getting keys as string, int, or bool with default fallback
 * - Updating existing keys and refreshing the file accordingly
 */

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

/**
 * @class H2DE_Settings
 * @brief Interface to load and save INI-style settings for the engine.
 * 
 * Manages sections and keys inside a settings file ("settings.ini" by default).
 * Provides methods to add sections, add keys, get values as strings, ints, or bools,
 * and update keys while keeping the file and memory in sync.

 * Internally caches the whole file contents in a nested map structure for quick access.
 */
class H2DE_Settings {
public:
    /**
     * @brief Adds a new section to the settings file.
     * 
     * Checks if the section already exists. If it does, returns false.
     * Otherwise, opens the file in append mode and writes the section header.
     * After writing, refreshes the in-memory cache by reloading the file.
     * 
     * @param name Name of the section to add.
     * @return true if the section was successfully added, false if it already exists or file error.
     */
    bool addSection(const std::string& name);
    /**
     * @brief Adds a key-value pair under a specific section.
     * 
     * Checks if the key already exists in the section; if so, returns false.
     * Otherwise, inserts or updates the value in the in-memory cache.
     * Then writes the whole cache back to the file.
     * 
     * @param section Section name where to add the key.
     * @param key Key name to add.
     * @param value Value associated with the key.
     * @return true if key was added, false if it already existed.
     */
    bool addKey(const std::string& section, const std::string& key, const std::string& value);

    /**
     * @brief Retrieves a string value for a given key in a section.
     * 
     * Returns the value if the key exists, otherwise returns the provided default value.
     * 
     * @param section Section name to look into.
     * @param key Key name to retrieve.
     * @param defaultValue Value to return if key is not found.
     * @return The string value associated with the key or defaultValue.
     */
    std::string getKeyString(const std::string& section, const std::string& key, const std::string& defaultValue) const;
    /**
     * @brief Retrieves an integer value for a given key in a section.
     * 
     * Tries to convert the stored string value to an int.
     * If the key does not exist or conversion fails, returns the default value.
     * Errors during conversion are logged.
     * 
     * @param section Section name to look into.
     * @param key Key name to retrieve.
     * @param defaultValue Value to return if key is not found or conversion fails.
     * @return The integer value associated with the key or defaultValue.
     */
    int getKeyInteger(const std::string& section, const std::string& key, int defaultValue) const;
    /**
     * @brief Retrieves a boolean value for a given key in a section.
     * 
     * Returns true if the stored string equals "true" (exact match), otherwise false.
     * If the key does not exist, returns the default value.
     * 
     * @param section Section name to look into.
     * @param key Key name to retrieve.
     * @param defaultValue Value to return if key is not found.
     * @return The boolean value associated with the key or defaultValue.
     */
    bool getKeyBoolean(const std::string& section, const std::string& key, bool defaultValue) const;

    /**
     * @brief Updates the value of an existing key within a section.
     * 
     * If the key does not exist in the given section, the function returns false.
     * Otherwise, it updates the value, writes changes back to the file, and returns true.
     * 
     * @param section The section containing the key.
     * @param key The key whose value is to be updated.
     * @param value The new value to set for the key.
     * @return true if the key existed and was updated; false otherwise.
     */
    bool setKeyValue(const std::string& section, const std::string& key, const std::string& value);

    friend class H2DE_Engine;

private:
    H2DE_Engine* engine;

    const std::filesystem::path path = "settings.ini";
    std::map<std::string, std::map<std::string, std::string>> values = {};

    H2DE_Settings(H2DE_Engine* engine);
    ~H2DE_Settings() noexcept = default;

    void initFile();
    void refreshValues();
    void refreshFile();

    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    bool isSection(const std::string& line) const;
    bool isKey(const std::string& line) const noexcept;
    const std::pair<std::string, std::string> getKeyAndValue(const std::string& line) const;
};

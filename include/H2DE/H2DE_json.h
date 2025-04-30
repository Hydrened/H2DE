#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * Creates a JSON file on disk.
 *
 * If a file already exists, it will only be overwritten if `override` is true.
 * If `encode` is true, the JSON data will be base64-encoded before being written.
 *
 * \param path The destination path for the JSON file.
 * \param override Whether to overwrite the file if it already exists.
 * \param encode Whether to encode the data in base64 before writing.
 * \return True on success, false on failure (e.g., permission denied, file exists and override is false).
 */
bool H2DE_CreateJsonFile(const std::filesystem::path& path, bool override, bool encode);
/**
 * Creates a new JSON file at the specified path with the given data.
 *
 * If a file already exists, it will only be overwritten if `override` is true.
 * If `encode` is true, the JSON data will be base64-encoded before being written.
 *
 * \param path The destination path for the JSON file.
 * \param data The JSON data to write into the file.
 * \param override Whether to overwrite the file if it already exists.
 * \param encode Whether to encode the data in base64 before writing.
 * \return True on success, false on failure (e.g., permission denied, file exists and override is false).
 */
bool H2DE_CreateJsonFile(const std::filesystem::path& path, const json& data, bool override, bool encode);
/**
 * Reads a JSON file from the specified path.
 *
 * If the file is encoded in base64, it should be decoded internally.
 * The function returns an empty JSON object if reading or parsing fails.
 *
 * \param path The path to the JSON file to read.
 * \return A JSON object containing the file's contents, or an empty object on failure.
 */
json H2DE_ReadJsonFile(const std::filesystem::path& path);
/**
 * Writes JSON data to a file at the specified path.
 *
 * If `encode` is true, the JSON data is base64-encoded before being written.
 * This function overwrites the file if it already exists.
 *
 * \param path The path to the file where the JSON data will be written.
 * \param data The JSON data to write.
 * \param encode Whether to encode the data in base64 before writing.
 * \return True if the data was successfully written, false otherwise.
 */
bool H2DE_WriteJsonFile(const std::filesystem::path& path, const json& data, bool encode);

#endif

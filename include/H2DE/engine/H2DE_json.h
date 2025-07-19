#ifndef H2DE_JSON_H
#define H2DE_JSON_H

/**
 * @file H2DE_json.h
 * @brief Utility class for JSON file handling with optional base64 encoding/decoding.
 * 
 * This file defines the H2DE_Json class, which provides static methods to
 * create, read, and write JSON files, with support for base64 encoding.
 * It also offers convenient template functions to extract common types and
 * custom vector/rect/color structs from JSON data.
 * 
 * Uses nlohmann::json for JSON parsing and serialization.
 */

#include <H2DE/utils/H2DE_utils.h>
#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * @namespace H2DE_Json
 * @brief Utility namespace for JSON file operations and data extraction.
 * 
 * Provides functions to create, read, and write JSON files with optional
 * base64 encoding. Also includes utilities to convert JSON values into
 * basic C++ types, 2D vectors, rectangles, and colors used in the H2DE engine.
 */
namespace H2DE_Json {
    /**
     * @brief Create an empty JSON file at the specified path.
     * 
     * If `override` is false and the file already exists, the creation fails.
     * The file content is either plain JSON or encoded in base64 depending on `encode`.
     * 
     * @param path The path to the JSON file to create.
     * @param override Whether to overwrite an existing file.
     * @param encode Whether to encode the JSON content in base64.
     * @return true if the file was successfully created, false otherwise.
     */
    bool create(const std::filesystem::path& path, bool override, bool encode);
    /**
     * @brief Create a JSON file with the given data.
     * 
     * If `override` is false and the file already exists, the creation fails.
     * The JSON data can be optionally encoded in base64 before writing.
     * 
     * @param path The path to the JSON file to create.
     * @param data The JSON data to write.
     * @param override Whether to overwrite an existing file.
     * @param encode Whether to encode the JSON content in base64.
     * @return true if the file was successfully created, false otherwise.
     */
    bool create(const std::filesystem::path& path, const json& data, bool override, bool encode);

    /**
     * @brief Read JSON data from a file.
     * 
     * Attempts to open and read the file at `path`.
     * If the content is not valid JSON, tries to decode base64 and parse again.
     * Logs errors if file is missing, cannot be opened, or parsing fails.
     * 
     * @param path The path to the JSON file to read.
     * @return Parsed JSON object on success, empty JSON object on failure.
     */
    json read(const std::filesystem::path& path);

    /**
     * @brief Write JSON data to a file.
     * 
     * Opens the file at `path` for writing.
     * If `encode` is true, writes the base64-encoded JSON string.
     * Otherwise, writes the plain JSON text.
     * Logs errors on failure to open or write.
     * 
     * @param path The path to the JSON file to write.
     * @param data The JSON data to write.
     * @param encode Whether to encode the JSON content in base64.
     * @return true if writing was successful, false otherwise.
     */
    bool write(const std::filesystem::path& path, const json& data, bool encode);

    /**
     * @brief Check if a file exists at the given path.
     * 
     * Verifies whether a file or directory exists at the specified `path`.
     * Uses the filesystem API to perform the check.
     * 
     * @param path The path to check for existence.
     * @return true if the file or directory exists, false otherwise.
     */
    inline bool exists(const std::filesystem::path& path) {
        return std::filesystem::exists(path);
    }

    /**
     * @brief Extract an integer from a JSON value.
     * 
     * Converts the JSON value to an int.
     * 
     * @param integer JSON value representing an integer.
     * @return The integer extracted from JSON.
     */
    constexpr int getInteger(const json& integer) {
        return integer.get<int>();
    } 
    /**
     * @brief Extract a float from a JSON value.
     * 
     * Converts the JSON value to a float.
     * 
     * @param floater JSON value representing a floating point number.
     * @return The float extracted from JSON.
     */
    constexpr float getFloat(const json& floater) {
        return floater.get<float>();
    } 
    /**
     * @brief Extract a boolean from a JSON value.
     * 
     * Converts the JSON value to a bool.
     * 
     * @param boolean JSON value representing a boolean.
     * @return The bool extracted from JSON.
     */
    constexpr bool getBool(const json& boolean) {
        return boolean.get<bool>();
    } 
    /**
     * @brief Extract a char from a JSON value.
     * 
     * Converts the JSON value to a char.
     * 
     * @param character JSON value representing a character.
     * @return The char extracted from JSON.
     */
    constexpr char getChar(const json& character) {
        return character.get<char>();
    } 
    /**
     * @brief Extract a std::string from a JSON value.
     * 
     * Converts the JSON value to a std::string.
     * 
     * @param string JSON value representing a string.
     * @return The string extracted from JSON.
     */
    inline std::string getString(const json& string) {
        return string.get<std::string>();
    }

    /**
     * @brief Extract a generic value of type T from a JSON value.
     * 
     * Template function that converts the JSON value to any type T supported by `json::get<T>()`.
     * 
     * @tparam T The type to extract.
     * @param value JSON value to convert.
     * @return The value of type T extracted from JSON.
     */
    template<typename T>
    inline T getValue(const json& value) {
        return value.get<T>();
    }

    /**
     * @brief Extract a 2D vector from a JSON object.
     * 
     * Assumes the JSON object has fields "x" and "y" representing the vector components.
     * 
     * @tparam H2DE_Vector2D_T Type of the vector components (e.g., float, int).
     * @param translate JSON object containing "x" and "y".
     * @return H2DE_Vector2D with components extracted from JSON.
     */
    template<typename H2DE_Vector2D_T>
    inline H2DE_Vector2D<H2DE_Vector2D_T> getVector2D(const json& translate) {
        return H2DE_Vector2D<H2DE_Vector2D_T>{
            static_cast<H2DE_Vector2D_T>(translate["x"]),
            static_cast<H2DE_Vector2D_T>(translate["y"]),
        };
    }

    /**
     * @brief Extract a rectangle from a JSON object.
     * 
     * Assumes the JSON object has fields "x", "y", "w", and "h" representing the rectangle position and size.
     * 
     * @tparam H2DE_Rect_T Type of the rectangle components (e.g., float, int).
     * @param rect JSON object containing rectangle data.
     * @return H2DE_Rect with components extracted from JSON.
     */
    template<typename H2DE_Rect_T>
    inline H2DE_Rect<H2DE_Rect_T> getRect(const json& rect) {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(rect["x"]),
            static_cast<H2DE_Rect_T>(rect["y"]),
            static_cast<H2DE_Rect_T>(rect["w"]),
            static_cast<H2DE_Rect_T>(rect["h"]),
        };
    }

    /**
     * @brief Extract an RGB color from a JSON object.
     * 
     * Assumes the JSON object has fields "r", "g", "b", and optionally "a" for alpha.
     * If alpha is not used, it defaults to max value (255).
     * 
     * @param color JSON object containing color components.
     * @param alpha If true, reads the alpha component; otherwise defaults to max alpha.
     * @return H2DE_ColorRGB struct representing the color.
     */
    inline H2DE_ColorRGB getColorRGB(const json& color, bool alpha) {
        return H2DE_ColorRGB{
            static_cast<uint8_t>(static_cast<int>(color["r"])),
            static_cast<uint8_t>(static_cast<int>(color["g"])),
            static_cast<uint8_t>(static_cast<int>(color["b"])),
            ((alpha) ? static_cast<uint8_t>(static_cast<int>(color["a"])) : static_cast<uint8_t>(static_cast<int>(H2DE_OPACITY_MAX))),
        };
    }
    /**
     * @brief Extract an HSV color from a JSON object.
     * 
     * Assumes the JSON object has fields "h", "s", "v", and optionally "a" for alpha.
     * If alpha is not used, it defaults to 1.0f (fully opaque).
     * 
     * @param color JSON object containing color components.
     * @param alpha If true, reads the alpha component; otherwise defaults to 1.0f.
     * @return H2DE_ColorHSV struct representing the color.
     */
    inline H2DE_ColorHSV getColorHSV(const json& color, bool alpha) {
        return H2DE_ColorHSV{
            static_cast<float>(color["h"]),
            static_cast<float>(color["s"]),
            static_cast<float>(color["v"]),
            ((alpha) ? static_cast<float>(color["a"]) : 1.0f),
        };
    }

    /**
     * @brief Convert a value to a JSON object.
     * 
     * Generic fallback for types that are already JSON-compatible.
     * 
     * @tparam T The type of the value to convert.
     * @param v The value to convert.
     * @return JSON object representing the value.
     */
    template<typename T>
    inline json toJson(const T& v) {
        return v;
    }

    /**
     * @brief Convert a 2D vector to a JSON object.
     * 
     * Serializes a vector of type H2DE_Vector2D<T> into a JSON object
     * with "x" and "y" fields.
     * 
     * @tparam H2DE_Vector2D_T The type of the vector components.
     * @param vector The vector to convert.
     * @return JSON object with "x" and "y" fields.
     */
    template<typename H2DE_Vector2D_T>
    inline json toJson(const H2DE_Vector2D<H2DE_Vector2D_T>& vector) {
        return { { "x", vector.x }, { "y", vector.y } };
    }

    /**
     * @brief Convert a rectangle to a JSON object.
     * 
     * Serializes a rectangle of type H2DE_Rect<T> into a JSON object
     * with "x", "y", "w", and "h" fields.
     * 
     * @tparam H2DE_Rect_T The type of the rectangle components.
     * @param rect The rectangle to convert.
     * @return JSON object with "x", "y", "w", and "h" fields.
     */
    template<typename H2DE_Rect_T>
    inline json toJson(const H2DE_Rect<H2DE_Rect_T>& rect) {
        return { { "x", rect.x }, { "y", rect.y }, { "w", rect.w }, { "h", rect.h } };
    }

    /**
     * @brief Convert an RGB color to a JSON object.
     * 
     * Serializes a H2DE_ColorRGB struct into a JSON object.
     * If `alpha` is true, includes the "a" field.
     * 
     * @param rgb The RGB color to convert.
     * @param alpha Whether to include the alpha component.
     * @return JSON object with "r", "g", "b", and optionally "a".
     */
    inline json toJson(const H2DE_ColorRGB& rgb, bool alpha) {
        return alpha
            ? json{
                { "r", rgb.r },
                { "g", rgb.g },
                { "b", rgb.b },
                { "a", rgb.a }
            }
            : json {
                { "r", rgb.r },
                { "g", rgb.g },
                { "b", rgb.b }
            };
    }

    /**
     * @brief Convert an HSV color to a JSON object.
     * 
     * Serializes a H2DE_ColorHSV struct into a JSON object.
     * If `alpha` is true, includes the "a" field.
     * 
     * @param hsv The HSV color to convert.
     * @param alpha Whether to include the alpha component.
     * @return JSON object with "h", "s", "v", and optionally "a".
     */
    inline json toJson(const H2DE_ColorHSV& hsv, bool alpha) {
        return alpha
            ? json{
                { "h", hsv.h },
                { "s", hsv.s },
                { "v", hsv.v },
                { "a", hsv.a }
            }
            : json {
                { "h", hsv.h },
                { "s", hsv.s },
                { "v", hsv.v }
            };
    }
};

#endif

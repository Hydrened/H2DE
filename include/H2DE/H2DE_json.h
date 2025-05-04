#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <H2DE/H2DE_utils.h>
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

/**
 * @brief Extracts an integer from a JSON value.
 * 
 * @param integer JSON value expected to contain an integer.
 * @return int Parsed integer.
 */
inline int H2DE_GetIntFromJson(const json& integer) { return integer.get<int>(); }
/**
 * @brief Extracts a float from a JSON value.
 * 
 * @param floater JSON value expected to contain a float.
 * @return float Parsed float.
 */
inline float H2DE_GetFloatFromJson(const json& floater) { return floater.get<float>(); }
/**
 * @brief Extracts a boolean from a JSON value.
 * 
 * @param boolean JSON value expected to contain a boolean.
 * @return bool Parsed boolean.
 */
inline bool H2DE_GetBoolFromJson(const json& boolean) { return boolean.get<bool>(); }
/**
 * @brief Extracts a character from a JSON value.
 * 
 * @param c JSON value expected to contain a character.
 * @return char Parsed character.
 */
inline char H2DE_GetCharFromJson(const json& c) { return c.get<char>(); }
/**
 * @brief Extracts a string from a JSON value.
 * 
 * @param stringean JSON value expected to contain a string.
 * @return std::string Parsed string.
 */
inline std::string H2DE_GetStringFromJson(const json& stringean) { return stringean.get<std::string>(); }
/**
 * @brief Extracts a 2D position (vector) from a JSON object.
 * 
 * The JSON must contain "x" and "y" fields.
 * 
 * @tparam H2DE_Vector2D_T Type of the vector components (e.g., int or float).
 * @param pos JSON object with "x" and "y" keys.
 * @return H2DE_Vector2D<H2DE_Vector2D_T> Parsed vector.
 */
template<typename H2DE_Vector2D_T>
inline H2DE_Vector2D<H2DE_Vector2D_T> H2DE_GetPosFromJson(const json& pos) {
    return H2DE_Vector2D<H2DE_Vector2D_T>{
        static_cast<H2DE_Vector2D_T>(pos["x"]),
        static_cast<H2DE_Vector2D_T>(pos["y"]),
    };
}
/**
 * @brief Extracts a 2D size (vector) from a JSON object.
 * 
 * The JSON must contain "w" and "h" fields.
 * 
 * @tparam H2DE_Vector2D_T Type of the size components.
 * @param size JSON object with "w" and "h" keys.
 * @return H2DE_Vector2D<H2DE_Vector2D_T> Parsed size.
 */
template<typename H2DE_Vector2D_T>
inline H2DE_Vector2D<H2DE_Vector2D_T> H2DE_GetSizeFromJson(const json& size) {
    return H2DE_Vector2D<H2DE_Vector2D_T>{
        static_cast<H2DE_Vector2D_T>(size["w"]),
        static_cast<H2DE_Vector2D_T>(size["h"]),
    };
}
/**
 * @brief Extracts a velocity vector from a JSON object.
 * 
 * The JSON must contain "x" and "y" fields.
 * 
 * @param velocity JSON object with velocity data.
 * @return H2DE_LevelVelocity Parsed velocity vector.
 */
inline H2DE_LevelVelocity H2DE_GetVelocityFromJson(const json& velocity) {
    return H2DE_LevelVelocity{
        static_cast<float>(velocity["x"]),
        static_cast<float>(velocity["y"]),
    };
}
/**
 * @brief Extracts a rectangle from a JSON object.
 * 
 * The JSON must contain "x", "y", "w", and "h" fields.
 * 
 * @tparam H2DE_Rect_T Type of the rectangle's components.
 * @param rect JSON object with rectangle data.
 * @return H2DE_Rect<H2DE_Rect_T> Parsed rectangle.
 */
template<typename H2DE_Rect_T>
inline H2DE_Rect<H2DE_Rect_T> H2DE_GetRectFromJson(const json& rect) {
    return H2DE_Rect<H2DE_Rect_T>{
        static_cast<H2DE_Rect_T>(rect["x"]),
        static_cast<H2DE_Rect_T>(rect["y"]),
        static_cast<H2DE_Rect_T>(rect["w"]),
        static_cast<H2DE_Rect_T>(rect["h"]),
    };
}
/**
 * @brief Extracts an RGB color from a JSON object.
 * 
 * The JSON must contain "r", "g", and "b" fields. If alpha is true, it must also contain "a".
 * 
 * @param color JSON object with RGB color data.
 * @param alpha Whether to extract the alpha channel (transparency).
 * @return H2DE_ColorRGB Parsed color.
 */
inline H2DE_ColorRGB H2DE_GetColorRgbFromJson(const json& color, bool alpha) {
    return H2DE_ColorRGB{
        static_cast<Uint8>(static_cast<int>(color["r"])),
        static_cast<Uint8>(static_cast<int>(color["g"])),
        static_cast<Uint8>(static_cast<int>(color["b"])),
        ((alpha) ? static_cast<Uint8>(static_cast<int>(color["a"])) : static_cast<Uint8>(static_cast<int>(UINT8_MAX))),
    };
}
/**
 * @brief Extracts an HSV color from a JSON object.
 * 
 * The JSON must contain "h", "s", and "v" fields. If alpha is true, it must also contain "a".
 * 
 * @param color JSON object with HSV color data.
 * @param alpha Whether to extract the alpha channel.
 * @return H2DE_ColorHSV Parsed color.
 */
inline H2DE_ColorHSV H2DE_GetColorHsvFromJson(const json& color, bool alpha) {
    return H2DE_ColorHSV{
        static_cast<float>(color["h"]),
        static_cast<float>(color["s"]),
        static_cast<float>(color["v"]),
        ((alpha) ? static_cast<float>(color["a"]) : 1.0f),
    };
}

#endif

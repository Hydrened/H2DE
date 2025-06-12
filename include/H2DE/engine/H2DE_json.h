#ifndef H2DE_JSON_H
#define H2DE_JSON_H

#include <H2DE/utils/H2DE_utils.h>
#include <nlohmann/json.hpp>
#include <base64/base64.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class H2DE_Json {
public:
    static bool create(const std::filesystem::path& path, bool override, bool encode);
    static bool create(const std::filesystem::path& path, const json& data, bool override, bool encode);

    static json read(const std::filesystem::path& path);

    static bool write(const std::filesystem::path& path, const json& data, bool encode);

    static constexpr int getInteger(const json& integer) { return integer.get<int>(); } 
    static constexpr float getFloat(const json& floater) { return floater.get<float>(); } 
    static constexpr bool getBool(const json& boolean) { return boolean.get<bool>(); } 
    static constexpr char getChar(const json& character) { return character.get<char>(); } 
    static inline std::string getString(const json& string) { return string.get<std::string>(); }

    template<typename H2DE_Vector2D_T>
    static inline H2DE_Vector2D<H2DE_Vector2D_T> getVector2D(const json& translate) {
        return H2DE_Vector2D<H2DE_Vector2D_T>{
            static_cast<H2DE_Vector2D_T>(translate["x"]),
            static_cast<H2DE_Vector2D_T>(translate["y"]),
        };
    }

    template<typename H2DE_Rect_T>
    static inline H2DE_Rect<H2DE_Rect_T> getRect(const json& rect) {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(rect["x"]),
            static_cast<H2DE_Rect_T>(rect["y"]),
            static_cast<H2DE_Rect_T>(rect["w"]),
            static_cast<H2DE_Rect_T>(rect["h"]),
        };
    }

    static inline H2DE_ColorRGB getColorRGB(const json& color, bool alpha) {
        return H2DE_ColorRGB{
            static_cast<uint8_t>(static_cast<int>(color["r"])),
            static_cast<uint8_t>(static_cast<int>(color["g"])),
            static_cast<uint8_t>(static_cast<int>(color["b"])),
            ((alpha) ? static_cast<uint8_t>(static_cast<int>(color["a"])) : static_cast<uint8_t>(static_cast<int>(H2DE_UINT8_MAX))),
        };
    }
    static inline H2DE_ColorHSV getColorHSV(const json& color, bool alpha) {
        return H2DE_ColorHSV{
            static_cast<float>(color["h"]),
            static_cast<float>(color["s"]),
            static_cast<float>(color["v"]),
            ((alpha) ? static_cast<float>(color["a"]) : 1.0f),
        };
    }
};

#endif

#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <cmath>
#include <functional>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>
#include <SDL2/SDL_video.h>
template<typename H2DE_Rect_T>
struct H2DE_Rect;
struct H2DE_ColorHSV;
class H2DE_LevelObject;

template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x;
    H2DE_Vector2D_T y;

    H2DE_Vector2D operator+(const H2DE_Vector2D& other) const;
    H2DE_Vector2D operator-(const H2DE_Vector2D& other) const;
    H2DE_Vector2D operator*(const float& multiplier) const;
    H2DE_Vector2D operator/(const float& divider) const;

    H2DE_Vector2D& operator+=(const H2DE_Vector2D& other);
    H2DE_Vector2D& operator-=(const H2DE_Vector2D& other);
    H2DE_Vector2D& operator*=(const float& multiplier);
    H2DE_Vector2D& operator/=(const float& divider);

    bool operator==(const H2DE_Vector2D& other) const;
    bool operator!=(const H2DE_Vector2D& other) const;
    bool operator>(const H2DE_Vector2D& other) const;
    bool operator>=(const H2DE_Vector2D& other) const;
    bool operator<(const H2DE_Vector2D& other) const;
    bool operator<=(const H2DE_Vector2D& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D& vec) {
        os << std::string("x: ") << vec.x << ", y: " << vec.y;
        return os;
    }

    H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D& size) const;

    bool isNull() const;
    H2DE_Vector2D<H2DE_Vector2D_T> getCenter() const;
};

using H2DE_AbsPos = H2DE_Vector2D<int>;
using H2DE_AbsSize = H2DE_Vector2D<int>;
using H2DE_LevelPos = H2DE_Vector2D<float>;
using H2DE_LevelSize = H2DE_Vector2D<float>;
using H2DE_LevelVelocity = H2DE_Vector2D<float>;

template<typename H2DE_Rect_T>
struct H2DE_Rect {
    H2DE_Rect_T x;
    H2DE_Rect_T y;
    H2DE_Rect_T w;
    H2DE_Rect_T h;

    H2DE_Rect operator+(const H2DE_Rect& other) const;
    H2DE_Rect operator-(const H2DE_Rect& other) const;
    H2DE_Rect operator*(const float& multiplier) const;
    H2DE_Rect operator/(const float& divider) const;

    H2DE_Rect& operator+=(const H2DE_Rect& other);
    H2DE_Rect& operator-=(const H2DE_Rect& other);
    H2DE_Rect& operator*=(const float& multiplier);
    H2DE_Rect& operator/=(const float& divider);

    bool operator==(const H2DE_Rect& other) const;
    bool operator!=(const H2DE_Rect& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_Rect& rect) {
        os << std::string("x: ") << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h;
        return os;
    }

    H2DE_Rect addPos(const H2DE_Vector2D<H2DE_Rect_T>& pos);
    H2DE_Rect addSize(const H2DE_Vector2D<H2DE_Rect_T>& size);
    H2DE_Rect substractPos(const H2DE_Vector2D<H2DE_Rect_T>& pos);
    H2DE_Rect substractSize(const H2DE_Vector2D<H2DE_Rect_T>& size);
    H2DE_Rect multiplyPos(const float& multiplier);
    H2DE_Rect multiplySize(const float& multiplier);
    H2DE_Rect dividePos(const float& divider);
    H2DE_Rect divideSize(const float& divider);

    H2DE_Vector2D<H2DE_Rect_T> getPos() const;
    H2DE_Vector2D<H2DE_Rect_T> getSize() const;
    H2DE_Vector2D<H2DE_Rect_T> getCenter() const;
};

using H2DE_AbsRect = H2DE_Rect<int>;
using H2DE_LevelRect = H2DE_Rect<float>;

struct H2DE_ColorRGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    operator H2DE_ColorHSV() const;
    operator Uint32() const;

    bool operator==(const H2DE_ColorRGB& other) const;
    bool operator!=(const H2DE_ColorRGB& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& color) {
        os << std::string("r: ") << static_cast<int>(color.r) << ", g: " << static_cast<int>(color.g) << ", b: " << static_cast<int>(color.b) << ", a: " << static_cast<int>(color.a);
        return os;
    }

    H2DE_ColorRGB addHue(const float& hue) const;
    H2DE_ColorRGB addSaturation(const float& saturation) const;
    H2DE_ColorRGB addValue(const float& value) const;

    H2DE_ColorRGB subtractHue(const float& hue) const;
    H2DE_ColorRGB subtractSaturation(const float& saturation) const;
    H2DE_ColorRGB subtractValue(const float& value) const;

    H2DE_ColorRGB multiplyHue(const float& multiplier) const;
    H2DE_ColorRGB multiplySaturation(const float& multiplier) const;
    H2DE_ColorRGB multiplyValue(const float& multiplier) const;

    H2DE_ColorRGB divideHue(const float& divider) const;
    H2DE_ColorRGB divideSaturation(const float& divider) const;
    H2DE_ColorRGB divideValue(const float& divider) const;
};

struct H2DE_ColorHSV {
    float h;
    float s;
    float v;
    float a;

    operator H2DE_ColorRGB() const;

    bool operator==(const H2DE_ColorHSV& other) const;
    bool operator!=(const H2DE_ColorHSV& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& color) {
        os << std::string("h: ") << color.h << ", s: " << color.s << ", v: " << color.v << ", a: " << color.a;
        return os;
    }

    H2DE_ColorHSV addHue(const float& hue) const;
    H2DE_ColorHSV addSaturation(const float& saturation) const;
    H2DE_ColorHSV addValue(const float& value) const;

    H2DE_ColorHSV subtractHue(const float& hue) const;
    H2DE_ColorHSV subtractSaturation(const float& saturation) const;
    H2DE_ColorHSV subtractValue(const float& value) const;

    H2DE_ColorHSV multiplyHue(const float& multiplier) const;
    H2DE_ColorHSV multiplySaturation(const float& multiplier) const;
    H2DE_ColorHSV multiplyValue(const float& multiplier) const;

    H2DE_ColorHSV divideHue(const float& divider) const;
    H2DE_ColorHSV divideSaturation(const float& divider) const;
    H2DE_ColorHSV divideValue(const float& divider) const;
};

struct H2DE_WindowData {
    unsigned int fps = 60;
    const char* title = "H2DE window";
    H2DE_AbsPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
    H2DE_AbsSize size = { 1280, 720 };
    bool fullscreen = false;
    bool resizable = false;
    bool saveState = false;
};

struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();
};

struct H2DE_Hitbox {
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    int collisionIndex = 0;
    bool snap = false;
    std::optional<std::function<void(H2DE_LevelObject*)>> onCollide = std::nullopt;
};

struct H2DE_LevelObjectData {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    bool absolute = false;
    int index = 0;
};

float H2DE_Lerp(float min, float max, float blend);
float H2DE_RandomFloatInRange(float min, float max);
int H2DE_RandomIntegerInRange(int min, int max);
bool H2DE_RandomBool();

#endif

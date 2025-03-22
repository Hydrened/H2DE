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
class H2DE_Object;

enum H2DE_Face {
    H2DE_FACE_TOP,
    H2DE_FACE_RIGHT,
    H2DE_FACE_BOTTOM,
    H2DE_FACE_LEFT,
};

template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x;
    H2DE_Vector2D_T y;

    H2DE_Vector2D operator+(const H2DE_Vector2D& other) const;
    H2DE_Vector2D operator-(const H2DE_Vector2D& other) const;
    H2DE_Vector2D operator*(float multiplier) const;
    H2DE_Vector2D operator/(float divider) const;

    H2DE_Vector2D& operator+=(const H2DE_Vector2D& other);
    H2DE_Vector2D& operator-=(const H2DE_Vector2D& other);
    H2DE_Vector2D& operator*=(float multiplier);
    H2DE_Vector2D& operator/=(float divider);

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
    H2DE_Rect operator*(float multiplier) const;
    H2DE_Rect operator/(float divider) const;

    H2DE_Rect& operator+=(const H2DE_Rect& other);
    H2DE_Rect& operator-=(const H2DE_Rect& other);
    H2DE_Rect& operator*=(float multiplier);
    H2DE_Rect& operator/=(float divider);

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
    H2DE_Rect multiplyPos(float multiplier);
    H2DE_Rect multiplySize(float multiplier);
    H2DE_Rect dividePos(float divider);
    H2DE_Rect divideSize(float divider);

    H2DE_Vector2D<H2DE_Rect_T> getPos() const;
    H2DE_Vector2D<H2DE_Rect_T> getSize() const;
    H2DE_Vector2D<H2DE_Rect_T> getCenter() const;

    bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const;
    std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const;
};

using H2DE_AbsRect = H2DE_Rect<int>;
using H2DE_LevelRect = H2DE_Rect<float>;

struct H2DE_ColorRGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    explicit operator H2DE_ColorHSV() const;
    explicit operator Uint32() const;

    bool operator==(const H2DE_ColorRGB& other) const;
    bool operator!=(const H2DE_ColorRGB& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& color) {
        os << std::string("r: ") << static_cast<int>(color.r) << ", g: " << static_cast<int>(color.g) << ", b: " << static_cast<int>(color.b) << ", a: " << static_cast<int>(color.a);
        return os;
    }

    H2DE_ColorRGB addHue(float hue) const;
    H2DE_ColorRGB addSaturation(float saturation) const;
    H2DE_ColorRGB addValue(float value) const;

    H2DE_ColorRGB subtractHue(float hue) const;
    H2DE_ColorRGB subtractSaturation(float saturation) const;
    H2DE_ColorRGB subtractValue(float value) const;

    H2DE_ColorRGB multiplyHue(float multiplier) const;
    H2DE_ColorRGB multiplySaturation(float multiplier) const;
    H2DE_ColorRGB multiplyValue(float multiplier) const;

    H2DE_ColorRGB divideHue(float divider) const;
    H2DE_ColorRGB divideSaturation(float divider) const;
    H2DE_ColorRGB divideValue(float divider) const;
};

struct H2DE_ColorHSV {
    float h;
    float s;
    float v;
    float a;

    explicit operator H2DE_ColorRGB() const;

    bool operator==(const H2DE_ColorHSV& other) const;
    bool operator!=(const H2DE_ColorHSV& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& color) {
        os << std::string("h: ") << color.h << ", s: " << color.s << ", v: " << color.v << ", a: " << color.a;
        return os;
    }

    H2DE_ColorHSV addHue(float hue) const;
    H2DE_ColorHSV addSaturation(float saturation) const;
    H2DE_ColorHSV addValue(float value) const;

    H2DE_ColorHSV subtractHue(float hue) const;
    H2DE_ColorHSV subtractSaturation(float saturation) const;
    H2DE_ColorHSV subtractValue(float value) const;

    H2DE_ColorHSV multiplyHue(float multiplier) const;
    H2DE_ColorHSV multiplySaturation(float multiplier) const;
    H2DE_ColorHSV multiplyValue(float multiplier) const;

    H2DE_ColorHSV divideHue(float divider) const;
    H2DE_ColorHSV divideSaturation(float divider) const;
    H2DE_ColorHSV divideValue(float divider) const;
};

struct H2DE_LevelPadding {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;
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

struct H2DE_CameraData {
    H2DE_LevelPos defaultPos = { 0.0f, 0.0f };
    float width = 20.0f;
    float smoothing = 0.0f;
    H2DE_LevelPadding padding = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();
    H2DE_CameraData camera = H2DE_CameraData();
};

struct H2DE_Hitbox {
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    int collisionIndex = 0;
    bool snap = false;
    std::optional<std::function<void(H2DE_Object*)>> onCollide = std::nullopt;
};

struct H2DE_ObjectData {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    H2DE_LevelSize size = { 1.0f, 1.0f };
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    bool absolute = false;
    int index = 0;
};

struct H2DE_BarObjectData {

};

struct H2DE_BasicObjectData {

};

struct H2DE_ButtonObjectData {

};

struct H2DE_TextObjectData {

};

float H2DE_Lerp(float min, float max, float blend);
float H2DE_RandomFloatInRange(float min, float max);
int H2DE_RandomIntegerInRange(int min, int max);
bool H2DE_RandomBool();

#endif

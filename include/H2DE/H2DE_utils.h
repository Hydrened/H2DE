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
class H2DE_Surface;

enum H2DE_Face {
    H2DE_FACE_TOP = 0b00,
    H2DE_FACE_RIGHT = 0b01,
    H2DE_FACE_BOTTOM = 0b10,
    H2DE_FACE_LEFT = 0b11,
};

enum H2DE_WindowRatio {
    H2DE_WINDOW_RATIO_NO_RATIO,
    H2DE_WINDOW_RATIO_CUSTOM,
    H2DE_WINDOW_RATIO_4_3,
    H2DE_WINDOW_RATIO_3_2,
    H2DE_WINDOW_RATIO_5_4,
    H2DE_WINDOW_RATIO_16_10,
    H2DE_WINDOW_RATIO_16_9,
    H2DE_WINDOW_RATIO_21_9,
    H2DE_WINDOW_RATIO_32_9,
};

enum H2DE_Flip {
    H2DE_FLIP_NONE = 0b00,
    H2DE_FLIP_X = 0b01,
    H2DE_FLIP_Y = 0b10,
    H2DE_FLIP_XY = 0b11,
};

enum H2DE_ScaleMode {
    H2DE_SCALE_MODE_NEAREST,
    H2DE_SCALE_MODE_LINEAR,
    H2DE_SCALE_MODE_BEST,
};

enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_LEFT,
    H2DE_TEXT_ALIGN_RIGHT,
    H2DE_TEXT_ALIGN_CENTER,
};

enum H2DE_Easing {
    H2DE_EASING_LINEAR,
    H2DE_EASING_EASE_IN,
    H2DE_EASING_EASE_OUT,
    H2DE_EASING_EASE_IN_OUT,
    H2DE_EASING_BACK_IN,
    H2DE_EASING_BACK_OUT,
    H2DE_EASING_BACK_IN_OUT,
    H2DE_EASING_ELASTIC_IN,
    H2DE_EASING_ELASTIC_OUT,
    H2DE_EASING_ELASTIC_IN_OUT,
    H2DE_EASING_BOUNCE_IN,
    H2DE_EASING_BOUNCE_OUT,
    H2DE_EASING_BOUNCE_IN_OUT,
    H2DE_EASING_SINE_IN,
    H2DE_EASING_SINE_OUT,
    H2DE_EASING_SINE_IN_OUT,
    H2DE_EASING_EXPO_IN,
    H2DE_EASING_EXPO_OUT,
    H2DE_EASING_EXPO_IN_OUT,
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

    const bool operator==(const H2DE_Vector2D& other) const;
    const bool operator!=(const H2DE_Vector2D& other) const;
    const bool operator>(const H2DE_Vector2D& other) const;
    const bool operator>=(const H2DE_Vector2D& other) const;
    const bool operator<(const H2DE_Vector2D& other) const;
    const bool operator<=(const H2DE_Vector2D& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D& vec) {
        os << std::string("x: ") << vec.x << ", y: " << vec.y;
        return os;
    }

    operator SDL_Point() const;

    H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D& size) const;

    const bool isNull() const;
    H2DE_Vector2D<H2DE_Vector2D_T> rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& center, float angle);
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

    const bool operator==(const H2DE_Rect& other) const;
    const bool operator!=(const H2DE_Rect& other) const;

    friend std::ostream& operator<<(std::ostream& os, const H2DE_Rect& rect) {
        os << std::string("x: ") << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h;
        return os;
    }

    operator SDL_Rect() const;

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

    const bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const;
    const bool collides(const H2DE_Vector2D<H2DE_Rect_T>& pos) const;
    const std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const;
};

using H2DE_AbsRect = H2DE_Rect<int>;
using H2DE_LevelRect = H2DE_Rect<float>;

struct H2DE_ColorRGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a = SDL_MAX_UINT8;

    explicit operator H2DE_ColorHSV() const;
    explicit operator Uint32() const;

    const bool operator==(const H2DE_ColorRGB& other) const;
    const bool operator!=(const H2DE_ColorRGB& other) const;

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

    const bool isVisible() const;
};

struct H2DE_ColorHSV {
    float h;
    float s;
    float v;
    float a = 1.0f;

    explicit operator H2DE_ColorRGB() const;

    const bool operator==(const H2DE_ColorHSV& other) const;
    const bool operator!=(const H2DE_ColorHSV& other) const;

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

    const bool isVisible() const;
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
    H2DE_WindowRatio ratio = H2DE_WINDOW_RATIO_NO_RATIO;
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
    std::function<void(H2DE_Object*, H2DE_Face)> onCollide = nullptr;
};

struct H2DE_Font {
    std::string textureName = "";
    H2DE_AbsSize charSize = { 0, 0 };
    int spacing = 0;
    std::string charOrder = "";
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
};

struct H2DE_ObjectData {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    H2DE_LevelSize size = { 1.0f, 1.0f };
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    H2DE_LevelPos pivot = { 0.0f, 0.0f };
    float rotation = 0.0f;
    H2DE_Flip flip = H2DE_FLIP_NONE;
    bool absolute = false;
    int index = 0;
};

struct H2DE_SurfaceData {
    std::string textureName = "";
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    H2DE_LevelPos pivot = { 0.0f, 0.0f };
    float rotation = 0.0f;
    H2DE_Flip flip = H2DE_FLIP_NONE;
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
};

struct H2DE_SurfaceBuffer {
    H2DE_Surface* surface = nullptr;
    H2DE_LevelPos offset = { 0.0f, 0.0f };
    H2DE_LevelSize size = { 0.0f, 0.0f };
};

struct H2DE_TextureData {
    std::optional<H2DE_AbsRect> srcRect = std::nullopt;
};

struct H2DE_SpriteData {
    H2DE_AbsPos startingPos = { 0, 0 };
    H2DE_AbsSize size = { 1, 1 };
    int spacing = 0;
    unsigned int nbFrame = 0;
    unsigned int delay = 200;
    bool pauseSensitive = true;
};

struct H2DE_BarObjectData {
    std::unordered_map<std::string, H2DE_Surface*> front = {};
    std::unordered_map<std::string, H2DE_Surface*> background = {};
    float min = 0.0f;
    float max = 100.0f;
    float defaultValue = 0.0f;
};

struct H2DE_BasicObjectData {
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};
};

struct H2DE_ButtonObjectData {
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};
    std::function<void(H2DE_Object*)> onMouseDown = nullptr;
    std::function<void(H2DE_Object*)> onMouseUp = nullptr;
    std::function<void(H2DE_Object*)> onHover = nullptr;
    std::function<void(H2DE_Object*)> onBlur = nullptr;
    bool pauseSensitive = true;
};

struct H2DE_TextObjectData {
    std::string text = "";
    std::string font = "";
    H2DE_LevelSize fontSize = { 1.0f, 1.0f };
    H2DE_LevelSize spacing = { 0.1f, 0.5f };
    H2DE_TextAlign textAlign = H2DE_TEXT_ALIGN_LEFT;
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
};

float H2DE_Lerp(float min, float max, float blend, H2DE_Easing easing);
float H2DE_RandomFloatInRange(float min, float max);
int H2DE_RandomIntegerInRange(int min, int max);
bool H2DE_RandomBool();
H2DE_Flip H2DE_AddFlip(H2DE_Flip flip1, H2DE_Flip flip2);

#endif

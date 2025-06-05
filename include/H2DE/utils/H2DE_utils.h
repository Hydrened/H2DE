#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <functional>
#include <iostream>
#include <optional>
#include <random>
#include <SDL2/SDL_video.h>

#undef max

using H2DE_TimelineID = uint32_t;
using H2DE_ChannelID = int8_t;

#define H2DE_UINT8_MAX 255

class H2DE_Object;
class H2DE_ButtonObject;
class H2DE_Surface;

template<typename H2DE_Rect_T>
struct H2DE_Rect;
struct H2DE_ColorHSV;

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

enum H2DE_Face {
    H2DE_FACE_TOP       = 0b00,
    H2DE_FACE_RIGHT     = 0b01,
    H2DE_FACE_BOTTOM    = 0b10,
    H2DE_FACE_LEFT      = 0b11,
};

enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_TOP_LEFT        = 0b00001001,
    H2DE_TEXT_ALIGN_TOP_CENTER      = 0b00001010,
    H2DE_TEXT_ALIGN_TOP_RIGHT       = 0b00001100,
    H2DE_TEXT_ALIGN_CENTER_LEFT     = 0b00010001,
    H2DE_TEXT_ALIGN_CENTER_CENTER   = 0b00010010,
    H2DE_TEXT_ALIGN_CENTER_RIGHT    = 0b00010100,
    H2DE_TEXT_ALIGN_BOTTOM_LEFT     = 0b00100001,
    H2DE_TEXT_ALIGN_BOTTOM_CENTER   = 0b00100010,
    H2DE_TEXT_ALIGN_BOTTOM_RIGHT    = 0b00100100,
};

enum H2DE_ScaleMode {
    H2DE_SCALE_MODE_NEAREST,
    H2DE_SCALE_MODE_LINEAR,
    H2DE_SCALE_MODE_BEST,
};

enum H2DE_BlendMode {
    H2DE_BLEND_MODE_BLEND,
    H2DE_BLEND_MODE_ADD,
    H2DE_BLEND_MODE_MOD,
    H2DE_BLEND_MODE_MUL,
    H2DE_BLEND_MODE_INVALID,
    H2DE_BLEND_MODE_NONE,
};

template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x = 0.0f;
    H2DE_Vector2D_T y = 0.0f;

    inline H2DE_Vector2D<H2DE_Vector2D_T> operator+(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x + other.x, y + other.y };
    }
    inline H2DE_Vector2D<H2DE_Vector2D_T> operator-(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x - other.x, y - other.y };
    }
    inline H2DE_Vector2D<H2DE_Vector2D_T> operator*(float multiplier) const {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x * multiplier), static_cast<H2DE_Vector2D_T>(y * multiplier) };
    }
    inline H2DE_Vector2D<H2DE_Vector2D_T> operator/(float divider) const {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x / divider), static_cast<H2DE_Vector2D_T>(y / divider) };
    }

    H2DE_Vector2D<H2DE_Vector2D_T>& operator+=(const H2DE_Vector2D<H2DE_Vector2D_T>& other);
    H2DE_Vector2D<H2DE_Vector2D_T>& operator-=(const H2DE_Vector2D<H2DE_Vector2D_T>& other);
    H2DE_Vector2D<H2DE_Vector2D_T>& operator*=(float multiplier);
    H2DE_Vector2D<H2DE_Vector2D_T>& operator/=(float divider);

    inline bool operator==(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return (x == other.x && y == other.y); }
    inline bool operator!=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return !(*this == other); }
    inline bool operator>(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return std::abs(x) + std::abs(y) > std::abs(other.x) + std::abs(other.y); }
    inline bool operator>=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return std::abs(x) + std::abs(y) >= std::abs(other.x) + std::abs(other.y); }
    inline bool operator<(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return !(*this >= other); }
    inline bool operator<=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return !(*this > other); }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D<H2DE_Vector2D_T>& vec) {
        os << std::string("x: ") << vec.x << ", y: " << vec.y;
        return os;
    }

    inline operator SDL_Point() const { return { static_cast<int>(x), static_cast<int>(y) }; }


    inline H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D<H2DE_Vector2D_T>& scale) const{
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, scale.x, scale.y };   
    }
    inline H2DE_Rect<H2DE_Vector2D_T> makeNullRect() const {
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, static_cast<H2DE_Vector2D_T>(0.0f), static_cast<H2DE_Vector2D_T>(0.0f) };
    }

    inline bool isNull() const { return (x == 0 && y == 0); }
    H2DE_Vector2D<H2DE_Vector2D_T> rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& pivot, float angle) const;
    inline H2DE_Vector2D<H2DE_Vector2D_T> getCenter() const { return *this * 0.5f; }
    inline H2DE_Vector2D_T getDistanceSquared(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
        return std::pow(other.x - x, static_cast<H2DE_Vector2D_T>(2.0f)) + std::pow(other.y - y, static_cast<H2DE_Vector2D_T>(2.0f));
    }
    inline H2DE_Vector2D_T getDistance(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const { return std::sqrt(getDistanceSquared(other)); }
};

using H2DE_PixelPos = H2DE_Vector2D<int>;
using H2DE_PixelSize = H2DE_Vector2D<int>;
using H2DE_PixelPivot = H2DE_Vector2D<int>;
using H2DE_Translate = H2DE_Vector2D<float>;
using H2DE_Scale = H2DE_Vector2D<float>;
using H2DE_Pivot = H2DE_Vector2D<float>;

template<typename H2DE_Rect_T>
struct H2DE_Rect {
    H2DE_Rect_T x = 0.0f;
    H2DE_Rect_T y = 0.0f;
    H2DE_Rect_T w = 1.0f;
    H2DE_Rect_T h = 1.0f;

    inline H2DE_Rect<H2DE_Rect_T> operator+(const H2DE_Rect<H2DE_Rect_T>& other) const {
        return H2DE_Rect<H2DE_Rect_T>{
            x + other.x,
            y + other.y,
            w + other.w,
            h + other.h
        };
    }
    inline H2DE_Rect<H2DE_Rect_T> operator-(const H2DE_Rect<H2DE_Rect_T>& other) const {
        return H2DE_Rect<H2DE_Rect_T>{
            x - other.x,
            y - other.y,
            w - other.w,
            h - other.h
        };
    }
    inline H2DE_Rect<H2DE_Rect_T> operator*(float multiplier) const {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x * multiplier),
            static_cast<H2DE_Rect_T>(y * multiplier),
            static_cast<H2DE_Rect_T>(w * multiplier),
            static_cast<H2DE_Rect_T>(h * multiplier)
        };
    }
    inline H2DE_Rect<H2DE_Rect_T> operator/(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x / divider),
            static_cast<H2DE_Rect_T>(y / divider),
            static_cast<H2DE_Rect_T>(w / divider),
            static_cast<H2DE_Rect_T>(h / divider)
        };
    }

    H2DE_Rect<H2DE_Rect_T>& operator+=(const H2DE_Rect<H2DE_Rect_T>& other);
    H2DE_Rect<H2DE_Rect_T>& operator-=(const H2DE_Rect<H2DE_Rect_T>& other);
    H2DE_Rect<H2DE_Rect_T>& operator*=(float multiplier);
    H2DE_Rect<H2DE_Rect_T>& operator/=(float divider);

    inline bool operator==(const H2DE_Rect<H2DE_Rect_T>& other) const { return (x == other.x && y == other.y && w == other.w && h == other.h); }
    inline bool operator!=(const H2DE_Rect<H2DE_Rect_T>& other) const { return !(*this == other); }

    inline operator SDL_Rect() const {
        return SDL_Rect{
            static_cast<int>(std::round(x - w * 0.5f)),
            static_cast<int>(std::round(y - h * 0.5f)),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Rect<H2DE_Rect_T>& rect) {
        os << std::string("x: ") << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h;
        return os;
    }

    inline H2DE_Rect<H2DE_Rect_T> addTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const { return H2DE_Rect<H2DE_Rect_T>{ x + translate.x, y + translate.y, w, h }; }
    inline H2DE_Rect<H2DE_Rect_T> addScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const { return H2DE_Rect<H2DE_Rect_T>{ x, y, w + scale.x, h + scale.y }; }
    inline H2DE_Rect<H2DE_Rect_T> substractTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const { return H2DE_Rect<H2DE_Rect_T>{ x - translate.x, y - translate.y, w, h }; }
    inline H2DE_Rect<H2DE_Rect_T> substractScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const { return H2DE_Rect<H2DE_Rect_T>{ x, y, w - scale.x, h - scale.y }; }
    inline H2DE_Rect<H2DE_Rect_T> multiplyTranslate(float multiplier) const {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x * multiplier), static_cast<H2DE_Rect_T>(y * multiplier), w, h };
    }
    inline H2DE_Rect<H2DE_Rect_T> multiplyScale(float multiplier) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w * multiplier), static_cast<H2DE_Rect_T>(h * multiplier) };
    }
    inline H2DE_Rect<H2DE_Rect_T> divideTranslate(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x / divider), static_cast<H2DE_Rect_T>(y / divider), w, h };
    }
    inline H2DE_Rect<H2DE_Rect_T> divideScale(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w / divider), static_cast<H2DE_Rect_T>(h / divider) };
    }

    inline H2DE_Vector2D<H2DE_Rect_T> getTranslate() const { return H2DE_Vector2D<H2DE_Rect_T>{ x, y }; }
    inline H2DE_Vector2D<H2DE_Rect_T> getScale() const { return H2DE_Vector2D<H2DE_Rect_T>{ w, h }; }

    inline H2DE_Rect_T getMinX() const { return x - static_cast<H2DE_Rect_T>(w * 0.5f); }
    inline H2DE_Rect_T getMaxX() const { return x + static_cast<H2DE_Rect_T>(w * 0.5f); }
    inline H2DE_Rect_T getMinY() const { return y - static_cast<H2DE_Rect_T>(h * 0.5f); }
    inline H2DE_Rect_T getMaxY() const { return y + static_cast<H2DE_Rect_T>(h * 0.5f); }

    inline bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const {
        return (
            (std::abs(x - rect.x) < ((w + rect.w) * 0.5f)) &&
            (std::abs(y - rect.y) < ((h + rect.h) * 0.5f))
        );
    }
    inline bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate) const {
        return (
            (translate.x >= x - w * 0.5f) &&
            (translate.x <= x + w * 0.5f) &&
            (translate.y >= y - h * 0.5f) &&
            (translate.y <= y + h * 0.5f)
        );
    }
    bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate, float radius) const;
    const std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const;
    void snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face);

    std::array<H2DE_Vector2D<H2DE_Rect_T>, 4> getCorners() const;
};

using H2DE_PixelRect = H2DE_Rect<int>;
using H2DE_LevelRect = H2DE_Rect<float>;

struct H2DE_ColorRGB {
    uint8_t r = H2DE_UINT8_MAX;
    uint8_t g = H2DE_UINT8_MAX;
    uint8_t b = H2DE_UINT8_MAX;
    uint8_t a = H2DE_UINT8_MAX;

    explicit operator H2DE_ColorHSV() const;
    explicit operator uint32_t() const;

    inline bool operator==(const H2DE_ColorRGB& other) const { return (r == other.r&& g == other.g && b == other.b && a == other.a); }
    inline bool operator!=(const H2DE_ColorRGB& other) const { return !(*this == other); }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& color) {
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

    inline bool isVisible() const { return (a != 0); }
};

struct H2DE_ColorHSV {
    float h = 1.0f;
    float s = 1.0f;
    float v = 1.0f;
    float a = 1.0f;

    explicit operator H2DE_ColorRGB() const;

    inline bool operator==(const H2DE_ColorHSV& other) const { return (h == other.h && s == other.s && v == other.v, a == other.a); }
    inline bool operator!=(const H2DE_ColorHSV& other) const { return !(*this == other); }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& color) {
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

    inline bool isVisible() const { return (a != 0); }
};

struct H2DE_Padding {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;
};

struct H2DE_WindowData {
    uint16_t fps = 60;
    const char* title = "H2DE Window";
    H2DE_PixelPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
    H2DE_PixelSize size = { 1280, 720 };
    bool fullscreen = false;
    bool resizable = false;
    bool saveState = false;
    H2DE_WindowRatio ratio = H2DE_WINDOW_RATIO_NO_RATIO;
};

struct H2DE_CameraData {
    H2DE_Translate translate = { 0.0f, 0.0f };
    float gameWidth = 20.0f;
    float interfaceWidth = 20.0f;
    float smoothing = 0.0f;
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };
    H2DE_Face xOrigin = H2DE_FACE_LEFT;
    H2DE_Face yOrigin = H2DE_FACE_TOP;
    bool grid = true;
};

struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();
    H2DE_CameraData camera = H2DE_CameraData();
};

struct H2DE_Transform {
    H2DE_Translate translate = { 0.0f, 0.0f };
    H2DE_Scale scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    H2DE_Pivot pivot = { 0.0f, 0.0f };
};

struct H2DE_Hitbox {
    H2DE_Transform transform = H2DE_Transform();
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    int collisionIndex = 0;
    std::function<void(H2DE_Object*, H2DE_Face)> onCollide = nullptr;
};

struct H2DE_Text {
    std::string text = "";
    std::string font = "";
    H2DE_Scale fontSize = { 1.0f, 1.0f };
    H2DE_Scale spacing = { 0.1f, 0.3f };
    H2DE_TextAlign textAlign = H2DE_TEXT_ALIGN_CENTER_CENTER;
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct H2DE_Time {
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    uint16_t milliseconds = 0;
};

struct H2DE_ObjectData {
    H2DE_Transform transform = H2DE_Transform();
    uint8_t opacity = H2DE_UINT8_MAX;
    bool absolute = false;
    int index = 0;
};

struct H2DE_BarObjectData {
    float min = 0.0f;
    float max = 100.0f;
    float value = 0.0f;
};

struct H2DE_ButtonObjectData {
    H2DE_Text text = H2DE_Text();
    std::function<void(H2DE_ButtonObject*)> onMouseDown = nullptr;
    std::function<void(H2DE_ButtonObject*)> onMouseUp = nullptr;
    std::function<void(H2DE_ButtonObject*)> onHover = nullptr;
    std::function<void(H2DE_ButtonObject*)> onBlur = nullptr;
    bool pauseSensitive = true;
};

struct H2DE_TextObjectData {
    H2DE_Text text = H2DE_Text();
};

struct H2DE_TimerObjectData {
    H2DE_Time time = H2DE_Time();
    H2DE_Text text = H2DE_Text();
    char separator = ':';
    bool displayHours = false;
    bool displayMinutes = false;
    bool displaySeconds = true;
    bool displayMilliseconds = false;
    bool pauseSensitive = true;
};

struct H2DE_SurfaceData {
    H2DE_Transform transform = H2DE_Transform();
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;
    int index = 0;
};

struct H2DE_TextureData {
    std::string textureName = "";
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    std::optional<H2DE_PixelRect> srcRect = std::nullopt;
};

struct H2DE_SpriteData {
    std::string textureName = "";
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    H2DE_PixelPos startingPos = { 0, 0 };
    H2DE_PixelSize size = { 1, 1 };
    int spacing = 0;
    uint16_t nbFrame = 0;
    uint32_t delay = 200;
    bool pauseSensitive = true;
};

struct H2DE_ColorData {
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
};

struct H2DE_Font {
    std::string textureName = "";
    H2DE_PixelSize charSize = { 0, 0 };
    int spacing = 0;
    std::string charOrder = "";
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;
};

int H2DE_RandomIntegerInRange(int min, int max);
float H2DE_RandomFloatInRange(float min, float max);
bool H2DE_RandomBool();

float H2DE_Lerp(float min, float max, float blend, H2DE_Easing easing);

#endif

#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
struct H2DE_AbsSize;
struct H2DE_AbsRect;
struct H2DE_LevelSize;
struct H2DE_LevelVelocity;
struct H2DE_LevelHitbox;
struct H2DE_ColorHSV;

enum H2DE_Face {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

enum H2DE_Flip {
    H2DE_NO_FLIP,
    H2DE_FLIP_HORIZONTAL,
    H2DE_FLIP_VERTICAL,
};

enum H2DE_ScaleMode {
    H2DE_SCALE_MODE_NEAREST,
    H2DE_SCALE_MODE_LINEAR,
    H2DE_SCALE_MODE_BEST,
};

struct H2DE_AbsPos {
    int x;
    int y;

    H2DE_AbsRect makeRect(H2DE_AbsSize size) const;

    operator SDL_Point() const;
};

struct H2DE_AbsSize {
    int w;
    int h;

    H2DE_AbsRect makeRect(H2DE_AbsPos pos) const;
};

struct H2DE_AbsRect {
    int x;
    int y;
    int w;
    int h;

    operator SDL_Rect() const;
};

struct H2DE_LevelPos {
    float x;
    float y;

    H2DE_LevelPos operator+(const H2DE_LevelPos& other) const;
    H2DE_LevelPos operator-(const H2DE_LevelPos& other) const;
    H2DE_LevelPos operator*(const float& multiplier) const;
    H2DE_LevelPos operator/(const float& divider) const;

    H2DE_LevelPos operator+(const H2DE_LevelVelocity& velocity) const;
    H2DE_LevelPos operator-(const H2DE_LevelVelocity& velocity) const;

    bool operator==(const H2DE_LevelPos& other) const;

    H2DE_LevelHitbox makeHitbox(const H2DE_LevelSize& size) const;
};

struct H2DE_LevelSize {
    float w;
    float h;

    H2DE_LevelSize operator+(const H2DE_LevelSize& other) const;
    H2DE_LevelSize operator-(const H2DE_LevelSize& other) const;
    H2DE_LevelSize operator*(const float& multiplier) const;
    H2DE_LevelSize operator/(const float& divider) const;

    bool operator==(const H2DE_LevelSize& other) const;
    bool operator>=(const H2DE_LevelSize& other) const;
    bool operator<=(const H2DE_LevelSize& other) const;
    bool operator>(const H2DE_LevelSize& other) const;
    bool operator<(const H2DE_LevelSize& other) const;

    H2DE_LevelHitbox makeHitbox(const H2DE_LevelPos& pos) const;
    H2DE_LevelPos toPos() const;
};

struct H2DE_LevelVelocity {
    float x;
    float y;

    H2DE_LevelVelocity operator+(const H2DE_LevelVelocity& other) const;
    H2DE_LevelVelocity operator-(const H2DE_LevelVelocity& other) const;
    H2DE_LevelVelocity operator*(const float& multiplier) const;
    H2DE_LevelVelocity operator/(const float& divider) const;

    H2DE_LevelPos operator+(const H2DE_LevelPos& pos) const;
    
    bool operator==(const H2DE_LevelVelocity& other) const;
    bool operator>=(const H2DE_LevelVelocity& other) const;
    bool operator<=(const H2DE_LevelVelocity& other) const;
    bool operator>(const H2DE_LevelVelocity& other) const;
    bool operator<(const H2DE_LevelVelocity& other) const;

    bool isNull() const;
};

struct H2DE_LevelHitbox {
    float x;
    float y;
    float w;
    float h;

    H2DE_LevelHitbox operator+(const H2DE_LevelPos& pos) const;
    H2DE_LevelHitbox operator-(const H2DE_LevelPos& pos) const;

    H2DE_LevelHitbox operator+(const H2DE_LevelSize& size) const;
    H2DE_LevelHitbox operator-(const H2DE_LevelSize& size) const;

    H2DE_LevelPos getPos() const;
    H2DE_LevelSize getSize() const;
    H2DE_LevelPos getCenter() const;

    H2DE_Face collides(const H2DE_LevelHitbox& other) const;
};

struct H2DE_Scale {
    float x;
    float y;
};

struct H2DE_ColorRGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    explicit operator H2DE_ColorHSV() const;

    bool operator==(const H2DE_ColorRGB& other) const;

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

    explicit operator H2DE_ColorRGB() const;

    bool operator==(const H2DE_ColorHSV& other) const;

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
    std::string title = "";
    H2DE_AbsPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
    H2DE_AbsSize size = { 640, 360 };
    bool fullscreen = false;
    bool resizable = false;
    bool saveState = false;
};

struct H2DE_CameraData {
    float blockOnWidth = 20.0f;
};

struct H2DE_EngineData {
    unsigned int fps = 60;
    H2DE_WindowData window = H2DE_WindowData();
    H2DE_CameraData camera = H2DE_CameraData();
};

std::ostream& operator<<(std::ostream& os, const H2DE_AbsPos& pos);
std::ostream& operator<<(std::ostream& os, const H2DE_AbsSize& size);
std::ostream& operator<<(std::ostream& os, const H2DE_AbsRect& rect);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelPos& pos);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelSize& size);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelVelocity& velocity);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelHitbox& hitbox);
std::ostream& operator<<(std::ostream& os, const H2DE_Scale& hsv);
std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& rgb);
std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& hsv);

#endif

#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
struct H2DE_AbsSize;
struct H2DE_AbsRect;
struct H2DE_LevelSize;
struct H2DE_LevelVelocity;
struct H2DE_LevelRect;
struct H2DE_ColorHSV;
class H2DE_LevelObject;
class H2DE_Surface;

/**
 * Types used to identify faces
 * \since H2DE-2.0.2
 */
enum H2DE_Face {
    H2DE_NO_FACE,                                                                           // No face
    H2DE_TOP_FACE,                                                                          // Top face
    H2DE_BOTTOM_FACE,                                                                       // Bottom face
    H2DE_LEFT_FACE,                                                                         // Left face
    H2DE_RIGHT_FACE,                                                                        // right face
};

/**
 * Types used to identify flips
 * \since H2DE-2.0.2
 */
enum H2DE_Flip {
    H2DE_NO_FLIP,                                                                           // No flip
    H2DE_FLIP_HORIZONTAL,                                                                   // Flip horizontaly
    H2DE_FLIP_VERTICAL,                                                                     // Flip verticaly
};

/**
 * Types used to identify scale modes
 * \since H2DE-2.0.2
 */
enum H2DE_ScaleMode {
    H2DE_SCALE_MODE_NEAREST,                                                                // Nearest scale mode
    H2DE_SCALE_MODE_LINEAR,                                                                 // Linear scale mode
    H2DE_SCALE_MODE_BEST,                                                                   // Best scale mode
};

/**
 * Types used to identify timeline effects
 * \since H2DE-1.0.9
 */
enum H2DE_TimelineEffect {
    LINEAR,                                                                                 // Linear
    EASE_IN_OUT,                                                                            // Ease in out
    EASE_IN,                                                                                // Ease in
    EASE_OUT,                                                                               // Ease out
};

/**
 * Types used to identify absolute positions
 * \since H2DE-2.0.2
 */
struct H2DE_AbsPos {
    int x;                                                                                  // Absolute x position
    int y;                                                                                  // Absolute y position

    H2DE_AbsPos operator+(const H2DE_AbsSize& size) const;

    /**
     * Makes a rectangle with a size
     * \param size absolute size of the rectangle
     * \returns an absolute rectangle
     * \since H2DE-2.0.2
     */
    H2DE_AbsRect makeRect(H2DE_AbsSize size) const;

    operator SDL_Point() const;
};

/**
 * Types used to identify absolute sizes
 * \since H2DE-2.0.2
 */
struct H2DE_AbsSize {
    int w;                                                                                  // Absolute width
    int h;                                                                                  // Absolute height

    /**
     * Makes a rectangle with a position
     * \param position absolute position of the rectangle
     * \returns an absolute rectangle
     * \since H2DE-2.0.2
     */
    H2DE_AbsRect makeRect(H2DE_AbsPos pos) const;
};

/**
 * Types used to identify absolute rectangles
 * \since H2DE-2.0.2
 */
struct H2DE_AbsRect {
    int x;                                                                                  // Absolute x position
    int y;                                                                                  // Absolute y position
    int w;                                                                                  // Absolute width
    int h;                                                                                  // Absolute height

    operator SDL_Rect() const;

    /**
     * Checks if a absolute position is in this rectangle
     * \param pos absolute position of the point
     * \returns a boolean
     * \since H2DE-2.0.10
     */
    bool contains(const H2DE_AbsPos& pos);
};

/**
 * Types used to identify level positions
 * \since H2DE-2.0.2
 */
struct H2DE_LevelPos {
    float x;                                                                                // Level x position
    float y;                                                                                // Level y position

    H2DE_LevelPos operator+(const H2DE_LevelPos& other) const;
    H2DE_LevelPos operator-(const H2DE_LevelPos& other) const;
    H2DE_LevelPos operator*(const float& multiplier) const;
    H2DE_LevelPos operator/(const float& divider) const;

    H2DE_LevelPos operator+(const H2DE_LevelVelocity& velocity) const;
    H2DE_LevelPos operator-(const H2DE_LevelVelocity& velocity) const;

    bool operator==(const H2DE_LevelPos& other) const;

    /**
     * Makes a rectangle with a size
     * \param size level size of the rectangle
     * \returns a level rectangle
     * \since H2DE-2.0.2
     */
    H2DE_LevelRect makeHitbox(const H2DE_LevelSize& size) const;
};

/**
 * Types used to identify level sizes
 * \since H2DE-2.0.2
 */
struct H2DE_LevelSize {
    float w;                                                                                // Level width
    float h;                                                                                // Level height

    H2DE_LevelSize operator+(const H2DE_LevelSize& other) const;
    H2DE_LevelSize operator-(const H2DE_LevelSize& other) const;
    H2DE_LevelSize operator*(const float& multiplier) const;
    H2DE_LevelSize operator/(const float& divider) const;

    bool operator==(const H2DE_LevelSize& other) const;
    bool operator>=(const H2DE_LevelSize& other) const;
    bool operator<=(const H2DE_LevelSize& other) const;
    bool operator>(const H2DE_LevelSize& other) const;
    bool operator<(const H2DE_LevelSize& other) const;

    /**
     * Makes a rectangle with a position
     * \param position level position of the rectangle
     * \returns a level rectangle
     * \since H2DE-2.0.2
     */
    H2DE_LevelRect makeHitbox(const H2DE_LevelPos& pos) const;
    /**
     * Converts to a level position
     * \returns a level position
     * \since H2DE-2.0.5
     */
    H2DE_LevelPos toPos() const;
};

/**
 * Types used to identify level velocities
 * \since H2DE-2.0.2
 */
struct H2DE_LevelVelocity {
    float x;                                                                                // Level x velocity
    float y;                                                                                // Level y velocity

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

    /**
     * Says if the velocity is null (x == 0.0f && y == 0.0f)
     * \returns boolean
     * \since H2DE-2.0.5
     */
    bool isNull() const;
};

/**
 * Types used to identify level rectangles
 * \since H2DE-2.0.2
 */
struct H2DE_LevelRect {
    float x;                                                                                // Level x position
    float y;                                                                                // Level y position
    float w;                                                                                // Level width
    float h;                                                                                // Level height

    H2DE_LevelRect operator+(const H2DE_LevelPos& pos) const;
    H2DE_LevelRect operator-(const H2DE_LevelPos& pos) const;

    H2DE_LevelRect operator+(const H2DE_LevelSize& size) const;
    H2DE_LevelRect operator-(const H2DE_LevelSize& size) const;

    /**
     * Gets the level position from the rectangle
     * \returns the level position of the rectangle
     * \since H2DE-2.0.3
     */
    H2DE_LevelPos getPos() const;
    /**
     * Gets the level size from the rectangle
     * \returns the level size of the rectangle
     * \since H2DE-2.0.3
     */
    H2DE_LevelSize getSize() const;
    /**
     * Gets the center from the rectangle
     * \returns the center of the rectangle
     * \since H2DE-2.0.3
     */
    H2DE_LevelPos getCenter() const;

    /**
     * Gets the face that collides with another level rectangle
     * \param other the other rectangle
     * \returns the face that collides with the other level rectangle
     * \since H2DE-2.0.2
     */
    H2DE_Face collides(const H2DE_LevelRect& other) const;
};

/**
 * Types used to identify rgb colors
 * \since H2DE-1.0.0
 */
struct H2DE_ColorRGB {
    Uint8 r;                                                                            // Red
    Uint8 g;                                                                            // Green
    Uint8 b;                                                                            // Blue
    Uint8 a;                                                                            // Alpha (opacity)

    explicit operator H2DE_ColorHSV() const;
    operator Uint32() const;

    bool operator==(const H2DE_ColorRGB& other) const;

    /**
     * Adds a hue to a rgb color
     * \param hue hue to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB addHue(const float& hue) const;
    /**
     * Adds saturation to a rgb color
     * \param saturation saturation to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB addSaturation(const float& saturation) const;
    /**
     * Adds value to a rgb color
     * \param value value to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB addValue(const float& value) const;

    /**
     * Substracts hue to a rgb color
     * \param hue hue to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB subtractHue(const float& hue) const;
    /**
     * Substracts saturation to a rgb color
     * \param saturation saturation to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB subtractSaturation(const float& saturation) const;
    /**
     * Substracts value to a rgb color
     * \param value value to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB subtractValue(const float& value) const;  

    /**
     * Multiplies hue to a rgb color
     * \param multiplier hue multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB multiplyHue(const float& multiplier) const;
    /**
     * Multiplies saturation to a rgb color
     * \param multiplier saturation multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB multiplySaturation(const float& multiplier) const;
    /**
     * Multiplies value to a rgb color
     * \param multiplier value multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB multiplyValue(const float& multiplier) const;

    /**
     * Divides hue to a rgb color
     * \param divider hue divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB divideHue(const float& divider) const;
    /**
     * Divides saturation to a rgb color
     * \param divider saturation divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB divideSaturation(const float& divider) const;
    /**
     * Divides value to a rgb color
     * \param divider value divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorRGB divideValue(const float& divider) const;
};

/**
 * Types used to identify hsv colors
 * \since H2DE-1.2.0
 */
struct H2DE_ColorHSV {
    float h;                                                                            // Hue
    float s;                                                                            // Saturation
    float v;                                                                            // Value / lightness
    float a;                                                                            // Alpha (opacity)

    explicit operator H2DE_ColorRGB() const;

    bool operator==(const H2DE_ColorHSV& other) const;

    /**
     * Adds a hue to a hsv color
     * \param hue hue to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV addHue(const float& hue) const;
    /**
     * Adds saturation to a hsv color
     * \param saturation saturation to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV addSaturation(const float& saturation) const;
    /**
     * Adds value to a hsv color
     * \param value value to add
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV addValue(const float& value) const;

    /**
     * Substracts hue to a hsv color
     * \param hue hue to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV subtractHue(const float& hue) const;
    /**
     * Substracts saturation to a hsv color
     * \param saturation saturation to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV subtractSaturation(const float& saturation) const;
    /**
     * Substracts value to a hsv color
     * \param value value to substract
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV subtractValue(const float& value) const;

    /**
     * Multiplies hue to a hsv color
     * \param multiplier hue multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV multiplyHue(const float& multiplier) const;
    /**
     * Multiplies saturation to a hsv color
     * \param multiplier saturation multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV multiplySaturation(const float& multiplier) const;
    /**
     * Multiplies value to a hsv color
     * \param multiplier value multiplier
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV multiplyValue(const float& multiplier) const;

    /**
     * Divides hue to a hsv color
     * \param divider hue divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV divideHue(const float& divider) const;
    /**
     * Divides saturation to a hsv color
     * \param divider saturation divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV divideSaturation(const float& divider) const;
    /**
     * Divides value to a hsv color
     * \param divider value divider
     * \since H2DE-1.2.0
     */
    H2DE_ColorHSV divideValue(const float& divider) const;
};

/**
 * Types used to identify window data
 * \since H2DE-2.0.0
 */
struct H2DE_WindowData {
    std::string title = "";                                                             // Title of a window
    H2DE_AbsPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };               // Absolute position of a window
    H2DE_AbsSize size = { 640, 360 };                                                   // Absolute size of a window
    bool fullscreen = false;                                                            // Whether the window should be on fullscreen
    bool resizable = false;                                                             // Whether the window should be resizable
    bool saveState = false;                                                             // Whether the window data should be saved
};

/**
 * Types used to identify camera data
 * \since H2DE-2.0.2
 */
struct H2DE_CameraData {
    float width = 20.0f;                                                                // Width of the camera
    float smoothing = 0.0f;                                                             // Smoothing (0.0f = no smoothing, 1.0f = max smoothing)
};

/**
 * Types used to identify engine data
 * \since H2DE-2.0.0
 */
struct H2DE_EngineData {
    unsigned int fps = 60;                                                              // FPS limit for an engine and the window (60 recommended)
    H2DE_WindowData window = H2DE_WindowData();                                         // Window data
    H2DE_CameraData camera = H2DE_CameraData();                                         // Camera data
};

/**
 * Types used to identify texture data
 * \since H2DE-2.0.13
 */
struct H2DE_TextureData {
    std::string name = "";                                                              // Name of the texture
    H2DE_LevelSize size = { 1.0f, 1.0f };                                               // Level size of the texture
    std::optional<H2DE_AbsRect> srcRect = std::nullopt;                                 // Part of the texture you wanna display
    H2DE_ColorRGB color = { 255, 255, 255, 255 };                                       // RGB color aplied on the texture
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;                                  // Scale mode the texture will be rendered by
};

/**
 * Types used to identify sprite data
 * \since H2DE-2.0.13
 */
struct H2DE_SpriteData {
    std::string defaultAnimationName = "";                                              // Name of the sprite animation
    unsigned int nbFrame = 0;                                                           // Number of sprite's frame
    unsigned int delay = 200;                                                           // Delay between frames
};

/**
 * Types used to identify hitboxes data
 * \since H2DE-2.0.5
 */
struct H2DE_Hitbox {
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };                                   // Level rectangle of the hitbox
    H2DE_ColorRGB color = { 255, 0, 0, 255 };                                           // RGB color of the hitbox
    int collisionIndex = 0;                                                             // Collision index of the hitbox (it can only collides with same collision index)
    bool snap = false;                                                                  // Snap when colliding to another hitbox
    std::optional<std::function<void()>> onCollide = std::nullopt;                      // Function called when colliding to another hitbox
};

/**
 * Types used to identify level object transform
 * \since H2DE-2.0.2
 */
struct H2DE_LevelObjectTransform {
    H2DE_LevelPos origin = { 0.0f, 0.0f };                                              // Level position of the point for the roatation
    float rotation = 0.0f;                                                              // Rotation of the level object
    H2DE_Flip flip = H2DE_NO_FLIP;                                                      // Flip of the level object
};

/**
 * Types used to identify level object data
 * \since H2DE-2.0.2
 */
struct H2DE_LevelObjectData {
    H2DE_LevelPos pos = { 0.0f, 0.0f };                                                 // Level position of the level object
    H2DE_LevelVelocity velocity = { 0.0f, 0.0f };                                       // Level velocity of the level object
    std::vector<H2DE_Hitbox> hitboxes = {};                                             // Hitboxes of the level object

    H2DE_Surface* texture;                                                              // Texture / sprite of the level object

    bool absolute = false;                                                              // Whether the level object is absolute
    bool gravity = false;                                                               // Whether the level object has default gravity

    H2DE_LevelObjectTransform transform = H2DE_LevelObjectTransform();                  // Transform data of the level object

    std::optional<H2DE_LevelObject*> parent = std::nullopt;                             // Parent of the level object
    int index = 0;                                                                      // Z-index of the level object (Greather == on top of other level objects)
};

/**
 * Types used to identify button data
 * \since H2DE-2.0.10
 */
struct H2DE_ButtonData {
    H2DE_LevelPos pos = { 0.0f, 0.0f };                                                 // Level position of the button (ignores camera"s position)
    H2DE_Surface* texture;                                                              // Texture / sprite of the button
    std::function<void()> onClick = NULL;                                               // On click function call
};

std::ostream& operator<<(std::ostream& os, const H2DE_AbsPos& pos);
std::ostream& operator<<(std::ostream& os, const H2DE_AbsSize& size);
std::ostream& operator<<(std::ostream& os, const H2DE_AbsRect& rect);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelPos& pos);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelSize& size);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelVelocity& velocity);
std::ostream& operator<<(std::ostream& os, const H2DE_LevelRect& hitbox);
std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& rgb);
std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& hsv);

/**
 * \since HH2DE-2.0.6
 */
float H2DE_Lerp(float min, float max, float blend);

#endif

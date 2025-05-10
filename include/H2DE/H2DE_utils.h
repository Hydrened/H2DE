#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

/**
 * @file H2DE_utils.h
 * @brief This file contains utility functions and common helpers used throughout the H2DE engine.
 * 
 * The functions and utilities defined here are designed to facilitate various tasks such as 
 * mathematical operations, randomization, and common object manipulations.
 */

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

/**
 * Represents the side (or face) of an object.
 *
 * These values are typically used for collision and orientation.
 */
enum H2DE_Face {
    H2DE_FACE_TOP = 0b00,       //< Top face
    H2DE_FACE_RIGHT = 0b01,     //< Right face
    H2DE_FACE_BOTTOM = 0b10,    //< Bottom face
    H2DE_FACE_LEFT = 0b11,      //< Left face
};

/**
 * Describes predefined window aspect ratios.
 *
 * Used to maintain consistent scaling and display proportions.
 */
enum H2DE_WindowRatio {
    H2DE_WINDOW_RATIO_NO_RATIO,  //< No ratio constraint (free resize)
    H2DE_WINDOW_RATIO_CUSTOM,    //< User-defined custom ratio
    H2DE_WINDOW_RATIO_4_3,       //< 4:3 ratio (standard)
    H2DE_WINDOW_RATIO_3_2,       //< 3:2 ratio
    H2DE_WINDOW_RATIO_5_4,       //< 5:4 ratio
    H2DE_WINDOW_RATIO_16_10,     //< 16:10 ratio (widescreen)
    H2DE_WINDOW_RATIO_16_9,      //< 16:9 ratio (HD widescreen)
    H2DE_WINDOW_RATIO_21_9,      //< 21:9 ratio (ultrawide)
    H2DE_WINDOW_RATIO_32_9,      //< 32:9 ratio (super ultrawide)
};

/**
 * Represents horizontal and vertical flipping options for rendering.
 */
enum H2DE_Flip {
    H2DE_FLIP_NONE = 0b00,  //< No flipping
    H2DE_FLIP_X = 0b01,     //< Flip horizontally
    H2DE_FLIP_Y = 0b10,     //< Flip vertically
    H2DE_FLIP_XY = 0b11,    //< Flip both horizontally and vertically
};

/**
 * Defines scaling quality modes for rendering textures.
 */
enum H2DE_ScaleMode {
    H2DE_SCALE_MODE_NEAREST, //< Nearest-neighbor scaling (fastest, pixelated)
    H2DE_SCALE_MODE_LINEAR,  //< Linear interpolation (smooth)
    H2DE_SCALE_MODE_BEST,    //< Best quality available (may be hardware-dependent)
};

/**
 * Defines blending modes used during rendering.
 */
enum H2DE_BlendMode {
    H2DE_BLEND_MODE_BLEND,   //< Standard alpha blending
    H2DE_BLEND_MODE_ADD,     //< Additive blending
    H2DE_BLEND_MODE_MOD,     //< Modulation blending (multiplicative color)
    H2DE_BLEND_MODE_MUL,     //< True multiplicative blending
    H2DE_BLEND_MODE_INVALID, //< Invalid or undefined blending mode
    H2DE_BLEND_MODE_NONE,    //< No blending (overwrite)
};

/**
 * Horizontal alignment for rendering text.
 */
enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_LEFT,   //< Align text to the left
    H2DE_TEXT_ALIGN_RIGHT,  //< Align text to the right
    H2DE_TEXT_ALIGN_CENTER, //< Center the text horizontally
};

/**
 * Easing functions used for animations and interpolations.
 *
 * These define how a value progresses over time, such as gradually speeding up,
 * slowing down, or applying more complex motions like bouncing or elastic effects.
 */
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

/**
 * @brief Generic 2D vector template.
 *
 * This class represents a 2D vector with x and y components of any numeric type.
 * It supports basic arithmetic operations, comparisons, and geometric utilities.
 *
 * @tparam H2DE_Vector2D_T The numeric type used for coordinates (e.g., int, float).
 */
template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x; //< Horizontal component
    H2DE_Vector2D_T y; //< Vertical component

    /**
     * @brief Adds two vectors component-wise.
     */
    inline H2DE_Vector2D operator+(const H2DE_Vector2D& other) const { return H2DE_Vector2D{ x + other.x, y + other.y }; }
    /**
     * @brief Subtracts two vectors component-wise.
     */
    inline H2DE_Vector2D operator-(const H2DE_Vector2D& other) const { return H2DE_Vector2D{ x - other.x, y - other.y }; }
    /**
     * @brief Multiplies the vector by a scalar.
     */
    inline H2DE_Vector2D operator*(float multiplier) const { return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x * multiplier), static_cast<H2DE_Vector2D_T>(y * multiplier) }; }
    /**
     * @brief Divides the vector by a scalar.
     */
    inline H2DE_Vector2D operator/(float divider) const { return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x / divider), static_cast<H2DE_Vector2D_T>(y / divider) }; }

    /**
     * @brief Adds another vector to this one.
     */
    H2DE_Vector2D& operator+=(const H2DE_Vector2D& other);
    /**
     * @brief Subtracts another vector from this one.
     */
    H2DE_Vector2D& operator-=(const H2DE_Vector2D& other);
    /**
     * @brief Multiplies this vector by a scalar.
     */
    H2DE_Vector2D& operator*=(float multiplier);
    /**
     * @brief Divides this vector by a scalar.
     */
    H2DE_Vector2D& operator/=(float divider);

    /**
     * @brief Checks if two vectors are exactly equal.
     */
    inline bool operator==(const H2DE_Vector2D& other) const { return (x == other.x && y == other.y); }
    /**
     * @brief Checks if two vectors are not equal.
     */
    inline bool operator!=(const H2DE_Vector2D& other) const { return !(*this == other); }
    /**
     * @brief Checks if this vector's magnitude is strictly greater than the other's.
     *
     * @param other The vector to compare against.
     * @return True if the length of this vector is greater than that of the other.
     */
    inline bool operator>(const H2DE_Vector2D& other) const { return std::abs(x) + std::abs(y) > std::abs(other.x) + std::abs(other.y); }
    /**
     * @brief Checks if this vector's magnitude is greater than or equal to the other's.
     *
     * @param other The vector to compare against.
     * @return True if the length of this vector is greater than or equal to that of the other.
     */
    inline bool operator>=(const H2DE_Vector2D& other) const { return std::abs(x) + std::abs(y) >= std::abs(other.x) + std::abs(other.y); }
    /**
     * @brief Checks if this vector's magnitude is strictly less than the other's.
     *
     * @param other The vector to compare against.
     * @return True if the length of this vector is less than that of the other.
     */
    inline bool operator<(const H2DE_Vector2D& other) const { return !(*this >= other); 
    }
    /**
     * @brief Checks if this vector's magnitude is less than or equal to the other's.
     *
     * @param other The vector to compare against.
     * @return True if the length of this vector is less than or equal to that of the other.
     */
    inline bool operator<=(const H2DE_Vector2D& other) const { return !(*this > other); }

    /**
     * @brief Outputs the vector in the format "x: value, y: value".
     */
    friend std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D& vec) {
        os << std::string("x: ") << vec.x << ", y: " << vec.y;
        return os;
    }

    /**
     * @brief Converts the vector to an SDL_Point (int).
     */
    inline operator SDL_Point() const { return { static_cast<int>(x), static_cast<int>(y) }; }

    /**
     * @brief Creates a rectangle from this position and a given size.
     *
     * @param size The size of the rectangle.
     * @return A rectangle starting at this position.
     */
    inline H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D& size) const { return H2DE_Rect<H2DE_Vector2D_T>{ x, y, size.x, size.y }; }
    /**
     * @brief Creates a zero-sized rectangle at this position.
     */
    inline H2DE_Rect<H2DE_Vector2D_T> makeNullRect() const { return H2DE_Rect<H2DE_Vector2D_T>{ x, y, static_cast<H2DE_Vector2D_T>(0.0f), static_cast<H2DE_Vector2D_T>(0.0f) }; }

    /**
     * @brief Checks if both x and y are zero.
     */
    inline bool isNull() const { return (x == 0 && y == 0); }
    /**
     * @brief Rotates the vector around a center point by a given angle in degrees.
     *
     * @param center The point around which to rotate.
     * @param angle The angle in degrees.
     * @return The rotated vector.
     */
    H2DE_Vector2D<H2DE_Vector2D_T> rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& center, float angle);
    /**
     * @brief Returns the center of the vector (useful for sizes).
     *
     * For example, for a size of (w, h), this returns (w/2, h/2).
     */
    inline H2DE_Vector2D<H2DE_Vector2D_T> getCenter() const { return *this / 2; }
};

/// @brief Absolute pixel position on screen (int-based)
using H2DE_AbsPos = H2DE_Vector2D<int>;
/// @brief Absolute pixel size on screen (int-based)
using H2DE_AbsSize = H2DE_Vector2D<int>;
/// @brief Position in level/world coordinates (float-based)
using H2DE_LevelPos = H2DE_Vector2D<float>;
/// @brief Size in level/world coordinates (float-based)
using H2DE_LevelSize = H2DE_Vector2D<float>;
/// @brief Velocity in level/world coordinates (float-based)
using H2DE_LevelVelocity = H2DE_Vector2D<float>;

/**
 * @brief Represents a 2D rectangle with a position (x, y) and size (w, h).
 * 
 * @tparam H2DE_Rect_T The numeric type used (e.g., int, float).
 */
template<typename H2DE_Rect_T>
struct H2DE_Rect {
    H2DE_Rect_T x; //< X position of the rectangle.
    H2DE_Rect_T y; //< Y position of the rectangle.
    H2DE_Rect_T w; //< Width of the rectangle.
    H2DE_Rect_T h; //< Height of the rectangle.

    /**
     * @brief Adds the position and size of another rectangle to this one.
     */
    inline H2DE_Rect operator+(const H2DE_Rect& other) const {
        return H2DE_Rect{
            x + other.x,
            y + other.y,
            w + other.w,
            h + other.h
        };
    }
    /**
     * @brief Subtracts the position and size of another rectangle from this one.
     */
    inline H2DE_Rect operator-(const H2DE_Rect& other) const {
        return H2DE_Rect{
            x - other.x,
            y - other.y,
            w - other.w,
            h - other.h
        };
    }
    /**
     * @brief Multiplies all components of the rectangle by a scalar.
     */
    inline H2DE_Rect operator*(float multiplier) const {
        return H2DE_Rect{
            static_cast<H2DE_Rect_T>(x * multiplier),
            static_cast<H2DE_Rect_T>(y * multiplier),
            static_cast<H2DE_Rect_T>(w * multiplier),
            static_cast<H2DE_Rect_T>(h * multiplier)
        };
    }
    /**
     * @brief Divides all components of the rectangle by a scalar.
     */
    inline H2DE_Rect operator/(float divider) const {
        return H2DE_Rect{
            static_cast<H2DE_Rect_T>(x / divider),
            static_cast<H2DE_Rect_T>(y / divider),
            static_cast<H2DE_Rect_T>(w / divider),
            static_cast<H2DE_Rect_T>(h / divider)
        };
    }

    /**
     * @brief Adds all components of another rectangle to this one.
     * 
     * @param other The rectangle to add.
     * @return Reference to this rectangle.
     */
    H2DE_Rect& operator+=(const H2DE_Rect& other);
    /**
     * @brief Subtracts all components of another rectangle from this one.
     * 
     * @param other The rectangle to subtract.
     * @return Reference to this rectangle.
     */
    H2DE_Rect& operator-=(const H2DE_Rect& other);
    /**
     * @brief Multiplies all components of the rectangle by a scalar.
     * 
     * @param multiplier The value to multiply with.
     * @return Reference to this rectangle.
     */
    H2DE_Rect& operator*=(float multiplier);
    /**
     * @brief Divides all components of the rectangle by a scalar.
     * 
     * @param divider The value to divide by.
     * @return Reference to this rectangle.
     */
    H2DE_Rect& operator/=(float divider);

    /**
     * @brief Checks if all components of both rectangles are equal.
     */
    inline bool operator==(const H2DE_Rect& other) const { return (x == other.x && y == other.y && w == other.w && h == other.h); }
    /**
     * @brief Checks if any component differs between the rectangles.
     */
    inline bool operator!=(const H2DE_Rect& other) const { return !(*this == other); }

    /**
     * @brief Converts to SDL_Rect.
     */
    inline operator SDL_Rect() const {
        return SDL_Rect{
            static_cast<int>(x),
            static_cast<int>(y),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }

    /**
     * @brief Prints the rectangle in the format: x: ?, y: ?, w: ?, h: ?.
     */
    friend std::ostream& operator<<(std::ostream& os, const H2DE_Rect& rect) {
        os << std::string("x: ") << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h;
        return os;
    }

    /**
     * @brief Adds a vector to the position (x, y) of the rectangle.
     * 
     * @param pos The position vector to add.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect addPos(const H2DE_Vector2D<H2DE_Rect_T>& pos);
    /**
     * @brief Adds a vector to the size (w, h) of the rectangle.
     * 
     * @param size The size vector to add.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect addSize(const H2DE_Vector2D<H2DE_Rect_T>& size);
    /**
     * @brief Subtracts a vector from the position (x, y) of the rectangle.
     * 
     * @param pos The position vector to subtract.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect substractPos(const H2DE_Vector2D<H2DE_Rect_T>& pos);
    /**
     * @brief Subtracts a vector from the size (w, h) of the rectangle.
     * 
     * @param size The size vector to subtract.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect substractSize(const H2DE_Vector2D<H2DE_Rect_T>& size);
    /**
     * @brief Multiplies the position (x, y) of the rectangle by a scalar.
     * 
     * @param multiplier The value to multiply with.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect multiplyPos(float multiplier);
    /**
     * @brief Multiplies the size (w, h) of the rectangle by a scalar.
     * 
     * @param multiplier The value to multiply with.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect multiplySize(float multiplier);
    /**
     * @brief Divides the position (x, y) of the rectangle by a scalar.
     * 
     * @param divider The value to divide by.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect dividePos(float divider);
    /**
     * @brief Divides the size (w, h) of the rectangle by a scalar.
     * 
     * @param divider The value to divide by.
     * @return A copy of the modified rectangle.
     */
    H2DE_Rect divideSize(float divider);

    /**
     * @brief Returns the position (x, y) of the rectangle.
     */
    inline H2DE_Vector2D<H2DE_Rect_T> getPos() const { return H2DE_Vector2D<H2DE_Rect_T>{ x, y }; }
    /**
     * @brief Returns the size (w, h) of the rectangle.
     */
    inline H2DE_Vector2D<H2DE_Rect_T> getSize() const { return H2DE_Vector2D<H2DE_Rect_T>{ w, h }; }
    /**
     * @brief Returns the center position of the rectangle.
     */
    inline H2DE_Vector2D<H2DE_Rect_T> getCenter() const  { return getPos() + getSize().getCenter(); }

    /**
     * @brief Checks if this rectangle overlaps another rectangle.
     */
    inline bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const {
        return (
            rect.x + rect.w > x &&
            rect.x < x + w &&
            rect.y + rect.h > y &&
            rect.y < y + h
        );
    }
    /**
     * @brief Checks if a point is inside this rectangle.
     */
    inline bool collides(const H2DE_Vector2D<H2DE_Rect_T>& pos) const {
        return (
            pos.x >= x &&
            pos.x <= x + w &&
            pos.y >= y &&
            pos.y <= y + h
        );
    }
    /**
     * @brief Determines the face (side) that was collided with, if any.
     * 
     * @return Optional face of collision (TOP, RIGHT, BOTTOM, LEFT).
     */
    const std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const;
    /**
     * @brief Aligns this rectangle to another rectangle based on the specified collision face.
     * 
     * This function adjusts the position of this rectangle so that it "snaps" to the given rectangle along
     * the specified face (TOP, RIGHT, BOTTOM, or LEFT). It is typically used after a collision is detected
     * to reposition the rectangle and resolve overlap.
     * 
     * @param rect The rectangle to align to.
     * @param face The face of collision indicating the side to snap against.
     */
    void snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face);
};

/// @brief A rectangle using integer coordinates, typically used for absolute positions on screen.
using H2DE_AbsRect = H2DE_Rect<int>;
/// @brief A rectangle using floating-point coordinates, typically used for subpixel-level positioning and movement.
using H2DE_LevelRect = H2DE_Rect<float>;

/**
 * @brief Represents an RGB color with an alpha channel.
 *
 * The H2DE_ColorRGB structure stores red, green, blue, and alpha components as 8-bit unsigned integers.
 * It includes operations to manipulate hue, saturation, and value by converting to and from HSV color space.
 */
struct H2DE_ColorRGB {
    Uint8 r;                    //< Red component (0–255).
    Uint8 g;                    //< Green component (0–255).
    Uint8 b;                    //< Blue component (0–255).
    Uint8 a = SDL_MAX_UINT8;    //< Alpha (opacity) component (0–255). Defaults to 255 (fully opaque).

    /**
     * @brief Converts this RGB color to HSV color space.
     */
    explicit operator H2DE_ColorHSV() const;
    /**
     * @brief Converts this color to a 32-bit integer (typically 0xRRGGBBAA).
     */
    explicit operator Uint32() const;

    /**
     * @brief Checks if two colors are exactly equal.
     */
    inline bool operator==(const H2DE_ColorRGB& other) const { return (r == other.r&& g == other.g && b == other.b && a == other.a); }
    /**
     * @brief Checks if two colors are not equal.
     */
    inline bool operator!=(const H2DE_ColorRGB& other) const { return !(*this == other); }

    /**
     * @brief Outputs the color components to a stream in readable format.
     * @param os The output stream.
     * @param color The color to print.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& color) {
        os << std::string("r: ") << static_cast<int>(color.r) << ", g: " << static_cast<int>(color.g) << ", b: " << static_cast<int>(color.b) << ", a: " << static_cast<int>(color.a);
        return os;
    }

    /**
     * @brief Adds hue to the color (HSV-based).
     * @param hue The hue value to add.
     * @return New H2DE_ColorRGB with modified hue.
     */
    H2DE_ColorRGB addHue(float hue) const;
    /**
     * @brief Adds saturation to the color (HSV-based).
     * @param saturation The saturation value to add.
     * @return New H2DE_ColorRGB with modified saturation.
     */
    H2DE_ColorRGB addSaturation(float saturation) const;
    /**
     * @brief Adds value (brightness) to the color (HSV-based).
     * @param value The value to add.
     * @return New H2DE_ColorRGB with modified value.
     */
    H2DE_ColorRGB addValue(float value) const;

    /**
     * @brief Subtracts hue from the color (HSV-based).
     * @param hue The hue value to subtract.
     * @return New H2DE_ColorRGB with modified hue.
     */
    H2DE_ColorRGB subtractHue(float hue) const;
    /**
     * @brief Subtracts saturation from the color (HSV-based).
     * @param saturation The saturation value to subtract.
     * @return New H2DE_ColorRGB with modified saturation.
     */
    H2DE_ColorRGB subtractSaturation(float saturation) const;
    /**
     * @brief Subtracts value (brightness) from the color (HSV-based).
     * @param value The value to subtract.
     * @return New H2DE_ColorRGB with modified value.
     */
    H2DE_ColorRGB subtractValue(float value) const;

    /**
     * @brief Multiplies the hue of the color (HSV-based).
     * @param multiplier The hue multiplier.
     * @return New H2DE_ColorRGB with modified hue.
     */
    H2DE_ColorRGB multiplyHue(float multiplier) const;
    /**
     * @brief Multiplies the saturation of the color (HSV-based).
     * @param multiplier The saturation multiplier.
     * @return New H2DE_ColorRGB with modified saturation.
     */
    H2DE_ColorRGB multiplySaturation(float multiplier) const;
    /**
     * @brief Multiplies the value (brightness) of the color (HSV-based).
     * @param multiplier The value multiplier.
     * @return New H2DE_ColorRGB with modified value.
     */
    H2DE_ColorRGB multiplyValue(float multiplier) const;

    /**
     * @brief Divides the hue of the color (HSV-based).
     * @param divider The hue divider.
     * @return New H2DE_ColorRGB with modified hue.
     */
    H2DE_ColorRGB divideHue(float divider) const;
    /**
     * @brief Divides the saturation of the color (HSV-based).
     * @param divider The saturation divider.
     * @return New H2DE_ColorRGB with modified saturation.
     */
    H2DE_ColorRGB divideSaturation(float divider) const;
    /**
     * @brief Divides the value (brightness) of the color (HSV-based).
     * @param divider The value divider.
     * @return New H2DE_ColorRGB with modified value.
     */
    H2DE_ColorRGB divideValue(float divider) const;

    /**
     * @brief Checks if the color is visible (alpha > 0).
     * @return True if alpha is greater than 0, otherwise false.
     */
    inline bool isVisible() const { return a != 0; }
};

/**
 * @brief Represents a color in the HSV color space with an alpha channel.
 *
 * The H2DE_ColorHSV structure stores hue, saturation, value (brightness), and alpha components.
 * It includes operations to manipulate the color in HSV space and convert it to RGB space.
 */
struct H2DE_ColorHSV {
    float h;            //< Hue component (0.0 to 360.0).
    float s;            //< Saturation component (0.0 to 1.0).
    float v;            //< Value (brightness) component (0.0 to 1.0).
    float a = 1.0f;     //< Alpha (opacity) component (0.0 to 1.0). Defaults to 1.0 (fully opaque).

    /**
     * @brief Converts this HSV color to RGB color space.
     */
    explicit operator H2DE_ColorRGB() const;

    /**
     * @brief Checks if two colors are exactly equal.
     */
    inline bool operator==(const H2DE_ColorHSV& other) const { return (h == other.h && s == other.s && v == other.v, a == other.a); }
    /**
     * @brief Checks if two colors are not equal.
     */
    inline bool operator!=(const H2DE_ColorHSV& other) const { return !(*this == other); }

    /**
     * @brief Outputs the color components to a stream in a readable format.
     * @param os The output stream.
     * @param color The color to print.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& color) {
        os << std::string("h: ") << color.h << ", s: " << color.s << ", v: " << color.v << ", a: " << color.a;
        return os;
    }

    /**
     * @brief Adds hue to the color (HSV-based).
     * @param hue The hue value to add.
     * @return New H2DE_ColorHSV with modified hue.
     */
    H2DE_ColorHSV addHue(float hue) const;
    /**
     * @brief Adds saturation to the color (HSV-based).
     * @param saturation The saturation value to add.
     * @return New H2DE_ColorHSV with modified saturation.
     */
    H2DE_ColorHSV addSaturation(float saturation) const;
    /**
     * @brief Adds value (brightness) to the color (HSV-based).
     * @param value The value to add.
     * @return New H2DE_ColorHSV with modified value.
     */
    H2DE_ColorHSV addValue(float value) const;

    /**
     * @brief Subtracts hue from the color (HSV-based).
     * @param hue The hue value to subtract.
     * @return New H2DE_ColorHSV with modified hue.
     */
    H2DE_ColorHSV subtractHue(float hue) const;
    /**
     * @brief Subtracts saturation from the color (HSV-based).
     * @param saturation The saturation value to subtract.
     * @return New H2DE_ColorHSV with modified saturation.
     */
    H2DE_ColorHSV subtractSaturation(float saturation) const;
    /**
     * @brief Subtracts value (brightness) from the color (HSV-based).
     * @param value The value to subtract.
     * @return New H2DE_ColorHSV with modified value.
     */
    H2DE_ColorHSV subtractValue(float value) const;

    /**
     * @brief Multiplies the hue of the color (HSV-based).
     * @param multiplier The hue multiplier.
     * @return New H2DE_ColorHSV with modified hue.
     */
    H2DE_ColorHSV multiplyHue(float multiplier) const;
    /**
     * @brief Multiplies the saturation of the color (HSV-based).
     * @param multiplier The saturation multiplier.
     * @return New H2DE_ColorHSV with modified saturation.
     */
    H2DE_ColorHSV multiplySaturation(float multiplier) const;
    /**
     * @brief Multiplies the value (brightness) of the color (HSV-based).
     * @param multiplier The value multiplier.
     * @return New H2DE_ColorHSV with modified value.
     */
    H2DE_ColorHSV multiplyValue(float multiplier) const;

    /**
     * @brief Divides the hue of the color (HSV-based).
     * @param divider The hue divider.
     * @return New H2DE_ColorHSV with modified hue.
     */
    H2DE_ColorHSV divideHue(float divider) const;
    /**
     * @brief Divides the saturation of the color (HSV-based).
     * @param divider The saturation divider.
     * @return New H2DE_ColorHSV with modified saturation.
     */
    H2DE_ColorHSV divideSaturation(float divider) const;
    /**
     * @brief Divides the value (brightness) of the color (HSV-based).
     * @param divider The value divider.
     * @return New H2DE_ColorHSV with modified value.
     */
    H2DE_ColorHSV divideValue(float divider) const;

    /**
     * @brief Checks if the color is visible (alpha > 0).
     * @return True if alpha is greater than 0, otherwise false.
     */
    inline bool isVisible() const { return a != 0; }
};

/**
 * @brief Represents padding values for a level, specifying top, right, bottom, and left padding.
 *
 * The H2DE_LevelPadding structure stores the padding values for each side of a level or object,
 * which can be used to move the camera.
 */
struct H2DE_LevelPadding {
    float top = 0.0f;       //< Top padding value. Default is 0.0f;
    float right = 0.0f;     //< Right padding value. Default is 0.0f;
    float bottom = 0.0f;    //< Bottom padding value. Default is 0.0f;
    float left = 0.0f;      //< Left padding value. Default is 0.0f;
};

/**
 * @brief Represents window configuration data for the H2DE engine.
 *
 * The `H2DE_WindowData` structure defines the configuration options for the window, 
 * including the frame rate, title, position, size, fullscreen mode, resizable state, 
 * and the ability to save and restore the window state (position and size) across sessions.
 */
struct H2DE_WindowData {
    unsigned int fps = 60;                                                  //< The desired frame rate for the window. Default is 60.
    const char* title = "H2DE window";                                      //< The title of the window. The title will be displayed in the window's title bar.
    H2DE_AbsPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };   //< The position of the window on the screen. Default is centered on the screen
    H2DE_AbsSize size = { 1280, 720 };                                      //< The size of the window.
    bool fullscreen = false;                                                //< Whether the window should be fullscreen.
    bool resizable = false;                                                 //< Whether the window should be resizable.
    bool saveState = false;                                                 //< If true, the engine will save the window's position and size in `settings.ini` upon closing.
    H2DE_WindowRatio ratio = H2DE_WINDOW_RATIO_NO_RATIO;                    //< The aspect ratio setting for the window.
};

struct H2DE_CameraData {
    H2DE_LevelPos defaultPos = { 0.0f, 0.0f };                  //< The default position of the camera. The camera starts at this position by default.
    float width = 20.0f;                                        //< The width of the camera's view in number of blocks.
    float smoothing = 0.0f;                                     //< The smoothing factor for camera movement. Ranges from 0 (no smoothing) to 1 (maximum smoothing).
    H2DE_LevelPadding padding = { 0.0f, 0.0f, 0.0f, 0.0f };     //< The padding around the camera's view, influencing how the camera follows an object based on the padding.
};

/**
 * @brief Holds the configuration data for the engine, including window and camera settings.
 */
struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();   //< The data related to the window configuration (e.g., size, title, fullscreen settings).
    H2DE_CameraData camera = H2DE_CameraData();   //< The data related to the camera configuration (e.g., position, smoothing, padding).
};

/**
 * @brief Represents a hitbox used for collision detection and interactions.
 * 
 * Note: Hitboxes are not visible by default. They can only be seen when the debug mode is enabled 
 * by calling the function `H2DE_DebugObjects(engine, true)`.
 */
struct H2DE_Hitbox {
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };                   //< The rectangular area representing the hitbox.
    H2DE_ColorRGB color = { 255, 255, 255, 255 };                       //< The color of the hitbox, useful for debugging or visualization. Only visible when debug mode is enabled.
    int collisionIndex = 0;                                             //< A unique index used for collision filtering. Only hitboxes with the same index can collide with each other.
    bool snap = false;                                                  //< If true, the object with this hitbox will "snap" to the hitbox it collided with.
    std::function<void(H2DE_Object*, H2DE_Face)> onCollide = nullptr;   //< A callback function that is called when a collision occurs with this hitbox. Takes the object and the face of collision.
};

/**
 * @brief Represents a font used for rendering text from a texture.
 * 
 * Note: The texture contains all the letters arranged horizontally, and the font size is uniform for each character.
 * It is recommended that the letters in the texture be white to allow for better color application when rendering the text.
 */
struct H2DE_Font {
    std::string textureName = "";                       //< The name of the texture that contains all the letters written horizontally.
    H2DE_AbsSize charSize = { 0, 0 };                   //< The size of each character in the texture (in pixels). All characters have the same size.
    int spacing = 0;                                    //< The number of pixels between each character in the texture.
    std::string charOrder = "";                         //< The order of characters in the texture. For example: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ..."
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;  //< The scaling mode to use when rendering the font. Default is linear scaling.
};

/**
 * @brief Represents the data associated with an object in the engine.
 * 
 * This data is used for rendering and collision detection, including the object's position,
 * hitboxes, rotation, and more.
 */
struct H2DE_ObjectData {
    H2DE_LevelRect rect = { 0.0f, 0.0f };                           //< The rectangle representing the object's position and size.
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};     //< A map of hitboxes associated with the object, where the key is the name of the hitbox.
    H2DE_LevelPos pivot = { 0.0f, 0.0f };                           //< The pivot point of the object, used for rotation and scaling.
    float rotation = 0.0f;                                          //< The rotation of the object in degrees.
    H2DE_Flip flip = H2DE_FLIP_NONE;                                //< The flip state of the object.
    bool absolute = false;                                          //< If true, the object does not depend on the camera (its position is absolute on the screen).
    int index = 0;                                                  //< The index to control rendering order (used to render the object below or above others).
};

/**
 * @brief Represents the data for a surface in the engine.
 * 
 * A surface can be either a texture or a sprite, and contains information about
 * its position, appearance, transformation, and rendering options.
 */
struct H2DE_SurfaceData {
    std::string textureName = "";                           //< The name of the texture to use for the surface. Could refer to a sprite sheet or a single texture.
    H2DE_LevelRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };       //< The rectangle representing the position and size of the surface.
    H2DE_ColorRGB color = { 255, 255, 255, 255 };           //< The color tint applied to the surface, useful for color effects or shading.
    H2DE_LevelPos pivot = { 0.0f, 0.0f };                   //< The pivot point of the surface, used for positioning and rotation.
    float rotation = 0.0f;                                  //< The rotation of the surface in degrees.
    H2DE_Flip flip = H2DE_FLIP_NONE;                        //< The flip state of the surface.
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;      //< The scaling mode for the surface.
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;       //< The blend mode to use for rendering the surface.
    int index = 0;                                          //< The index used to control the rendering order of the surface, determining whether it's rendered above or below other surfaces.
};

/**
 * @brief Represents the data specific to a texture in the engine.
 */
struct H2DE_TextureData {
    std::optional<H2DE_AbsRect> srcRect = std::nullopt;     //< Optional source rectangle for the texture, defining a specific region of the texture to be used. If not set, the entire texture is used.
};

/**
 * @brief Represents the data specific to a sprite in the engine.
 * 
 * A sprite is a sequence of textures or frames that are displayed in sequence to create animation. 
 * This structure contains the information necessary to manage the sprite's positioning, size, 
 * the number of frames, and the frame delay.
 */
struct H2DE_SpriteData {
    H2DE_AbsPos startingPos = { 0, 0 };     //< The starting position of the sprite on the texture.
    H2DE_AbsSize size = { 1, 1 };           //< The size of each frame of the sprite in pixels.
    int spacing = 0;                        //< The spacing between frames on the sprite sheet.
    unsigned int nbFrame = 0;               //< The number of frames in the sprite's animation.
    unsigned int delay = 200;               //< The delay in milliseconds between each frame of the sprite.
    bool pauseSensitive = true;             //< Whether the sprite's animation is sensitive to pause events.
};

/**
 * @brief Represents the data for a bar object in the engine.
 * 
 * A bar object displays a bar with functionality to modify its value. 
 * It includes separate surfaces for the front (which represents the filled portion) 
 * and background of the bar, allowing customization of its appearance.
 */
struct H2DE_BarObjectData {
    std::unordered_map<std::string, H2DE_Surface*> front = {};          //< A map of surfaces representing the front (filled portion) of the bar, where the key is the name of the surface.
    std::unordered_map<std::string, H2DE_Surface*> background = {};     //< A map of surfaces representing the background of the bar, where the key is the name of the surface.
    float min = 0.0f;                                                   //< The minimum value of the bar.
    float max = 100.0f;                                                 //< The maximum value of the bar.
    float defaultValue = 0.0f;                                          //< The default value of the bar when it is initialized.
};

/**
 * @brief Represents the data for a basic object in the engine.
 * 
 * This type of object consists solely of surfaces (textures or sprites), 
 * where each surface is identified by a name. It doesn't have any additional 
 * complex behavior or properties.
 */
struct H2DE_BasicObjectData {
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};  //< A map of surfaces (textures or sprites) associated with the object, where the key is the name of the surface.
};

/**
 * @brief Represents the data for a button object in the engine.
 * 
 * A button object is interactive, with functionality for handling mouse events such as 
 * mouse down, mouse up, hover, and blur. It also includes surfaces for rendering the button's appearance.
 */
struct H2DE_ButtonObjectData {
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};   //< A map of surfaces representing the button's appearance, where the key is the name of the surface.
    std::function<void(H2DE_Object*)> onMouseDown = nullptr;        //< A callback function that is triggered when the mouse button is pressed down on the button.
    std::function<void(H2DE_Object*)> onMouseUp = nullptr;          //< A callback function that is triggered when the mouse button is released on the button.
    std::function<void(H2DE_Object*)> onHover = nullptr;            //< A callback function that is triggered when the mouse cursor hovers over the button.
    std::function<void(H2DE_Object*)> onBlur = nullptr;             //< A callback function that is triggered when the mouse cursor leaves the button area.
    bool pauseSensitive = true;                                     //< If true, the button's behavior will be paused when the game is paused.
};

/**
 * @brief Represents the data for a text object in the engine.
 * 
 * A text object is used to display text on the screen with customizable font, size, alignment, and color.
 * It also allows for controlling the spacing between characters and lines.
 */
struct H2DE_TextObjectData {
    std::string text = "";                              //< The text to be displayed by the object.
    std::string font = "";                              //< The name of the font to be used (initialized with H2DE_InitFont).
    H2DE_LevelSize fontSize = { 1.0f, 1.0f };           //< The size of the font (in pixels), applied to the width and height of each character.
    H2DE_LevelSize spacing = { 0.1f, 0.5f };            //< The spacing between characters (x-spacing) and between lines (y-spacing).
    H2DE_TextAlign textAlign = H2DE_TEXT_ALIGN_LEFT;    //< The alignment of the text within the object (left, center, or right).
    H2DE_ColorRGB color = { 255, 255, 255, 255 };       //< The color of the text.
};

struct H2DE_SurfaceBuffer {
    H2DE_Surface* surface = nullptr;
    H2DE_LevelPos offset = { 0.0f, 0.0f };
    H2DE_LevelSize size = { 0.0f, 0.0f };
};

/**
 * @brief Performs a linear interpolation between two values based on a blend factor.
 * 
 * @param min The starting value.
 * @param max The ending value.
 * @param blend A factor between 0 and 1 that defines the interpolation point.
 * @param easing The easing function to apply during interpolation.
 * @return The interpolated value.
 */
float H2DE_Lerp(float min, float max, float blend, H2DE_Easing easing);

/**
 * @brief Generates a random floating-point number within a specified range.
 * 
 * @param min The minimum value for the random range.
 * @param max The maximum value for the random range.
 * @return A random floating-point number between `min` and `max`.
 */
float H2DE_RandomFloatInRange(float min, float max);
/**
 * @brief Generates a random integer within a specified range.
 * 
 * @param min The minimum value for the random range.
 * @param max The maximum value for the random range.
 * @return A random integer between `min` and `max`.
 */
int H2DE_RandomIntegerInRange(int min, int max);
/**
 * @brief Generates a random boolean value.
 * 
 * @return A random boolean value, either true or false.
 */
bool H2DE_RandomBool();

/**
 * @brief Combines two flip states into one.
 * 
 * This function performs a bitwise OR on the two flip states to combine them.
 * It can be used to combine multiple flip states, such as flipping horizontally and vertically.
 * 
 * @param flip1 The first flip state.
 * @param flip2 The second flip state.
 * @return A combined flip state.
 */
H2DE_Flip H2DE_AddFlip(H2DE_Flip flip1, H2DE_Flip flip2);

#endif

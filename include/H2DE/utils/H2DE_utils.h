#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#undef min
#undef max

#include <functional>
#include <iostream>
#include <optional>
#include <random>
#include <SDL2/SDL_video.h>

class H2DE_Object;
class H2DE_ButtonObject;
class H2DE_Surface;
class H2DE_Timeline;

template<typename H2DE_Rect_T>
struct H2DE_Rect;
struct H2DE_ColorHSV;

/** @brief Minimum value for unsigned 8-bit integer. */
#define H2DE_UINT8_MIN 0u
/** @brief Maximum value for unsigned 8-bit integer. */
#define H2DE_UINT8_MAX 255u

/** @brief Minimum value for opacity (fully transparent). */
#define H2DE_OPACITY_MIN H2DE_UINT8_MIN
/** @brief Maximum value for opacity (fully opaque). */
#define H2DE_OPACITY_MAX H2DE_UINT8_MAX

/** @brief Minimum value for 32-bit signed index. */
#define H2DE_INDEX_MIN (-2147483648)
/** @brief Maximum value for 32-bit signed index. */
#define H2DE_INDEX_MAX 2147483647

/** @brief Special value representing an invalid delay ID. */
#define H2DE_INVALID_DELAY_ID 4294967295u
/** @brief Special value representing an invalid timeline ID. */
#define H2DE_INVALID_TIMELINE_ID 4294967295u

/** @brief Special value representing an invalid channel ID. */
#define H2DE_INVALID_CHANNEL_ID 255

/** @brief Minimum volume value. */
#define H2DE_MIN_VOLUME 0
/** @brief Maximum volume value. */
#define H2DE_MAX_VOLUME 100

/** @brief Constant used to represent an infinite loop in timelines or animations. */
#define H2DE_INFINITE_LOOP 4294967295u

/** @brief Type alias for delay identifiers. */
using H2DE_ChannelID = int8_t;

/**
 * @class H2DE_Delay
 * @brief Represents a one-shot time delay or timeout handler.
 * 
 * The H2DE_Delay class provides a simple mechanism for executing a callback 
 * after a specified duration. It is useful for scheduling one-time events, 
 * timeouts, or delayed actions without interpolation or animation.
 * 
 * Features include:
 * 
 * - Configurable delay duration.
 * - Pause, resume, reset, and stop control.
 * - Callback execution upon completion.
 * 
 * Delays are updated by the H2DE_TimelineManager and are automatically 
 * removed once completed or manually stopped.
 */
using H2DE_Delay = H2DE_Timeline;

/**
 * @enum H2DE_WindowRatio
 * @brief Enum representing common window aspect ratios for the engine.
 * 
 * Used to specify how the game window scales or adapts to different screen shapes.
 */
enum H2DE_WindowRatio {
    H2DE_WINDOW_RATIO_NO_RATIO,     /**< No fixed ratio, free scaling */
    H2DE_WINDOW_RATIO_CUSTOM,       /**< Custom ratio defined by user */
    H2DE_WINDOW_RATIO_4_3,          /**< Classic 4:3 aspect ratio */
    H2DE_WINDOW_RATIO_3_2,          /**< 3:2 aspect ratio */
    H2DE_WINDOW_RATIO_5_4,          /**< 5:4 aspect ratio */
    H2DE_WINDOW_RATIO_16_10,        /**< 16:10 aspect ratio */
    H2DE_WINDOW_RATIO_16_9,         /**< Widescreen 16:9 aspect ratio */
    H2DE_WINDOW_RATIO_21_9,         /**< Ultra-wide 21:9 aspect ratio */
    H2DE_WINDOW_RATIO_32_9,         /**< Super ultra-wide 32:9 aspect ratio */
};

/**
 * @enum H2DE_Easing
 * @brief Types of easing functions for animations and transitions.
 * 
 * These easing modes define how values interpolate over time,
 * controlling the acceleration and deceleration of animations.
 */
enum H2DE_Easing {
    H2DE_EASING_LINEAR,             /**< Constant speed interpolation */
    H2DE_EASING_EASE_IN,            /**< Accelerating from zero velocity */
    H2DE_EASING_EASE_OUT,           /**< Decelerating to zero velocity */
    H2DE_EASING_EASE_IN_OUT,        /**< Acceleration until halfway, then deceleration */
    H2DE_EASING_BACK_IN,            /**< Overshoots slightly before accelerating */
    H2DE_EASING_BACK_OUT,           /**< Overshoots slightly before decelerating */
    H2DE_EASING_BACK_IN_OUT,        /**< Overshoot at both start and end */
    H2DE_EASING_ELASTIC_IN,         /**< Elastic bounce effect at start */
    H2DE_EASING_ELASTIC_OUT,        /**< Elastic bounce effect at end */
    H2DE_EASING_ELASTIC_IN_OUT,     /**< Elastic bounce at start and end */
    H2DE_EASING_BOUNCE_IN,          /**< Bounce effect at start */
    H2DE_EASING_BOUNCE_OUT,         /**< Bounce effect at end */
    H2DE_EASING_BOUNCE_IN_OUT,      /**< Bounce effect at start and end */
    H2DE_EASING_SINE_IN,            /**< Sinusoidal acceleration at start */
    H2DE_EASING_SINE_OUT,           /**< Sinusoidal deceleration at end */
    H2DE_EASING_SINE_IN_OUT,        /**< Sinusoidal acceleration and deceleration */
    H2DE_EASING_EXPO_IN,            /**< Exponential acceleration at start */
    H2DE_EASING_EXPO_OUT,           /**< Exponential deceleration at end */
    H2DE_EASING_EXPO_IN_OUT,        /**< Exponential acceleration and deceleration */
};

/**
 * @enum H2DE_Face
 * @brief Represents directional faces or orientations using 2-bit flags.
 * 
 * Useful for indicating the facing direction of an object, sprite, or tile.
 */
enum H2DE_Face {
    H2DE_FACE_TOP       = 0b00,     /**< Facing upwards (top) */
    H2DE_FACE_RIGHT     = 0b01,     /**< Facing right */
    H2DE_FACE_BOTTOM    = 0b10,     /**< Facing downwards (bottom) */
    H2DE_FACE_LEFT      = 0b11,     /**< Facing left */
};

/**
 * @enum H2DE_TextAlign
 * @brief Defines text alignment options combining vertical and horizontal positions.
 * 
 * Bits encode vertical and horizontal alignment to position text within a rectangle:
 * - Vertical: Top (0b00001000), Center (0b00010000), Bottom (0b00100000)
 * - Horizontal: Left (0b00000001), Center (0b00000010), Right (0b00000100)
 * 
 * Allows specifying precise alignment like top-left, center-center, bottom-right, etc.
 */
enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_TOP_LEFT        = 0b00001001,   /**< Align top vertically and left horizontally */
    H2DE_TEXT_ALIGN_TOP_CENTER      = 0b00001010,   /**< Align top vertically and center horizontally */
    H2DE_TEXT_ALIGN_TOP_RIGHT       = 0b00001100,   /**< Align top vertically and right horizontally */
    H2DE_TEXT_ALIGN_CENTER_LEFT     = 0b00010001,   /**< Align center vertically and left horizontally */
    H2DE_TEXT_ALIGN_CENTER_CENTER   = 0b00010010,   /**< Align center vertically and center horizontally */
    H2DE_TEXT_ALIGN_CENTER_RIGHT    = 0b00010100,   /**< Align center vertically and right horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_LEFT     = 0b00100001,   /**< Align bottom vertically and left horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_CENTER   = 0b00100010,   /**< Align bottom vertically and center horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_RIGHT    = 0b00100100,   /**< Align bottom vertically and right horizontally */
};

/**
 * @enum H2DE_BorderType
 * @brief Represents the type of border shape.
 * 
 * - H2DE_BORDER_TYPE_RECTANGLE: A standard rectangular border.
 * - H2DE_BORDER_TYPE_CIRCLE: A circular border.
 */
enum H2DE_BorderType {
    H2DE_BORDER_TYPE_RECTANGLE, /**< A standard rectangular border */
    H2DE_BORDER_TYPE_CIRCLE     /**< A circular border */
};

/**
 * @enum H2DE_BorderPlacement
 * @brief Represents the placement of the border relative to the shape's outline.
 * 
 * - H2DE_BORDER_PLACEMENT_CENTER: Border is centered on the shape's edge (half inside, half outside).
 * - H2DE_BORDER_PLACEMENT_INNER: Border is fully inside the shape's boundary.
 * - H2DE_BORDER_PLACEMENT_OUTER: Border is fully outside the shape's boundary.
 */
enum H2DE_BorderPlacement {
    H2DE_BORDER_PLACEMENT_CENTER,   /**< Border centered on the edge */
    H2DE_BORDER_PLACEMENT_INNER,    /**< Border fully inside the shape */
    H2DE_BORDER_PLACEMENT_OUTER,    /**< Border fully outside the shape */
};

/**
 * @enum H2DE_MouseButton
 * @brief Represents mouse buttons that can be used to trigger interactions.
 * 
 * This enum allows you to specify which mouse button(s) should be detected
 * for an interaction.
 * You can combine multiple values using bitwise OR to allow several buttons.
 * 
 * - H2DE_MOUSE_BUTTON_LEFT: Left mouse button.
 * - H2DE_MOUSE_BUTTON_RIGHT: Right mouse button.
 * - H2DE_MOUSE_BUTTON_MIDDLE: Middle mouse button (usually the scroll wheel).
 * - H2DE_MOUSE_BUTTON_LEFT_AND_RIGHT: Both left and right buttons.
 * - H2DE_MOUSE_BUTTON_ALL: All mouse buttons (left, right, and middle).
 */
enum H2DE_MouseButton {
    H2DE_MOUSE_BUTTON_LEFT              = 0b0001,   /**< Left mouse button */
    H2DE_MOUSE_BUTTON_RIGHT             = 0b0010,   /**< Right mouse button */
    H2DE_MOUSE_BUTTON_MIDDLE            = 0b0100,   /**< Middle mouse button (scroll wheel) */
    H2DE_MOUSE_BUTTON_LEFT_AND_RIGHT    = 0b0011,   /**< Both left and right buttons */
    H2DE_MOUSE_BUTTON_ALL               = 0b0111,   /**< All mouse buttons */
};

/**
 * @enum H2DE_ScaleMode
 * @brief Defines how textures or sprites are scaled.
 * 
 * - NEAREST: Pixelated scaling, preserves hard edges (nearest neighbor).
 * - LINEAR: Smooth scaling with linear interpolation.
 * - BEST: Chooses the best available filtering method (may vary by platform).
 */
enum H2DE_ScaleMode {   
    H2DE_SCALE_MODE_NEAREST,    /**< Fast pixelated scaling */
    H2DE_SCALE_MODE_LINEAR,     /**< Smooth linear interpolation */
    H2DE_SCALE_MODE_BEST,       /**< Best quality scaling available */
};

/**
 * @enum H2DE_BlendMode
 * @brief Defines blending modes for rendering surfaces.
 * 
 * - BLEND: Standard alpha blending.
 * - ADD: Additive blending (lightens colors).
 * - MOD: Modulate (multiplicative) blending.
 * - MUL: Multiply blending.
 * - INVALID: Invalid or unsupported blend mode.
 * - NONE: No blending, opaque rendering.
 */
enum H2DE_BlendMode {
    H2DE_BLEND_MODE_BLEND,      /**< Alpha blend */
    H2DE_BLEND_MODE_ADD,        /**< Additive blend */
    H2DE_BLEND_MODE_MOD,        /**< Modulate blend */
    H2DE_BLEND_MODE_MUL,        /**< Multiply blend */
    H2DE_BLEND_MODE_INVALID,    /**< Invalid mode */
    H2DE_BLEND_MODE_NONE,       /**< No blending */
};

/**
 * @namespace H2DE
 * @brief Utility namespace with math and helper functions.
 * 
 * Contains constexpr generic functions for simple math (abs, pow, round, floor, ceil),
 * plus utilities for random value generation and easing-based interpolation.
 */
namespace H2DE {
    /**
     * @brief Compute absolute value (constexpr).
     * 
     * @tparam T Numeric type (int, float, double, etc.)
     * @param value Value to get the absolute of.
     * @return Absolute value of `value`.
     */
    template<typename T>
    constexpr T abs(T value) noexcept {
        return (value < 0 ? -value : value);
    }

    /**
     * @brief Compute integer power (constexpr).
     * 
     * @tparam T Numeric type.
     * @param value Base value.
     * @param power Exponent (non-negative integer).
     * @return value raised to the power.
     */
    template<typename T>
    constexpr T pow(T value, unsigned int power) noexcept {
        return (power == 0) ? static_cast<T>(1) : value * H2DE::pow(value, power - 1);
    }

    /**
     * @brief Round to nearest integer (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to round.
     * @return Rounded integer.
     */
    template<typename T>
    constexpr int round(T value) noexcept {
        return (value >= 0) ? static_cast<int>(value + 0.5f) : static_cast<int>(value - 0.5f);
    }
    /**
     * @brief Floor (round down) (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to floor.
     * @return Largest integer less or equal to value.
     */
    template<typename T>
    constexpr int floor(T value) noexcept {
        return static_cast<int>(value) - (value < static_cast<T>(static_cast<int>(value)) ? 1 : 0);
    }
    /**
     * @brief Ceil (round up) (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to ceil.
     * @return Smallest integer greater or equal to value.
     */
    template<typename T>
    constexpr int ceil(T value) noexcept {
        return static_cast<int>(value) + (value > static_cast<T>(static_cast<int>(value)) ? 1 : 0);
    }

    /**
     * @brief Returns the smallest of two values (constexpr).
     * 
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return The lesser of a and b.
     */
    template<typename T>
    constexpr T min(T a, T b) noexcept {
        return ((a < b) ? a : b);
    }
    /**
     * @brief Returns the largest of two values (constexpr).
     * 
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return The greater of a and b.
     */
    template<typename T>
    constexpr T max(T a, T b) noexcept {
        return ((a > b) ? a : b);
    }
    /**
     * @brief Clamps a value between a minimum and a maximum (constexpr).
     * 
     * @tparam T Comparable type.
     * @param value Value to clamp.
     * @param min Minimum bound.
     * @param max Maximum bound.
     * @return min if value < min, max if value > max, otherwise value.
     */
    template<typename T>
    constexpr T clamp(T value, T min, T max) noexcept {
        return H2DE::min(H2DE::max(value, min), max);
    }

    /**
     * @brief Prints a value to std::cout followed by a newline.
     * 
     * Generic template for any type that supports operator<<.
     * 
     * @tparam T Type of the value to print.
     * @param value The value to print.
     */
    template<typename T>
    inline void print(const T& value) {
        std::cout << value << std::endl;
    }
    /**
     * @brief Prints a boolean value as "true" or "false" to std::cout followed by a newline.
     * 
     * Specialized template for bool type.
     * 
     * @tparam T Must be bool.
     * @param value Boolean value to print.
     */
    template<typename T>
    requires (std::same_as<T, bool>)
    inline void print(const T& value) {
        std::cout << (value ? "true" : "false") << std::endl;
    }
    /**
     * @brief Prints an unsigned integral value casted to long long to std::cout followed by a newline.
     * 
     * This avoids printing unsigned char or unsigned short as characters.
     * 
     * @tparam T Unsigned integral type except bool.
     * @param value Unsigned value to print.
     */
    template<typename T>
    requires (std::is_unsigned_v<T> && !std::same_as<T, bool>)
    inline void print(const T& value) {
        std::cout << static_cast<long long>(value) << std::endl;
    }

    /**
     * @brief Generate random integer in range [min, max].
     * 
     * @param min Minimum inclusive.
     * @param max Maximum inclusive.
     * @return Random integer between min and max.
     */
    int randomIntegerInRange(int min, int max);
    /**
     * @brief Generate random float in range [min, max].
     * 
     * @param min Minimum inclusive.
     * @param max Maximum inclusive.
     * @return Random float between min and max.
     */
    float randomFloatInRange(float min, float max);
    /**
     * @brief Generate a random float in range [0.0f, 1.0f].
     * 
     * @return A random float between 0.0f and 1.0f.
     */
    inline float randomFloat() {
        return H2DE::randomFloatInRange(0.0f, 1.0f);
    }
    /**
     * @brief Generate a random boolean.
     * 
     * @return true or false randomly.
     */
    bool randomBool();
    /**
     * @brief Get a random valid index from a vector.
     * 
     * @tparam T Type of elements in the vector.
     * @param vector The vector to get a random index from.
     * @return A random index between 0 and vector.size() - 1.
     */
    template<typename T>
    inline int randomIndexFromVector(const std::vector<T>& vector) {
        return H2DE::randomIntegerInRange(0, vector.size() - 1);
    }
    /**
     * @brief Get a random value from a vector.
     * 
     * @tparam T Type of elements in the vector.
     * @param vector The vector to get a random value from.
     * @return A randomly chosen element from the vector.
     * @warning The vector must not be empty.
     */
    template<typename T>
    inline T randomValueFromVector(const std::vector<T>& vector) {
        return vector.at(H2DE::randomIndexFromVector(vector));
    }

    /**
     * @brief Linear interpolation with easing.
     * 
     * Calculates a blend between min and max based on blend (0.0 to 1.0),
     * applying an easing function.
     * 
     * @param min Start value.
     * @param max End value.
     * @param blend Interpolation factor from 0.0 (min) to 1.0 (max).
     * @param easing Easing function to apply.
     * @return Interpolated value.
     */
    float lerp(float min, float max, float blend, H2DE_Easing easing);
}

/**
 * @brief A generic 2D vector class template.
 * 
 * Provides basic 2D vector operations (addition, subtraction, scalar multiplication/division),
 * comparison operators, rotation around a pivot point, and utility functions.
 * 
 * @tparam H2DE_Vector2D_T The type of the vector components (e.g., int, float).
 */
template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x = 0.0f;   /** The x component of the vector. */
    H2DE_Vector2D_T y = 0.0f;   /** The y component of the vector. */

    /**
     * @brief Default constructor initializes vector to (0,0).
     */
    constexpr H2DE_Vector2D() noexcept = default;
    /**
     * @brief Construct a vector with given x and y values.
     * @param x The x component.
     * @param y The y component.
     */
    constexpr H2DE_Vector2D(H2DE_Vector2D_T x, H2DE_Vector2D_T y) noexcept : x(x), y(y) {}

    /**
     * @brief Vector addition.
     * @param other The other vector to add.
     * @return The result of vector addition.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator+(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x + other.x, y + other.y };
    }
    /**
     * @brief Vector subtraction.
     * @param other The other vector to subtract.
     * @return The result of vector subtraction.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator-(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x - other.x, y - other.y };
    }
    /**
     * @brief Scalar multiplication.
     * @param multiplier The scalar multiplier.
     * @return The scaled vector.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator*(float multiplier) const noexcept {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x * multiplier), static_cast<H2DE_Vector2D_T>(y * multiplier) };
    }
    /**
     * @brief Scalar division.
     * @param divider The scalar divisor.
     * @return The scaled vector.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator/(float divider) const noexcept {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x / divider), static_cast<H2DE_Vector2D_T>(y / divider) };
    }

    /**
     * @brief Compound addition assignment.
     * @param other The other vector to add.
     * @return Reference to this vector after addition.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator+=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) noexcept;
    /**
     * @brief Compound subtraction assignment.
     * @param other The other vector to subtract.
     * @return Reference to this vector after subtraction.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator-=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) noexcept;
    /**
     * @brief Compound scalar multiplication assignment.
     * @param multiplier The scalar multiplier.
     * @return Reference to this vector after scaling.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator*=(float multiplier) noexcept;
    /**
     * @brief Compound scalar division assignment.
     * @param divider The scalar divisor.
     * @return Reference to this vector after scaling.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator/=(float divider) noexcept;

    /**
     * @brief Equality operator.
     * @param other The other vector to compare.
     * @return True if both x and y components are equal.
     */
    constexpr bool operator==(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (x == other.x && y == other.y);
    }
    /**
     * @brief Inequality operator.
     * @param other The other vector to compare.
     * @return True if any component differs.
     */
    constexpr bool operator!=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this == other);
    }
    /**
     * @brief Greater-than operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is greater.
     */
    constexpr bool operator>(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (H2DE::abs<H2DE_Vector2D_T>(x) + H2DE::abs<H2DE_Vector2D_T>(y) > H2DE::abs<H2DE_Vector2D_T>(other.x) + H2DE::abs<H2DE_Vector2D_T>(other.y));
    }
    /**
     * @brief Greater-than-or-equal operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is greater or equal.
     */
    constexpr bool operator>=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (H2DE::abs<H2DE_Vector2D_T>(x) + H2DE::abs<H2DE_Vector2D_T>(y) >= H2DE::abs<H2DE_Vector2D_T>(other.x) + H2DE::abs<H2DE_Vector2D_T>(other.y));
    }
    /**
     * @brief Less-than operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is less.
     */
    constexpr bool operator<(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this >= other);
    }
    /**
     * @brief Less-than-or-equal operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is less or equal.
     */
    constexpr bool operator<=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this > other);
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D<H2DE_Vector2D_T>& vec) {
        os << "[x: " << vec.x << ", y: " << vec.y << "]";
        return os;
    }

    /**
     * @brief Convert vector to SDL_Point (casting components to int).
     * @return SDL_Point with x and y components.
     */
    inline operator SDL_Point() const noexcept {
        return { static_cast<int>(x), static_cast<int>(y) };
    }

    /**
     * @brief Returns a new vector with the X component increased by a given value.
     * @param value Value to add to the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> addX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x + value, y };
    }
    /**
     * @brief Returns a new vector with the Y component increased by a given value.
     * @param value Value to add to the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> addY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, y + value };
    }

    /**
     * @brief Returns a new vector with the X component decreased by a given value.
     * @param value Value to subtract from the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> subtractX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x - value, y };
    }
    /**
     * @brief Returns a new vector with the Y component decreased by a given value.
     * @param value Value to subtract from the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> subtractY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, y - value };
    }

    /**
     * @brief Returns a new vector with the X component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the X component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> multiplyX(float multiplier) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ static_cast<H2DE_Vector2D_T>(x * multiplier), y };
    }
    /**
     * @brief Returns a new vector with the Y component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the Y component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> multiplyY(float multiplier) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, static_cast<H2DE_Vector2D_T>(y * multiplier) };
    }

    /**
     * @brief Returns a new vector with the X component divided by a given divider.
     * @param divider Divider to divide the X component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> divideX(float divider) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ static_cast<H2DE_Vector2D_T>(x / divider), y };
    }
    /**
     * @brief Returns a new vector with the Y component divided by a given divider.
     * @param divider Divider to divide the Y component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> divideY(float divider) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, static_cast<H2DE_Vector2D_T>(y / divider) };
    }

    /**
     * @brief Returns a new vector with the X component set to a given value.
     * @param value New value for the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> setX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ value, y };
    }
    /**
     * @brief Returns a new vector with the Y component set to a given value.
     * @param value New value for the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> setY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, value };
    }

    /**
     * @brief Create a rectangle at this vector's position with given size.
     * @param scale The size vector (width and height).
     * @return H2DE_Rect representing the rectangle.
     */
    constexpr H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D<H2DE_Vector2D_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, scale.x, scale.y };
    }
    /**
     * @brief Create a rectangle at this vector's position with zero size.
     * @return H2DE_Rect with zero width and height.
     */
    constexpr H2DE_Rect<H2DE_Vector2D_T> makeNullRect() const noexcept {
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, static_cast<H2DE_Vector2D_T>(0.0f), static_cast<H2DE_Vector2D_T>(0.0f) };
    }

    /**
     * @brief Check if the vector is (0,0).
     * @return True if both components are zero.
     */
    constexpr bool isNull() const noexcept {
        return (x == 0 && y == 0);
    }
    /**
     * @brief Check if at least one component is zero.
     * @return True if either x or y is zero.
     */
    constexpr bool hasNullMember() const noexcept {
        return (x == 0 || y == 0);
    }
    /**
     * @brief Rotate this vector around a pivot point by a given angle.
     * @param pivot The pivot point to rotate around.
     * @param angle The rotation angle in radians.
     * @return The rotated vector.
     */
    H2DE_Vector2D<H2DE_Vector2D_T> rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& pivot, float angle) const;
    /**
     * @brief Get the vector representing half of this vector (center point).
     * 
     * @return A new vector scaled by 0.5 (half the x and y).
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> getCenter() const noexcept {
        return (*this * 0.5f);
    }
    /**
     * @brief Calculate the squared distance between this vector and another.
     * 
     * Avoids the cost of a sqrt, useful for distance comparisons.
     * 
     * @param other The other vector to compute the distance to.
     * @return The squared distance as the sum of squared component differences.
     */
    constexpr H2DE_Vector2D_T getDistanceSquared(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE::pow(other.x - x, 2) + H2DE::pow(other.y - y, 2);
    }
    /**
     * @brief Calculate the Euclidean distance between this vector and another.
     * 
     * Uses sqrt on the squared distance.
     * 
     * @param other The other vector to compute the distance to.
     * @return The Euclidean distance as a float.
     */
    inline H2DE_Vector2D_T getDistance(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
        return std::sqrt(getDistanceSquared(other));
    }
};

/** @brief Position in pixels, represented as integer 2D vector. */
using H2DE_PixelPos = H2DE_Vector2D<int>;
/** @brief Size in pixels, represented as integer 2D vector. */
using H2DE_PixelSize = H2DE_Vector2D<int>;
/** @brief Pivot point in pixels, represented as integer 2D vector. */
using H2DE_PixelPivot = H2DE_Vector2D<int>;
/** @brief Translation vector, usually float for smooth movement. */
using H2DE_Translate = H2DE_Vector2D<float>;
/** @brief Scale vector, usually float for smooth scaling. */
using H2DE_Scale = H2DE_Vector2D<float>;
/** @brief Pivot point with floating-point precision. */
using H2DE_Pivot = H2DE_Vector2D<float>;

/**
 * @brief Represents a 2D rectangle with position and size.
 * 
 * The rectangle is defined by its center position (x, y) and dimensions (w, h).
 * Template allows using int, float or any numeric type.
 * 
 * @tparam H2DE_Rect_T Numeric type of the rectangle's components.
 */
template<typename H2DE_Rect_T>
struct H2DE_Rect {  
    H2DE_Rect_T x = 0.0f;   /**< Center position on the X axis */
    H2DE_Rect_T y = 0.0f;   /**< Center position on the Y axis */
    H2DE_Rect_T w = 1.0f;   /**< Width of the rectangle */
    H2DE_Rect_T h = 1.0f;   /**< Height of the rectangle */

    /**
     * @brief Default constructor, initializes rect to (0,0,1,1).
     */
    constexpr H2DE_Rect() noexcept = default;
    /**
     * @brief Constructs a rectangle from a translation and a uniform scale.
     * 
     * @param translate Value used for both x and y position
     * @param scale Value used for both width and height
     */
    constexpr H2DE_Rect(H2DE_Rect_T translate, H2DE_Rect_T scale) noexcept : x(translate), y(translate), w(scale), h(scale) {}
    /**
     * @brief Constructs a rectangle with specified position and size.
     * 
     * @param x Center X position
     * @param y Center Y position
     * @param w Width
     * @param h Height
     */
    constexpr H2DE_Rect(H2DE_Rect_T x, H2DE_Rect_T y, H2DE_Rect_T w, H2DE_Rect_T h) noexcept : x(x), y(y), w(w), h(h) {}

    /**
     * @brief Adds two rectangles component-wise.
     * @return A new rectangle with added position and size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator+(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            x + other.x,
            y + other.y,
            w + other.w,
            h + other.h
        };
    }
    /**
     * @brief Subtracts two rectangles component-wise.
     * @return A new rectangle with subtracted position and size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator-(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            x - other.x,
            y - other.y,
            w - other.w,
            h - other.h
        };
    }
    /**
     * @brief Multiplies rectangle's position and size by a float multiplier.
     * @param multiplier The value to multiply by.
     * @return A new scaled rectangle.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator*(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x * multiplier),
            static_cast<H2DE_Rect_T>(y * multiplier),
            static_cast<H2DE_Rect_T>(w * multiplier),
            static_cast<H2DE_Rect_T>(h * multiplier)
        };
    }
    /**
     * @brief Divides rectangle's position and size by a float divider.
     * @param divider The value to divide by.
     * @return A new scaled rectangle.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator/(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x / divider),
            static_cast<H2DE_Rect_T>(y / divider),
            static_cast<H2DE_Rect_T>(w / divider),
            static_cast<H2DE_Rect_T>(h / divider)
        };
    }

    /**
     * @brief Adds the values of another rect to this rect (in-place).
     * @param other The rect to add.
     * @return Reference to this rect after addition.
     */
    H2DE_Rect<H2DE_Rect_T>& operator+=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept;
    /**
     * @brief Subtracts the values of another rect from this rect (in-place).
     * @param other The rect to subtract.
     * @return Reference to this rect after subtraction.
     */
    H2DE_Rect<H2DE_Rect_T>& operator-=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept;
    /**
     * @brief Multiplies all values of this rect by a scalar (in-place).
     * @param multiplier The scalar to multiply by.
     * @return Reference to this rect after multiplication.
     */
    H2DE_Rect<H2DE_Rect_T>& operator*=(float multiplier) noexcept;
    /**
     * @brief Divides all values of this rect by a scalar (in-place).
     * @param divider The scalar to divide by.
     * @return Reference to this rect after division.
     */
    H2DE_Rect<H2DE_Rect_T>& operator/=(float divider) noexcept;

    /**
     * @brief Checks equality between two rectangles.
     * @return true if position and size are equal.
     */
    constexpr bool operator==(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return (x == other.x && y == other.y && w == other.w && h == other.h);
    }
    /**
     * @brief Checks inequality between two rectangles.
     * @return true if position or size differ.
     */
    constexpr bool operator!=(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return !(*this == other);
    }

    /**
     * @brief Converts this rectangle to SDL_Rect for SDL rendering.
     * 
     * The rectangle is converted from center-position + size to SDL's top-left + size.
     */
    inline operator SDL_Rect() const noexcept {
        return SDL_Rect{
            static_cast<int>(H2DE::round(x - w * 0.5f)),
            static_cast<int>(H2DE::round(y - h * 0.5f)),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Rect<H2DE_Rect_T>& rect) {
        os << "[x: " << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h << "]";
        return os;
    }

    /**
     * @brief Returns a new rectangle with position translated by given vector.
     * @param translate Vector to add to position.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x + translate.x, y + translate.y, w, h };
    }
    /**
     * @brief Returns a new rectangle with size scaled by given vector.
     * @param scale Vector to add to size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w + scale.x, h + scale.y };
    }
    /**
     * @brief Returns a new rectangle with position translated negatively by given vector.
     * @param translate Vector to subtract from position.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x - translate.x, y - translate.y, w, h };
    }
    /**
     * @brief Returns a new rectangle with size scaled down by given vector.
     * @param scale Vector to subtract from size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w - scale.x, h - scale.y };
    }
    /**
     * @brief Returns a new rectangle with position multiplied by multiplier, size unchanged.
     * @param multiplier Value to multiply position by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyTranslate(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x * multiplier), static_cast<H2DE_Rect_T>(y * multiplier), w, h };
    }
    /**
     * @brief Returns a new rectangle with size multiplied by multiplier, position unchanged.
     * @param multiplier Value to multiply size by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyScale(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w * multiplier), static_cast<H2DE_Rect_T>(h * multiplier) };
    }
    /**
     * @brief Returns a new rectangle with position divided by divider, size unchanged.
     * @param divider Value to divide position by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideTranslate(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x / divider), static_cast<H2DE_Rect_T>(y / divider), w, h };
    }
    /**
     * @brief Returns a new rectangle with size divided by divider, position unchanged.
     * @param divider Value to divide size by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideScale(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w / divider), static_cast<H2DE_Rect_T>(h / divider) };
    }

    /**
     * @brief Returns a new rectangle with the X component increased by a given value.
     * @param value Value to add to the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x + value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component increased by a given value.
     * @param value Value to add to the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y + value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component increased by a given value.
     * @param value Value to add to the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w + value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component increased by a given value.
     * @param value Value to add to the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, h + value };
    }

    /**
     * @brief Returns a new rectangle with the X component decreased by a given value.
     * @param value Value to subtract from the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x - value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component decreased by a given value.
     * @param value Value to subtract from the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y - value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component decreased by a given value.
     * @param value Value to subtract from the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w - value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component decreased by a given value.
     * @param value Value to subtract from the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, h - value };
    }

    /**
     * @brief Returns a new rectangle with the X component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the X component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyX(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x * multiplier), y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the Y component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyY(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, static_cast<H2DE_Rect_T>(y * multiplier), w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the W component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyW(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w * multiplier), h };
    }
    /**
     * @brief Returns a new rectangle with the H component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the H component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyH(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, static_cast<H2DE_Rect_T>(h * multiplier) };
    }

    /**
     * @brief Returns a new rectangle with the X component divided by a given divider.
     * @param divider Divider to divide the X component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideX(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x / divider), y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component divided by a given divider.
     * @param divider Divider to divide the Y component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideY(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, static_cast<H2DE_Rect_T>(y / divider), w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component divided by a given divider.
     * @param divider Divider to divide the W component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideW(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w / divider), h };
    }
    /**
     * @brief Returns a new rectangle with the H component divided by a given divider.
     * @param divider Divider to divide the H component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideH(float divider) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, static_cast<H2DE_Rect_T>(h / divider) };
    }

    /**
     * @brief Returns a new rectangle with the X component set to a given value.
     * @param value New value for the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component set to a given value.
     * @param value New value for the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component set to a given value.
     * @param value New value for the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component set to a given value.
     * @param value New value for the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, value };
    }

    /**
     * @brief Returns position vector of the rectangle center.
     */
    constexpr H2DE_Vector2D<H2DE_Rect_T> getTranslate() const noexcept {
        return H2DE_Vector2D<H2DE_Rect_T>{ x, y };
    }
    /**
     * @brief Returns size vector of the rectangle.
     */
    constexpr H2DE_Vector2D<H2DE_Rect_T> getScale() const noexcept {
        return H2DE_Vector2D<H2DE_Rect_T>{ w, h };
    }

    /**
     * @brief Returns minimal X coordinate of the rectangle (left edge).
     */
    constexpr H2DE_Rect_T getMinX() const noexcept {
        return (x - static_cast<H2DE_Rect_T>(w * 0.5f));
    }
    /**
     * @brief Returns maximal X coordinate of the rectangle (right edge).
     */
    constexpr H2DE_Rect_T getMaxX() const noexcept {
        return (x + static_cast<H2DE_Rect_T>(w * 0.5f));
    }
    /**
     * @brief Returns minimal Y coordinate of the rectangle (top edge).
     */
    constexpr H2DE_Rect_T getMinY() const noexcept {
        return (y - static_cast<H2DE_Rect_T>(h * 0.5f));
    }
    /**
     * @brief Returns maximal Y coordinate of the rectangle (bottom edge).
     */
    constexpr H2DE_Rect_T getMaxY() const noexcept {
        return (y + static_cast<H2DE_Rect_T>(h * 0.5f));
    }

    /**
     * @brief Checks if this rectangle collides with another rectangle.
     * @param rect Rectangle to test collision against.
     * @return true if rectangles overlap.
     */
    constexpr bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const noexcept {
        return (
            (H2DE::abs(x - rect.x) < ((w + rect.w) * 0.5f)) &&
            (H2DE::abs(y - rect.y) < ((h + rect.h) * 0.5f))
        );
    }
    /**
     * @brief Checks if a point is inside this rectangle.
     * @param translate Point position vector.
     * @return true if point lies within rectangle bounds.
     */
    constexpr bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return (
            (translate.x >= x - w * 0.5f) &&
            (translate.x <= x + w * 0.5f) &&
            (translate.y >= y - h * 0.5f) &&
            (translate.y <= y + h * 0.5f)
        );
    }
    /**
     * @brief Checks collision with a circle (point + radius).
     * @param translate Circle center point.
     * @param radius Circle radius.
     * @return true if circle intersects rectangle.
     */
    bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate, float radius) const noexcept;
    /**
     * @brief Gets the face of this rectangle collided with another.
     * @param rect Rectangle to test against.
     * @return Optional face enum representing the side collided, or nullopt if none.
     */
    const std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const noexcept;
    /**
     * @brief Snaps this rectangle to another rectangle along the specified face.
     * @param rect Rectangle to snap to.
     * @param face Face of the rectangle to align.
     */
    void snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face) noexcept;

    /**
     * @brief Returns an array of the 4 corners of the rectangle.
     * @return std::array with corners ordered (top-left, top-right, bottom-right, bottom-left).
     */
    std::array<H2DE_Vector2D<H2DE_Rect_T>, 4> getCorners() const noexcept;
};

/** @brief Alias for rectangle with integer components (pixels). */
using H2DE_PixelRect = H2DE_Rect<int>;
/** @brief Alias for rectangle used in level coordinates (float). */
using H2DE_LevelRect = H2DE_Rect<float>;

/**
 * @struct H2DE_ColorRGB
 * @brief Represents a color in RGBA format with 8-bit channels.
 * 
 * Channels are stored as uint8_t (0-255). Alpha defaults to fully opaque (255).
 */
struct H2DE_ColorRGB {
    uint8_t r = H2DE_UINT8_MAX;     /**< Red channel (0-255) */
    uint8_t g = H2DE_UINT8_MAX;     /**< Green channel (0-255) */
    uint8_t b = H2DE_UINT8_MAX;     /**< Blue channel (0-255) */
    uint8_t a = H2DE_OPACITY_MAX;   /**< Alpha channel (0-255), default 255 (opaque) */

    /**
     * @brief Default constructor initializes to white opaque.
     */
    constexpr H2DE_ColorRGB() noexcept = default;
    /**
     * @brief Constructor with red, green and blue; alpha defaults to 255 (opaque).
     * @param red Red channel value.
     * @param green Green channel value.
     * @param blue Blue channel value.
     */
    constexpr H2DE_ColorRGB(uint8_t red, uint8_t green, uint8_t blue) noexcept : r(red), g(green), b(blue), a(H2DE_OPACITY_MAX) {}
    /**
     * @brief Constructor with red, green, blue and alpha channels.
     * @param red Red channel value.
     * @param green Green channel value.
     * @param blue Blue channel value.
     * @param alpha Alpha channel value.
     */
    constexpr H2DE_ColorRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept : r(red), g(green), b(blue), a(alpha) {}

    /**
     * @brief Converts this RGB color to HSV color space.
     * @return Equivalent color in HSV.
     */
    explicit operator H2DE_ColorHSV() const;
    /**
     * @brief Converts the color to a 32-bit integer in ARGB format.
     * @return 32-bit unsigned int representation of color.
     */
    constexpr explicit operator uint32_t() const noexcept {
        return ((a << 24) | (b << 16) | (g << 8) | r);
    }

    /**
     * @brief Equality operator.
     * @param other The other color to compare with.
     * @return True if all channels (RGBA) are equal.
     */
    constexpr bool operator==(const H2DE_ColorRGB& other) const noexcept {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }
    /**
     * @brief Inequality operator.
     * @param other The other color to compare with.
     * @return True if any channel differs.
     */
    constexpr bool operator!=(const H2DE_ColorRGB& other) const noexcept {
        return !(*this == other);
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& color) {
        os << "[r: " << static_cast<int>(color.r) << ", g: " << static_cast<int>(color.g) << ", b: " << static_cast<int>(color.b) << ", a: " << static_cast<int>(color.a) << "]";
        return os;
    }

    /**
     * @brief Adds a hue offset to the color.
     * @param hue The amount to add to the hue (degrees).
     * @return New color with adjusted hue.
     */
    H2DE_ColorRGB addHue(float hue) const;
    /**
     * @brief Adds saturation to the color.
     * @param saturation The amount to add to the saturation (0-1 range).
     * @return New color with increased saturation.
     */
    H2DE_ColorRGB addSaturation(float saturation) const;
    /**
     * @brief Adds brightness (value) to the color.
     * @param value The amount to add to the brightness (0-1 range).
     * @return New color with increased brightness.
     */
    H2DE_ColorRGB addValue(float value) const;

    /**
     * @brief Subtracts a hue offset from the color.
     * @param hue The amount to subtract from the hue (degrees).
     * @return New color with adjusted hue.
     */
    H2DE_ColorRGB subtractHue(float hue) const;
    /**
     * @brief Subtracts saturation from the color.
     * @param saturation The amount to subtract from the saturation (0-1 range).
     * @return New color with decreased saturation.
     */
    H2DE_ColorRGB subtractSaturation(float saturation) const;
    /**
     * @brief Subtracts brightness (value) from the color.
     * @param value The amount to subtract from the brightness (0-1 range).
     * @return New color with decreased brightness.
     */
    H2DE_ColorRGB subtractValue(float value) const;

    /**
     * @brief Multiplies the hue of the color by a factor.
     * @param multiplier The factor to multiply the hue by.
     * @return New color with scaled hue.
     */
    H2DE_ColorRGB multiplyHue(float multiplier) const;
    /**
     * @brief Multiplies the saturation of the color by a factor.
     * @param multiplier The factor to multiply the saturation by.
     * @return New color with scaled saturation.
     */
    H2DE_ColorRGB multiplySaturation(float multiplier) const;
    /**
     * @brief Multiplies the brightness (value) of the color by a factor.
     * @param multiplier The factor to multiply the brightness by.
     * @return New color with scaled brightness.
     */
    H2DE_ColorRGB multiplyValue(float multiplier) const;

    /**
     * @brief Divides the hue of the color by a divisor.
     * @param divider The divisor to divide the hue by.
     * @return New color with scaled hue.
     */
    H2DE_ColorRGB divideHue(float divider) const;
    /**
     * @brief Divides the saturation of the color by a divisor.
     * @param divider The divisor to divide the saturation by.
     * @return New color with scaled saturation.
     */
    H2DE_ColorRGB divideSaturation(float divider) const;
    /**
     * @brief Divides the brightness (value) of the color by a divisor.
     * @param divider The divisor to divide the brightness by.
     * @return New color with scaled brightness.
     */
    H2DE_ColorRGB divideValue(float divider) const;

    /**
     * @brief Checks if the color is visible (alpha not zero).
     * @return True if alpha is not zero.
     */
    constexpr bool isVisible() const noexcept {
        return (a != 0);
    }
};

/**
 * @struct H2DE_ColorHSV
 * @brief HSV color structure (Hue, Saturation, Value + Alpha).
 * 
 * Components range:
 * - h: 0.0 - 360.0 (degrees)
 * - s: 0.0 - 1.0
 * - v: 0.0 - 1.0
 * - a: 0.0 - 1.0
 */
struct H2DE_ColorHSV {
    float h = 1.0f;     /** Hue component (0.0 to 360.0) */
    float s = 1.0f;     /** Saturation component (0.0 to 1.0) */
    float v = 1.0f;     /** Value (brightness) component (0.0 to 1.0) */
    float a = 1.0f;     /** Alpha component (0.0 to 1.0) */

    /**
     * @brief Default constructor. Initializes to opaque white (1.0, 1.0, 1.0, 1.0).
     */
    constexpr H2DE_ColorHSV() noexcept = default;
    /**
     * @brief Constructs an HSV color without specifying alpha.
     * @param hue Hue value (0.0 - 360.0)
     * @param saturation Saturation value (0.0 - 1.0)
     * @param value Value (brightness) (0.0 - 1.0)
     */
    constexpr H2DE_ColorHSV(float hue, float saturation, float value) noexcept : h(hue), s(saturation), v(value), a(1.0f) {}
    /**
     * @brief Constructs an HSV color with specified alpha.
     * @param hue Hue value (0.0 - 360.0)
     * @param saturation Saturation value (0.0 - 1.0)
     * @param value Value (brightness) (0.0 - 1.0)
     * @param alpha Alpha value (0.0 - 1.0)
     */
    constexpr H2DE_ColorHSV(float hue, float saturation, float value, float alpha) noexcept : h(hue), s(saturation), v(value), a(alpha) {}

    /**
     * @brief Converts this HSV color to its RGB equivalent.
     * @return RGB representation of this HSV color.
     */
    explicit operator H2DE_ColorRGB() const;

    /**
     * @brief Compares if two HSV colors are equal.
     * @param other The other HSV color.
     * @return True if all components match.
     */
    constexpr bool operator==(const H2DE_ColorHSV& other) const noexcept {
        return (h == other.h && s == other.s && v == other.v, a == other.a);
    }
    /**
     * @brief Compares if two HSV colors are different.
     * @param other The other HSV color.
     * @return True if any component is different.
     */
    constexpr bool operator!=(const H2DE_ColorHSV& other) const noexcept {
        return !(*this == other);
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& color) {
        os << "[h: " << color.h << ", s: " << color.s << ", v: " << color.v << ", a: " << color.a << "]";
        return os;
    }

    /**
     * @brief Adds a hue offset to the color.
     * @param hue The amount to add to the hue (degrees).
     * @return New color with adjusted hue.
     */
    H2DE_ColorHSV addHue(float hue) const;
    /**
     * @brief Adds saturation to the color.
     * @param saturation The amount to add to the saturation (0-1 range).
     * @return New color with increased saturation.
     */
    H2DE_ColorHSV addSaturation(float saturation) const;
    /**
     * @brief Adds brightness (value) to the color.
     * @param value The amount to add to the brightness (0-1 range).
     * @return New color with increased brightness.
     */
    H2DE_ColorHSV addValue(float value) const;

/**
     * @brief Subtracts a hue offset from the color.
     * @param hue The amount to subtract from the hue (degrees).
     * @return New color with adjusted hue.
     */
    H2DE_ColorHSV subtractHue(float hue) const;
    /**
     * @brief Subtracts saturation from the color.
     * @param saturation The amount to subtract from the saturation (0-1 range).
     * @return New color with decreased saturation.
     */
    H2DE_ColorHSV subtractSaturation(float saturation) const;
    /**
     * @brief Subtracts brightness (value) from the color.
     * @param value The amount to subtract from the brightness (0-1 range).
     * @return New color with decreased brightness.
     */
    H2DE_ColorHSV subtractValue(float value) const;

    /**
     * @brief Multiplies the hue of the color by a factor.
     * @param multiplier The factor to multiply the hue by.
     * @return New color with scaled hue.
     */
    H2DE_ColorHSV multiplyHue(float multiplier) const;
    /**
     * @brief Multiplies the saturation of the color by a factor.
     * @param multiplier The factor to multiply the saturation by.
     * @return New color with scaled saturation.
     */
    H2DE_ColorHSV multiplySaturation(float multiplier) const;
    /**
     * @brief Multiplies the brightness (value) of the color by a factor.
     * @param multiplier The factor to multiply the brightness by.
     * @return New color with scaled brightness.
     */
    H2DE_ColorHSV multiplyValue(float multiplier) const;

    /**
     * @brief Divides the hue of the color by a divisor.
     * @param divider The divisor to divide the hue by.
     * @return New color with scaled hue.
     */
    H2DE_ColorHSV divideHue(float divider) const;
    /**
     * @brief Divides the saturation of the color by a divisor.
     * @param divider The divisor to divide the saturation by.
     * @return New color with scaled saturation.
     */
    H2DE_ColorHSV divideSaturation(float divider) const;
    /**
     * @brief Divides the brightness (value) of the color by a divisor.
     * @param divider The divisor to divide the brightness by.
     * @return New color with scaled brightness.
     */
    H2DE_ColorHSV divideValue(float divider) const;

    /**
     * @brief Checks if the color is visible (alpha > 0.0).
     * @return True if the color is not fully transparent.
     */
    constexpr bool isVisible() const noexcept {
        return (a != 0);
    }
};

/**
 * @struct H2DE_Padding
 * @brief Defines padding values for each side of a rectangle.
 */
struct H2DE_Padding {
    float top = 0.0f;       /** Padding on the top side. */
    float right = 0.0f;     /** Padding on the right side. */
    float bottom = 0.0f;    /** Padding on the bottom side. */
    float left = 0.0f;      /** Padding on the left side. */

    /**
     * @brief Default constructor. Initializes all paddings to 0.
     */
    constexpr H2DE_Padding() noexcept = default;
    /**
     * @brief Uniform padding constructor. Applies the same value to all sides.
     * @param all The padding to apply to top, right, bottom, and left.
     */
    constexpr H2DE_Padding(float all) noexcept : top(all), right(all), bottom(all), left(all) {}
    /**
     * @brief Block/inline padding constructor.
     * 
     * Applies the first value to top & bottom (block), and the second to left & right (inline).
     * 
     * @param block Padding for top and bottom.
     * @param inLine Padding for left and right.
     */
    constexpr H2DE_Padding(float block, float inLine) noexcept : top(block), right(inLine), bottom(block), left(inLine) {}
    /**
     * @brief Full custom padding constructor.
     * @param top Top padding.
     * @param right Right padding.
     * @param bottom Bottom padding.
     * @param left Left padding.
     */
    constexpr H2DE_Padding(float top, float right, float bottom, float left) noexcept : top(top), right(right), bottom(bottom), left(left) {}

    /**
     * @brief Outputs the H2DE_Padding to an output stream in a readable format.
     * 
     * This operator allows H2DE_Padding instances to be streamed (e.g., to std::cout)
     * with a format like: "top: 10, right: 5, bottom: 10, left: 5".
     *
     * Useful for debugging or logging layout information.
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Padding& padding) {
        os << "[top: " << padding.top;
        os << ", right: " << padding.right;
        os << ", bottom: " << padding.bottom;
        os << ", left: " << padding.left << "]";
        return os;
    }
};

/**
 * @struct H2DE_WindowData
 * @brief Contains configuration data for initializing an H2DE window.
 * 
 * This structure includes all necessary parameters to define the window's 
 * default state such as size, position, title, framerate, and other behaviors.
 */
struct H2DE_WindowData {
    uint16_t fps = 60;                                                          /** Target frames per second of the window. */
    const char* title = "H2DE Window";                                          /** Title text shown in the window's title bar. */
    H2DE_PixelPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };     /** Initial position of the window on the screen. */
    H2DE_PixelSize size = { 1280, 720 };                                        /** Initial dimensions of the window in pixels. */
    bool fullscreen = false;                                                    /** Whether the window should launch in fullscreen mode. */
    bool resizable = false;                                                     /** Whether the window can be resized by the user. */
    bool saveState = false;                                                     /** Whether to save window state (position, size, etc.) on exit. */
    H2DE_WindowRatio ratio = H2DE_WINDOW_RATIO_NO_RATIO;                        /** Defines the window's aspect ratio behavior. */
};

/**
 * @struct H2DE_CameraData
 * @brief Contains settings for configuring the in-game camera.
 * 
 * This structure defines how the camera behaves in the game world, including
 * translation, dimensions, smoothing, padding, origin, and debug options.
 */
struct H2DE_CameraData {
    H2DE_Translate translate = { 0.0f, 0.0f };              /** Initial camera offset in world units. */
    float gameWidth = 20.0f;                                /** Width of the game world visible by the camera. */
    float interfaceWidth = 20.0f;                           /** Width used for UI scaling relative to the interface. */
    float smoothing = 0.0f;                                 /** Smooth transition factor when moving the camera. */
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };      /** Padding around the camera view. */
    H2DE_Face xOrigin = H2DE_FACE_LEFT;                     /** Horizontal alignment of the camera's origin. */
    H2DE_Face yOrigin = H2DE_FACE_TOP;                      /** Vertical alignment of the camera's origin. */
    bool grid = true;                                       /** Whether to display a grid overlay (usually for debugging). */
};

/**
 * @struct H2DE_EngineData
 * @brief Holds the engine's core configuration.
 * 
 * This structure aggregates the essential components required to initialize
 * and manage the game engine, including window and camera settings.
 */
struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();     /** Settings related to the main application window. */
    H2DE_CameraData camera = H2DE_CameraData();     /** Settings related to the main in-game camera. */
};

/**
 * @struct H2DE_Transform
 * @brief Represents a 2D transformation for positioning, scaling, and rotating objects.
 * 
 * This structure is used to apply transformations to entities in a 2D space,
 * such as their position, scale, rotation, and pivot point.
 */
struct H2DE_Transform {
public:
    H2DE_Translate translate = { 0.0f, 0.0f };      /** Object position in the 2D space. */
    H2DE_Scale scale = { 1.0f, 1.0f };              /** Object scale on both axes. */
    float rotation = 0.0f;                          /** Object rotation in degrees. */
    H2DE_Pivot pivot = { 0.0f, 0.0f };              /** Pivot point for rotation and scaling. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_Transform() noexcept = default;
    /**
     * @brief Initializes a transform with custom values.
     * 
     * @param translate The position of the object.
     * @param scale The scaling factors.
     * @param rotation The rotation in degrees.
     * @param pivot The pivot point.
     */
    constexpr H2DE_Transform(const H2DE_Translate& translate, const H2DE_Scale& scale, float rotation, const H2DE_Pivot& pivot) noexcept : translate(translate), defaultTranslate(translate), scale(scale), defaultScale(scale), rotation(rotation), pivot(pivot), defaultPivot(pivot) {}

    /**
     * @brief Outputs the H2DE_Transform to an output stream in a readable format.
     * 
     * This operator allows H2DE_Transform instances to be streamed (e.g., to std::cout)
     * 
     * Useful for debugging transformation data such as position, scale, rotation, and pivot.
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Transform& transform) {
        os << "[translate: " << transform.translate;
        os << ", scale: " << transform.scale;
        os << ", rotation: " << transform.rotation;
        os << ", pivot: " << transform.pivot << "]";
        return os;
    }

private:
    H2DE_Translate defaultTranslate = { 0.0f, 0.0f };
    H2DE_Scale defaultScale = { 1.0f, 1.0f };
    H2DE_Pivot defaultPivot = { 0.0f, 0.0f };

    friend class H2DE_Object;
    friend class H2DE_Surface;
};

/**
 * @struct H2DE_Hitbox
 * @brief Represents a hitbox used for collision detection.
 * 
 * Contains transformation data, visual color, collision identifier,
 * and an optional callback for collision events.
 */
struct H2DE_Hitbox {
    H2DE_Transform transform = H2DE_Transform();                            /** Hitbox transform (position, scale, rotation, pivot). */
    H2DE_ColorRGB color = H2DE_ColorRGB();                                  /** Debug color used when rendering the hitbox. */
    int collisionIndex = 0;                                                 /** Custom collision group index to filter interactions. */
    std::function<void(H2DE_Object*, H2DE_Face)> onCollide = nullptr;       /** Callback triggered when the hitbox collides with another object. */
};

/**
 * @struct H2DE_Text
 * @brief Represents all the properties to render a text element.
 * 
 * Contains the text content, font settings, size, spacing,
 * alignment, color, and padding around the text box.
 */
struct H2DE_Text {
    std::string text = "";                                          /** Text content to display. */
    std::string font = "";                                          /** Font name or path to use for rendering the text. */
    H2DE_Scale container = { 10.0f, 10.0f };                        /** Zone for the text to be written in. */
    H2DE_Scale fontSize = { 1.0f, 1.0f };                           /** Font size scale for X and Y axes. */
    H2DE_Scale spacing = { 0.1f, 0.3f };                            /** Spacing between characters (X) and lines (Y). */
    H2DE_TextAlign textAlign = H2DE_TEXT_ALIGN_CENTER_CENTER;       /** Text alignment (horizontal and vertical). */
    H2DE_ColorRGB color = H2DE_ColorRGB();                          /** Color of the text. */
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };              /** Padding around the text box (top, right, bottom, left). */
};

/**
 * @struct H2DE_Time
 * @brief Represents a time duration or timestamp with hours, minutes, seconds, and milliseconds.
 */
struct H2DE_Time {
    uint8_t hours = 0;          /** Number of hours. */
    uint8_t minutes = 0;        /** Number of minutes. */
    uint8_t seconds = 0;        /** Number of seconds. */
    uint16_t milliseconds = 0;  /** Number of milliseconds. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_Time() noexcept = default;
    /**
     * @brief Constructs a H2DE_Time with specified values.
     * @param hours Number of hours.
     * @param minutes Number of minutes.
     * @param seconds Number of seconds.
     * @param milliseconds Number of milliseconds.
     */
    constexpr H2DE_Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept : hours(hours), minutes(minutes), seconds(seconds), milliseconds(milliseconds) {}

    /**
     * @brief Converts the time to a float representing the total elapsed time in seconds.
     * 
     * This function converts the current time represented by the H2DE_Time object into a float value.
     * The result is the total number of seconds, including the fractional part from milliseconds.
     * 
     * @return Elapsed time in seconds as a float.
     */
    constexpr float toElapsed() const noexcept {
        return (hours * 3600.0f) + (minutes * 60.0f) + (seconds) + (milliseconds * 0.001f);
    }

    /**
     * @brief Converts a float elapsed time in seconds into an H2DE_Time object.
     * 
     * This static function takes a float representing a duration in seconds, and decomposes it
     * into hours, minutes, seconds, and milliseconds. It uses `H2DE::floor` and `H2DE::round` internally
     * to compute each part precisely. The result is clamped to valid ranges for each time unit.
     * 
     * @param elapsed The total elapsed time in seconds.
     * @return An H2DE_Time object representing the decomposed time.
     */
    static constexpr H2DE_Time toTime(float elapsed) noexcept {
        return H2DE_Time(
            static_cast<int>(elapsed) / 3600,
            H2DE::clamp((static_cast<int>(elapsed) % 3600) / 60, 0, 59),
            H2DE::clamp(static_cast<int>(elapsed) % 60, 0, 59),
            H2DE::clamp(H2DE::round((elapsed - H2DE::floor(elapsed)) * 1000.0f), 0, 999)
        );
    }

    /**
     * @brief Outputs the H2DE_Time to an output stream in a readable format.
     * 
     * This operator allows H2DE_Time instances to be streamed (e.g., to std::cout)
     * with a format like: "h: 1, m: 30, s: 45, ms: 250".
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Time& time) {
        os << "[hours: " << static_cast<int>(time.hours);
        os << ", minutes: " << static_cast<int>(time.minutes);
        os << ", seconds: " << static_cast<int>(time.seconds);
        os << ", milliseconds: " << static_cast<int>(time.milliseconds) << "]";
        return os;
    }
};

/**
 * @struct H2DE_ButtonEventData
 * @brief Stores event context for button callbacks.
 * 
 * This struct is passed to button event functions to provide access to the button that triggered the event
 * and the timeline in which it exists.
 */
struct H2DE_ButtonEventData {
    H2DE_ButtonObject* button;          /**< Pointer to the button that triggered the event. */
    H2DE_Timeline* timeline;            /**< Reference to the timeline the button is part of. */

    /**
     * @brief Constructs an event data object for button events.
     * @param button Pointer to the button triggering the event.
     * @param id Reference to the timeline where the event occurred.
     */
    constexpr H2DE_ButtonEventData(H2DE_ButtonObject* button, H2DE_Timeline* timeline) noexcept : button(button), timeline(timeline) {}
};

/**
 * @struct H2DE_ObjectData
 * @brief Contains basic data for an object, including its transform, opacity, position mode, and index.
 */
struct H2DE_ObjectData {
    H2DE_Transform transform = H2DE_Transform();    /**< Object's transformation data (position, scale, rotation, pivot). */
    uint8_t opacity = H2DE_OPACITY_MAX;             /**< Opacity value from 0 (transparent) to 255 (fully opaque). */
    bool absolute = false;                          /**< If true, the position is absolute; otherwise relative. */
    int index = 0;                                  /**< Object index identifier. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_ObjectData() noexcept = default;
    /**
     * @brief Constructs a H2DE_Time with specified values.
     * @param hours Number of hours.
     * @param minutes Number of minutes.
     * @param seconds Number of seconds.
     * @param milliseconds Number of milliseconds.
     */
    constexpr H2DE_ObjectData(const H2DE_Transform& transform, uint8_t opacity, bool absolute, int index) noexcept : transform(transform), opacity(opacity), absolute(absolute), index(index) {}
};

/**
 * @struct H2DE_BarObjectData
 * @brief Stores the minimum, maximum, and current value for a bar-like object (e.g., progress bar, health bar).
 */
struct H2DE_BarObjectData {
    float min = 0.0f;       /**< Minimum value of the bar. */
    float max = 100.0f;     /**< Maximum value of the bar. */
    float value = 0.0f;     /**< Current value of the bar. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_BarObjectData() noexcept = default;
    /**
     * @brief Constructs an H2DE_BarObjectData with specified min, max and current value.
     * @param min Minimum value.
     * @param max Maximum value.
     * @param value Current value.
     */
    constexpr H2DE_BarObjectData(float min, float max, float value) noexcept : min(min), max(max), value(value) {}
};

/**
 * @struct H2DE_ButtonObjectData
 * @brief Contains data and event callbacks for a button object.
 */
struct H2DE_ButtonObjectData {
    H2DE_Text text = H2DE_Text();                                       /**< Text displayed on the button. */
    H2DE_MouseButton mouseButton = H2DE_MOUSE_BUTTON_LEFT;              /**< Mouse button(s) that trigger interactions on the button. */
    std::function<void(H2DE_ButtonEventData&)> onMouseDown = nullptr;   /**< Callback triggered when the button is pressed down. */
    std::function<void(H2DE_ButtonEventData&)> onMouseUp = nullptr;     /**< Callback triggered when the button is released. */
    std::function<void(H2DE_ButtonEventData&)> onHover = nullptr;       /**< Callback triggered when the mouse hovers over the button. */
    std::function<void(H2DE_ButtonEventData&)> onBlur = nullptr;        /**< Callback triggered when the mouse stops hovering the button. */
    bool pauseSensitive = true;                                         /**< Whether the button events are sensitive to game pause state. */
};

/**
 * @struct H2DE_TextObjectData
 * @brief Holds text data for a text object.
 */
struct H2DE_TextObjectData {
    H2DE_Text text = H2DE_Text();       /**< Text content and styling for the text object. */
};

/**
 * @struct H2DE_TimerObjectData
 * @brief Stores timer data and display options for a timer object.
 */
struct H2DE_TimerObjectData {
    H2DE_Time time = H2DE_Time();           /**< Current timer time values. */
    H2DE_Text text = H2DE_Text();           /**< Text styling and content for the timer display. */
    char separator = ':';                   /**< Character used as separator between time units. */
    bool displayHours = false;              /**< Whether to display hours. */
    bool displayMinutes = false;            /**< Whether to display minutes. */
    bool displaySeconds = true;             /**< Whether to display seconds. */
    bool displayMilliseconds = false;       /**< Whether to display milliseconds. */
    bool increasing = true;                 /**< Whether the timer is increasing (false for decreasing). */
    bool pauseSensitive = true;             /**< Whether the timer is sensitive to pause state. */
};

/**
 * @struct H2DE_SurfaceData
 * @brief Contains transformation and rendering parameters for a surface.
 */
struct H2DE_SurfaceData {
    H2DE_Transform transform = H2DE_Transform();            /**< Transformation data (position, scale, rotation, pivot). */
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;      /**< Scaling interpolation mode. */
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;       /**< Blending mode used for rendering. */
    int index = 0;                                          /**< Identifier or order index of the surface. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_SurfaceData() noexcept = default;
    /**
     * @brief Constructs an H2DE_SurfaceData with specified transform, scale mode, blend mode, and index.
     * @param transform The transform data (position, scale, rotation, pivot).
     * @param scaleMode The scaling mode used for interpolation.
     * @param blendMode The blending mode for rendering.
     * @param index The index or order of the surface.
     */
    constexpr H2DE_SurfaceData(const H2DE_Transform& transform, H2DE_ScaleMode scaleMode, H2DE_BlendMode blendMode, int index) noexcept : transform(transform), scaleMode(scaleMode), blendMode(blendMode), index(index) {}
};

/**
 * @struct H2DE_TextureData
 * @brief Holds texture data including texture name, color tint, and optional source rectangle.
 */
struct H2DE_TextureData {
    std::string textureName = "";                               /**< Name or identifier of the texture */
    H2DE_ColorRGB color = H2DE_ColorRGB();                      /**< Color tint applied to the texture */
    std::optional<H2DE_PixelRect> srcRect = std::nullopt;       /**< Optional source rectangle to use a part of the texture */
};

/**
 * @struct H2DE_SpriteData
 * @brief Stores data for a sprite including texture info, position, size, animation and other settings.
 */
struct H2DE_SpriteData {
    std::string textureName = "";                   /**< Name or ID of the texture used for the sprite */
    H2DE_ColorRGB color = H2DE_ColorRGB();          /**< Color tint applied to the sprite */
    H2DE_PixelPos startingPos = { 0, 0 };           /**< Starting position of the sprite within the texture */
    H2DE_PixelSize size = { 1, 1 };                 /**< Size of the sprite frame */
    int spacing = 0;                                /**< Spacing between frames in the texture */
    uint16_t nbFrame = 0;                           /**< Number of frames for animation */
    uint32_t delay = 200;                           /**< Delay in milliseconds between animation frames */
    bool pauseSensitive = true;                     /**< Whether the sprite animation respects pause state */
};

/**
 * @struct H2DE_ColorData
 * @brief Holds RGB color data for an object.
 */
struct H2DE_ColorData {
    H2DE_ColorRGB color = H2DE_ColorRGB();      /**< The RGB color value */

    /** 
     * @brief Default constructor, initializes with default color.
     */
    constexpr H2DE_ColorData() noexcept = default;
    /**
     * @brief Constructs H2DE_ColorData with a given RGB color.
     * @param color The RGB color to initialize with.
     */
    constexpr H2DE_ColorData(const H2DE_ColorRGB& color) noexcept : color(color) {}
};

/**
 * @struct H2DE_BorderData
 * @brief Describes the border style used for border object.
 * 
 * Contains data such as border type, color, thickness, and fill status.
 */
struct H2DE_BorderData {
    H2DE_BorderType type = H2DE_BORDER_TYPE_RECTANGLE;                  /**< Shape of the border (rectangle or circle) */
    H2DE_BorderPlacement placement = H2DE_BORDER_PLACEMENT_CENTER;      /**< Placement of the border relative to the shape's edge */
    H2DE_ColorRGB color = H2DE_ColorRGB();                              /**< Color of the border */
    uint16_t thickness = 1;                                             /**< Thickness of the border in pixels */
    bool filled = false;                                                /**< Whether the shape is filled (true) or just outlined (false) */

    /** 
     * @brief Default constructor. 
     */
    constexpr H2DE_BorderData() noexcept = default;

    /**
     * @brief Custom constructor to initialize all fields.
     * @param type Border shape type
     * @param placement Border placement
     * @param color Border color
     * @param thickness Border thickness in pixels
     * @param filled Whether the shape is filled or not
     */
    constexpr H2DE_BorderData(H2DE_BorderType type, H2DE_BorderPlacement placement, const H2DE_ColorRGB& color, uint16_t thickness, bool filled) noexcept : type(type), placement(placement), color(color), thickness(thickness), filled(filled) {}
};

/**
 * @struct H2DE_Font
 * @brief Represents font data including texture, character sizing, spacing, and rendering modes.
 */
struct H2DE_Font {
    std::string textureName = "";                           /**< Name of the texture used for the font */ 
    H2DE_PixelSize charSize = { 0, 0 };                     /**< Size of each character in pixels */
    int spacing = 0;                                        /**< Spacing between characters */
    std::string charOrder = "";                             /**< Order of characters in the texture */
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;      /**< Scaling mode for rendering */
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;       /**< Blending mode for rendering */
};

#endif

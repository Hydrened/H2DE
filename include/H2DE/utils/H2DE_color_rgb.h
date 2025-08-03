#pragma once

#include <H2DE/utils/H2DE_utils.h>
struct H2DE_ColorHSV;

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

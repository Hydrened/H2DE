#pragma once

#include <H2DE/utils/H2DE_utils.h>

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

#ifndef H2DE_SURFACE_COLOR_H
#define H2DE_SURFACE_COLOR_H

/**
 * @file H2DE_color.h
 * @brief Defines the H2DE_Color class for solid color surfaces in the H2DE engine.
 * 
 * This file declares the H2DE_Color class, a concrete surface representing
 * a simple solid color fill. It manages color data and supports color modulation
 * animation with easing and completion callbacks.
 * 
 * Solid color surfaces can be attached to objects as drawable elements
 * and support instant or animated color changes.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @class H2DE_Color
 * @brief Represents a solid color drawable surface in the H2DE engine.
 * 
 * H2DE_Color extends H2DE_Surface and encapsulates a simple color fill
 * that can be modulated or animated over time.
 * 
 * Supports setting and getting color instantly, as well as animating color
 * transitions with easing functions and callbacks.
 */
class H2DE_Color : public H2DE_Surface {
public:
    /**
     * @brief Get the full color data structure.
     * 
     * Contains the current color value.
     * 
     * @return The H2DE_ColorData of the color surface.
     */
    constexpr H2DE_ColorData getColorData() const noexcept {
        return colorData;
    }
    /**
     * @brief Get the current modulating color.
     * 
     * @return The current color (H2DE_ColorRGB).
     */
    inline H2DE_ColorRGB getColor() const noexcept override {
        return colorData.color; 
    }

    /**
     * @brief Instantly set the modulating color.
     * 
     * @param color New color to set.
     */
    inline void setColor(const H2DE_ColorRGB& color) noexcept {
        colorData.color = color;
    }

    /**
     * @brief Animate the modulating color over time with easing and completion callback.
     * 
     * @param color Target color to animate to.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_ColorData;
    
    friend class H2DE_Object;

private:
    H2DE_ColorData colorData;

    H2DE_Color(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData) noexcept;
    ~H2DE_Color() override = default;

    inline std::string getTextureName() const override {
        return "/";
    }
    inline std::optional<H2DE_PixelRect> getSrcRect() const noexcept override {
        return std::nullopt;
    }
    inline bool isVisible() const noexcept override {
        return (!isHidden() && colorData.color.isVisible());
    }
};

#endif

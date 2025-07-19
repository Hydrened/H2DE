#ifndef H2DE_SURFACE_BORDER_H
#define H2DE_SURFACE_BORDER_H

/**
 * @file H2DE_border.h
 * @brief Defines the H2DE_Border class for drawable borders in the H2DE engine.
 * 
 * This file declares the H2DE_Border class, a surface representing a border shape,
 * either rectangle or circle, with customizable thickness, color, fill, and type.
 * 
 * Borders can be filled or outlined, animated over time, and used for UI elements,
 * selection highlights, or any visual needing a clean geometric shape.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @class H2DE_Border
 * @brief Represents a drawable border surface, either rectangular or circular.
 *
 * A border is a type of surface that can be rendered either filled or outlined,
 * with customizable thickness, color, and shape (rectangle or circle).
 * Supports animated changes of color and thickness.
 */
class H2DE_Border : public H2DE_Surface {
public:
    /**
     * @brief Returns the entire border data.
     * @return A copy of the current H2DE_BorderData.
     */
    constexpr H2DE_BorderData getBorderData() const noexcept {
        return borderData;
    }
    /**
     * @brief Returns the current border type (rectangle or circle).
     * @return The border type as a H2DE_BorderType enum value.
     */
    constexpr H2DE_BorderType getType() const noexcept {
        return borderData.type;
    }
    /**
     * @brief Returns the current border placement relative to the shape's edge.
     * @return The border placement as a H2DE_BorderPlacement enum value.
     */
    constexpr H2DE_BorderPlacement getPlacement() const noexcept {
        return borderData.placement;
    }
    /**
     * @brief Returns the current border color.
     * @return The color of the border as a H2DE_ColorRGB.
     */
    inline H2DE_ColorRGB getColor() const noexcept override {
        return borderData.color;
    }
    /**
     * @brief Returns the current border thickness in pixels.
     * @return Border thickness as an unsigned 16-bit integer.
     */
    constexpr uint16_t getThickness() const noexcept {
        return borderData.thickness;
    }
    /**
     * @brief Checks if the border is filled or just outlined.
     * @return true if the border is filled, false if it's only an outline.
     */
    constexpr bool isFilled() const noexcept {
        return borderData.filled;
    }

    /**
     * @brief Sets the border type.
     * @param type New border type to apply.
     */
    inline void setType(H2DE_BorderType type) noexcept {
        borderData.type = type;
    }
    /**
     * @brief Sets the border placement relative to the shape's edge.
     * @param placement New border placement to apply.
     */
    inline void setPlacement(H2DE_BorderPlacement placement) noexcept {
        borderData.placement = placement;
    }
    /**
     * @brief Instantly set the modulating color.
     * 
     * @param color New color to set.
     */
    inline void setColor(const H2DE_ColorRGB& color) noexcept {
        borderData.color = color;
    }
    /**
     * @brief Sets the border thickness.
     * @param thickness New thickness value in pixels.
     */
    inline void setThickness(uint16_t thickness) noexcept {
        borderData.thickness = thickness;
    }
    /**
     * @brief Sets whether the border should be filled or just outlined.
     * @param filled true to fill the border, false for outline only.
     */
    inline void setFilled(bool filled) noexcept {
        borderData.filled = filled;
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
    
    /**
     * @brief Animate the modulating thickness over time with easing and completion callback.
     * 
     * @param thickness Target thickness to animate to.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setThickness(uint16_t thickness, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_BorderData;
    
    friend class H2DE_Object;

private:
    H2DE_BorderData borderData;

    H2DE_Border(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_BorderData& borderData) noexcept;
    ~H2DE_Border() override = default;

    inline std::string getTextureName() const override {
        return "/";
    }
    inline std::optional<H2DE_PixelRect> getSrcRect() const noexcept override {
        return std::nullopt;
    }
    inline bool isVisible() const noexcept override {
        return (!isHidden() && borderData.color.isVisible() && borderData.thickness != 0);
    }
};

#endif

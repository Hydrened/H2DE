#ifndef H2DE_SURFACE_BORDER_H
#define H2DE_SURFACE_BORDER_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Border : public H2DE_Surface {
public:
    constexpr H2DE_BorderData getBorderData() const noexcept {
        return borderData;
    }

    constexpr H2DE_BorderType getType() const noexcept {
        return borderData.type;
    }

    inline H2DE_ColorRGB getColor() const noexcept override {
        return borderData.color; 
    }

    constexpr uint16_t getThickness() const noexcept {
        return borderData.thickness;
    }

    constexpr bool isFilled() const noexcept {
        return borderData.filled;
    }

    inline void setType(H2DE_BorderType type) noexcept {
        borderData.type = type;
    }

    /**
     * @brief Instantly set the modulating color.
     * 
     * @param color New color to set.
     */
    inline void setColor(const H2DE_ColorRGB& color) noexcept {
        borderData.color = color;
    }

    inline void setThickness(uint16_t thickness) noexcept {
        borderData.thickness = thickness;
    }

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
     * @return H2DE_TimelineID ID of the timeline controlling this animation.
     */
    H2DE_TimelineID setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    
    /**
     * @brief Animate the modulating thickness over time with easing and completion callback.
     * 
     * @param thickness Target thickness to animate to.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return H2DE_TimelineID ID of the timeline controlling this animation.
     */
    H2DE_TimelineID setThickness(uint16_t thickness, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

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

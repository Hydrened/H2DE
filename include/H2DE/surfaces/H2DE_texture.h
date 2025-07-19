#ifndef H2DE_SURFACE_TEXTURE_H
#define H2DE_SURFACE_TEXTURE_H

/**
 * @file H2DE_texture.h
 * @brief Texture surface class for H2DE engine.
 * 
 * Defines the H2DE_Texture class which represents a texture-based drawable surface.
 * It handles texture naming, color modulation, optional source rectangle cropping,
 * and animated color changes with easing and callbacks.
 * 
 * Textures are a type of surface and inherit common properties and behavior
 * from the abstract H2DE_Surface base class.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @class H2DE_Texture
 * @brief Concrete texture surface implementation for the H2DE engine.
 * 
 * This class extends H2DE_Surface to provide texture-specific features:
 * - Storing and retrieving texture name
 * - Color modulation (tinting)
 * - Optional source rectangle for partial texture rendering
 * - Animated color transitions with timeline, easing, and completion callbacks
 * 
 * H2DE_Texture supports instant property changes as well as smooth animations.
 */
class H2DE_Texture : public H2DE_Surface {
public:
    /**
     * @brief Get the full texture data structure.
     * 
     * Contains texture name, color tint, source rectangle, etc.
     * 
     * @return The H2DE_TextureData of the surface.
     */
    inline H2DE_TextureData getTextureData() const noexcept {
        return textureData;
    }
    /**
     * @brief Get the texture name.
     * 
     * @return The texture name string.
     */
    inline std::string getTextureName() const noexcept override { 
        return textureData.textureName;
    }
    /**
     * @brief Get the modulating color of the texture.
     * 
     * @return The H2DE_ColorRGB color.
     */
    inline H2DE_ColorRGB getColor() const noexcept override {
        return textureData.color; 
    }
    /**
     * @brief Get the source rectangle within the texture.
     * 
     * Allows rendering only part of the texture.
     * 
     * @return Optional source rectangle.
     */
    inline std::optional<H2DE_PixelRect> getSrcRect() const noexcept override {
        return textureData.srcRect;
    }

    /**
     * @brief Set the texture name instantly.
     * 
     * @param textureName New texture name.
     */
    void setTextureName(const std::string& textureName);
    /**
     * @brief Set the modulating color instantly.
     * 
     * @param color New color.
     */
    void setColor(const H2DE_ColorRGB& color);
    /**
     * @brief Set the source rectangle instantly.
     * 
     * @param srcRect New optional source rectangle.
     */
    void setSrcRect(const std::optional<H2DE_PixelRect>& srcRect);

    /**
     * @brief Animate the modulating color over time with easing and completion callback.
     * 
     * @param color Target color.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TextureData;
    
    friend class H2DE_Object;
    friend class H2DE_TextObject;

private:
    H2DE_TextureData textureData;

    H2DE_Texture(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData) noexcept;
    ~H2DE_Texture() override = default;

    inline bool isVisible() const noexcept override {
        return (!isHidden() && textureData.color.isVisible());
    }
};

#endif

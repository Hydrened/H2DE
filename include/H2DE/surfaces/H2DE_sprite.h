#ifndef H2DE_SURFACE_SPRITE_H
#define H2DE_SURFACE_SPRITE_H

/**
 * @file H2DE_sprite.h
 * @brief Defines the H2DE_Sprite class for animated sprite surfaces in H2DE engine.
 * 
 * This file declares the H2DE_Sprite class, a concrete surface type representing
 * animated sprites based on a spritesheet texture. It manages frame properties
 * such as starting position, frame size, spacing, frame count, animation delay,
 * color modulation, and pause sensitivity.
 * 
 * Sprites are drawable surfaces attached to objects, supporting frame animation
 * with timing and easing control.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @class H2DE_Sprite
 * @brief Represents an animated sprite surface with frame control in the H2DE engine.
 * 
 * H2DE_Sprite extends H2DE_Surface and provides functionality to manage
 * sprite animations from a texture spritesheet. It supports:
 * - Specifying the texture and sub-region for frames
 * - Controlling animation frames, delay between frames, and spacing
 * - Color tint modulation with instant or animated changes
 * - Pause-sensitive animation playback
 * 
 * The sprite updates its current frame automatically based on delay timing,
 * and provides an interface to query and modify animation properties.
 */
class H2DE_Sprite : public H2DE_Surface {
public:
    /**
     * @brief Get the full sprite data structure.
     * 
     * Contains texture name, color tint, frame size, spacing, frame count, delay, etc.
     * 
     * @return The H2DE_SpriteData of the sprite.
     */
    inline H2DE_SpriteData getSpriteData() const {
        return spriteData; 
    }
    /**
     * @brief Get the texture name used by the sprite.
     * 
     * @return Texture name string.
     */
    inline std::string getTextureName() const override {
        return spriteData.textureName;
    }
    /**
     * @brief Get the modulating color tint of the sprite.
     * 
     * @return Current color (H2DE_ColorRGB).
     */
    inline H2DE_ColorRGB getColor() const noexcept override {
        return spriteData.color;
    }
    /**
     * @brief Get the starting position of the sprite in the texture.
     * 
     * Usually the top-left corner of the first frame.
     * 
     * @return Starting position (H2DE_PixelPos).
     */
    constexpr H2DE_PixelPos getStartingPos() const {
        return spriteData.startingPos; 
    }
    /**
     * @brief Get the size of each frame in the sprite.
     * 
     * @return Frame size (H2DE_PixelSize).
     */
    constexpr H2DE_PixelSize getSize() const {
        return spriteData.size;
    }
    /**
     * @brief Get the spacing in pixels between frames.
     * 
     * Useful for spritesheets with padding.
     * 
     * @return Spacing in pixels.
     */
    constexpr int getSpacing() const {
        return spriteData.spacing; 
    }
    /**
     * @brief Get the number of frames in the sprite animation.
     * 
     * @return Number of frames.
     */
    constexpr uint16_t getNbFrame() const {
        return spriteData.nbFrame;
    }
    /**
     * @brief Get the delay between frames in milliseconds.
     * 
     * @return Frame delay.
     */
    constexpr uint32_t getDelay() const {
        return spriteData.delay;
    }
    /**
     * @brief Check if the animation is sensitive to pause state.
     * 
     * If true, animation pauses when the game is paused.
     * 
     * @return True if pause sensitive, false otherwise.
     */
    constexpr bool isPauseSensitive() const {
        return spriteData.pauseSensitive;
    }

    /**
     * @brief Instantly set the texture name used by the sprite.
     * 
     * @param textureName New texture name.
     */
    void setTextureName(const std::string& textureName);
    /**
     * @brief Instantly set the modulating color tint of the sprite.
     * 
     * @param color New color.
     */
    void setColor(const H2DE_ColorRGB& color);
    /**
     * @brief Instantly set the starting position of the sprite in the texture.
     * 
     * @param startingPos New starting position.
     */
    void setStartingPos(const H2DE_PixelPos& startingPos);
    /**
     * @brief Instantly set the size of each frame in the sprite.
     * 
     * @param size New frame size.
     */
    void setSize(const H2DE_PixelSize& size);
    /**
     * @brief Instantly set the spacing in pixels between frames.
     * 
     * @param spacing New spacing value.
     */
    void setSpacing(int spacing);
    /**
     * @brief Instantly set the number of frames in the sprite animation.
     * 
     * @param nbFrame New number of frames.
     */
    void setNbFrame(uint16_t nbFrame);
    /**
     * @brief Instantly set the delay between frames in milliseconds.
     * 
     * @param delay New delay value.
     */
    void setDelay(uint32_t delay);
    /**
     * @brief Instantly set whether the sprite animation is sensitive to pause.
     * 
     * @param pauseSensitive True to pause animation when game is paused.
     */
    void setPauseSensitive(bool pauseSensitive);

    /**
     * @brief Animate the modulating color over time with easing and completion callback.
     * 
     * @param color Target color.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return H2DE_TimelineID ID of the timeline controlling this animation.
     */
    H2DE_TimelineID setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_SpriteData;

    friend class H2DE_Object;

private:
    H2DE_SpriteData spriteData;

    int delayId = H2DE_INVALID_DELAY_ID;
    uint16_t currentFrame = 0;

    H2DE_Sprite(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
    ~H2DE_Sprite() override;

    void initDelay();
    void nextFrame() noexcept;

    std::optional<H2DE_PixelRect> getSrcRect() const noexcept override;
    inline bool isVisible() const noexcept override {
        return (!isHidden() && spriteData.color.isVisible()); 
    }
};

#endif

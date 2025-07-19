#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

/**
 * @file H2DE_surface.h
 * @brief Base class for all surface types in H2DE engine.
 * 
 * This file defines the abstract class H2DE_Surface, which is the
 * parent class for various surface implementations like textures,
 * sprites, and colors. It manages common properties such as
 * transform, scale mode, blend mode, visibility, and layering.
 * 
 * Surfaces represent drawable elements attached to objects and
 * can be shown, hidden, moved, scaled, rotated, and animated.
 */

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

/**
 * @class H2DE_Surface
 * @brief Abstract base class for all drawable surfaces in the H2DE engine.
 * 
 * Represents any drawable element (texture, sprite, color fill) that can be attached to
 * a game object. Manages common properties like transformation (translation, scale, rotation),
 * visibility, blend modes, layering order, and supports smooth animation via timelines.
 * 
 * This class is meant to be inherited by concrete surface implementations,
 * which provide texture or color data and specific rendering logic.
 * 
 * You can instantly set properties or animate them over time with easing
 * and callback support. Surfaces can be shown or hidden, reordered, and transformed.
 */
class H2DE_Surface {
public:
    /**
     * @brief Show the surface.
     * 
     * Makes the surface visible (hidden = false).
     */
    inline void show() noexcept {
        hidden = false; 
    }
    /**
     * @brief Hide the surface.
     * 
     * Makes the surface invisible (hidden = true).
     */
    inline void hide() noexcept {
        hidden = true; 
    }

    /**
     * @brief Get the full surface data structure.
     * 
     * Contains transform, scale mode, blend mode, index (layering order), etc.
     * 
     * @return The H2DE_SurfaceData of the surface.
     */
    constexpr H2DE_SurfaceData getSurfaceData() const noexcept {
        return surfaceData; 
    }
    /**
     * @brief Get the transform information of the surface.
     * 
     * Includes translation, scale, rotation, and pivot point.
     * 
     * @return The H2DE_Transform struct.
     */
    constexpr H2DE_Transform getTransform() const noexcept {
        return surfaceData.transform;
    }
    /**
     * @brief Get the translation vector of the surface.
     * 
     * @return The H2DE_Translate struct (position offset).
     */
    constexpr H2DE_Translate getTranslate() const noexcept {
        return surfaceData.transform.translate;
    }
    /**
     * @brief Get the scale factors of the surface.
     * 
     * @return The H2DE_Scale struct (x and y scale).
     */
    constexpr H2DE_Scale getScale() const noexcept {
        return surfaceData.transform.scale;
    }
    /**
     * @brief Get the rotation angle in degrees or radians.
     * 
     * @return The rotation value as a float.
     */
    constexpr float getRotation() const noexcept {
        return surfaceData.transform.rotation;
    }
    /**
     * @brief Get the pivot point for rotation and scaling.
     * 
     * @return The H2DE_Pivot struct.
     */
    constexpr H2DE_Pivot getPivot() const noexcept {
        return surfaceData.transform.pivot;
    }
    /**
     * @brief Get the scale mode (how the surface scales relative to its container).
     * 
     * @return The H2DE_ScaleMode enum value.
     */
    constexpr H2DE_ScaleMode getScaleMode() const noexcept {
        return surfaceData.scaleMode;
    }
    /**
     * @brief Get the blend mode used for rendering this surface.
     * 
     * @return The H2DE_BlendMode enum value.
     */
    constexpr H2DE_BlendMode getBlendMode() const noexcept {
        return surfaceData.blendMode;
    }
    /**
     * @brief Get the index used for draw order layering.
     * 
     * Surfaces with higher index values are drawn above lower ones.
     * 
     * @return The index as an integer.
     */
    constexpr int getIndex() const noexcept {
        return surfaceData.index;
    }
    /**
     * @brief Check if the surface is currently hidden.
     * 
     * @return true if hidden, false otherwise.
     */
    constexpr bool isHidden() const noexcept {
        return hidden;
    }

    /** 
     * @brief Set the translation vector instantly.
     * @param translate New translation (position offset).
     */
    void setTranslate(const H2DE_Translate& translate);
    /**
     * @brief Set the scale factors instantly.
     * @param scale New scale (x, y).
     */
    void setScale(const H2DE_Scale& scale);
    /**
     * @brief Set the rotation instantly.
     * @param rotation New rotation angle (float).
     */
    void setRotation(float rotation);
    /**
     * @brief Set the pivot point instantly.
     * @param pivot New pivot point.
     */
    void setPivot(const H2DE_Pivot& pivot);
    /**
     * @brief Set the scale mode instantly.
     * @param scaleMode New scale mode enum.
     */
    void setScaleMode(H2DE_ScaleMode scaleMode);
    /**
     * @brief Set the blend mode instantly.
     * @param blendMode New blend mode enum.
     */
    void setBlendMode(H2DE_BlendMode blendMode);
    /**
     * @brief Set the drawing order index instantly.
     * @param index New index (higher = drawn on top).
     */
    void setIndex(int index);

    /**
     * @brief Animate the translation over time with easing and callback on completion.
     * 
     * @param translate Target translation.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the scale over time with easing and callback on completion.
     * 
     * @param scale Target scale.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setScale(const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the rotation over time with easing and completion callback.
     * 
     * @param rotation Target rotation angle.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setRotation(float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    friend class H2DE_Renderer;
    friend class H2DE_Object;
    friend class H2DE_TextObject;
    friend class H2DE_Geometry;

protected:
    H2DE_Engine* engine;
    H2DE_Object* object;

    H2DE_Surface(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData) noexcept;
    virtual ~H2DE_Surface() = default;

private:
    H2DE_SurfaceData surfaceData;

    bool hidden = false;
    bool fromText = false;

    virtual std::string getTextureName() const = 0;
    virtual H2DE_ColorRGB getColor() const noexcept = 0;
    virtual std::optional<H2DE_PixelRect> getSrcRect() const noexcept = 0;
    virtual bool isVisible() const noexcept = 0;
};

#include <H2DE/surfaces/H2DE_texture.h>
#include <H2DE/surfaces/H2DE_sprite.h>
#include <H2DE/surfaces/H2DE_color.h>
#include <H2DE/surfaces/H2DE_border.h>

#endif

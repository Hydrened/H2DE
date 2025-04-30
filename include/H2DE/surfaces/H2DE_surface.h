#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

/**
 * @file H2DE_surface.h
 * @brief Defines the base class `H2DE_Surface` for managing surfaces within the engine.
 * 
 * The `H2DE_Surface` class is the parent class for both `H2DE_Texture` and `H2DE_Sprite`.
 * It provides methods for handling surface properties such as texture, color, scaling, rotation, and flipping.
 * These methods are used by the engine to manipulate and render surfaces during gameplay.
 */

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

/**
 * @brief Base class representing a surface in the engine.
 * 
 * The `H2DE_Surface` class serves as a base class for `H2DE_Texture` and `H2DE_Sprite`.
 * It contains methods to manipulate surfaces such as texture name, color, scaling, rotation, and flipping.
 */
class H2DE_Surface {
private:
    H2DE_SurfaceData sd;

    virtual std::optional<H2DE_AbsRect> getSrcRect() const = 0;

protected:
    H2DE_Engine* engine;

    H2DE_Surface(H2DE_Engine* engine, const H2DE_SurfaceData& sd);
    virtual ~H2DE_Surface();

public:
    /**
     * @brief Sets the texture name of the surface.
     * 
     * This function sets the texture name associated with the surface.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param textureName The name of the texture to set.
     */
    friend void H2DE_SetSurfaceTextureName(H2DE_Surface* surface, const std::string& textureName);
    /**
     * @brief Sets the color of the surface.
     * 
     * This function sets the color of the surface.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param color The color to set the surface to.
     */
    friend void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color);
    /**
     * @brief Sets the color of the surface with animation.
     * 
     * This function sets the color of the surface with optional animation.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param color The color to set the surface to.
     * @param duration The duration of the animation.
     * @param easing The easing function for the animation.
     * @param pauseSensitive Whether the animation should pause when the game is paused.
     */
    friend void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    /**
     * @brief Sets the scaling mode of the surface.
     * 
     * This function sets how the surface should scale.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param scaleMode The scale mode to set (e.g., stretch, fit).
     */
    friend void H2DE_SetSurfaceScaleMode(H2DE_Surface* surface, H2DE_ScaleMode scaleMode);
    /**
     * @brief Sets the rotation of the surface.
     * 
     * This function sets the rotation of the surface.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param rotation The rotation angle to apply (in degrees).
     */
    friend void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation);
    /**
     * @brief Sets the rotation of the surface with animation.
     * 
     * This function sets the rotation of the surface with optional animation.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param rotation The rotation angle to apply (in degrees).
     * @param duration The duration of the animation.
     * @param easing The easing function for the animation.
     * @param pauseSensitive Whether the animation should pause when the game is paused.
     */
    friend void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    /**
     * @brief Sets the pivot point of the surface.
     * 
     * This function sets the pivot point for the surface.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param pivot The desired pivot point as an (x, y) position.
     */
    friend void H2DE_SetSurfacePivot(H2DE_Surface* surface, const H2DE_LevelPos& pivot);
    /**
     * @brief Sets the flip state of the surface.
     * 
     * This function flips the surface horizontally or vertically.
     * 
     * @param surface A pointer to the `H2DE_Surface`.
     * @param flip The desired flip state (horizontal, vertical, or none).
     */
    friend void H2DE_SetSurfaceFlip(H2DE_Surface* surface, H2DE_Flip flip);

    friend class H2DE_Engine;
    friend class H2DE_BarObject;
    friend class H2DE_BasicObject;
    friend class H2DE_ButtonObject;
    friend class H2DE_TextObject;
    friend class H2DE_Object;
};

#include <H2DE/surfaces/H2DE_texture.h>
#include <H2DE/surfaces/H2DE_sprite.h>

#endif

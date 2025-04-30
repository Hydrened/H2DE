#ifndef H2DE_SPRITE_H
#define H2DE_SPRITE_H

/**
 * @file H2DE_sprite.h
 * @brief Defines the `H2DE_Sprite` class for managing sprite surfaces in the engine.
 * 
 * The `H2DE_Sprite` class is a derived class of `H2DE_Surface`, specifically handling sprite surfaces.
 * It provides functionality to manipulate sprite-specific properties like animation frames and delays.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @brief Class representing a sprite surface.
 * 
 * The `H2DE_Sprite` class inherits from `H2DE_Surface` and is used for handling sprite-based surfaces,
 * supporting frame-based animation and various properties like size, spacing, and delay.
 */
class H2DE_Sprite : public H2DE_Surface {
private:
    H2DE_SpriteData spd;

    int delayId = -1;
    unsigned int currentFrame = 0;

    H2DE_Sprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& spd);
    ~H2DE_Sprite() override;

    void initDelay();
    void nextFrame();
    std::optional<H2DE_AbsRect> getSrcRect() const override;
        
public:
    /**
     * @brief Creates a new sprite surface.
     * 
     * This function creates a new `H2DE_Sprite` with the provided engine, surface data, and sprite data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param surfaceData The surface data to initialize the surface with.
     * @param spriteData The sprite data to initialize the sprite with.
     * @return A pointer to the created `H2DE_Sprite`.
     */
    friend H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
    /**
     * @brief Sets the starting position of the sprite.
     * 
     * This function sets the initial position of the sprite.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param startingPos The starting position to set.
     */
    friend void H2DE_SetSpriteStartingPos(H2DE_Surface* sprite, const H2DE_AbsPos& startingPos);
    /**
     * @brief Sets the size of the sprite.
     * 
     * This function sets the size of the sprite.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param size The size to set.
     */
    friend void H2DE_SetSpriteSize(H2DE_Surface* sprite, const H2DE_AbsSize& size);
    /**
     * @brief Sets the spacing between sprite frames.
     * 
     * This function sets the spacing between the frames of the sprite.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param spacing The spacing to set.
     */
    friend void H2DE_SetSpriteSpacing(H2DE_Surface* sprite, int spacing);
    /**
     * @brief Sets the number of frames in the sprite animation.
     * 
     * This function sets the total number of frames in the sprite's animation.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param nbFrame The number of frames to set.
     */
    friend void H2DE_SetSpriteNbFrame(H2DE_Surface* sprite, unsigned int nbFrame);
    /**
     * @brief Sets the delay between frames in the sprite animation.
     * 
     * This function sets the delay time between frames in the sprite's animation.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param delay The delay to set.
     */
    friend void H2DE_SetSpriteDelay(H2DE_Surface* sprite, unsigned int delay);
    /**
     * @brief Sets whether the sprite's animation should be sensitive to pauses.
     * 
     * This function sets whether the sprite's animation should pause during engine pauses.
     * 
     * @param sprite A pointer to the `H2DE_Surface` (which is a sprite).
     * @param pauseSensitive True if the animation should be paused, false otherwise.
     */
    friend void H2DE_SetSpritePauseSensitive(H2DE_Surface* sprite, bool pauseSensitive);
};

H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
void H2DE_SetSpriteStartingPos(H2DE_Surface* sprite, const H2DE_AbsPos& startingPos);
void H2DE_SetSpriteSize(H2DE_Surface* sprite, const H2DE_AbsSize& size);
void H2DE_SetSpriteSpacing(H2DE_Surface* sprite, int spacing);
void H2DE_SetSpriteNbFrame(H2DE_Surface* sprite, unsigned int nbFrame);
void H2DE_SetSpriteDelay(H2DE_Surface* sprite, unsigned int delay);
void H2DE_SetSpritePauseSensitive(H2DE_Surface* sprite, bool pauseSensitive);

#endif

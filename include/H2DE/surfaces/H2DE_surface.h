#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Surface {
private:
    H2DE_SurfaceData sd;

    // float percentage = 100.0f;

    virtual std::optional<H2DE_AbsRect> getSrcRect() const = 0;

protected:
    H2DE_Engine* engine;

    H2DE_Surface(H2DE_Engine* engine, const H2DE_SurfaceData& sd);
    virtual ~H2DE_Surface();

public:
    friend void H2DE_SetSurfaceTextureName(H2DE_Surface* surface, const std::string& textureName);
    friend void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color);
    friend void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetSurfaceScaleMode(H2DE_Surface* surface, H2DE_ScaleMode scaleMode);
    friend void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation);
    friend void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetSurfacePivot(H2DE_Surface* surface, const H2DE_LevelPos& pivot);
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

H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect);

H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
void H2DE_SetSpriteStartingPos(H2DE_Surface* sprite, const H2DE_AbsPos& startingPos);
void H2DE_SetSpriteSize(H2DE_Surface* sprite, const H2DE_AbsSize& size);
void H2DE_SetSpriteSpacing(H2DE_Surface* sprite, int spacing);
void H2DE_SetSpriteNbFrame(H2DE_Surface* sprite, unsigned int nbFrame);
void H2DE_SetSpriteDelay(H2DE_Surface* sprite, unsigned int delay);
void H2DE_SetSpritePauseSensitive(H2DE_Surface* sprite, bool pauseSensitive);

#endif

#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Surface {
private:
    H2DE_SurfaceData sd;

    float percentage = 100.0f;

    virtual std::optional<H2DE_AbsRect> getSrcRect() const = 0;

protected:
    H2DE_Engine* engine;

    H2DE_Surface(H2DE_Engine* engine, const H2DE_SurfaceData& sd);
    virtual ~H2DE_Surface();

public:
    friend void H2DE_SetSurfaceTextureName(H2DE_Surface* surface, const std::string& textureName);
    friend void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color);
    friend void H2DE_SetSurfaceScaleMode(H2DE_Surface* surface, H2DE_ScaleMode scaleMode);
    // friend void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation);
    // friend void H2DE_SetSurfacePivot(H2DE_Surface* surface, const H2DE_LevelPos& pivot);
    // friend void H2DE_SetSurfaceFlip(H2DE_Surface* surface, H2DE_Flip flip);

    friend class H2DE_Engine;
    friend class H2DE_BarObject;
    friend class H2DE_BasicObject;
    friend class H2DE_ButtonObject;
    friend class H2DE_TextObject;
};



class H2DE_Texture : public H2DE_Surface {
private:
    H2DE_TextureData ted;

    H2DE_Texture(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_TextureData& ted);
    ~H2DE_Texture() override;

    std::optional<H2DE_AbsRect> getSrcRect() const override;
    
public:
    friend H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
    friend void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect);
};

H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect);



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
    friend H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
    friend void H2DE_SetSpriteStartingPos(H2DE_Surface* sprite, const H2DE_AbsPos& startingPos);
    friend void H2DE_SetSpriteSize(H2DE_Surface* sprite, const H2DE_AbsSize& size);
    friend void H2DE_SetSpriteSpacing(H2DE_Surface* sprite, int spacing);
    friend void H2DE_SetSpriteNbFrame(H2DE_Surface* sprite, unsigned int nbFrame);
    friend void H2DE_SetSpriteDelay(H2DE_Surface* sprite, unsigned int delay);
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

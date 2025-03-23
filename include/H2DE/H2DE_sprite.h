#ifndef H2DE_SPRITE_H
#define H2DE_SPRITE_H

#include <H2DE/H2DE_engine.h>

class H2DE_Sprite : public H2DE_Surface {
private:
    H2DE_SpriteData spd;

    unsigned int currentFrame = 0;

    H2DE_Sprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& spd);
    ~H2DE_Sprite();

    void nextFrame();
    std::optional<H2DE_AbsRect> getSrcRect() const override;
        
public:
    friend H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
};

#endif

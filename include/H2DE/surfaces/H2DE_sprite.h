#ifndef H2DE_SPRITE_H
#define H2DE_SPRITE_H

#include <H2DE/surfaces/H2DE_surface.h>

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

#endif

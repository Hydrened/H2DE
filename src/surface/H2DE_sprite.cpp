#include "H2DE/H2DE_sprite.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& s) : H2DE_Surface(engine, sd), spd(s) {
    H2DE_Delay(engine, spd.delay, [this]() {
        nextFrame();
    }, -1, spd.pauseSensitive);
}

H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& spd) {
    return new H2DE_Sprite(engine, sd, spd);
}

// CLEANUP
H2DE_Sprite::~H2DE_Sprite() {
    
}

// EVENTS
void H2DE_Sprite::nextFrame() {
    currentFrame++;

    if (currentFrame >= spd.nbFrame) {
        currentFrame = 0;
    }
}

// GETTER
std::optional<H2DE_AbsRect> H2DE_Sprite::getSrcRect() const {
    H2DE_AbsRect srcRect = H2DE_AbsRect();

    srcRect.x = currentFrame * (spd.size.x + spd.spacing);
    srcRect.y = 0;
    srcRect.addSize(spd.size);

    return srcRect;
}

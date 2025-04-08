#include "H2DE/surfaces/H2DE_sprite.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& s) : H2DE_Surface(engine, sd), spd(s) {
    initDelay();
}

H2DE_Surface* H2DE_CreateSprite(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_SpriteData& spd) {
    return new H2DE_Sprite(engine, sd, spd);
}

void H2DE_Sprite::initDelay() {
    if (delayId != -1) {
        H2DE_StopDelay(engine, delayId, false);
    }

    delayId = H2DE_Delay(engine, spd.delay, [this]() {
        nextFrame();
    }, spd.pauseSensitive);
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

    srcRect.x = currentFrame * (spd.size.x + spd.spacing) + spd.startingPos.x;
    srcRect.y = spd.startingPos.y;
    srcRect.addSize(spd.size);

    return srcRect;
}

// SETTER
void H2DE_SetSpriteStartingPos(H2DE_Surface* sprite, const H2DE_AbsPos& startingPos) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.startingPos = startingPos;
    }
}

void H2DE_SetSpriteSize(H2DE_Surface* sprite, const H2DE_AbsSize& size) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.size = size;
    }
}

void H2DE_SetSpriteSpacing(H2DE_Surface* sprite, int spacing) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.spacing = spacing;
    }
}

void H2DE_SetSpriteNbFrame(H2DE_Surface* sprite, unsigned int nbFrame) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.nbFrame = nbFrame;
    }
}

void H2DE_SetSpriteDelay(H2DE_Surface* sprite, unsigned int delay) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.delay = delay;
        spr->initDelay();
    }
}

void H2DE_SetSpritePauseSensitive(H2DE_Surface* sprite, bool pauseSensitive) {
    H2DE_Sprite* spr = dynamic_cast<H2DE_Sprite*>(sprite);
    
    if (spr) {
        spr->spd.pauseSensitive = pauseSensitive;
    }
}

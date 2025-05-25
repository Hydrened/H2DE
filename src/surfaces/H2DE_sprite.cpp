#include "H2DE/surfaces/H2DE_sprite.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_SpriteData& td) : H2DE_Surface(e, o, sd), spriteData(td) {
    initDelay();
}

void H2DE_Sprite::initDelay() {
    if (delayId != -1) {
        engine->stopDelay(delayId, false);
    }

    delayId = engine->delay(spriteData.delay, [this]() {
        nextFrame();
    }, spriteData.pauseSensitive);
}

// CLEANUP
H2DE_Sprite::~H2DE_Sprite() {
    if (delayId != -1) {
        engine->stopDelay(delayId, false);
    }
}

// ACTIONS
void H2DE_Sprite::nextFrame() {
    currentFrame++;

    if (currentFrame >= spriteData.nbFrame) {
        currentFrame = 0;
    }
}

// GETTER
std::optional<H2DE_PixelRect> H2DE_Sprite::getSrcRect() const {
    H2DE_PixelRect srcRect = H2DE_PixelRect();

    srcRect.x = currentFrame * (spriteData.size.x + spriteData.spacing) + spriteData.startingPos.x;
    srcRect.y = spriteData.startingPos.y;
    srcRect.addScale(spriteData.size);

    return srcRect;
}

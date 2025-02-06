#include "H2DE/H2DE_surface.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* e, H2DE_TextureData td, H2DE_SpriteData sd) : H2DE_Surface(e, td), spriteData(sd), frameDelay(H2DE_GetSteps(engine, sd.delay)), animation(sd.defaultAnimationName) {
    setCurrentTexture();
}

H2DE_Sprite* H2DE_CreateSprite(H2DE_Engine* engine, H2DE_TextureData textureData, H2DE_SpriteData spriteData) {
    return new H2DE_Sprite(engine, textureData, spriteData);    
}

// CLEANUP
H2DE_Sprite::~H2DE_Sprite() {

}

// UPDATE
void H2DE_Sprite::update() {
    frames++;
    if (frames < frameDelay) return;

    frames = 0;
    currentSpriteFrame += 1;
    if (currentSpriteFrame > spriteData.nbFrame) currentSpriteFrame = 1;

    setCurrentTexture();
}

// SETTER
void H2DE_Sprite::setCurrentTexture() {
    size_t extentionIndex = textureData.name.find_last_of('.');
    std::string extention = textureData.name.substr(extentionIndex);
    currentTexture = textureData.name.substr(0, extentionIndex) + '-' + animation + '-' + std::to_string(currentSpriteFrame) + extention;
}

void H2DE_SetSpriteAnimation(H2DE_Sprite* sprite, std::string animation) {
    sprite->animation = animation;
}

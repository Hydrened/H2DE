#include "H2DE/surfaces/H2DE_sprite.h"

#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_SpriteData& td) : H2DE_Surface(e, o, sd), spriteData(td) {
    initDelay();
}

void H2DE_Sprite::initDelay() {
    delay = engine->createTimeline(spriteData.delay, H2DE_EASING_LINEAR, nullptr, [this]() {
        nextFrame();
    }, H2DE_INFINITE_LOOP, spriteData.pauseSensitive);
}

// CLEANUP
H2DE_Sprite::~H2DE_Sprite() {
    if (delay != nullptr) {
        delay->stop(false);
        delay = nullptr;
    }
}

// ACTIONS
void H2DE_Sprite::nextFrame() noexcept {
    currentFrame++;

    if (currentFrame >= spriteData.nbFrame) {
        currentFrame = 0;
    }
}

// GETTER
std::optional<H2DE_PixelRect> H2DE_Sprite::getSrcRect() const noexcept {
    H2DE_PixelPos res = H2DE_PixelPos();

    res.x = currentFrame * (spriteData.size.x + spriteData.spacing) + spriteData.startingPos.x;
    res.y = spriteData.startingPos.y;

    return res.makeRect(spriteData.size);
}

// SETTER

// -- no lerp
void H2DE_Sprite::setTextureName(const std::string& textureName) {
    spriteData.textureName = textureName;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setColor(const H2DE_ColorRGB& color) {
    spriteData.color = color;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setStartingPos(const H2DE_PixelPos& startingPos) {
    spriteData.startingPos = startingPos;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setSize(const H2DE_PixelSize& size) {
    spriteData.size = size;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setSpacing(int spacing) {
    spriteData.spacing = spacing;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setNbFrame(uint16_t nbFrame) {
    spriteData.nbFrame = nbFrame;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setDelay(uint32_t delay) {
    spriteData.delay = delay;
    object->refreshSurfaceBuffers();
}

void H2DE_Sprite::setPauseSensitive(bool pauseSensitive) {
    spriteData.pauseSensitive = pauseSensitive;
    object->refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_Sprite::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, spriteData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

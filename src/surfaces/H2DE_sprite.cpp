#include "H2DE/surfaces/H2DE_sprite.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Sprite::H2DE_Sprite(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_SpriteData& td) : H2DE_Surface(e, o, sd), _spriteData(td) {
    _initDelay();
}

void H2DE_Sprite::_initDelay() {
    _delay = _engine->createTimeline(_spriteData.delay, H2DE_EASING_LINEAR, nullptr, [this]() {
        _nextFrame();
    }, H2DE_INFINITE_LOOP, _spriteData.pauseSensitive);
}

// CLEANUP
H2DE_Sprite::~H2DE_Sprite() {
    if (_delay != nullptr) {
        _delay->stop(false);
        _delay = nullptr;
    }
}

// ACTIONS
void H2DE_Sprite::_nextFrame() noexcept {
    _currentFrame++;

    if (_currentFrame >= _spriteData.nbFrame) {
        _currentFrame = 0;
    }
}

// GETTER
std::optional<H2DE_PixelRect> H2DE_Sprite::getSrcRect() const noexcept {
    H2DE_PixelPos res = H2DE_PixelPos();

    res.x = _currentFrame * (_spriteData.size.x + _spriteData.spacing) + _spriteData.startingPos.x;
    res.y = _spriteData.startingPos.y;

    return res.makeRect(_spriteData.size);
}

// SETTER

// -- no lerp
void H2DE_Sprite::setTextureName(const std::string& textureName) {
    _spriteData.textureName = textureName;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setColor(const H2DE_ColorRGB& color) {
    _spriteData.color = color;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setStartingPos(const H2DE_PixelPos& startingPos) {
    _spriteData.startingPos = startingPos;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setSize(const H2DE_PixelSize& size) {
    _spriteData.size = size;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setSpacing(int spacing) {
    _spriteData.spacing = spacing;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setNbFrame(uint16_t nbFrame) {
    _spriteData.nbFrame = nbFrame;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setDelay(uint32_t delay) {
    _spriteData.delay = delay;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Sprite::setPauseSensitive(bool pauseSensitive) {
    _spriteData.pauseSensitive = pauseSensitive;
    _object->_refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_Sprite::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(_engine, _spriteData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

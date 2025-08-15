#include "H2DE/surfaces/H2DE_texture.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#include "H2DE/engine/H2DE_error.h"

// SETTER

// -- no lerp
void H2DE_Texture::setColor(const H2DE_ColorRGB& color) {
    _textureData.color = color;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Texture::setSrcRect(const std::optional<H2DE_PixelRect>& srcRect) {
    _textureData.srcRect = srcRect;
    _object->_refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_Texture::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(_engine, _textureData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

#include "H2DE/surfaces/H2DE_texture.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Texture::H2DE_Texture(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_TextureData& td) : H2DE_Surface(e, o, sd), textureData(td) {

}

// GETTER
bool H2DE_Texture::isVisible() const {
    bool surfaceIsNotHidden = !(isHidden());
    bool colorIsVisible = (textureData.color.isVisible());
    
    return (surfaceIsNotHidden && colorIsVisible);
}

// SETTER

// -- no lerp
void H2DE_Texture::setColor(const H2DE_ColorRGB& color) {
    textureData.color = color;
    object->refreshSurfaceBuffers();
}

void H2DE_Texture::setSrcRect(const std::optional<H2DE_PixelRect>& srcRect) {
    textureData.srcRect = srcRect;
    object->refreshSurfaceBuffers();
}

// -- lerp
H2DE_TimelineID H2DE_Texture::setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, textureData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

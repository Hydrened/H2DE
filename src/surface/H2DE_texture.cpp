#include "H2DE/surfaces/H2DE_texture.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Texture::H2DE_Texture(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_TextureData& t) : H2DE_Surface(engine, sd), ted(t) {

}

H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_TextureData& ted) {
    H2DE_Error::checkEngine(engine);
    return new H2DE_Texture(engine, sd, ted);
}

// CLEANUP
H2DE_Texture::~H2DE_Texture() {
    
}

// GETTER
std::optional<H2DE_AbsRect> H2DE_Texture::getSrcRect() const {
    return ted.srcRect;
}

// SETTER
void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect) {
    H2DE_Error::checkSurface(texture);

    H2DE_Texture* tex = dynamic_cast<H2DE_Texture*>(texture);
    
    if (tex) {
        tex->ted.srcRect = srcRect;
    }
}

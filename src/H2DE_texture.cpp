#include "H2DE/H2DE_surface.h"

// INIT
H2DE_Texture::H2DE_Texture(H2DE_Engine* e, H2DE_TextureData td) : H2DE_Surface(e, td) {
    currentTexture = td.name;
}

H2DE_Texture* H2DE_CreateTexture(H2DE_Engine* engine, H2DE_TextureData textureData) {
    return new H2DE_Texture(engine, textureData);
}

// CLEANUP
H2DE_Texture::~H2DE_Texture() {

}

// UPDATE
void H2DE_Texture::update() {
    
}

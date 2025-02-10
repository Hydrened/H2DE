#include "H2DE/H2DE_surface.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, H2DE_TextureData td) : engine(e), textureData(td) {

}

// CLEANUP
H2DE_Surface::~H2DE_Surface() {

}

// GETTER
std::string H2DE_Surface::get() const {
    return currentTexture;
}

H2DE_TextureData* H2DE_Surface::getData() {
    return &textureData;
}

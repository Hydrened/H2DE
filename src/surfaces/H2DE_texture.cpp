#include "H2DE/surfaces/H2DE_texture.h"

// INIT
H2DE_Texture::H2DE_Texture(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_TextureData& td) : H2DE_Surface(e, o, sd), textureData(td) {

}

// CLEANUP
H2DE_Texture::~H2DE_Texture() {

}

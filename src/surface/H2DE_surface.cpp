#include "H2DE/H2DE_surface.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, const H2DE_SurfaceData& s) : engine(e), sd(s) {

}

// CLEANUP
H2DE_Surface::~H2DE_Surface() {

}

// SETTER
void H2DE_SetSurfaceTextureName(H2DE_Surface* surface, const std::string& textureName) {
    surface->sd.textureName = textureName;
}

void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color) {
    surface->sd.color = color;
}

void H2DE_SetSurfaceScaleMode(H2DE_Surface* surface, H2DE_ScaleMode scaleMode) {
    surface->sd.scaleMode = scaleMode;
}

// void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation) {
//     surface->sd.transform.rotation = rotation;
// }

// void H2DE_SetSurfacePivot(H2DE_Surface* surface, const H2DE_LevelPos& pivot) {
//     surface->sd.transform.pivot = pivot;
// }

// void H2DE_SetSurfaceFlip(H2DE_Surface* surface, H2DE_Flip flip) {
//     surface->sd.transform.flip = flip;
// }

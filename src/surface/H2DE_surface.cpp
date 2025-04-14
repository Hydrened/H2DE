#include "H2DE/surfaces/H2DE_surface.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, const H2DE_SurfaceData& s) : engine(e), sd(s) {

}

// CLEANUP
H2DE_Surface::~H2DE_Surface() {

}

// SETTER
void H2DE_SetSurfaceTextureName(H2DE_Surface* surface, const std::string& textureName) {
    H2DE_Error::checkSurface(surface);
    surface->sd.textureName = textureName;
}

void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color) {
    H2DE_Error::checkSurface(surface);
    surface->sd.color = color;
}

void H2DE_SetSurfaceColor(H2DE_Surface* surface, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkSurface(surface);

    const H2DE_ColorRGB defaultColor = surface->sd.color;
    
    H2DE_CreateTimeline(surface->engine, duration, easing, [surface, defaultColor, color](float blend) {
        H2DE_ColorRGB interpolatedColor = H2DE_ColorRGB();
        interpolatedColor.r = static_cast<Uint8>(defaultColor.r + (color.r - defaultColor.r) * blend);
        interpolatedColor.g = static_cast<Uint8>(defaultColor.g + (color.g - defaultColor.g) * blend);
        interpolatedColor.b = static_cast<Uint8>(defaultColor.b + (color.b - defaultColor.b) * blend);
        interpolatedColor.a = static_cast<Uint8>(defaultColor.a + (color.a - defaultColor.a) * blend);

        H2DE_SetSurfaceColor(surface, interpolatedColor);
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetSurfaceScaleMode(H2DE_Surface* surface, H2DE_ScaleMode scaleMode) {
    H2DE_Error::checkSurface(surface);
    surface->sd.scaleMode = scaleMode;
}

void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation) {
    H2DE_Error::checkSurface(surface);
    surface->sd.rotation = rotation;
}

void H2DE_SetSurfaceRotation(H2DE_Surface* surface, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkSurface(surface);

    const float defaultRotation = surface->sd.rotation;
    
    H2DE_CreateTimeline(surface->engine, duration, easing, [surface, defaultRotation, rotation](float blend) {
        const float r = defaultRotation + (rotation - defaultRotation) * blend;
        H2DE_SetSurfaceRotation(surface, r);
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetSurfacePivot(H2DE_Surface* surface, const H2DE_LevelPos& pivot) {
    H2DE_Error::checkSurface(surface);
    surface->sd.pivot = pivot;
}

void H2DE_SetSurfaceFlip(H2DE_Surface* surface, H2DE_Flip flip) {
    H2DE_Error::checkSurface(surface);
    surface->sd.flip = flip;
}

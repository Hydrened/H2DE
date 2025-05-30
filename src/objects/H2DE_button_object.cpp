#include "H2DE/objects/H2DE_button_object.h"
#undef max

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) : H2DE_Object(e, od), buttonObjectData(bod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    H2DE_Object::destroySurfaces(surfaces);
}

void H2DE_ButtonObject::refreshSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::getSortedSurfaces(surfaces);

    surfaceBuffers.clear();
    surfaceBuffers.reserve(sortedSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
}

// ACTIONS
void H2DE_ButtonObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxSurfaceRadius = getMaxSurfaceRadius(surfaces);
    
    maxRadius = std::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_ButtonObject::mouseDown() {
    if (buttonObjectData.onMouseDown) {
        buttonObjectData.onMouseDown(this);
    }
}

void H2DE_ButtonObject::mouseUp() {
    if (buttonObjectData.onMouseUp) {
        buttonObjectData.onMouseUp(this);
    }
}

void H2DE_ButtonObject::mouseHover() {
    if (buttonObjectData.onHover) {
        buttonObjectData.onHover(this);
    }
}

void H2DE_ButtonObject::mouseBlur() {
    if (buttonObjectData.onBlur) {
        buttonObjectData.onBlur(this);
    }
}

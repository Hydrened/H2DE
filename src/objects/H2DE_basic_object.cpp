#include "H2DE/objects/H2DE_basic_object.h"
#undef max

// INIT
H2DE_BasicObject::H2DE_BasicObject(H2DE_Engine* e, const H2DE_ObjectData& od) : H2DE_Object(e, od) {
    updateSurfaceBuffers();
    updateMaxRadius();
}

// CLEANUP
H2DE_BasicObject::~H2DE_BasicObject() {
    H2DE_Object::destroySurfaces(surfaces);
}

// ACTIONS
void H2DE_BasicObject::updateSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::getSortedSurfaces(surfaces);

    surfaceBuffers.reserve(sortedSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
}

void H2DE_BasicObject::updateMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxSurfaceRadius = getMaxSurfaceRadius(surfaces);
    
    maxRadius = std::max(maxHitboxesRadius, maxSurfaceRadius);
}

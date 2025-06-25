#include "H2DE/objects/H2DE_basic_object.h"
#undef max

// INIT
H2DE_BasicObject::H2DE_BasicObject(H2DE_Engine* e, const H2DE_ObjectData& od) : H2DE_Object(e, od) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_BasicObject::~H2DE_BasicObject() {
    H2DE_Object::destroySurfaces(surfaces);
}

// ACTIONS
void H2DE_BasicObject::refreshSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::getSortedSurfaces(surfaces);

    surfaceBuffers.clear();
    surfaceBuffers.reserve(sortedSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    rescaleSurfaceBuffers();
}

void H2DE_BasicObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxSurfaceRadius = getMaxSurfaceRadius(surfaces);
    
    maxRadius = std::max(maxHitboxesRadius, maxSurfaceRadius);
}

#include "H2DE/objects/H2DE_single_surface_object.h"

// INIT
H2DE_SingleSurfaceObject::H2DE_SingleSurfaceObject(H2DE_Engine* e, const H2DE_ObjectData& od) : H2DE_Object(e, od) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_SingleSurfaceObject::~H2DE_SingleSurfaceObject() {
    H2DE_Object::_destroySurfaces(_surfaces);
}

// ACTIONS
void H2DE_SingleSurfaceObject::_refreshSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::_getSortedSurfaces(_surfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    _rescaleSurfaceBuffers();
}

void H2DE_SingleSurfaceObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius(_surfaces);
    
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

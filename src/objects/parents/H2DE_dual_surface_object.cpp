#include "H2DE/objects/parents/H2DE_dual_surface_object.h"

// INIT
H2DE_DualSurfaceObject::H2DE_DualSurfaceObject(H2DE_Engine* e, const H2DE_ObjectData& od) : H2DE_Object(e, od) {
    _refreshSurfaceBuffers();
}

// CLEANUP
H2DE_DualSurfaceObject::~H2DE_DualSurfaceObject() {
    H2DE_Object::_destroySurfaces(_fillSurfaces);
    H2DE_Object::_destroySurfaces(_backgroundSurfaces);
}

// ACTIONS
void H2DE_DualSurfaceObject::_refreshSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedBackgroundSurfaces = H2DE_Object::_getSortedSurfaces(_backgroundSurfaces);
    const std::vector<H2DE_Surface*> sortedFillSurfaces = H2DE_Object::_getSortedSurfaces(_fillSurfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedBackgroundSurfaces.size() + sortedFillSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedBackgroundSurfaces.begin(), sortedBackgroundSurfaces.end());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedFillSurfaces.begin(), sortedFillSurfaces.end());
    _surfaceBuffers = H2DE_Object::_getSortedSurfaces(_surfaceBuffers);

    _rescaleSurfaceBuffers();
}

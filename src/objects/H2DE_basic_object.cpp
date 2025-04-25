#include "H2DE/objects/H2DE_basic_object.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_BasicObject::H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData b) : H2DE_Object(engine, od), bod(b) {
    resetSurfaceBuffers();
}

H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BasicObjectData& bod) {
    H2DE_Error::checkEngine(engine);
    
    H2DE_BasicObject* object = new H2DE_BasicObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BasicObject::~H2DE_BasicObject() {
    H2DE_Object::destroySurfaces(bod.surfaces);
}

// SURFACES
void H2DE_AddSurfaceToBasicObject(H2DE_BasicObject* basicObject, H2DE_Surface* surface, const std::string& name) {
    H2DE_Error::checkObject(basicObject);
    
    basicObject->addSurface(basicObject->bod.surfaces, surface, name);
    basicObject->resetSurfaceBuffers();
}

void H2DE_RemoveSurfaceFromBasicObject(H2DE_BasicObject* basicObject, const std::string& name) {
    H2DE_Error::checkObject(basicObject);
    
    basicObject->removeSurface(basicObject->bod.surfaces, name);
    basicObject->resetSurfaceBuffers();
}

void H2DE_BasicObject::resetSurfaceBuffers() {
    clearSurfaceBuffers();
    surfaceBuffers.reserve(bod.surfaces.size());

    for (H2DE_Surface* surface : H2DE_Object::getSortedSurfaces(bod.surfaces)) {
        H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();
        surfaceOffset = surfaceOffset.rotate(od.pivot, od.rotation);

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        surfaceBuffers.push_back(buffer);
    }
}

// GETTER
H2DE_Surface* H2DE_GetBasicObjectSurface(const H2DE_BasicObject* basicObject, const std::string& name) {
    H2DE_Error::checkObject(basicObject);
    return H2DE_Object::getSurface(basicObject->bod.surfaces, name);
}

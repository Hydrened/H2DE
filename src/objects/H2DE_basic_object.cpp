#include "H2DE/H2DE_object.h"

// INIT
H2DE_BasicObject::H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData b) : H2DE_Object(engine, od), bod(b) {

}

H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BasicObjectData& bod) {
    H2DE_BasicObject* object = new H2DE_BasicObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BasicObject::~H2DE_BasicObject() {
    if (bod.surface) {
        delete bod.surface;
    }
}

// UPDATE
void H2DE_BasicObject::update() {
    
}

// GETTER
std::vector<H2DE_SurfaceBuffer> H2DE_BasicObject::getSurfaceBuffers() const {
    H2DE_LevelPos surfaceOffset = { 0.0f, 0.0f };
    surfaceOffset = surfaceOffset.rotate(od.pivot, od.rotation);

    H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
    buffer.surface = bod.surface;
    buffer.offset = surfaceOffset;
    buffer.size = od.size;
    buffer.rotation = od.rotation;
    buffer.flip = od.flip;
    return { buffer };
}

H2DE_Surface* H2DE_GetBasicObjectSurface(const H2DE_BasicObject* basicObject) {
    return basicObject->bod.surface;
}

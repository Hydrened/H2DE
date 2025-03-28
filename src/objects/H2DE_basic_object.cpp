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
std::vector<H2DE_SurfaceBuffer> H2DE_BasicObject::getSurfaces() const {
    H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
    buffer.surface = bod.surface;
    buffer.offset = { 0.0f, 0.0f };
    buffer.size = od.size;
    return { buffer };
}

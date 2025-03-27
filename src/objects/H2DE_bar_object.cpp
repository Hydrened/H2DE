#include "H2DE/H2DE_object.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData b) : H2DE_Object(engine, od), bod(b), percentage(b.defaultPercentage) {

}

H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) {
    H2DE_BarObject* object = new H2DE_BarObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    if (bod.front) {
        delete bod.front;
    }
    if (bod.background) {
        delete bod.background;
    }
}

// UPDATE
void H2DE_BarObject::update() {
    if (bod.front) {
        bod.front->percentage = percentage;
    }
}

// GETTER
std::vector<H2DE_SurfaceBuffer> H2DE_BarObject::getSurfaces() const {
    H2DE_SurfaceBuffer backgroundBuffer = H2DE_SurfaceBuffer();
    backgroundBuffer.surface = bod.background;
    backgroundBuffer.offset = { 0.0f, 0.0f };
    backgroundBuffer.size = od.size;

    H2DE_SurfaceBuffer frontBuffer = H2DE_SurfaceBuffer();
    frontBuffer.surface = bod.front;
    frontBuffer.offset = { 0.0f, 0.0f };
    frontBuffer.size = od.size;
    frontBuffer.size.x *= percentage;

    return { backgroundBuffer, frontBuffer };
}

H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar) {
    return bar->bod.front;
}

H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar) {
    return bar->bod.background;
}

// SETTER
void H2DE_SetBarPercentage(H2DE_BarObject* bar, float percentage) {
    bar->percentage = std::clamp(percentage, 0.0f, 100.0f);
}

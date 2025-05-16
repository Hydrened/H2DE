#include "H2DE/objects/H2DE_bar_object.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData b) : H2DE_Object(engine, od), bod(b), value(b.defaultValue) {
    refreshPercentage();
}

H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) {
    H2DE_Error::checkEngine(engine);
    
    H2DE_BarObject* object = new H2DE_BarObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    H2DE_Object::destroySurfaces(bod.front);
    H2DE_Object::destroySurfaces(bod.background);
}

// SURFACES
void H2DE_AddSurfaceToBarObjectFront(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name) {
    H2DE_Error::checkObject(bar);

    bar->addSurface(bar->bod.front, surface, name);
    bar->resetSurfaceBuffers();
}

void H2DE_AddSurfaceToBarObjectBackground(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name) {
    H2DE_Error::checkObject(bar);

    bar->addSurface(bar->bod.background, surface, name);
    bar->resetSurfaceBuffers();
}

void H2DE_RemoveSurfaceFromBarObjectFront(H2DE_BarObject* bar, const std::string& name) {
    H2DE_Error::checkObject(bar);

    bar->removeSurface(bar->bod.front, name);
    bar->resetSurfaceBuffers();
}

void H2DE_RemoveSurfaceFromBarObjectBackground(H2DE_BarObject* bar, const std::string& name) {
    H2DE_Error::checkObject(bar);

    bar->removeSurface(bar->bod.background, name);
    bar->resetSurfaceBuffers();
}

void H2DE_BarObject::refreshPercentage() {
    value = std::clamp(value, bod.min, bod.max);
    percentage = (value - bod.min) / (bod.max - bod.min) * 100;
    resetSurfaceBuffers();
}

void H2DE_BarObject::resetSurfaceBuffers() {
    const float blend = std::clamp(percentage, 0.0f, 100.0f) / 100.0f;

    clearSurfaceBuffers();
    surfaceBuffers.reserve(bod.background.size() + bod.front.size());

    for (H2DE_Surface* surface : H2DE_Object::getSortedSurfaces(bod.background)) {
        const H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        surfaceBuffers.push_back(buffer);
    }

    for (H2DE_Surface* surface : H2DE_Object::getSortedSurfaces(bod.front)) {
        const H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        buffer.size.x *= blend;
        surfaceBuffers.push_back(buffer);
    }
}

// GETTER
H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar, const std::string& name) {
    H2DE_Error::checkObject(bar);
    return H2DE_Object::getSurface(bar->bod.front, name);
}

H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar, const std::string& name) {
    H2DE_Error::checkObject(bar);
    return H2DE_Object::getSurface(bar->bod.background, name);
}

// SETTER
void H2DE_SetBarValue(H2DE_BarObject* bar, float value) {
    H2DE_Error::checkObject(bar);

    bar->value = std::clamp(value, bar->bod.min, bar->bod.max);
    bar->refreshPercentage();
}

unsigned int H2DE_SetBarValue(H2DE_BarObject* bar, float value, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Error::checkObject(bar);

    value = std::clamp(value, bar->bod.min, bar->bod.max);

    const float defaultValue = bar->value;
    const float valueToAdd = value - defaultValue;

    return H2DE_CreateTimeline(bar->engine, duration, easing, [bar, defaultValue, valueToAdd](float blend) {
        H2DE_SetBarValue(bar, defaultValue + (valueToAdd * blend));
    }, completed, 0, pauseSensitive);
}

void H2DE_SetBarMin(H2DE_BarObject* bar, float min) {
    H2DE_Error::checkObject(bar);
    
    bar->bod.min = min;
    bar->refreshPercentage();
}

void H2DE_SetBarMax(H2DE_BarObject* bar, float max) {
    H2DE_Error::checkObject(bar);

    bar->bod.max = max;
    bar->refreshPercentage();
}

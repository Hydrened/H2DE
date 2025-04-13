#include "H2DE/objects/H2DE_bar_object.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData b) : H2DE_Object(engine, od), bod(b), value(b.defaultValue) {
    refreshPercentage();
}

H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) {
    H2DE_BarObject* object = new H2DE_BarObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    H2DE_Object::destroySurfaces(bod.front);
    H2DE_Object::destroySurfaces(bod.background);
}

// UPDATE
void H2DE_BarObject::update() {
    // if (bod.front) {
    //     bod.front->percentage = percentage;
    // }
}

void H2DE_BarObject::refreshPercentage() {
    value = std::clamp(value, bod.min, bod.max);
    percentage = (value - bod.min) / (bod.max - bod.min) * 100;
}

// GETTER
std::vector<H2DE_SurfaceBuffer> H2DE_BarObject::getSurfaceBuffers() const {
    std::vector<H2DE_SurfaceBuffer> res = {};

    for (const auto& [name, surface] : bod.front) {
        H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();
        surfaceOffset = surfaceOffset.rotate(od.pivot, od.rotation);

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        buffer.size.x *= std::clamp(percentage, 0.0f, 100.0f) / 100.0f;
        res.push_back(buffer);
    }

    for (const auto& [name, surface] : bod.background) {
        H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();
        surfaceOffset = surfaceOffset.rotate(od.pivot, od.rotation);

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        res.push_back(buffer);
    }

    return res;
}

H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar, const std::string& name) {
    return H2DE_Object::getSurface(bar->bod.front, name);
}

H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar, const std::string& name) {
    return H2DE_Object::getSurface(bar->bod.background, name);
}

// SETTER
void H2DE_SetBarValue(H2DE_BarObject* bar, float value) {
    bar->value = std::clamp(value, bar->bod.min, bar->bod.max);
    bar->refreshPercentage();
}

void H2DE_SetBarValue(H2DE_BarObject* bar, float value, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    value =  std::clamp(value, bar->bod.min, bar->bod.max);

    const float defaultValue = bar->value;
    const float valueToAdd = value - defaultValue;

    H2DE_CreateTimeline(bar->engine, duration, easing, [bar, defaultValue, valueToAdd](float blend) {
        H2DE_SetBarValue(bar, defaultValue + (valueToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetBarMin(H2DE_BarObject* bar, float min) {
    bar->bod.min = min;
    bar->refreshPercentage();
}

void H2DE_SetBarMax(H2DE_BarObject* bar, float max) {
    bar->bod.max = max;
    bar->refreshPercentage();
}

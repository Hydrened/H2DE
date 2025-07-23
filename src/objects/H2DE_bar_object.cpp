#include "H2DE/objects/H2DE_bar_object.h"

#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) : H2DE_Object(e, od), barObjectData(bod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    H2DE_Object::destroySurfaces(frontSurfaces);
    H2DE_Object::destroySurfaces(backgroundSurfaces);
}

// ACTIONS
void H2DE_BarObject::refreshSurfaceBuffers() {
    const std::vector<H2DE_Surface*> sortedBackgroundSurfaces = H2DE_Object::getSortedSurfaces(backgroundSurfaces);
    const std::vector<H2DE_Surface*> sortedFrontSurfaces = H2DE_Object::getSortedSurfaces(frontSurfaces);

    surfaceBuffers.clear();
    surfaceBuffers.reserve(sortedFrontSurfaces.size() + sortedBackgroundSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedBackgroundSurfaces.begin(), sortedBackgroundSurfaces.end());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedFrontSurfaces.begin(), sortedFrontSurfaces.end());
    rescaleSurfaceBuffers();
}

void H2DE_BarObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxFrontSurfaceRadius = getMaxSurfaceRadius(frontSurfaces);
    float maxBackgroundSurfaceRadius = getMaxSurfaceRadius(backgroundSurfaces);

    maxRadius = H2DE::max(maxHitboxesRadius, maxFrontSurfaceRadius, maxBackgroundSurfaceRadius);
}

// SETTER

// -- no lerp
void H2DE_BarObject::setMin(float min) {
    barObjectData.min = min;
    refreshSurfaceBuffers();
}

void H2DE_BarObject::setMax(float max) {
    barObjectData.max = max;
    refreshSurfaceBuffers();
}

void H2DE_BarObject::setValue(float value) {
    barObjectData.value = value;
    refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_BarObject::setMin(float min, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(engine, barObjectData.min, min, duration, easing, [this](float iv) {
        setMin(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_BarObject::setMax(float max, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(engine, barObjectData.max, max, duration, easing, [this](float iv) {
        setMax(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_BarObject::setValue(float value, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(engine, barObjectData.value, value, duration, easing, [this](float iv) {
        setValue(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

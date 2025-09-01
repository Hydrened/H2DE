#include "H2DE/objects/H2DE_bar_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData) : H2DE_DualSurfaceObject(engine, objectData), _barObjectData(barObjectData) {
    
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    
}

// GETTER
bool H2DE_BarObject::_isSurfaceFill(H2DE_Surface* surface) const {
    for (const auto& [name, fillSurface] : _fillSurfaces) {
        if (surface == fillSurface) {
            return true;
        }
    }

    return false;
}

// SETTER

// -- no lerp
void H2DE_BarObject::setMin(float min) {
    if (min == _barObjectData.min) {
        return;
    }

    _barObjectData.min = min;
    _refreshSurfaceBuffers();
}

void H2DE_BarObject::setMax(float max) {
    if (max == _barObjectData.max) {
        return;
    }

    _barObjectData.max = max;
    _refreshSurfaceBuffers();
}

void H2DE_BarObject::setValue(float value) {
    if (value == _barObjectData.value) {
        return;
    }
    
    _barObjectData.value = value;
    _refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_BarObject::setMin(float min, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(_engine, _barObjectData.min, min, duration, easing, [this](float iv) {
        setMin(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_BarObject::setMax(float max, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(_engine, _barObjectData.max, max, duration, easing, [this](float iv) {
        setMax(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_BarObject::setValue(float value, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    value = H2DE::clamp(value, _barObjectData.min, _barObjectData.max);

    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(_engine, _barObjectData.value, value, duration, easing, [this](float iv) {
        setValue(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

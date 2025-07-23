#include "H2DE/surfaces/H2DE_border.h"

#include "H2DE/engine/H2DE_lerp_manager.h"

// SETTER
H2DE_Timeline* H2DE_Border::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, borderData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

H2DE_Timeline* H2DE_Border::setThickness(uint16_t thickness, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<uint16_t>(engine, borderData.thickness, thickness, duration, easing, [this](uint16_t iv) {
        setThickness(iv);
    }, completed, pauseSensitive);
}

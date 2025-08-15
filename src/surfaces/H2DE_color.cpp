#include "H2DE/surfaces/H2DE_color.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// SETTER
H2DE_Timeline* H2DE_Color::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(_engine, _colorData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

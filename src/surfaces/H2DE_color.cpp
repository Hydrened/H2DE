#include "H2DE/surfaces/H2DE_color.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Color::H2DE_Color(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd, const H2DE_ColorData& cd) : H2DE_Surface(e, o, sd), colorData(cd) {

}

// CLEANUP
H2DE_Color::~H2DE_Color() {

}

// GETTER
bool H2DE_Color::isVisible() const {
    bool surfaceIsNotHidden = !(isHidden());
    bool colorIsVisible = (colorData.color.isVisible());
    
    return (surfaceIsNotHidden && colorIsVisible);
}

// SETTER
unsigned int H2DE_Color::setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, colorData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

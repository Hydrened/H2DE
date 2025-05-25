#include "H2DE/surfaces/H2DE_surface.h"
#include "H2DE/H2DE_lerp_manager.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd) : engine(e), object(o), surfaceData(sd) {

}

// CLEANUP
H2DE_Surface::~H2DE_Surface() {

}

// SETTER

// -- no lerp
void H2DE_Surface::setTranslate(const H2DE_Translate& translate) {
    surfaceData.transform.translate = translate;
    object->updateMaxRadius();
}

void H2DE_Surface::setScale(const H2DE_Scale& scale) {
    surfaceData.transform.scale = scale;
    object->updateMaxRadius();
}

void H2DE_Surface::setRotation(float rotation) {
    surfaceData.transform.rotation = rotation;
    object->updateMaxRadius();
}

void H2DE_Surface::setPivot(const H2DE_Pivot& pivot) {
    surfaceData.transform.pivot = pivot;
    object->updateMaxRadius();
}

// -- lerp
unsigned int H2DE_Surface::setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, surfaceData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Surface::setScale(const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, surfaceData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Surface::setRotation(float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, surfaceData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Surface::setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, surfaceData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

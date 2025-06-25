#include "H2DE/surfaces/H2DE_surface.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd) : engine(e), object(o), surfaceData(sd) {
    surfaceData.transform.defaultTranslate = surfaceData.transform.translate;
    surfaceData.transform.defaultScale = surfaceData.transform.scale;
    surfaceData.transform.defaultPivot= surfaceData.transform.pivot;
}

// SETTER

// -- no lerp
void H2DE_Surface::setTranslate(const H2DE_Translate& translate) {
    surfaceData.transform.translate = translate;
    surfaceData.transform.defaultTranslate = translate;
    object->refreshSurfaceBuffers();
    object->refreshMaxRadius();
}

void H2DE_Surface::setScale(const H2DE_Scale& scale) {
    surfaceData.transform.scale = scale;
    surfaceData.transform.defaultScale = scale;
    object->refreshSurfaceBuffers();
    object->refreshMaxRadius();
}

void H2DE_Surface::setRotation(float rotation) {
    surfaceData.transform.rotation = rotation;
    object->refreshSurfaceBuffers();
    object->refreshMaxRadius();
}

void H2DE_Surface::setPivot(const H2DE_Pivot& pivot) {
    surfaceData.transform.pivot = pivot;
    surfaceData.transform.defaultPivot = pivot;
    object->refreshSurfaceBuffers();
    object->refreshMaxRadius();
}

void H2DE_Surface::setScaleMode(H2DE_ScaleMode scaleMode) {
    surfaceData.scaleMode = scaleMode;
    object->refreshSurfaceBuffers();
}

void H2DE_Surface::setBlendMode(H2DE_BlendMode blendMode) {
    surfaceData.blendMode = blendMode;
    object->refreshSurfaceBuffers();
}

void H2DE_Surface::setIndex(int index) {
    surfaceData.index = index;
    object->refreshSurfaceBuffers();
}

// -- lerp
H2DE_TimelineID H2DE_Surface::setTranslate(const H2DE_Translate& translate, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, surfaceData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

H2DE_TimelineID H2DE_Surface::setScale(const H2DE_Scale& scale, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, surfaceData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);
}

H2DE_TimelineID H2DE_Surface::setRotation(float rotation, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, surfaceData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);
}

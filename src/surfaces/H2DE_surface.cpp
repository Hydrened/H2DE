#include "H2DE/surfaces/H2DE_surface.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_Surface::H2DE_Surface(H2DE_Engine* e, H2DE_Object* o, const H2DE_SurfaceData& sd) noexcept : _engine(e), _object(o), _surfaceData(sd) {
    _surfaceData.transform._defaultTranslate = _surfaceData.transform.translate;
    _surfaceData.transform._defaultScale = _surfaceData.transform.scale;
    _surfaceData.transform._defaultPivot = _surfaceData.transform.pivot;
}

// SETTER

// -- no lerp
void H2DE_Surface::setTranslate(const H2DE_Translate& translate) {
    _surfaceData.transform.translate = translate;
    _surfaceData.transform._defaultTranslate = translate;
    _object->_refreshSurfaceBuffers();
    _object->_refreshMaxRadius();
}

void H2DE_Surface::setScale(const H2DE_Scale& scale) {
    _surfaceData.transform.scale = scale;
    _surfaceData.transform._defaultScale = scale;
    _object->_refreshSurfaceBuffers();
    _object->_refreshMaxRadius();
}

void H2DE_Surface::setRotation(float rotation) {
    _surfaceData.transform.rotation = rotation;
    _object->_refreshSurfaceBuffers();
    _object->_refreshMaxRadius();
}

void H2DE_Surface::setPivot(const H2DE_Pivot& pivot) {
    _surfaceData.transform.pivot = pivot;
    _surfaceData.transform._defaultPivot = pivot;
    _object->_refreshSurfaceBuffers();
    _object->_refreshMaxRadius();
}

void H2DE_Surface::setScaleMode(H2DE_ScaleMode scaleMode) {
    _surfaceData.scaleMode = scaleMode;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Surface::setBlendMode(H2DE_BlendMode blendMode) {
    _surfaceData.blendMode = blendMode;
    _object->_refreshSurfaceBuffers();
}

void H2DE_Surface::setIndex(int index) {
    _surfaceData.index = index;
    _object->_refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_Surface::setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(_engine, _surfaceData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

H2DE_Timeline* H2DE_Surface::setScale(const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(_engine, _surfaceData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);
}

H2DE_Timeline* H2DE_Surface::setRotation(float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(_engine, _surfaceData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);
}

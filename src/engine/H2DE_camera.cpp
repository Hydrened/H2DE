#include "H2DE/H2DE_camera.h"
#include "H2DE/H2DE_lerp_manager.h"
#undef max
#undef min

// INIT
H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d), t(d.defaultTranslate) {

}

// CLEANUP
H2DE_Camera::~H2DE_Camera() {

}

// UPDATE
void H2DE_Camera::update() {
    
}

// GETTER
H2DE_LevelRect H2DE_Camera::getWorldRect() const {
    const H2DE_Scale cameraScale = getScale();
    float cameraHalfWidth = cameraScale.x * 0.5f;
    float cameraHalfHeight = cameraScale.y * 0.5f;

    return { t.x - cameraHalfWidth, t.x - cameraHalfHeight, cameraScale.x, cameraScale.y };
}

bool H2DE_Camera::containsObject(const H2DE_Object* object) const {
    const H2DE_LevelRect world_cameraRect = getWorldRect();
    const H2DE_Translate world_objectTranslate = object->getTranslate();

    return world_cameraRect.collides(world_objectTranslate, object->maxRadius);
}

H2DE_Scale H2DE_Camera::getScale() const {
    H2DE_Scale res;

    res.x = data.width;
    res.y = static_cast<float>(engine->data.window.size.y) / static_cast<float>(engine->data.window.size.x) * data.width;

    return res;
}

// SETTER
unsigned int H2DE_Camera::setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed,bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, t, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Camera::setWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, data.width, width, duration, easing, [this](float iv) {
        setWidth(iv);
    }, completed, pauseSensitive);
}

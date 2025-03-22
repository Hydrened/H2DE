#include "H2DE/H2DE_camera.h"

// INIT
H2DE_Engine::H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d), pos(d.defaultPos) {

}

// CLEANUP
H2DE_Engine::H2DE_Camera::~H2DE_Camera() {

}

// GETTER
H2DE_LevelPos H2DE_GetCameraPos(const H2DE_Engine* engine) {
    return engine->camera->pos;
}

H2DE_LevelSize H2DE_GetCameraSize(const H2DE_Engine* engine) {
    H2DE_LevelSize res;
    res.x = engine->camera->data.width;
    res.y = static_cast<float>(engine->data.window.size.y) / static_cast<float>(engine->data.window.size.x) * engine->camera->data.width;
    return res;
}

bool H2DE_CameraContainsObject(const H2DE_Engine* engine, H2DE_Object* object) {
    H2DE_Hitbox objectHitbox = H2DE_Hitbox();
    objectHitbox.rect = H2DE_GetObjectPos(object).makeRect(H2DE_GetObjectSize(object));
    return H2DE_CameraContainsHitbox(engine, objectHitbox, H2DE_IsObjectAbsolute(object));
}

bool H2DE_CameraContainsHitbox(const H2DE_Engine* engine, const H2DE_Hitbox& hitbox, bool absolute = false) {
    H2DE_LevelRect camHitbox = (absolute)
        ? engine->camera->pos.makeRect(H2DE_GetCameraSize(engine))
        : H2DE_LevelPos{ 0.0f, 0.0f }.makeRect(H2DE_GetCameraSize(engine));

    return camHitbox.collides(hitbox.rect);
}

// SETTER
void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos) {
    engine->camera->pos = pos;
}

void H2DE_SetCameraWidth(const H2DE_Engine* engine, float width) {
    engine->camera->data.width = width;
}

void H2DE_SetCameraSmoothing(const H2DE_Engine* engine, float smoothing) {
    engine->camera->data.smoothing = smoothing;
}

void H2DE_SetCameraReference(const H2DE_Engine* engine, H2DE_Object* object) {
    engine->camera->reference = object;
}

void H2DE_SetCameraLockedToReference(const H2DE_Engine* engine, bool state) {
    engine->camera->lockedToReference = state;
}

void H2DE_SetCameraPadding(const H2DE_Engine* engine, const H2DE_LevelPadding& padding) {
    engine->camera->data.padding =  padding;
}

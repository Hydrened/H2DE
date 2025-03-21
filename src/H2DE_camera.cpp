#include "H2DE/H2DE_camera.h"

// INIT
H2DE_Engine::H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d), pos(d.defaultPos) {

}

// CLEANUP
H2DE_Engine::H2DE_Camera::~H2DE_Camera() {

}

// GETTER
H2DE_LevelPos H2DE_GetCameraPos(H2DE_Engine* engine) {
    return engine->camera->pos;
}

H2DE_LevelSize H2DE_GetCameraSize(H2DE_Engine* engine) {
    H2DE_LevelSize res;
    res.x = engine->camera->data.width;
    res.y = static_cast<float>(engine->data.window.size.y) / static_cast<float>(engine->data.window.size.x) * engine->camera->data.width;
    return res;
}

bool H2DE_CameraContains(H2DE_Engine* engine, const H2DE_Hitbox& hitbox) {
    H2DE_LevelRect camHitbox = engine->camera->pos.makeRect(H2DE_GetCameraSize(engine));
    return camHitbox.collides(hitbox.rect);
}

// SETTER
void H2DE_SetCameraPos(H2DE_Engine* engine, const H2DE_LevelPos& pos) {
    engine->camera->pos = pos;
}

void H2DE_SetCameraWidth(H2DE_Engine* engine, const float& width) {
    engine->camera->data.width = width;
}

void H2DE_SetCameraSmoothing(H2DE_Engine* engine, const float& smoothing) {
    engine->camera->data.smoothing = smoothing;
}

void H2DE_SetCameraReference(H2DE_Engine* engine, H2DE_LevelObject* object) {
    engine->camera->reference = object;
}

void H2DE_SetCameraLockedToReference(H2DE_Engine* engine, const bool& state) {
    engine->camera->lockedToReference = state;
}

void H2DE_SetCameraPadding(H2DE_Engine* engine, const H2DE_LevelPadding& padding) {
    engine->camera->data.padding =  padding;
}

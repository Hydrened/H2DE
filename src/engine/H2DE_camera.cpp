#include "H2DE/H2DE/H2DE_camera.h"

// INIT
H2DE_Engine::H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d), pos(d.defaultPos) {

}

// CLEANUP
H2DE_Engine::H2DE_Camera::~H2DE_Camera() {

}

// UPDATE
void H2DE_Engine::H2DE_Camera::update() {
    if (!reference) {
        return;
    }

    H2DE_LevelRect refRect = H2DE_GetObjectPos(reference).makeRect(H2DE_GetObjectSize(reference));
    H2DE_LevelSize camSize = H2DE_GetCameraSize(engine);
    float smoothing = std::abs(1.0f - std::max(std::min(data.smoothing, 0.999f), 0.0f));

    if (!lockedToReference) {
        if (refRect.x < pos.x + data.padding.left) {
            pos.x =  H2DE_Lerp(pos.x, refRect.x - data.padding.left, smoothing, H2DE_EASING_LINEAR);
        }
        else if (refRect.x + refRect.w > pos.x + camSize.x - data.padding.right) {
            pos.x = H2DE_Lerp(pos.x, refRect.x + refRect.w + data.padding.right - camSize.x, smoothing, H2DE_EASING_LINEAR);
        }
        if (refRect.y < pos.y + data.padding.top) {
            pos.y = H2DE_Lerp(pos.y, refRect.y - data.padding.top, smoothing, H2DE_EASING_LINEAR);
        }
        else if (refRect.y + refRect.h > pos.y + camSize.y - data.padding.bottom) {
            pos.y = H2DE_Lerp(pos.y, refRect.y + refRect.h + data.padding.bottom - camSize.y, smoothing, H2DE_EASING_LINEAR);
        }

    } else {
        H2DE_LevelPos offset = { lockedPadding.left, lockedPadding.top };
        pos = refRect.getPos() - offset;
    }
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

bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object) {
    const H2DE_LevelPos pos = H2DE_GetObjectPos(object);
    const H2DE_LevelSize size = H2DE_GetObjectSize(object);
    const H2DE_LevelRect rect = pos.makeRect(size);
    const bool absolute = H2DE_IsObjectAbsolute(object);

    if (H2DE_CameraContainsRect(engine, rect, absolute)) {
        return true;
    }

    for (const H2DE_SurfaceBuffer& surfaceBuffer : object->getSurfaceBuffers()) {
        const H2DE_LevelRect surfaceRect = (object->od.pos + surfaceBuffer.offset).makeRect(surfaceBuffer.size);

        if (H2DE_CameraContainsRect(engine, surfaceRect, absolute)) {
            return true;
        }
    }

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (H2DE_CameraContainsHitbox(engine, pos, hitbox, absolute)) {
            return true;
        }
    }

    return false;
}

bool H2DE_CameraContainsHitbox(const H2DE_Engine* engine, const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute) {
    H2DE_LevelRect hitboxRect = hitbox.rect + pos.makeRect({ 0.0f, 0.0f });
    return H2DE_CameraContainsRect(engine, hitboxRect, absolute);
}

bool H2DE_CameraContainsRect(const H2DE_Engine* engine, const H2DE_LevelRect& rect, bool absolute) {
    H2DE_LevelRect camHitbox = (absolute)
        ? H2DE_LevelPos{ 0.0f, 0.0f }.makeRect(H2DE_GetCameraSize(engine))
        : engine->camera->pos.makeRect(H2DE_GetCameraSize(engine));

    return camHitbox.collides(rect);
}

// SETTER
void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos) {
    engine->camera->pos = pos;
}

void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    const H2DE_LevelPos defaultPos = H2DE_GetCameraPos(engine);
    const H2DE_LevelPos posToAdd = pos - defaultPos;

    H2DE_CreateTimeline(engine, duration, easing, [engine, defaultPos, posToAdd](float blend) {
        H2DE_SetCameraPos(engine, defaultPos + (posToAdd * blend));
    }, nullptr, 0, pauseSensitive);
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
    engine->camera->data.padding = padding;
}

void H2DE_SetCameraPaddingFromReference(const H2DE_Engine* engine, const H2DE_LevelPadding& padding) {
    engine->camera->lockedPadding = padding;
}

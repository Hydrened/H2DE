#include "H2DE/objects/H2DE_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#include "H2DE/engine/H2DE_geometry.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Object::H2DE_Object(H2DE_Engine* e, const H2DE_ObjectData& od) noexcept : engine(e), objectData(od) {

}

// CLEANUP
H2DE_Object::~H2DE_Object() {
    stopTimelines();
}

void H2DE_Object::destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    for (const auto& [name, surface] : surfaces) {
        delete surface;
    }

    surfaces.clear();
}

void H2DE_Object::destroySurfaces(std::vector<H2DE_Surface*>& surfaces) {
    for (H2DE_Surface* surface : surfaces) {
        delete surface;
    }

    surfaces.clear();
}

void H2DE_Object::removeTimeline(H2DE_Timeline* timeline) {
    auto it = std::find(timelinesBuffer.begin(), timelinesBuffer.end(), timeline);
    if (it != timelinesBuffer.end()) {
        timelinesBuffer.erase(it);
    }
}

void H2DE_Object::stopTimelines() {
    for (H2DE_Timeline* timeline : timelinesBuffer) {
        if (!engine->timelineManager->isStoped(timeline)) {
            timeline->stop(false);
        }
    }
    timelinesBuffer.clear();
}

// UPDATE
void H2DE_Object::update() {
    updateCollisions();
    updateTimelineBuffer();
}

void H2DE_Object::updateCollisions() {
    if (hidden || objectData.absolute || isGrid) {
        return;
    }

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    for (const auto& [name, hitbox] : hitboxes) {
        if (!hitbox.onCollide) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(this, hitbox, xIsInverted, yIsInverted);
        const int& collisionIndex = hitbox.collisionIndex;

        for (H2DE_Object* otherObject : engine->objects) {
            if (otherObject == this || otherObject->objectData.absolute || otherObject->hidden || otherObject->isGrid) {
                continue;
            }

            const H2DE_ObjectData& otherObjectData = otherObject->objectData;

            for (const auto& [otherName, otherHitbox] : otherObject->hitboxes) {
                if (collisionIndex != otherHitbox.collisionIndex) {
                    continue;
                }

                const H2DE_LevelRect world_otherHitboxRect = G::getHitboxRect(otherObject, otherHitbox, xIsInverted, yIsInverted);

                if (!world_hitboxRect.collides(world_otherHitboxRect)) {
                    continue;
                }

                const std::optional<H2DE_Face> possibleCollidedFace = world_hitboxRect.getCollidedFace(world_otherHitboxRect);
                if (!possibleCollidedFace.has_value()) {
                    continue;
                }

                if (hitbox.onCollide) {
                    hitbox.onCollide(otherObject, possibleCollidedFace.value());
                }
            }
        }
    }
}

void H2DE_Object::updateTimelineBuffer() {
    for (auto it = timelinesBuffer.begin(); it != timelinesBuffer.end(); ) {
        if (engine->timelineManager->isStoped(*it)) {
            it = timelinesBuffer.erase(it);
        } else {
            ++it;
        }
    }
}

// ACTIONS

// -- surfaces
bool H2DE_Object::removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    auto it = surfaces.find(name);

    bool removed = (it != surfaces.end());

    if (removed) {
        surfaces.erase(it);
        refreshMaxRadius();
        refreshSurfaceBuffers();
    }

    return removed;
}

// -- hitboxes
void H2DE_Object::addHitbox(const std::string& name, const H2DE_Hitbox& hitbox) {
    hitboxes[name] = hitbox;

    hitboxes[name].transform.defaultTranslate = hitbox.transform.translate;
    hitboxes[name].transform.defaultScale = hitbox.transform.scale;
    hitboxes[name].transform.defaultPivot = hitbox.transform.pivot;
    
    refreshMaxRadius();
}

bool H2DE_Object::removeHitbox(const std::string& name) {
    auto it = hitboxes.find(name);
    
    bool removed = (it != hitboxes.end());

    if (removed) {
        hitboxes.erase(it);
        refreshMaxRadius();
    }

    return removed;
}

// -- rescale
void H2DE_Object::rescaleSurfaceBuffers() noexcept {
    const float absoluteObjectScaleX = H2DE::abs(objectData.transform.scale.x);
    const float absoluteObjectScaleY = H2DE::abs(objectData.transform.scale.y);
    const H2DE_Scale absoluteObjectScale = { absoluteObjectScaleX, absoluteObjectScaleY };

    for (H2DE_Surface* surface : surfaceBuffers) {
        H2DE_Object::rescaleTransform(surface->surfaceData.transform, absoluteObjectScale);
    }
}

void H2DE_Object::rescaleHitboxes() noexcept {
    const float absoluteObjectScaleX = H2DE::abs(objectData.transform.scale.x);
    const float absoluteObjectScaleY = H2DE::abs(objectData.transform.scale.y);
    const H2DE_Scale absoluteObjectScale = { absoluteObjectScaleX, absoluteObjectScaleY };

    for (auto& [name, hitbox] : hitboxes) {
        H2DE_Object::rescaleTransform(hitbox.transform, absoluteObjectScale);
    }
}

void H2DE_Object::rescaleTransform(H2DE_Transform& transform, const H2DE_Scale& scale) noexcept {
    transform.translate.x = transform.defaultTranslate.x * scale.x;
    transform.translate.y = transform.defaultTranslate.y * scale.y;

    transform.scale.x = transform.defaultScale.x * scale.x;
    transform.scale.y = transform.defaultScale.y * scale.y;

    transform.pivot.x = transform.defaultPivot.x * scale.x;
    transform.pivot.y = transform.defaultPivot.y * scale.y;
}

// GETTER
const std::vector<H2DE_Surface*> H2DE_Object::getSortedSurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    std::vector<H2DE_Surface*> res;
    res.reserve(surfaces.size());

    for (const auto& [name, surface] : surfaces) {
        res.push_back(surface);
    }

    std::sort(res.begin(), res.end(), [](H2DE_Surface* a, H2DE_Surface* b) {
        return a->surfaceData.index < b->surfaceData.index;
    });

    return res;
}

const std::array<H2DE_Translate, 4> H2DE_Object::getCorners(const H2DE_Transform& transform) {
    const H2DE_Translate& translate = transform.translate;
    const H2DE_Pivot& pivot = transform.pivot;
    const float& rotation = transform.rotation;

    float offsetX = pivot.x - translate.x;
    float offsetY = pivot.y - translate.y;
    
    float scaledX = translate.x - offsetX;
    float scaledY = translate.y - offsetY;
    
    float halfW = transform.scale.x * 0.5f;
    float halfH = transform.scale.y * 0.5f;
    
    return {
        H2DE_Translate{ scaledX - halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY + halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX - halfW, scaledY + halfH }.rotate(pivot, rotation),
    };
}

const H2DE_Hitbox& H2DE_Object::getHitbox(const std::string& name) const {
    auto it = hitboxes.find(name);
    if (it == hitboxes.end()) {
        H2DE_Error::logError("Hitbox named \"" + name + "\" not found");
    }
        
    return it->second;
}

const H2DE_LevelRect H2DE_Object::getHitboxWorldRect(const std::string& name) const {
    const H2DE_Transform& hitboxTransform = getHitbox(name).transform;
    const H2DE_Scale absScale = { H2DE::abs(hitboxTransform.scale.x), H2DE::abs(hitboxTransform.scale.y) };
    return hitboxTransform.translate.makeRect(absScale).addTranslate(getTranslate());
}

float H2DE_Object::getMaxHitboxRadius() const {
    float res = 0.0f;
    const H2DE_Translate world_objectTranslate = objectData.transform.translate;

    for (const auto& [name, hitbox] : hitboxes) {
        for (const H2DE_Translate& corner : H2DE_Object::getCorners(hitbox.transform)) {

            const H2DE_Translate world_hitboxCorner = corner + world_objectTranslate;
            float distance = H2DE::abs(world_objectTranslate.getDistanceSquared(world_hitboxCorner));

            if (distance > res) {
                res = distance;
            }
        }
    }

    return std::sqrt(res);
}

float H2DE_Object::getMaxSurfaceRadius(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) const {
    float res = 0.0f;
    const H2DE_Translate world_objectTranslate = objectData.transform.translate;

    for (const auto& [name, surface] : surfaces) {
        for (const H2DE_Translate& corner : H2DE_Object::getCorners(surface->getTransform())) {

            const H2DE_Translate world_hitboxCorner = corner + world_objectTranslate;
            float distance = H2DE::abs(world_objectTranslate.getDistanceSquared(world_hitboxCorner));

            if (distance > res) {
                res = distance;
            }
        }
    }

    return std::sqrt(res);
}

// SETTER

// -- non lerp
void H2DE_Object::setTranslate(const H2DE_Translate& translate) {
    objectData.transform.translate = translate;
    refreshSurfaceBuffers();
}

void H2DE_Object::setScale(const H2DE_Scale& scale) {
    objectData.transform.scale = scale;
    refreshSurfaceBuffers();
    rescaleHitboxes();
}

void H2DE_Object::setRotation(float rotation) {
    objectData.transform.rotation = rotation;
    refreshSurfaceBuffers();
}

void H2DE_Object::setPivot(const H2DE_Pivot& pivot) {
    objectData.transform.pivot = pivot;
    refreshSurfaceBuffers();
}

void H2DE_Object::setOpacity(uint8_t opacity) {
    objectData.opacity = opacity;
    refreshSurfaceBuffers();
}

void H2DE_Object::setAbsolute(bool absolute) {
    objectData.absolute = absolute;
    refreshSurfaceBuffers();
}

void H2DE_Object::setIndex(int index) {
    objectData.index = index;
    refreshSurfaceBuffers();
}

void H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.translate = translate;
        hitboxes[name].transform.defaultTranslate = translate;
        refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.scale = scale;
        hitboxes[name].transform.defaultScale = scale;
        refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxRotation(const std::string& name, float rotation) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.rotation = rotation;
        refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.pivot = pivot;
        hitboxes[name].transform.defaultPivot = pivot;
        refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color) {
    if (hasHitbox(name)) {
        hitboxes[name].color = color;
    }
}

void H2DE_Object::setHitboxCollisionIndex(const std::string& name, int collisionIndex) {
    if (hasHitbox(name)) {
        hitboxes[name].collisionIndex = collisionIndex;
    }
}

void H2DE_Object::setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide) {
    if (hasHitbox(name)) {
        hitboxes[name].onCollide = onCollide;
    }
}

// -- lerp
H2DE_Timeline* H2DE_Object::setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Translate>(engine, objectData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setScale(const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(engine, objectData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setRotation(float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline =  H2DE_LerpManager::lerp<float>(engine, objectData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setOpacity(uint8_t opacity, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(engine, objectData.opacity, opacity, duration, easing, [this](uint8_t iv) {
        setOpacity(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Translate>(engine, getHitbox(name).transform.translate, translate, duration, easing, [this, name](H2DE_Translate iv) {
        setHitboxTranslate(name, iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(engine, getHitbox(name).transform.scale, scale, duration, easing, [this, name](H2DE_Scale iv) {
        setHitboxScale(name, iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxRotation(const std::string& name, float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(engine, getHitbox(name).transform.rotation, rotation, duration, easing, [this, name](float iv) {
        setHitboxRotation(name, iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(engine, getHitbox(name).color, color, duration, easing, [this, name](H2DE_ColorRGB iv) {
        setHitboxColor(name, iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

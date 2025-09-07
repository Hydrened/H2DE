#include "H2DE/objects/parents/H2DE_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#include "H2DE/engine/H2DE_geometry.h"
#include "H2DE/engine/H2DE_error.h"

// CLEANUP
H2DE_Object::~H2DE_Object() {
    _stopTimelines();
}

void H2DE_Object::_destroySurfaces(H2DE_SurfaceMap& surfaces) {
    for (const auto& [name, surface] : surfaces) {
        delete surface;
    }

    surfaces.clear();
}

void H2DE_Object::_destroySurfaces(std::vector<H2DE_Surface*>& surfaces) {
    for (H2DE_Surface* surface : surfaces) {
        delete surface;
    }

    surfaces.clear();
}

void H2DE_Object::_removeTimeline(H2DE_Timeline* timeline) {
    auto it = std::find(_timelinesBuffer.begin(), _timelinesBuffer.end(), timeline);
    if (it != _timelinesBuffer.end()) {
        _timelinesBuffer.erase(it);
    }
}

void H2DE_Object::_stopTimelines() {
    for (H2DE_Timeline* timeline : _timelinesBuffer) {
        if (!_engine->_timelineManager->isStoped(timeline)) {
            timeline->stop(false);
        }
    }
    
    _timelinesBuffer.clear();
}

// UPDATE
void H2DE_Object::_update() {
    _updateCollisions();
    _updateTimelineBuffer();
}

void H2DE_Object::_updateCollisions() {
    if (_hidden || _objectData.absolute || _isGrid) {
        return;
    }

    bool xIsInverted = _engine->_camera->isXOriginInverted();
    bool yIsInverted = _engine->_camera->isYOriginInverted();

    for (const auto& [name, hitbox] : _hitboxes) {
        if (!hitbox.onCollide) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(this, hitbox, xIsInverted, yIsInverted);
        const int& collisionIndex = hitbox.collisionIndex;

        for (H2DE_Object* otherObject : _engine->_objects) {
            if (otherObject == this || otherObject->_objectData.absolute || otherObject->_hidden || otherObject->_isGrid) {
                continue;
            }

            const H2DE_ObjectData& otherObjectData = otherObject->_objectData;

            for (const auto& [otherName, otherHitbox] : otherObject->_hitboxes) {
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

void H2DE_Object::_updateTimelineBuffer() {
    for (auto it = _timelinesBuffer.begin(); it != _timelinesBuffer.end(); ) {
        if (_engine->_timelineManager->isStoped(*it)) {
            it = _timelinesBuffer.erase(it);
        } else {
            ++it;
        }
    }
}

// ACTIONS

// -- refresh
void H2DE_Object::_refreshSurfaceBuffers() {
    _surfaceBuffers.clear();
}

void H2DE_Object::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius();
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

// -- surfaces
bool H2DE_Object::_removeSurface(H2DE_SurfaceMap& surfaces, const std::string& name) {
    auto it = surfaces.find(name);

    bool removed = (it != surfaces.end());

    if (removed) {
        surfaces.erase(it);
        _refreshMaxRadius();
        _refreshSurfaceBuffers();
    }

    return removed;
}

// -- hitboxes
void H2DE_Object::addHitbox(const std::string& name, const H2DE_Hitbox& hitbox) {
    _hitboxes[name] = hitbox;

    _hitboxes[name].transform._defaultTranslate = hitbox.transform.translate;
    _hitboxes[name].transform._defaultScale = hitbox.transform.scale;
    _hitboxes[name].transform._defaultPivot = hitbox.transform.pivot;
    
    _refreshMaxRadius();
}

bool H2DE_Object::removeHitbox(const std::string& name) {
    auto it = _hitboxes.find(name);
    
    bool removed = (it != _hitboxes.end());

    if (removed) {
        _hitboxes.erase(it);
        _refreshMaxRadius();
    }

    return removed;
}

// -- rescale
void H2DE_Object::_rescaleSurfaceBuffers() {
    const float absoluteObjectScaleX = H2DE::abs(_objectData.transform.scale.x);
    const float absoluteObjectScaleY = H2DE::abs(_objectData.transform.scale.y);
    const H2DE_Scale absoluteObjectScale = { absoluteObjectScaleX, absoluteObjectScaleY };

    for (H2DE_Surface* surface : _surfaceBuffers) {
        H2DE_Object::_rescaleTransform(surface->_surfaceData.transform, absoluteObjectScale);
    }
}

void H2DE_Object::_rescaleHitboxes() {
    if (_isGrid) {
        return;
    }

    const float absoluteObjectScaleX = H2DE::abs(_objectData.transform.scale.x);
    const float absoluteObjectScaleY = H2DE::abs(_objectData.transform.scale.y);
    const H2DE_Scale absoluteObjectScale = { absoluteObjectScaleX, absoluteObjectScaleY };

    for (auto& [name, hitbox] : _hitboxes) {
        H2DE_Object::_rescaleTransform(hitbox.transform, absoluteObjectScale);
    }
}

void H2DE_Object::_rescaleTransform(H2DE_Transform& transform, const H2DE_Scale& scale) {
    transform.translate.x = transform._defaultTranslate.x * scale.x;
    transform.translate.y = transform._defaultTranslate.y * scale.y;

    transform.scale.x = transform._defaultScale.x * scale.x;
    transform.scale.y = transform._defaultScale.y * scale.y;

    transform.pivot.x = transform._defaultPivot.x * scale.x;
    transform.pivot.y = transform._defaultPivot.y * scale.y;
}

// -- text object
H2DE_TextObject* H2DE_Object::_refreshTextObject(H2DE_TextObject* textObject, const H2DE_Text& text) {
    if (textObject != H2DE_NULL_OBJECT) {
        if (_engine->destroyObject(textObject)) {
            textObject = H2DE_NULL_OBJECT;
        }
    }

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    tod.text = text;

    return _engine->createObject<H2DE_TextObject>(_objectData, tod);
}

// GETTER
const std::vector<H2DE_Surface*> H2DE_Object::_getSortedSurfaces(H2DE_SurfaceMap& surfaces) {
    std::vector<H2DE_Surface*> res;
    res.reserve(surfaces.size());

    for (const auto& [name, surface] : surfaces) {
        res.push_back(surface);
    }

    return _getSortedSurfaces(res);
}

const std::vector<H2DE_Surface*> H2DE_Object::_getSortedSurfaces(std::vector<H2DE_Surface*>& surfaces) {
    std::vector<H2DE_Surface*> res = surfaces;

    std::sort(res.begin(), res.end(), [](H2DE_Surface* a, H2DE_Surface* b) {
        return (a->_surfaceData.index < b->_surfaceData.index);
    });

    return res;
}

const std::array<H2DE_Translate, 4> H2DE_Object::_getCorners(const H2DE_Transform& transform) {
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
    auto it = _hitboxes.find(name);
    if (it == _hitboxes.end()) {
        H2DE_Error::logError("Hitbox named \"" + name + "\" not found");
    }
        
    return it->second;
}

const H2DE_LevelRect H2DE_Object::getHitboxWorldRect(const std::string& name) const {
    const H2DE_Transform& hitboxTransform = getHitbox(name).transform;
    const H2DE_Scale absScale = { H2DE::abs(hitboxTransform.scale.x), H2DE::abs(hitboxTransform.scale.y) };
    return hitboxTransform.translate.makeRect(absScale).addTranslate(getTranslate());
}

float H2DE_Object::_getMaxHitboxRadius() const {
    float res = 0.0f;
    const H2DE_Translate& world_objectTranslate = _objectData.transform.translate;

    for (const auto& [name, hitbox] : _hitboxes) {
        for (const H2DE_Translate& corner : H2DE_Object::_getCorners(hitbox.transform)) {

            const H2DE_Translate world_hitboxCorner = corner + world_objectTranslate;
            float distance = H2DE::abs(world_objectTranslate.getDistanceSquared(world_hitboxCorner));

            if (distance > res) {
                res = distance;
            }
        }
    }

    return std::sqrt(res);
}

float H2DE_Object::_getMaxSurfaceRadius() const {
    float res = 0.0f;
    const H2DE_Translate& world_objectTranslate = _objectData.transform.translate;

    for (const H2DE_Surface* surface : _surfaceBuffers) {
        for (const H2DE_Translate& corner : H2DE_Object::_getCorners(surface->getTransform())) {

            const H2DE_Translate world_surfaceCorner = corner + world_objectTranslate;
            float distance = H2DE::abs(world_objectTranslate.getDistanceSquared(world_surfaceCorner));

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
    if (translate == _objectData.transform.translate) {
        return;
    }

    _objectData.transform.translate = translate;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setScale(const H2DE_Scale& scale) {
    if (scale == _objectData.transform.scale) {
        return;
    }

    _objectData.transform.scale = scale;
    _refreshSurfaceBuffers();
    _rescaleHitboxes();
}

void H2DE_Object::setRotation(float rotation) {
    if (rotation == _objectData.transform.rotation) {
        return;
    }
    
    _objectData.transform.rotation = rotation;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setPivot(const H2DE_Pivot& pivot) {
    if (pivot == _objectData.transform.pivot) {
        return;
    }

    _objectData.transform.pivot = pivot;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setOpacity(uint8_t opacity) {
    if (opacity == _objectData.opacity) {
        return;
    }

    _objectData.opacity = opacity;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setAbsolute(bool absolute) {
    if (absolute == _objectData.absolute) {
        return;
    }

    _objectData.absolute = absolute;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setIndex(int index) {
    if (index == _objectData.index) {
        return;
    }

    _objectData.index = index;
    _refreshSurfaceBuffers();
}

void H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate) {
    if (hasHitbox(name)) {
        _hitboxes[name].transform.translate = translate;
        _hitboxes[name].transform._defaultTranslate = translate;
        _refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale) {
    if (hasHitbox(name)) {
        _hitboxes[name].transform.scale = scale;
        _hitboxes[name].transform._defaultScale = scale;
        _refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxRotation(const std::string& name, float rotation) {
    if (hasHitbox(name)) {
        _hitboxes[name].transform.rotation = rotation;
        _refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot) {
    if (hasHitbox(name)) {
        _hitboxes[name].transform.pivot = pivot;
        _hitboxes[name].transform._defaultPivot = pivot;
        _refreshMaxRadius();
    }
}

void H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color) {
    if (hasHitbox(name)) {
        _hitboxes[name].color = color;
    }
}

void H2DE_Object::setHitboxCollisionIndex(const std::string& name, int collisionIndex) {
    if (hasHitbox(name)) {
        _hitboxes[name].collisionIndex = collisionIndex;
    }
}

void H2DE_Object::setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide) {
    if (hasHitbox(name)) {
        _hitboxes[name].onCollide = onCollide;
    }
}

// -- lerp
H2DE_Timeline* H2DE_Object::setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Translate>(_engine, _objectData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setScale(const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(_engine, _objectData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setRotation(float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline =  H2DE_LerpManager::lerp<float>(_engine, _objectData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setOpacity(uint8_t opacity, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(_engine, _objectData.opacity, opacity, duration, easing, [this](uint8_t iv) {
        setOpacity(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Translate>(_engine, getHitbox(name).transform.translate, translate, duration, easing, [this, name](H2DE_Translate iv) {
        setHitboxTranslate(name, iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(_engine, getHitbox(name).transform.scale, scale, duration, easing, [this, name](H2DE_Scale iv) {
        setHitboxScale(name, iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxRotation(const std::string& name, float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(_engine, getHitbox(name).transform.rotation, rotation, duration, easing, [this, name](float iv) {
        setHitboxRotation(name, iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(_engine, getHitbox(name).color, color, duration, easing, [this, name](H2DE_ColorRGB iv) {
        setHitboxColor(name, iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

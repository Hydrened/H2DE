#include "H2DE/H2DE_camera.h"
#include "H2DE/H2DE_lerp_manager.h"
#undef min
#undef max

// INIT
H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d) {
    initGrid();
}

void H2DE_Camera::initGrid() {
    H2DE_ObjectData od = H2DE_ObjectData();
    od.index = std::numeric_limits<int>::min();

    grid = engine->createBasicObject(od);
    grid->isGrid = true;

    updateGrid();
}

// CLEANUP
H2DE_Camera::~H2DE_Camera() {
    destroyGrid();
}

void H2DE_Camera::destroyGrid() {
    engine->destroyObject(grid);
}

void H2DE_Camera::destroyGridHitboxes() {
    for (const auto& [name, hitbox] : grid->getHitboxes()) {
        grid->removeHitbox(name);
    }
}

// UPDATE
void H2DE_Camera::update() {

}

void H2DE_Camera::updateGrid() {
    if (!data.grid) {
        return;
    }

    destroyGridHitboxes();

    const H2DE_LevelRect world_cameraRect = getWorldRect();
    const H2DE_Scale cameraTranslate = world_cameraRect.getTranslate();
    const H2DE_Scale cameraScale = world_cameraRect.getScale();
    const H2DE_Scale cameraHalfScale = cameraScale * 0.5f;

    float minX = world_cameraRect.getMinX();
    float maxX = world_cameraRect.getMaxX();
    float minY = world_cameraRect.getMinY();
    float maxY = world_cameraRect.getMaxY();

    constexpr H2DE_ColorRGB secondGridColor = { 20, 20, 20, 255 };
    constexpr H2DE_ColorRGB mainGridColor = { 69, 69, 69, 255 };

    grid->setTranslate(cameraTranslate);
    grid->setScale(cameraScale);

    float step = std::max(std::floor(data.gameWidth / 50.0f) * 2.0f, 1.0f);

    for (float x = -cameraTranslate.x; x < cameraHalfScale.x; x += step) {
        float absX = x + cameraTranslate.x;
        if (absX < minX) {
            continue;
        }

        bool isMain = (std::abs(std::fmod(absX, step * 5.0f)) < 0.001f);

        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate = { x, 0.0f };
        hitbox.transform.scale = { 0.0f, cameraScale.x };
        hitbox.color = (isMain) ? mainGridColor : secondGridColor;
        grid->addHitbox("x-" + std::to_string(x), hitbox);
    }

    for (float x = -cameraTranslate.x; x > -cameraHalfScale.x; x -= step) {
        float absX = x + cameraTranslate.x;
        if (absX > maxX) {
            continue;
        }

        bool isMain = (std::abs(std::fmod(absX, step * 5.0f)) < 0.001f);

        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate = { x, 0.0f };
        hitbox.transform.scale = { 0.0f, cameraScale.x };
        hitbox.color = (isMain) ? mainGridColor : secondGridColor;
        grid->addHitbox("x-" + std::to_string(x), hitbox);
    }

    for (float y = -cameraTranslate.y; y < cameraHalfScale.y; y += step) {
        float absY = y + cameraTranslate.y;
        if (absY < minY) {
            continue;
        }

        bool isMain = (std::abs(std::fmod(absY, step * 5.0f)) < 0.001f);
        
        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate = { 0.0f, y };
        hitbox.transform.scale = { cameraScale.x, 0.0f };
        hitbox.color = (isMain) ? mainGridColor : secondGridColor;
        grid->addHitbox("y-" + std::to_string(y), hitbox);
    }

    for (float y = -cameraTranslate.y; y > -cameraHalfScale.y; y -= step) {
        float absY = y + cameraTranslate.y;
        if (absY > maxY) {
            continue;
        }

        bool isMain = (std::abs(std::fmod(absY, step * 5.0f)) < 0.001f);
        
        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate = { 0.0f, y };
        hitbox.transform.scale = { cameraScale.x, 0.0f };
        hitbox.color = (isMain) ? mainGridColor : secondGridColor;
        grid->addHitbox("y-" + std::to_string(y), hitbox);
    }
}

// GETTER
bool H2DE_Camera::containsObject(const H2DE_Object* object) const {
    const H2DE_LevelRect world_cameraRect = getWorldRect();
    const H2DE_Translate world_objectTranslate = object->getTranslate();

    return world_cameraRect.collides(world_objectTranslate, object->maxRadius);
}

H2DE_Scale H2DE_Camera::getScale() const {
    H2DE_Scale res;

    res.x = data.gameWidth;
    res.y = static_cast<float>(engine->data.window.size.y) / static_cast<float>(engine->data.window.size.x) * data.gameWidth;

    return res;
}

// SETTER

// -- no lerp
void H2DE_Camera::setTranslate(const H2DE_Translate& translate) {
    data.translate = translate;
    updateGrid();
}

void H2DE_Camera::setGameWidth(float width) {
    data.gameWidth = width;
    updateGrid();
}

// lerp
unsigned int H2DE_Camera::setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed,bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, data.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Camera::setGameWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, data.gameWidth, width, duration, easing, [this](float iv) {
        setGameWidth(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Camera::setInterfaceWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, data.interfaceWidth, width, duration, easing, [this](float iv) {
        setInterfaceWidth(iv);
    }, completed, pauseSensitive);
}

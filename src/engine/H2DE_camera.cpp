#include "H2DE/engine/H2DE_camera.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#undef min
#undef max

// INIT
H2DE_Camera::H2DE_Camera(H2DE_Engine* e, const H2DE_CameraData& d) : engine(e), data(d) {
    initGrid();
}

void H2DE_Camera::initGrid() {
    H2DE_ObjectData od = H2DE_ObjectData();
    od.index = H2DE_INDEX_MIN;

    grid = engine->createObject<H2DE_BasicObject>(od);
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
    destroyGridHitboxes();

    if (data.grid) {
        updateGridObjectTransform();
        updateCrosshair();
        updateGridObjectHitboxes();
    }
}

void H2DE_Camera::updateGridObjectTransform() {
    const H2DE_LevelRect world_cameraRect = getWorldRect();

    grid->setTranslate(world_cameraRect.getTranslate());
    grid->setScale(world_cameraRect.getScale());
}

void H2DE_Camera::updateGridObjectHitboxes() {
    const H2DE_LevelRect world_cameraRect = getWorldRect();
    const H2DE_Scale cameraTranslate = world_cameraRect.getTranslate();
    const H2DE_Scale cameraScale = world_cameraRect.getScale();

    float minX = world_cameraRect.getMinX();
    float maxX = world_cameraRect.getMaxX();
    float minY = world_cameraRect.getMinY();
    float maxY = world_cameraRect.getMaxY();

    constexpr H2DE_ColorRGB SECOND_GRID_COLOR = { 20, 20, 20, 255 };
    constexpr H2DE_ColorRGB MAIN_GRID_COLOR = { 69, 69, 69, 255 };
    constexpr H2DE_ColorRGB CENTER_GRID_COLOR = { 150, 150, 150, 255 };

    constexpr float REISIZE_GRID_FACTOR = 0.02f;
    constexpr float MAIN_GRID_SPACING = 5.0f;
    constexpr float EPSILON = 0.001f;

    float step = std::max(H2DE::floor(data.gameWidth * REISIZE_GRID_FACTOR) * 2.0f, 1.0f);

    float startX = H2DE::round(minX) + H2DE::round(std::fmod(minX, step)) * -1;

    for (float x = startX; x < maxX; x += step) {
        const H2DE_ColorRGB& color = (x == 0.0f)
            ? CENTER_GRID_COLOR
            : (H2DE::abs(std::fmod(x, step * MAIN_GRID_SPACING)) < EPSILON)
                ? MAIN_GRID_COLOR : SECOND_GRID_COLOR;

        float localX = x - cameraTranslate.x;

        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate.x = localX;
        hitbox.transform.scale = { 0.0f, cameraScale.y };
        hitbox.color = color;
        grid->addHitbox("x-" + std::to_string(x), hitbox);
    }

    float startY = H2DE::round(minY) + H2DE::round(std::fmod(minY, step)) * -1;

    for (float y = startY; y < maxY; y += step) {
        const H2DE_ColorRGB& color = (y == 0.0f)
            ? CENTER_GRID_COLOR
            : (H2DE::abs(std::fmod(y, step * MAIN_GRID_SPACING)) < EPSILON)
                ? MAIN_GRID_COLOR : SECOND_GRID_COLOR;

        float localY = y - cameraTranslate.y;

        H2DE_Hitbox hitbox = H2DE_Hitbox();
        hitbox.transform.translate.y = localY;
        hitbox.transform.scale = { cameraScale.x, 0.0f };
        hitbox.color = color;
        grid->addHitbox("y-" + std::to_string(y), hitbox);
    }
}

void H2DE_Camera::updateCrosshair() {
    H2DE_Hitbox crossHairW = H2DE_Hitbox();
    crossHairW.transform.scale = { data.gameWidth * 0.025f, 0.0f };
    crossHairW.color = { 255, 255, 255, 255 };
    grid->addHitbox("cw", crossHairW);

    H2DE_Hitbox crossHairH = H2DE_Hitbox();
    crossHairH.transform.scale = { 0.0f, data.gameWidth * 0.025f };
    crossHairH.color = { 255, 255, 255, 255 };
    grid->addHitbox("ch", crossHairH);
}

// GETTER
bool H2DE_Camera::containsObject(const H2DE_Object* object) const {
    const H2DE_LevelRect world_cameraRect = getWorldRect();
    H2DE_Translate world_objectTranslate = object->getTranslate();

    return world_cameraRect.collides(world_objectTranslate, object->maxRadius);
}

H2DE_Scale H2DE_Camera::getScale(float width) const {
    H2DE_Scale res;

    const H2DE_PixelSize windowSize = engine->window->getSize();

    res.x = width;
    res.y = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x) * width;

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
H2DE_TimelineID H2DE_Camera::setTranslate(const H2DE_Translate& translate, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, data.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

H2DE_TimelineID H2DE_Camera::setGameWidth(float width, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, data.gameWidth, width, duration, easing, [this](float iv) {
        setGameWidth(iv);
    }, completed, pauseSensitive);
}

H2DE_TimelineID H2DE_Camera::setInterfaceWidth(float width, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, data.interfaceWidth, width, duration, easing, [this](float iv) {
        setInterfaceWidth(iv);
    }, completed, pauseSensitive);
}

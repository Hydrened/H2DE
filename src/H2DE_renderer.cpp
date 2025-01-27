#include "H2DE_renderer.h"

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, std::unordered_map<std::string, SDL_Texture*>* t, std::vector<H2DE_LevelObject*>* o) : engine(e), textures(t), objects(o) {
    static bool once = false;
    if (once) throw std::runtime_error("H2DE-109: Can't create more than one renderer");
    once = true;
}

// CLEANUP
H2DE_Renderer::~H2DE_Renderer() {

}

// UPDATE
void H2DE_Renderer::update() {

}

// RENDER
void H2DE_Renderer::render() {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    // 1 => Clear renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2 => Render objects
    for (H2DE_LevelObject* object : *objects) renderObject(object);
    SDL_RenderPresent(renderer);

    // 3 => Clear objects
    for (H2DE_LevelObject* object : *objects) H2DE_DestroyLevelObject(object);
    objects->clear();
}

void H2DE_Renderer::renderObject(H2DE_LevelObject* object) {
    if (object->texture.name != "" && (*textures).find(object->texture.name) != (*textures).end()) renderTexture(object);
    if (object->hitbox.has_value()) renderHitbox(object);
}

void H2DE_Renderer::renderTexture(H2DE_LevelObject* object) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    SDL_Texture* texture = (*textures)[object->texture.name];
    SDL_Rect destRect = lvlToAbs(object->rect.getPos(), object->absolute).makeRect(lvlToAbs(object->rect.getSize()));
    float rotation = object->transform.rotation;
    SDL_Point pivot = lvlToAbs(object->transform.origin, object->absolute);
    SDL_RendererFlip flip = getFlip(object->transform.flip);

    if (object->texture.srcRect.has_value()) {
        SDL_Rect srcRect = object->texture.srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, &pivot, flip);
}

void H2DE_Renderer::renderHitbox(H2DE_LevelObject* object) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    H2DE_LevelHitbox hitbox = object->hitbox.value();
    H2DE_AbsPos offset = lvlToAbs(object->rect.getPos() + hitbox.getPos(), object->absolute);
    H2DE_AbsSize size = lvlToAbs(hitbox.getSize());

    Sint16 offsetX = offset.x;
    Sint16 offsetY = offset.y;

    std::vector<Sint16> vx = { offsetX, static_cast<Sint16>(offsetX + size.w), static_cast<Sint16>(offsetX + size.w), offsetX };
    std::vector<Sint16> vy = { offsetY, offsetY, static_cast<Sint16>(offsetY + size.h), static_cast<Sint16>(offsetY + size.h) };

    polygonColor(renderer, vx.data(), vy.data(), 4, object->texture.color);
}

// GETTER
int H2DE_Renderer::getBlockSize() const {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);
    return H2DE_GetWindowSize(window).w / camSize.w;
}

SDL_RendererFlip H2DE_Renderer::getFlip(H2DE_Flip flip) {
    return (flip == H2DE_NO_FLIP) ? SDL_FLIP_NONE : (flip == H2DE_FLIP_HORIZONTAL) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL;
}

void H2DE_Renderer::whileParent(H2DE_LevelObject* object, std::function<void(H2DE_LevelObject*)> call) const {
    if (!call) return;

    while (true) {
        call(object);
        if (!object->parent.has_value()) break;
        else object = object->parent.value();
    }
}

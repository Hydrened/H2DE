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
    for (H2DE_LevelObject* object : *objects) object->update();
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
}

void H2DE_Renderer::renderObject(H2DE_LevelObject* object) {
    H2DE_LevelObjectData data = object->getData();

    if (data.texture.name != "" && (*textures).find(data.texture.name) != (*textures).end()) renderTexture(data);
    if (data.hitobxes.size() > 0) renderHitboxes(data);
}

void H2DE_Renderer::renderTexture(H2DE_LevelObjectData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    if (data.texture.size.w == 0.0f || data.texture.size.h == 0.0f) return;
    if (data.texture.color.a == 0) return;

    SDL_Texture* texture = (*textures)[data.texture.name];
    SDL_Rect destRect = lvlToAbs(data.pos, data.absolute).makeRect(lvlToAbs(data.texture.size));
    float rotation = data.transform.rotation;
    SDL_Point pivot = lvlToAbs(data.transform.origin, data.absolute);
    SDL_RendererFlip flip = getFlip(data.transform.flip);

    SDL_SetTextureColorMod(texture, data.texture.color.r, data.texture.color.g, data.texture.color.b);
    SDL_SetTextureAlphaMod(texture, data.texture.color.a);
    SDL_SetTextureScaleMode(texture, getScaleMode(data.texture.scaleMode));

    if (data.texture.srcRect.has_value()) {
        SDL_Rect srcRect = data.texture.srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, &pivot, flip);
}

void H2DE_Renderer::renderHitboxes(H2DE_LevelObjectData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    for (H2DE_Hitbox hitbox : data.hitobxes) {
        if (hitbox.rect.w == 0.0f || hitbox.rect.h == 0.0f) continue;
        if (hitbox.color.a == 0) continue;

        H2DE_AbsPos offset = lvlToAbs(data.pos + hitbox.rect.getPos(), data.absolute);
        H2DE_AbsSize size = lvlToAbs(hitbox.rect.getSize());

        Sint16 offsetX = offset.x;
        Sint16 offsetY = offset.y;

        std::vector<Sint16> vx = { offsetX, static_cast<Sint16>(offsetX + size.w), static_cast<Sint16>(offsetX + size.w), offsetX };
        std::vector<Sint16> vy = { offsetY, offsetY, static_cast<Sint16>(offsetY + size.h), static_cast<Sint16>(offsetY + size.h) };

        polygonColor(renderer, vx.data(), vy.data(), 4, hitbox.color);
    }
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

SDL_ScaleMode H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) {
    return (scaleMode == H2DE_SCALE_MODE_LINEAR) ? SDL_ScaleModeLinear : (scaleMode == H2DE_SCALE_MODE_NEAREST) ? SDL_ScaleModeNearest : SDL_ScaleModeBest;
}

void H2DE_Renderer::whileParent(H2DE_LevelObjectData data, std::function<void(H2DE_LevelObjectData)> call) const {
    if (!call) return;

    while (true) {
        call(data);
        if (!data.parent.has_value()) break;
        else data = data.parent.value()->getData();
    }
}

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

// RENDER
void H2DE_Renderer::render() {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    // 1 => Clear renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2 => Sort objects per their index
    std::map<int, std::vector<H2DE_LevelObject*>> groupedIndexes;
    for (H2DE_LevelObject* object : *objects) groupedIndexes[H2DE_GetObjectData(object)->index].push_back(object);

    std::vector<H2DE_LevelObject*> sortedObjects;
    for (auto& [index, objs] : groupedIndexes) {
        sort(objs.begin(), objs.end(), &H2DE_Renderer::isPositionGreater);
        for (int i = 0; i < objs.size(); i++) sortedObjects.push_back(objs[i]);
    }

    // 3 => Render objects
    for (H2DE_LevelObject* object : sortedObjects) renderObject(object);
    SDL_RenderPresent(renderer);

    // 4 => Clear
    groupedIndexes.clear();
    sortedObjects.clear();
}

void H2DE_Renderer::renderObject(H2DE_LevelObject* object) {
    H2DE_LevelObjectData data = *H2DE_GetObjectData(object);

    if (data.texture.name != "" && (*textures).find(data.texture.name) != (*textures).end()) renderTexture(data);
    if (data.hitboxes.size() > 0) renderHitboxes(data);
}

void H2DE_Renderer::renderTexture(H2DE_LevelObjectData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    if (data.texture.size.w == 0.0f || data.texture.size.h == 0.0f) return;
    if (data.texture.color.a == 0) return;

    H2DE_LevelPos posFromParents = getPosFromParents(data);

    SDL_Texture* texture = (*textures)[data.texture.name];
    SDL_Rect destRect = lvlToAbs(posFromParents, data.absolute).makeRect(lvlToAbs(data.texture.size));
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

    for (H2DE_Hitbox hitbox : data.hitboxes) {
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

H2DE_LevelPos H2DE_Renderer::getPosFromParents(H2DE_LevelObjectData data) const {
    H2DE_LevelPos res = { 0.0f, 0.0f };
    H2DE_Renderer::whileParent(&data, [&res](H2DE_LevelObjectData* d) {
        res = res + d->pos;
    });
    return res;
}

bool H2DE_Renderer::isPositionGreater(H2DE_LevelObject* obj1, H2DE_LevelObject* obj2) {
    H2DE_LevelObjectData* objData1 = H2DE_GetObjectData(obj1);
    H2DE_LevelObjectData* objData2 = H2DE_GetObjectData(obj2);

    bool equals = (objData1->pos.x == objData2->pos.x);
    if (equals) return objData1->pos.y < objData2->pos.y;
    else return objData1->pos.x < objData2->pos.x;
}

void H2DE_Renderer::whileParent(H2DE_LevelObjectData* data, std::function<void(H2DE_LevelObjectData*)> call) {
    if (!call) return;

    while (true) {
        call(data);
        if (!data->parent.has_value()) break;
        else data = H2DE_GetObjectData(data->parent.value());
    }
}

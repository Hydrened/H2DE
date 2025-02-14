#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, std::unordered_map<std::string, SDL_Texture*>* t, std::vector<H2DE_LevelObject*>* o, std::vector<H2DE_Button*>* b) : engine(e), textures(t), objects(o), buttons(b) {
    static bool once = false;
    if (once) throw std::runtime_error("H2DE-109: Can't create more than one renderer");
    once = true;
}

// CLEANUP
H2DE_Renderer::~H2DE_Renderer() {

}

// EVENTS
void H2DE_Renderer::debugObjectNumber(bool state) {
    debugNumber = state;
}

void H2DE_Renderer::debugObjectHitboxes(bool state) {
    debugHitboxes = state;
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

    // 4 => Render buttons
    for (H2DE_Button* button : *buttons) renderButton(button);
    SDL_RenderPresent(renderer);

    // 5 => Debug number
    if (debugNumber) {
        std::cout << "H2DE => Rendered " << renderedObjects << " objects" << std::endl;
        renderedObjects = 0;
    }

    // 6 => Clear
    groupedIndexes.clear();
    sortedObjects.clear();
}

void H2DE_Renderer::renderObject(H2DE_LevelObject* object) {
    static H2DE_Camera* camera = H2DE_GetCamera(engine);

    H2DE_LevelObjectData data = *H2DE_GetObjectData(object);
    H2DE_Sprite* sprite = dynamic_cast<H2DE_Sprite*>(data.texture);
    if (sprite) {
        H2DE_SpriteData spriteData = H2DE_GetSpriteData(sprite);
        bool isPausedAndUpdates = spriteData.updateOnPause && H2DE_IsPaused(engine);
        bool notPausedAndDontUpdate = !spriteData.updateOnPause && !H2DE_IsPaused(engine);
        if (isPausedAndUpdates || notPausedAndDontUpdate) data.texture->update();
    }
    std::string texture = data.texture->get();
    
    bool textureIsNull = texture == "";
    bool textureExists = (*textures).find(texture) != (*textures).end();
    bool textureIsOnScreen = H2DE_CameraContains(camera, data.pos.makeHitbox(data.texture->getData()->size));
    if (!textureIsNull && textureExists && textureIsOnScreen) renderObjectTexture(data);

    bool atLeastOneHitbox = data.hitboxes.size() > 0;
    if (debugHitboxes && atLeastOneHitbox) renderObjectHitboxes(data);
}

void H2DE_Renderer::renderObjectTexture(H2DE_LevelObjectData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    H2DE_TextureData* textureData = data.texture->getData();

    if (textureData->size.w == 0.0f || textureData->size.h == 0.0f) return;
    if (textureData->color.a == 0) return;

    H2DE_LevelPos posFromParents = getPosFromParents(data);

    SDL_Texture* texture = (*textures)[data.texture->get()];
    SDL_Rect destRect = lvlToAbs(posFromParents, data.absolute).makeRect(lvlToAbs(textureData->size));
    float rotation = data.transform.rotation;
    SDL_Point pivot = lvlToAbs(data.transform.origin, data.absolute);
    SDL_RendererFlip flip = getFlip(data.transform.flip);

    SDL_SetTextureColorMod(texture, textureData->color.r, textureData->color.g, textureData->color.b);
    SDL_SetTextureAlphaMod(texture, textureData->color.a);
    SDL_SetTextureScaleMode(texture, getScaleMode(textureData->scaleMode));

    if (textureData->srcRect.has_value()) {
        SDL_Rect srcRect = textureData->srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, &pivot, flip);

    renderedObjects++;
}

void H2DE_Renderer::renderObjectHitboxes(H2DE_LevelObjectData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);
    static H2DE_Camera* camera = H2DE_GetCamera(engine);

    for (const auto& [key, hitbox] : data.hitboxes) {
        if (hitbox.rect.w == 0.0f || hitbox.rect.h == 0.0f) continue;
        if (hitbox.color.a == 0) continue;

        bool hitboxIsOnScreen = H2DE_CameraContains(camera, hitbox.rect + data.pos);
        if (!hitboxIsOnScreen) continue;

        H2DE_AbsPos offset = lvlToAbs(data.pos + hitbox.rect.getPos(), data.absolute);
        H2DE_AbsSize size = lvlToAbs(hitbox.rect.getSize());

        Sint16 offsetX = offset.x;
        Sint16 offsetY = offset.y;

        std::vector<Sint16> vx = { offsetX, static_cast<Sint16>(offsetX + size.w), static_cast<Sint16>(offsetX + size.w), offsetX };
        std::vector<Sint16> vy = { offsetY, offsetY, static_cast<Sint16>(offsetY + size.h), static_cast<Sint16>(offsetY + size.h) };

        polygonColor(renderer, vx.data(), vy.data(), 4, hitbox.color);
        renderedObjects++;
    }
}

void H2DE_Renderer::renderButton(H2DE_Button* button) {
    H2DE_ButtonData data = *H2DE_GetButtonData(button);
    H2DE_Sprite* sprite = dynamic_cast<H2DE_Sprite*>(data.texture);
    if (sprite) {
        H2DE_SpriteData spriteData = H2DE_GetSpriteData(sprite);
        bool isPausedAndUpdates = spriteData.updateOnPause && H2DE_IsPaused(engine);
        bool notPausedAndDontUpdate = !spriteData.updateOnPause && !H2DE_IsPaused(engine);
        if (isPausedAndUpdates || notPausedAndDontUpdate) data.texture->update();
    }
    std::string texture = data.texture->get();
    if (texture != "" && (*textures).find(texture) != (*textures).end()) renderButtonTexture(data);
}

void H2DE_Renderer::renderButtonTexture(H2DE_ButtonData data) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);

    H2DE_TextureData* textureData = data.texture->getData();

    if (textureData->size.w == 0.0f || textureData->size.h == 0.0f) return;
    if (textureData->color.a == 0) return;
    
    SDL_Texture* texture = (*textures)[data.texture->get()];
    SDL_Rect destRect = lvlToAbs(data.pos, true).makeRect(lvlToAbs(textureData->size));
    SDL_Point pivot = { 0, 0 };

    SDL_SetTextureColorMod(texture, textureData->color.r, textureData->color.g, textureData->color.b);
    SDL_SetTextureAlphaMod(texture, textureData->color.a);
    SDL_SetTextureScaleMode(texture, getScaleMode(textureData->scaleMode));

    if (textureData->srcRect.has_value()) {
        SDL_Rect srcRect = textureData->srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0f, &pivot, SDL_FLIP_NONE);
    } else SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, 0.0f, &pivot, SDL_FLIP_NONE);

    renderedObjects++;
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

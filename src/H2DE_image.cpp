#include "H2DE_renderer.h"

void H2DE_Renderer::renderImage(H2DE_GraphicObject* object) {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static SDL_Renderer* renderer = H2DE_GetWindowsRenderer(window);


    auto it = (*textures).find(object->image.texture);
    if (it == (*textures).end()) return;

    // 1 => Translate
    H2DE_LevelPos translatedPos = getTranslatedPos(object);

    // // 2 => Scale
    // H2DE_LevelPos scaledPos = getScaledPos(object);




    H2DE_AbsPos pos = lvlToAbs(translatedPos, object->absolute);
    H2DE_AbsSize size = lvlToAbs(object->image.size);
    SDL_Point pivot = lvlToAbs(object->transform.origin, object->absolute);

    SDL_Texture* texture = (*textures)[object->image.texture];
    SDL_Rect destRect = pos.makeRect(size);
    float rotation = object->transform.rotation;
    SDL_RendererFlip flip = getFlip(object);

    if (object->image.srcRect.has_value()) {
        SDL_Rect srcRect = object->image.srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &pivot, flip);
}

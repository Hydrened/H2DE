#include "H2DE_renderer.h"

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, std::unordered_map<std::string, SDL_Texture*>* t, std::unordered_map<std::string, Mix_Chunk*>* s, std::vector<H2DE_GraphicObject*>* o) : engine(e), textures(t), sounds(s), objects(o) {
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
    for (H2DE_GraphicObject* object : *objects) {
        switch (object->type) {
            case IMAGE: renderImage(object); break;
            case POLYGON: renderPolygon(object); break;
            case CIRCLE: renderCircle(object); break;
            default: break;
        }
    }
    SDL_RenderPresent(renderer);

    // 3 => Clear objects
    for (H2DE_GraphicObject* object : *objects) H2DE_DestroyGraphicObject(object);
    objects->clear();
}

// GETTER
int H2DE_Renderer::getBlockSize() const {
    static H2DE_Window* window = H2DE_GetWindow(engine);
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);
    return H2DE_GetWindowSize(window).w / camSize.w;
}

H2DE_LevelPos H2DE_Renderer::getPos(H2DE_GraphicObject* object) const {
    return (object->type == IMAGE) ? object->image.pos : (object->type == POLYGON) ? object->polygon.pos : object->circle.pos;
}

SDL_RendererFlip H2DE_Renderer::getFlip(H2DE_GraphicObject* object) const {
    return (object->transform.flip == H2DE_FLIP_HORIZONTAL) ? SDL_FLIP_HORIZONTAL : (object->transform.flip == H2DE_FLIP_VERTICAL) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
}

void H2DE_Renderer::whileParent(H2DE_GraphicObject* object, std::function<void(H2DE_GraphicObject*)> call) const {
    if (!call) return;

    while (true) {
        call(object);
        if (!object->parent.has_value()) break;
        else object = object->parent.value();
    }
}

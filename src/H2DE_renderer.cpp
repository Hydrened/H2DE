#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_bar_object.h"
#include "H2DE/H2DE_basic_object.h"
#include "H2DE/H2DE_button_object.h"
#include "H2DE/H2DE_text_object.h"

// INIT
H2DE_Engine::H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) : engine(e), renderer(r), objects(o) {
    
}

// CLEANUP
H2DE_Engine::H2DE_Renderer::~H2DE_Renderer() {
    destroyTextures();
    destroySounds();
}

void H2DE_Engine::H2DE_Renderer::destroyTextures() {
    for (const auto& [name, texture] : textures) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    textures.clear();
}

void H2DE_Engine::H2DE_Renderer::destroySounds() {
    for (const auto& [name, sound] : sounds) {
        if (sound) {
            // delete sound; // crash -1073740940
        }
    }

    sounds.clear();
}

// RENDER
void H2DE_Engine::H2DE_Renderer::render() {
    clearRenderer();
    sortObjects();
    renderObjects();
    // throw std::runtime_error("Anti-loop");
}

void H2DE_Engine::H2DE_Renderer::clearRenderer() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void H2DE_Engine::H2DE_Renderer::sortObjects() {
    std::sort(objects.begin(), objects.end(), [](H2DE_Object* a, H2DE_Object* b) {
        int indexA = H2DE_GetObjectIndex(a);
        int indexB = H2DE_GetObjectIndex(b);

        if (indexA == indexB) {
            return H2DE_Renderer::isPositionGreater(a, b);
        }

        return indexA < indexB;
    });
}

void H2DE_Engine::H2DE_Renderer::renderObjects() const {
    for (H2DE_Object* object : objects) {
        renderObject(object);
    }
    SDL_RenderPresent(renderer);
}

void H2DE_Engine::H2DE_Renderer::renderObject(H2DE_Object* object) const {
    if (H2DE_CameraContainsObject(engine, object)) {
        H2DE_BarObject* bar = dynamic_cast<H2DE_BarObject*>(object);
        H2DE_BasicObject* basic = dynamic_cast<H2DE_BasicObject*>(object);
        H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(object);
        H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);

        if (bar) {

        } else if (basic) {

        } else if (button) {

        } else if (text) {

        } else throw std::runtime_error("H2DE-401: Can't find polymorphic class");
    }

    H2DE_LevelPos pos = H2DE_GetObjectPos(object);
    bool absolute = H2DE_IsObjectAbsolute(object);

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (isVisible(hitbox.color)) {
            if (H2DE_CameraContainsHitbox(engine, hitbox, absolute)) {
                renderHitbox(pos, hitbox, absolute);
            }
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderHitbox(const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute) const {
    H2DE_AbsPos absPos = lvlToAbs(pos + hitbox.rect.getPos(), absolute);
    H2DE_AbsSize absSize = lvlToAbs(hitbox.rect.getSize());

    Sint16 minX = absPos.x;
    Sint16 maxX = absPos.x + absSize.x;
    Sint16 minY = absPos.y;
    Sint16 maxY = absPos.y + absSize.y;

    std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(hitbox.color));
}

// GETTER
bool H2DE_Engine::H2DE_Renderer::isPositionGreater(H2DE_Object* object1, H2DE_Object* object2) {
    H2DE_LevelPos object1Pos = H2DE_GetObjectPos(object1);
    H2DE_LevelPos object2Pos = H2DE_GetObjectPos(object2);

    bool equalsX = object1Pos.x == object2Pos.x;
    return (equalsX) ? object1Pos.y < object2Pos.y : object1Pos.x < object2Pos.x;
}

bool H2DE_Engine::H2DE_Renderer::isVisible(const H2DE_ColorRGB& color) {
    return color.a != 0;
}

const unsigned int H2DE_Engine::H2DE_Renderer::getBlockSize() const {
    return H2DE_GetWindowSize(engine).x / H2DE_GetCameraSize(engine).x;
}

H2DE_AbsPos H2DE_Engine::H2DE_Renderer::lvlToAbs(const H2DE_LevelPos& pos, bool absolute) const {
    H2DE_LevelPos camPos = H2DE_GetCameraPos(engine);
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>((pos.x - ((absolute) ? 0.0f : camPos.x)) * blockSize),
        static_cast<int>((pos.y - ((absolute) ? 0.0f : camPos.y)) * blockSize),
    };
}

H2DE_AbsSize H2DE_Engine::H2DE_Renderer::lvlToAbs(const H2DE_LevelSize& size) const {
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(size.x * blockSize),
        static_cast<int>(size.y * blockSize),
    };
}

// SETTER
void H2DE_Engine::H2DE_Renderer::setTextures(std::unordered_map<std::string, SDL_Texture*> t) {
    destroyTextures();
    textures = t;
}

void H2DE_Engine::H2DE_Renderer::setSounds(std::unordered_map<std::string, Mix_Chunk*> s) {
    destroySounds();
    sounds = s;
}

#include "H2DE/engine/H2DE_object_manager.h"
#include "H2DE/engine/H2DE_geometry.h"

// INIT
H2DE_ObjectManager::H2DE_ObjectManager(H2DE_Engine* e) : engine(e) {
    buttons.reserve(100);
    inputs.reserve(100);
}

// EVENTS
void H2DE_ObjectManager::handleEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            handleEvents_buttons_mouseDown(event);
            handleEvents_inputs_mouseDown(event);
            break;

        case SDL_MOUSEBUTTONUP:
            handleEvents_buttons_mouseUp(event);
            break;

        case SDL_MOUSEMOTION:
            handleEvents_buttons_mouseMotion();
            handleEvents_inputs_mouseMotion();
            break;

        case SDL_KEYDOWN:
            handleEvents_inputs_keydown(event);
            break;

        case SDL_TEXTINPUT:
            handleEvents_inputs_keydown_default(event);
            break;

        default: return;
    }
}

// ACTIONS
template void H2DE_ObjectManager::refreshBuffer<H2DE_ButtonObject>(std::vector<H2DE_ButtonObject*>& buffer, const std::vector<H2DE_Object*>& objects);
template void H2DE_ObjectManager::refreshBuffer<H2DE_InputObject>(std::vector<H2DE_InputObject*>& buffer, const std::vector<H2DE_Object*>& objects);

template<typename H2DE_ObjectType>
void H2DE_ObjectManager::refreshBuffer(std::vector<H2DE_ObjectType*>& buffer, const std::vector<H2DE_Object*>& objects) {
    buffer.clear();

    for (H2DE_Object* object : objects) {
        H2DE_ObjectType* castedObject = dynamic_cast<H2DE_ObjectType*>(object);

        if (castedObject != H2DE_NULL_OBJECT) {
            buffer.push_back(castedObject);
        }
    }

    std::sort(buffer.begin(), buffer.end(), [](H2DE_ObjectType* a, H2DE_ObjectType* b) {
        int indexA = a->getIndex();
        int indexB = b->getIndex();

        if (indexA == indexB) {
            return H2DE_Engine::_isPositionGreater(a, b);
        }

        return (indexA > indexB);
    });
}

// GETTER

// -- default
bool H2DE_ObjectManager::isMouseCollidingObject(H2DE_Object* object) const {
    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    const H2DE_Translate mousePos = (object->_objectData.absolute) ? mouseInterfacePos : mouseGamePos;

    for (const auto& [name, hitbox] : object->_hitboxes) {
        const H2DE_LevelRect inputRect = G::getHitboxRect(object, hitbox);

        if (inputRect.collides(mousePos)) {
            return true;
        }
    }

    return false;
}

// -- static
H2DE_MouseButton H2DE_ObjectManager::getH2DEButton(Uint8 sdlButton) {
    switch (sdlButton) {
        case SDL_BUTTON_LEFT: return H2DE_MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT: return H2DE_MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE: return H2DE_MOUSE_BUTTON_MIDDLE;
        default: return H2DE_MOUSE_BUTTON_LEFT;
    }
}

size_t H2DE_ObjectManager::getFirstSpaceIndex(const std::string& text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text.at(i) == ' ' || text.at(i) == '\n') {
            return i;
        }
    }

    return -1;
}

size_t H2DE_ObjectManager::getLastSpaceIndex(const std::string& text) {
    for (size_t i = text.length() - 1; i != 0; i--) {
        if (text.at(i) == ' ' || text.at(i) == '\n') {
            return i;
        }
    }

    return -1;
}

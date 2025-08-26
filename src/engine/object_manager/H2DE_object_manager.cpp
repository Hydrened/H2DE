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
            handleEvents_mouseDown(event);
            break;

        case SDL_MOUSEBUTTONUP:
            handleEvents_mouseUp(event);
            break;

        case SDL_MOUSEMOTION:
            handleEvents_mouseMotion();
            break;

        case SDL_KEYDOWN:
            handleEvents_keydown_input(event);
            break;

        case SDL_TEXTINPUT:
            handleEvents_keydown_input_default(event);
            break;

        default: return;
    }
}

// -- mouse down
void H2DE_ObjectManager::handleEvents_mouseDown(SDL_Event event) {
    H2DE_Object* clickedObject = handleEvents_mouseDown_getClickedObject(event);

    H2DE_ButtonObject* clickedButton = dynamic_cast<H2DE_ButtonObject*>(clickedObject);
    handleEvents_mouseDown_button(clickedButton);

    bool leftClick = ((H2DE_ObjectManager::getH2DEButton(event.button.button) & H2DE_MOUSE_BUTTON_LEFT) != 0);
    if (leftClick) {
        H2DE_InputObject* clickedInput = dynamic_cast<H2DE_InputObject*>(clickedObject);
        handleEvents_mouseDown_input(clickedInput);
    }
}

H2DE_Object* H2DE_ObjectManager::handleEvents_mouseDown_getClickedObject(SDL_Event event) {
    for (H2DE_Object* object : hoverObjects) {
        if (!isMouseCollidingObject(object)) {
            continue;
        }

        H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(object);

        H2DE_MouseButton mouseButton = (button != H2DE_NULL_OBJECT)
            ? button->_buttonObjectData.mouseButton
            : H2DE_MOUSE_BUTTON_LEFT;

        bool isCorrectClick = ((mouseButton & H2DE_ObjectManager::getH2DEButton(event.button.button)) != 0);
        if (isCorrectClick) {
            return object;
        }
    }

    return H2DE_NULL_OBJECT;
}

// -- mouse up
void H2DE_ObjectManager::handleEvents_mouseUp(SDL_Event event) {
    handleEvents_mouseUp_button(event);
}

// -- mouse motion
void H2DE_ObjectManager::handleEvents_mouseMotion() {
    H2DE_Object* oldHoveredObject = hoveredObject;
    H2DE_Object* newHoveredObject = handleEvents_mouseMotion_getHoveredObject();

    bool hovering = (newHoveredObject != H2DE_NULL_OBJECT);

    if (hovering) {
        handleEvents_mouseMotion_hoveringObject(newHoveredObject);
    } else {
        handleEvents_mouseMotion_notHoveringObject();
    }

    handleEvents_mouseMotion_buttons(oldHoveredObject);
}

void H2DE_ObjectManager::handleEvents_mouseMotion_hoveringObject(H2DE_Object* object) {
    static H2DE_Window* window = engine->_window;

    bool isAnObjectAlreadyHovered = (hoveredObject != H2DE_NULL_OBJECT);
    if (!isAnObjectAlreadyHovered) {
        oldCursor = window->_currentCursor;
    }

    H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(object);
    H2DE_Cursor cursor = (button != H2DE_NULL_OBJECT)
        ? button->_buttonObjectData.cursor
        : H2DE_CURSOR_IBEAM;

    window->setCursor(cursor);
    hoveredObject = object;
}

H2DE_Object* H2DE_ObjectManager::handleEvents_mouseMotion_getHoveredObject() {
    for (H2DE_Object* object : hoverObjects) {
        if (isMouseCollidingObject(object)) {
            return object;
        }
    }

    return H2DE_NULL_OBJECT;
}

void H2DE_ObjectManager::handleEvents_mouseMotion_notHoveringObject() {
    static H2DE_Window* window = engine->_window;

    window->setCursor(oldCursor);
    hoveredObject = H2DE_NULL_OBJECT;
}

// ACTIONS
void H2DE_ObjectManager::refreshHoverObjectBuffer() {
    hoveredObject = H2DE_NULL_OBJECT;

    hoverObjects.clear();
    hoverObjects.reserve(buttons.size() + inputs.size());

    std::ranges::copy(buttons, std::back_inserter(hoverObjects));
    std::ranges::copy(inputs, std::back_inserter(hoverObjects));

    sortBuffer(hoverObjects);
}

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

    sortBuffer(buffer);
}

template void H2DE_ObjectManager::sortBuffer<H2DE_Object>(std::vector<H2DE_Object*>& buffer);
template void H2DE_ObjectManager::sortBuffer<H2DE_ButtonObject>(std::vector<H2DE_ButtonObject*>& buffer);
template void H2DE_ObjectManager::sortBuffer<H2DE_InputObject>(std::vector<H2DE_InputObject*>& buffer);

template<typename H2DE_ObjectType>
void H2DE_ObjectManager::sortBuffer(std::vector<H2DE_ObjectType*>& buffer) {
    std::sort(buffer.begin(), buffer.end(), [](H2DE_ObjectType* a, H2DE_ObjectType* b) {
        int indexA = a->getIndex();
        int indexB = b->getIndex();

        if (indexA == indexB) {
            return H2DE_Engine::_isTranslateGreater(a, b);
        }

        return (indexA > indexB);
    });
}

// GETTER

// -- default
bool H2DE_ObjectManager::isMouseCollidingObject(H2DE_Object* object) const {
    const H2DE_Translate mousePos = (object->_objectData.absolute)
        ? engine->getMouseGameTranslate()
        : engine->getMouseInterfaceTranslate();

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

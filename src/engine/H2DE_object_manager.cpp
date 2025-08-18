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
            handleEvents_inputs_keydown_normalCharacter(event);
            break;

        default: return;
    }
}

// -- buttons

// -- -- mouse  down
void H2DE_ObjectManager::handleEvents_buttons_mouseDown(SDL_Event event) {
    mouseDownButton = handleEvents_buttons_mouseDown_getMouseDownButton(event);

    if (mouseDownButton != nullptr) {
        handleEvents_buttons_mouseDown_mouseDownButton();
    }
}

H2DE_ButtonObject* H2DE_ObjectManager::handleEvents_buttons_mouseDown_getMouseDownButton(SDL_Event event) {
    H2DE_MouseButton mouseButton = H2DE_ObjectManager::getH2DEButton(event.button.button);

    for (H2DE_ButtonObject* button : getValidButtons()) {

        if (!(mouseButton & button->getMouseButton())) {
            continue;
        }

        if (isMouseCollidingObject(button)) {
            return button;
        }
    }

    return nullptr;
}

void H2DE_ObjectManager::handleEvents_buttons_mouseDown_mouseDownButton() {
    if (mouseDownButton->_buttonObjectData.onMouseDown) {
        mouseDownButton->_buttonObjectData.onMouseDown(mouseDownButton->_eventData);
    }
}

// -- -- mouse up
void H2DE_ObjectManager::handleEvents_buttons_mouseUp(SDL_Event event) {
    if (handleEvents_buttons_mouseUp_isOnHoveredButton(event)) {
        handleEvents_buttons_mouseDown_mouseUpButton();
    }
}

bool H2DE_ObjectManager::handleEvents_buttons_mouseUp_isOnHoveredButton(SDL_Event event) {
    if (mouseDownButton == nullptr) {
        return false;
    }

    if (mouseDownButton->_disabled || !mouseDownButton->_buttonObjectData.onMouseUp) {
        return false;
    }

    H2DE_MouseButton mouseButton = H2DE_ObjectManager::getH2DEButton(event.button.button);
    if (!(mouseButton & mouseDownButton->getMouseButton())) {
        return false;
    }

    return isMouseCollidingObject(mouseDownButton);
}

void H2DE_ObjectManager::handleEvents_buttons_mouseDown_mouseUpButton() {
    if (mouseDownButton->_buttonObjectData.onMouseUp) {
        mouseDownButton->_buttonObjectData.onMouseUp(mouseDownButton->_eventData);
    }

    mouseDownButton = nullptr;
}

// -- -- mouse motion
void H2DE_ObjectManager::handleEvents_buttons_mouseMotion() {
    H2DE_ButtonObject* oldHoveredButton = hoveredButton;

    hoveredButton = handleEvents_buttons_mouseMotion_getHoveredButton();

    if (hoveredButton == nullptr) {
        handleEvents_buttons_mouseMotion_blurButton(oldHoveredButton);
        return;
    }

    bool isButtonHoveredNew = (hoveredButton != oldHoveredButton);
    if (isButtonHoveredNew) {
        handleEvents_buttons_mouseMotion_hoverNewButton(oldHoveredButton);
    }
}

H2DE_ButtonObject* H2DE_ObjectManager::handleEvents_buttons_mouseMotion_getHoveredButton() {
    for (H2DE_ButtonObject* button : getValidButtons()) {
        if (isMouseCollidingObject(button)) {
            return button;
        }
    }

    return nullptr;
}

void H2DE_ObjectManager::handleEvents_buttons_mouseMotion_blurButton(H2DE_ButtonObject* button) {
    static H2DE_Window* window = engine->_window;

    if (button == nullptr) {
        return;
    }

    if (button->_buttonObjectData.onBlur) {
        button->_buttonObjectData.onBlur(button->_eventData);
    }

    window->_setHoverCursor(oldCursor);
}

void H2DE_ObjectManager::handleEvents_buttons_mouseMotion_hoverNewButton(H2DE_ButtonObject* oldHoveredButton) {
    static H2DE_Window* window = engine->_window;

    if (oldHoveredButton != nullptr) {
        if (oldHoveredButton->_buttonObjectData.onBlur) {
            oldHoveredButton->_buttonObjectData.onBlur(oldHoveredButton->_eventData);
        }
    }

    if (hoveredButton->_buttonObjectData.onHover) {
        hoveredButton->_buttonObjectData.onHover(hoveredButton->_eventData);
    }

    window->_setHoverCursor(hoveredButton->_buttonObjectData.cursor);
}

// -- inputs

// -- -- mouse down
void H2DE_ObjectManager::handleEvents_inputs_mouseDown(SDL_Event event) {
    H2DE_MouseButton mouseButton = H2DE_ObjectManager::getH2DEButton(event.button.button);

    if ((mouseButton & H2DE_MOUSE_BUTTON_LEFT_AND_RIGHT) == 0) {
        return;
    }

    H2DE_InputObject* oldFocusedInput = focusedInput;

    if (!handleEvents_inputs_mouseDown_isMouseCollidingInput()) {
        handleEvents_inputs_mouseDown_blurFocusedInput();
        return;
    }

    bool isInputFocusedNew = (focusedInput != oldFocusedInput);
    if (isInputFocusedNew) {
        handleEvents_inputs_mouseDown_focusNewInput(oldFocusedInput);
    }

    int letterIndex = handleEvents_inputs_mouseDown_getFocusedInputLetterIndex();
    focusedInput->_cursorPosition = letterIndex;
}

bool H2DE_ObjectManager::handleEvents_inputs_mouseDown_isMouseCollidingInput() {
    for (H2DE_InputObject* input : getValidInputs()) {

        if (isMouseCollidingObject(input)) {
            focusedInput = input;
            return true;
        }
    }

    return false;
}

void H2DE_ObjectManager::handleEvents_inputs_mouseDown_blurFocusedInput() {
    if (focusedInput != nullptr) {
        if (focusedInput->_inputObjectData.onBlur) {
            focusedInput->_inputObjectData.onBlur(focusedInput->_eventData);
        }
    }

    focusedInput = nullptr;
}

void H2DE_ObjectManager::handleEvents_inputs_mouseDown_focusNewInput(H2DE_InputObject* oldFocusedInput) {
    static H2DE_Window* window = engine->_window;
    
    if (oldFocusedInput != nullptr) {
        if (oldFocusedInput->_inputObjectData.onBlur) {
            oldFocusedInput->_inputObjectData.onBlur(oldFocusedInput->_eventData);
        }
    }

    if (focusedInput->_inputObjectData.onFocus) {
        focusedInput->_inputObjectData.onFocus(focusedInput->_eventData);
    }

    window->_setHoverCursor(H2DE_CURSOR_IBEAM);
}

int H2DE_ObjectManager::handleEvents_inputs_mouseDown_getFocusedInputLetterIndex() {
    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    const float relativeMouseTranslateX = ((focusedInput->_objectData.absolute) ? mouseInterfacePos.x : mouseGamePos.x) - focusedInput->_objectData.transform.translate.x;
    const std::vector<H2DE_Surface*>& letterSurfaces = focusedInput->_textObject->_surfaceBuffers;

    for (int i = 0; i < letterSurfaces.size(); i++) {
        H2DE_Surface* letterSurface = letterSurfaces.at(i);

        const float letterSurfaceX = letterSurface->getTranslate().x;

        if (letterSurfaceX > relativeMouseTranslateX) {
            return i;
        }
    }
    
    return letterSurfaces.size();
}

// -- -- mouse motion
void H2DE_ObjectManager::handleEvents_inputs_mouseMotion() {
    static H2DE_Window* window = engine->_window;

    H2DE_InputObject* oldHoveredInput = hoveredInput;

    hoveredInput = handleEvents_inputs_mouseMotion_getHoveredInput();

    bool isInputHoveredNew = (hoveredInput != oldHoveredInput);

    if (hoveredInput == nullptr && isInputHoveredNew) {
        window->_setHoverCursor(oldCursor);
        return;
    }

    if (isInputHoveredNew) {
        window->_setHoverCursor(H2DE_CURSOR_IBEAM);
    }
}

H2DE_InputObject* H2DE_ObjectManager::handleEvents_inputs_mouseMotion_getHoveredInput() {
    for (H2DE_InputObject* input : getValidInputs()) {
        if (isMouseCollidingObject(input)) {
            return input;
        }
    }

    return nullptr;
}

// -- -- keydown
void H2DE_ObjectManager::handleEvents_inputs_keydown(SDL_Event event) {
    if (focusedInput == nullptr) {
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
            handleEvents_inputs_keydown_deleteCharacter();
            break;

        case SDLK_DELETE:
            break;

        case SDLK_ESCAPE:
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            break;

        case SDLK_LEFT:
            break;

        case SDLK_RIGHT:
            break;

        default: break;
    }
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_normalCharacter(SDL_Event event) {
    int cursorPosition = focusedInput->_cursorPosition;
    if (cursorPosition == -1) {
        return;
    }

    char c = event.text.text[0];
    std::string inputText = focusedInput->getText();

    if (cursorPosition <= inputText.size()) {
        inputText.insert(inputText.begin() + cursorPosition, c);
    }

    focusedInput->setText(inputText);
    focusedInput->_cursorPosition++;
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_deleteCharacter() {
    int cursorPosition = focusedInput->_cursorPosition;
    if (cursorPosition == -1) {
        return;
    }

    int characterIndex = cursorPosition - 1;
    if (characterIndex < 0) {
        return;
    }

    std::string inputText = focusedInput->getText();

    if (cursorPosition <= inputText.size()) {
        inputText.erase(characterIndex, 1);
    }

    focusedInput->setText(inputText);
    focusedInput->_cursorPosition--;
}

// ACTIONS
template void H2DE_ObjectManager::refreshBuffer<H2DE_ButtonObject>(std::vector<H2DE_ButtonObject*>& buffer, const std::vector<H2DE_Object*>& objects);
template void H2DE_ObjectManager::refreshBuffer<H2DE_InputObject>(std::vector<H2DE_InputObject*>& buffer, const std::vector<H2DE_Object*>& objects);

template<typename H2DE_ObjectType>
void H2DE_ObjectManager::refreshBuffer(std::vector<H2DE_ObjectType*>& buffer, const std::vector<H2DE_Object*>& objects) {
    buffer.clear();

    for (H2DE_Object* object : objects) {
        H2DE_ObjectType* castedObject = dynamic_cast<H2DE_ObjectType*>(object);

        if (castedObject != nullptr) {
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

void H2DE_ObjectManager::refreshButtonBuffer(const std::vector<H2DE_Object*>& objects) {
    mouseDownButton = nullptr;
    hoveredButton = nullptr;
    refreshBuffer(buttons, objects);
}

void H2DE_ObjectManager::refreshInputBuffer(const std::vector<H2DE_Object*>& objects) {
    focusedInput = nullptr;
    refreshBuffer(inputs, objects);
}

// GETTER
const std::vector<H2DE_ButtonObject*> H2DE_ObjectManager::getValidButtons() const {
    std::vector<H2DE_ButtonObject*> res;

    for (H2DE_ButtonObject* button : buttons) {
        if (button->_hidden) {
            continue;
        }

        if (button->_disabled) {
            continue;
        }

        if (engine->_paused && button->_buttonObjectData.pauseSensitive) {
            continue;
        }

        res.push_back(button);
    } 

    return res;
}

const std::vector<H2DE_InputObject*> H2DE_ObjectManager::getValidInputs() const {
    std::vector<H2DE_InputObject*> res;

    for (H2DE_InputObject* input : inputs) {
        if (input->_hidden) {
            continue;
        }

        if (input->_disabled) {
            continue;
        }

        if (engine->_paused && input->_inputObjectData.pauseSensitive) {
            continue;
        }

        res.push_back(input);
    } 

    return res;
}

H2DE_MouseButton H2DE_ObjectManager::getH2DEButton(Uint8 sdlButton) {
    switch (sdlButton) {
        case SDL_BUTTON_LEFT: return H2DE_MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT: return H2DE_MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE: return H2DE_MOUSE_BUTTON_MIDDLE;
        default: return H2DE_MOUSE_BUTTON_LEFT;
    }
}

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

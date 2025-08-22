#include "H2DE/engine/H2DE_object_manager.h"
#include <H2DE/engine/H2DE_geometry.h>

// EVENTS

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
    focusedInput->_setCursorPosition(letterIndex);
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
    if (focusedInput != H2DE_NULL_OBJECT) {
        if (focusedInput->_inputObjectData.onBlur) {
            focusedInput->_inputObjectData.onBlur(focusedInput->_eventData);
        }

        focusedInput->_setCursorPosition(-1);
    }

    focusedInput = H2DE_NULL_OBJECT;
}

void H2DE_ObjectManager::handleEvents_inputs_mouseDown_focusNewInput(H2DE_InputObject* oldFocusedInput) {
    static H2DE_Window* window = engine->_window;
    
    if (oldFocusedInput != H2DE_NULL_OBJECT) {
        if (oldFocusedInput->_inputObjectData.onBlur) {
            oldFocusedInput->_inputObjectData.onBlur(oldFocusedInput->_eventData);
        }

        oldFocusedInput->_cursorPosition = -1;
    }

    if (focusedInput->_inputObjectData.onFocus) {
        focusedInput->_inputObjectData.onFocus(focusedInput->_eventData);
    }

    window->_setHoverCursor(H2DE_CURSOR_IBEAM);
}

int H2DE_ObjectManager::handleEvents_inputs_mouseDown_getFocusedInputLetterIndex() {
    if (focusedInput->_textObject == H2DE_NULL_OBJECT) {
        return 0;
    }

    const H2DE_Translate mouseTranslate = handle_inputs_mouseDown_getFixedMouseTranslate();

    const std::vector<H2DE_Surface*>& letterSurfaces = focusedInput->_textObject->_surfaceBuffers;

    float currentY = std::numeric_limits<float>::min();
    int currentLine = -1;

    float minDistance = std::numeric_limits<float>::max();
    int index = 0;

    for (int i = 0; i < letterSurfaces.size(); i++) {
        H2DE_Surface* surface = letterSurfaces.at(i);

        const H2DE_LevelRect local_surfaceRect = surface->getRect();

        const H2DE_LevelRect world_surfaceRect = G::getSurfaceRect(focusedInput, surface);
        const H2DE_Translate world_surfaceTranslate = world_surfaceRect.getTranslate();

        if (currentY != local_surfaceRect.y) {
            currentY = local_surfaceRect.y;
            currentLine++;
        }

        const float mouseDistance = mouseTranslate.getDistanceSquared(world_surfaceTranslate);
        if (mouseDistance >= minDistance) {
            continue;
        }

        minDistance = mouseDistance;
        index = ((mouseTranslate.x > world_surfaceTranslate.x) ? i + 1 : i) + currentLine;
    }

    return index;
}

H2DE_Translate H2DE_ObjectManager::handle_inputs_mouseDown_getFixedMouseTranslate() {
    static H2DE_Camera* camera = engine->getCamera();

    bool isXOriginInverted = camera->isXOriginInverted();
    bool isYOriginInverted = camera->isYOriginInverted();

    const H2DE_Translate& focusedInputTranslate = focusedInput->_objectData.transform.translate;

    const H2DE_Translate mouseTranslate = (!focusedInput->_objectData.absolute)
        ? engine->getMouseGameTranslate()
        : engine->getMouseInterfaceTranslate();

    H2DE_Translate local_mouseTranslate = mouseTranslate - focusedInputTranslate;

    if (isXOriginInverted) {
        local_mouseTranslate.x *= -1;
    }

    if (isYOriginInverted) {
        local_mouseTranslate.y *= -1;
    }

    return focusedInputTranslate + local_mouseTranslate;
}

// -- -- mouse motion
void H2DE_ObjectManager::handleEvents_inputs_mouseMotion() {
    static H2DE_Window* window = engine->_window;

    H2DE_InputObject* oldHoveredInput = hoveredInput;

    hoveredInput = handleEvents_inputs_mouseMotion_getHoveredInput();

    bool isInputHoveredNew = (hoveredInput != oldHoveredInput);

    if (hoveredInput == H2DE_NULL_OBJECT && isInputHoveredNew) {
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

    return H2DE_NULL_OBJECT;
}

// -- -- keydown
void H2DE_ObjectManager::handleEvents_inputs_keydown(SDL_Event event) {
    if (focusedInput == H2DE_NULL_OBJECT) {
        return;
    }

    if (!H2DE_ObjectManager::isASCII(event.text.text[0])) {
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
            handleEvents_inputs_keydown_delete(event);
            break;

        case SDLK_DELETE:
            handleEvents_inputs_keydown_suppr(event);
            break;

        case SDLK_ESCAPE:
            handleEvents_inputs_mouseDown_blurFocusedInput();
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            handleEvents_inputs_keydown_enter(event);
            break;

        case SDLK_LEFT:
            handleEvents_inputs_keydown_arrow(event);
            break;

        case SDLK_RIGHT:
            handleEvents_inputs_keydown_arrow(event);
            break;

        default: break;
    }
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_modifyText(SDL_Event event, const std::function<void(std::string, int, unsigned char)>& tasks) {
    if (tasks == H2DE_NULL_OBJECT) {
        return;
    }

    int cursorPosition = focusedInput->_cursorPosition;
    if (cursorPosition == -1) {
        return;
    }

    if (!H2DE_ObjectManager::isASCII(event.text.text[0])) {
        return;
    }

    unsigned char c = static_cast<unsigned char>(event.text.text[0]);
    tasks(focusedInput->getText(), cursorPosition, c);
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_default(SDL_Event event) {
    if (focusedInput == H2DE_NULL_OBJECT) {
        return;
    }

    if (!focusedInput->_isInputValid(event.text.text[0])) {
        return;
    }

    if (focusedInput->_isFull()) {
        return;
    }

    if (!H2DE_ObjectManager::isASCII(event.text.text[0])) {
        return;
    }

    bool isFontValid = (engine->_fonts.contains(focusedInput->_inputObjectData.text.font));
    if (!isFontValid) {
        return;
    }

    const H2DE_Font& focusedInputDFont = engine->_fonts.at(focusedInput->_inputObjectData.text.font);
    bool fontHaveCharacter = (focusedInputDFont._characters.contains(std::string(1, event.text.text[0])));
    if (!fontHaveCharacter) {
        return;
    }

    handleEvents_inputs_keydown_modifyText(event, [this](std::string text, int cursorPosition, unsigned char c) {
        if (!isCursorPositionValid()) {
            return;
        }

        text.insert(cursorPosition, 1, c);
        focusedInput->setText(text);
        focusedInput->_setCursorPosition(cursorPosition + 1);
    });
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_delete(SDL_Event event) {
    handleEvents_inputs_keydown_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
        int characterIndex = cursorPosition - 1;
        if (characterIndex < 0) {
            return;
        }

        if (!isCursorPositionValid()) {
            return;
        }

        bool ctrlDown = (H2DE_ObjectManager::isCtrlDown(event));

        if (ctrlDown) {
            std::string slicedText = text.substr(0, H2DE::max(cursorPosition - 1, 1));
            size_t lastSpaceIndex = H2DE_ObjectManager::getLastSpaceIndex(slicedText);
            lastSpaceIndex = ((lastSpaceIndex == -1) ? 0 : lastSpaceIndex + 1);

            text.erase(lastSpaceIndex, cursorPosition);
            focusedInput->setText(text);
            focusedInput->_setCursorPosition(lastSpaceIndex);
            
        } else {
            text.erase(characterIndex, 1);
            focusedInput->setText(text);
            focusedInput->_setCursorPosition(cursorPosition - 1);
        }
    });
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_suppr(SDL_Event event) {
    handleEvents_inputs_keydown_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
        if (!isCursorPositionValid()) {
            return;
        }
        
        bool ctrlDown = (H2DE_ObjectManager::isCtrlDown(event));

        if (ctrlDown) {
            const std::string splicedText = text.substr(cursorPosition);
            size_t firstSpaceIndex = H2DE_ObjectManager::getFirstSpaceIndex(splicedText);
            firstSpaceIndex = ((firstSpaceIndex == -1) ? splicedText.length() : firstSpaceIndex);

            text.erase(cursorPosition, firstSpaceIndex + 1);

        } else {
            text.erase(cursorPosition, 1);
        }

        focusedInput->setText(text);
    });
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_enter(SDL_Event event) {
    bool altDown = (H2DE_ObjectManager::isAltDown(event));

    if (!altDown) {
        submitInput(focusedInput);
        return;
    }

    handleEvents_inputs_keydown_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
        if (!isCursorPositionValid()) {
            return;
        }

        if (focusedInput->_isFull()) {
            return;
        }

        text.insert(cursorPosition, 1, '\n');
        focusedInput->setText(text);
        focusedInput->_setCursorPosition(cursorPosition + 1);
    });
}

void H2DE_ObjectManager::handleEvents_inputs_keydown_arrow(SDL_Event event) {
    if (focusedInput->_textObject == H2DE_NULL_OBJECT) {
        return;
    }

    int nbLetters = focusedInput->_textObject->_surfaceBuffers.size();

    const std::string text = focusedInput->_textObject->getText();
    int cursorPosition = focusedInput->_cursorPosition;

    bool ctrlDown = (H2DE_ObjectManager::isCtrlDown(event));
    bool left = (event.key.keysym.sym == SDLK_LEFT);

    if (ctrlDown) {

        if (left) {
            const std::string slicedText = text.substr(0, H2DE::max(cursorPosition - 1, 1));
            size_t lastSpaceIndex = H2DE_ObjectManager::getLastSpaceIndex(slicedText);
            lastSpaceIndex = ((lastSpaceIndex == -1) ? 0 : lastSpaceIndex + 1);

            focusedInput->_setCursorPosition(lastSpaceIndex); 

        } else {
            const std::string slicedText = text.substr(cursorPosition + 1);
            size_t firstSpaceIndex = H2DE_ObjectManager::getFirstSpaceIndex(slicedText);
            firstSpaceIndex = ((firstSpaceIndex == -1) ? nbLetters : cursorPosition + 1 + firstSpaceIndex);

            focusedInput->_setCursorPosition(firstSpaceIndex);
        }
            
    } else {
        int incr = ((left) ? -1 : 1);
        cursorPosition = H2DE::clamp<int>(cursorPosition + incr, 0, nbLetters);
        focusedInput->_setCursorPosition(cursorPosition);
    }
}

// ACTIONS
void H2DE_ObjectManager::refreshInputBuffer(const std::vector<H2DE_Object*>& objects) {
    focusedInput = H2DE_NULL_OBJECT;
    refreshBuffer(inputs, objects);
}

void H2DE_ObjectManager::focusInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (focusedInput == input) {
        return;
    }

    if (focusedInput != H2DE_NULL_OBJECT) {
        blurInput(focusedInput);
    }

    if (input->_inputObjectData.onFocus) {
        input->_inputObjectData.onFocus(input->_eventData);
    }

    focusedInput = input;

    if (focusedInput->_textObject != H2DE_NULL_OBJECT) {
        focusedInput->_setCursorPosition(focusedInput->_textObject->_surfaceBuffers.size());

    } else {
        focusedInput->_setCursorPosition(-1);
    }
}

void H2DE_ObjectManager::blurInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_inputObjectData.onBlur) {
        input->_inputObjectData.onBlur(input->_eventData);
    }

    if (focusedInput == input) {
        input->_cursorPosition = -1;
        focusedInput = H2DE_NULL_OBJECT;
    }
}

void H2DE_ObjectManager::submitInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_inputObjectData.onSubmit) {
        input->_inputObjectData.onSubmit(input->_eventData);
    }

    blurInput(input);
}

// GETTER
const std::vector<H2DE_InputObject*> H2DE_ObjectManager::getValidInputs() const {
    std::vector<H2DE_InputObject*> res;

    for (H2DE_InputObject* input : inputs) {
        if (input->_hidden) {
            continue;
        }

        if (input->_disabled) {
            continue;
        }

        res.push_back(input);
    } 

    return res;
}

bool H2DE_ObjectManager::isCursorPositionValid() const {
    if (focusedInput == H2DE_NULL_OBJECT) {
        return false;
    }

    const int& cursorPosition = focusedInput->_cursorPosition;
    const std::string& text = focusedInput->_inputObjectData.text.text;

    return (cursorPosition >= 0 && cursorPosition <= text.length());
}

#include "H2DE/engine/H2DE_object_manager.h"
#include "H2DE/engine/H2DE_geometry.h"

// EVENTS

// -- -- mouse down
void H2DE_ObjectManager::handleEvents_mouseDown_input(H2DE_InputObject* input) {
    H2DE_InputObject* oldFocusedInput = focusedInput;

    bool noInputClicked = (input == H2DE_NULL_OBJECT);
    if (noInputClicked) {
        handleEvents_mouseDown_input_blur(focusedInput);
        return;
    }

    bool isNewInput = (input != oldFocusedInput);
    if (isNewInput) {
        handleEvents_mouseDown_input_blur(focusedInput);
        handleEvents_mouseDown_input_focus(input);
    }

    int letterIndex = handleEvents_mouseDown_input_getFocusedInputLetterIndex();
    focusedInput->_setCursorPosition(letterIndex);
}

void H2DE_ObjectManager::handleEvents_mouseDown_input_focus(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_inputObjectData.onFocus) {
        input->_inputObjectData.onFocus({ input, input->getText() });
    }

    focusedInput = input;
}

void H2DE_ObjectManager::handleEvents_mouseDown_input_blur(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_inputObjectData.onBlur) {
        input->_inputObjectData.onBlur({ input, input->getText() });
    }

    input->_setCursorPosition(-1);

    focusedInput = H2DE_NULL_OBJECT;
}

int H2DE_ObjectManager::handleEvents_mouseDown_input_getFocusedInputLetterIndex() {
    constexpr float MAX_FLOAT = std::numeric_limits<float>::max();

    if (focusedInput->_textObject->getText() == "") {
        return 0;
    }

    const H2DE_Translate mouseTranslate = handleEvents_mouseDown_input_getFixedMouseTranslate();
    const std::vector<H2DE_Surface*>& letterSurfaces = focusedInput->_textObject->_surfaceBuffers;

    float minDistance = MAX_FLOAT;
    int index = 0;

    for (int i = 0; i < letterSurfaces.size(); i++) {
        H2DE_Surface* surface = letterSurfaces.at(i);

        const H2DE_LevelRect local_surfaceRect = surface->getRect();

        const H2DE_LevelRect world_surfaceRect = G::getSurfaceRect(focusedInput, surface);
        const H2DE_Translate world_surfaceTranslate = world_surfaceRect.getTranslate();

        const float mouseDistance = mouseTranslate.getDistanceSquared(world_surfaceTranslate);
        if (mouseDistance >= minDistance) {
            continue;
        }

        minDistance = mouseDistance;
        index = ((mouseTranslate.x > world_surfaceTranslate.x) ? i + 1 : i);
    }

    return index;
}

H2DE_Translate H2DE_ObjectManager::handleEvents_mouseDown_input_getFixedMouseTranslate() {
    static H2DE_Camera* camera = engine->_camera;

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

// -- -- keydown
void H2DE_ObjectManager::handleEvents_keydown_input(SDL_Event event) {
    bool noFocusedInput = (focusedInput == H2DE_NULL_OBJECT);
    if (noFocusedInput) {
        return;
    }

    bool textIsNotASCII = (!H2DE_ObjectManager::isASCII(event.text.text[0]));
    if (textIsNotASCII) {
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
            handleEvents_keydown_input_delete(event);
            break;

        case SDLK_DELETE:
            handleEvents_keydown_input_suppr(event);
            break;

        case SDLK_ESCAPE:
            handleEvents_mouseDown_input_blur(focusedInput);
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            handleEvents_keydown_input_enter(event);
            break;

        case SDLK_UP:
        case SDLK_DOWN:
            handleEvents_keydown_input_blockArrow(event);
            break;

        case SDLK_LEFT:
        case SDLK_RIGHT:
            handleEvents_keydown_input_inlineArrow(event);
            break;

        default: break;
    }
}

void H2DE_ObjectManager::handleEvents_keydown_input_modifyText(SDL_Event event, const std::function<void(std::string, int, unsigned char)>& tasks) {
    if (tasks == nullptr) {
        return;
    }

    char character = event.text.text[0];

    int cursorPosition = focusedInput->_cursorPosition;
    if (cursorPosition == -1) {
        return;
    }

    if (!H2DE_ObjectManager::isASCII(character)) {
        return;
    }

    tasks(focusedInput->_inputObjectData.text.text, cursorPosition, static_cast<unsigned char>(character));
}

void H2DE_ObjectManager::handleEvents_keydown_input_default(SDL_Event event) {
    bool noFocusedInput = (focusedInput == H2DE_NULL_OBJECT);
    if (noFocusedInput) {
        return;
    }

    bool isInputValid = (focusedInput->_isInputValid(event.text.text[0]));
    if (!isInputValid) {
        return;
    }

    bool textIsNotASCII = (!H2DE_ObjectManager::isASCII(event.text.text[0]));
    if (textIsNotASCII) {
        return;
    }

    bool isInputFull = (focusedInput->_isFull());
    if (isInputFull) {
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

    handleEvents_keydown_input_modifyText(event, [this](std::string text, int cursorPosition, unsigned char c) {
        if (!isCursorPositionValid()) {
            return;
        }

        text.insert(cursorPosition, 1, c);
        focusedInput->setText(text);
        focusedInput->_setCursorPosition(cursorPosition + 1);

        if (focusedInput->_inputObjectData.onInput) {
            focusedInput->_inputObjectData.onInput({ focusedInput, text, static_cast<char>(c) });
        }
    });
}

void H2DE_ObjectManager::handleEvents_keydown_input_delete(SDL_Event event) {
    handleEvents_keydown_input_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
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

            text.erase(lastSpaceIndex, cursorPosition - lastSpaceIndex);
            focusedInput->setText(text);
            focusedInput->_setCursorPosition(lastSpaceIndex);
            
        } else {
            text.erase(characterIndex, 1);
            focusedInput->setText(text);
            focusedInput->_setCursorPosition(cursorPosition - 1);
        }
    });
}

void H2DE_ObjectManager::handleEvents_keydown_input_suppr(SDL_Event event) {
    handleEvents_keydown_input_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
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
        focusedInput->_refreshCursor();
    });
}

void H2DE_ObjectManager::handleEvents_keydown_input_enter(SDL_Event event) {
    bool altDown = (H2DE_ObjectManager::isAltDown(event));
    if (!altDown) {
        submitInput(focusedInput);
        return;
    }

    handleEvents_keydown_input_modifyText(event, [this, event](std::string text, int cursorPosition, unsigned char c) {
        if (!isCursorPositionValid()) {
            return;
        }

        bool isInputFull = (focusedInput->_isFull());
        if (isInputFull) {
            return;
        }

        text.insert(cursorPosition, 1, '\n');
        focusedInput->setText(text);
        focusedInput->_setCursorPosition(cursorPosition + 1);

        if (focusedInput->_inputObjectData.onInput) {
            focusedInput->_inputObjectData.onInput({ focusedInput, text, '\n' });
        }
    });
}

void H2DE_ObjectManager::handleEvents_keydown_input_inlineArrow(SDL_Event event) {
    bool hasNoText = (focusedInput->_textObject->getText() == "");
    if (hasNoText) {
        return;
    }

    int nbLetters = focusedInput->_textObject->_surfaceBuffers.size();

    const std::string text = focusedInput->_textObject->getText();
    int cursorPosition = focusedInput->_cursorPosition;

    bool leftArrow = (event.key.keysym.sym == SDLK_LEFT);
    bool ctrlDown = (H2DE_ObjectManager::isCtrlDown(event));

    if (ctrlDown) {

        if (leftArrow) {
            const std::string slicedText = text.substr(0, H2DE::max(cursorPosition - 1, 1));
            size_t lastSpaceIndex = H2DE_ObjectManager::getLastSpaceIndex(slicedText);
            lastSpaceIndex = ((lastSpaceIndex == -1) ? 0 : lastSpaceIndex + 1);

            focusedInput->_setCursorPosition(lastSpaceIndex); 

        } else {
            const std::string slicedText = text.substr(H2DE::min(cursorPosition + 1, static_cast<int>(text.length())));
            size_t firstSpaceIndex = H2DE_ObjectManager::getFirstSpaceIndex(slicedText);
            firstSpaceIndex = ((firstSpaceIndex == -1) ? nbLetters : cursorPosition + 1 + firstSpaceIndex);

            focusedInput->_setCursorPosition(firstSpaceIndex);
        }
            
    } else {
        int incr = ((leftArrow) ? -1 : 1);
        cursorPosition = H2DE::clamp<int>(cursorPosition + incr, 0, nbLetters);
        focusedInput->_setCursorPosition(cursorPosition);
    }
}

void H2DE_ObjectManager::handleEvents_keydown_input_blockArrow(SDL_Event event) {
    constexpr float EPSILON = 0.00001f;
    constexpr float MIN_FLOAT = std::numeric_limits<float>::lowest();
    constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
    
    bool hasNoText = (focusedInput->_textObject->getText() == "");
    if (hasNoText) {
        return;
    }

    const H2DE_Translate cursorSurfaceTranslate = focusedInput->_cursor->getTranslate();
    const std::vector<H2DE_Surface*>& letterSurfaces = focusedInput->_textObject->_surfaceBuffers;

    bool upArrow = (event.key.keysym.sym == SDLK_UP);
    float minY = (upArrow) ? MIN_FLOAT : cursorSurfaceTranslate.y + EPSILON;
    float maxY = (upArrow) ? cursorSurfaceTranslate.y - EPSILON : MAX_FLOAT;

    float minDistanceX = MAX_FLOAT;
    int closestLetterIndex = -1;

    for (int i = 0; i < letterSurfaces.size(); i++) {
        const H2DE_Surface* letterSurface = letterSurfaces.at(i);
        const H2DE_Translate letterSurfaceTranslate = letterSurface->getTranslate();

        bool tooLow = (letterSurfaceTranslate.y < minY);
        if (tooLow) {
            continue;
        }

        bool tooHigh = (letterSurfaceTranslate.y > maxY);
        if (tooHigh) {
            break;
        }
    
        float distanceX = cursorSurfaceTranslate.getDistanceSquared(letterSurfaceTranslate);

        if (distanceX < minDistanceX) {
            minDistanceX = distanceX;
            closestLetterIndex = i;
        }
    }

    if (closestLetterIndex == -1) {
        return;
    }

    if (letterSurfaces.at(closestLetterIndex)->getTranslate().x < cursorSurfaceTranslate.x) {
        closestLetterIndex++;
    }

    focusedInput->_setCursorPosition(closestLetterIndex);
}

// ACTIONS
void H2DE_ObjectManager::refreshInputBuffer(const std::vector<H2DE_Object*>& objects) {
    focusedInput = H2DE_NULL_OBJECT;
    refreshBuffer(inputs, objects);
    refreshHoverObjectBuffer();
}

void H2DE_ObjectManager::focusInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (focusedInput == input) {
        return;
    }

    if (input->_disabled) {
        return;
    }

    if (focusedInput != H2DE_NULL_OBJECT) {
        blurInput(focusedInput);
    }

    if (input->_inputObjectData.onFocus) {
        input->_inputObjectData.onFocus({ input, input->getText() });
    }

    focusedInput = input;

    if (focusedInput->_textObject->getText() != "") {
        focusedInput->_setCursorPosition(focusedInput->_textObject->_surfaceBuffers.size());

    } else {
        focusedInput->_setCursorPosition(-1);
    }
}

void H2DE_ObjectManager::blurInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_disabled) {
        return;
    }

    if (input->_inputObjectData.onBlur) {
        input->_inputObjectData.onBlur({ input, input->getText() });
    }

    if (focusedInput == input) {
        input->_setCursorPosition(-1);
        focusedInput = H2DE_NULL_OBJECT;
    }
}

void H2DE_ObjectManager::submitInput(H2DE_InputObject* input) {
    if (input == H2DE_NULL_OBJECT) {
        return;
    }

    if (input->_disabled) {
        return;
    }

    if (input->_inputObjectData.onSubmit) {
        input->_inputObjectData.onSubmit({ input, input->getText() });
    }

    blurInput(input);
}

// GETTER
bool H2DE_ObjectManager::isCursorPositionValid() const {
    if (focusedInput == H2DE_NULL_OBJECT) {
        return false;
    }

    const int& cursorPosition = focusedInput->_cursorPosition;
    const std::string& text = focusedInput->_inputObjectData.text.text;

    return (cursorPosition >= 0 && cursorPosition <= text.length());
}

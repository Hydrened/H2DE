#include "H2DE/engine/H2DE_object_manager.h"

// EVENTS

// -- -- mouse  down
void H2DE_ObjectManager::handleEvents_buttons_mouseDown(SDL_Event event) {
    mouseDownButton = handleEvents_buttons_mouseDown_getMouseDownButton(event);

    if (mouseDownButton != H2DE_NULL_OBJECT) {
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

    return H2DE_NULL_OBJECT;
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
    if (mouseDownButton == H2DE_NULL_OBJECT) {
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

    mouseDownButton = H2DE_NULL_OBJECT;
}

// -- -- mouse motion
void H2DE_ObjectManager::handleEvents_buttons_mouseMotion() {
    H2DE_ButtonObject* oldHoveredButton = hoveredButton;

    hoveredButton = handleEvents_buttons_mouseMotion_getHoveredButton();

    if (hoveredButton == H2DE_NULL_OBJECT) {
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

    return H2DE_NULL_OBJECT;
}

void H2DE_ObjectManager::handleEvents_buttons_mouseMotion_blurButton(H2DE_ButtonObject* button) {
    static H2DE_Window* window = engine->_window;

    if (button == H2DE_NULL_OBJECT) {
        return;
    }

    if (button->_buttonObjectData.onBlur) {
        button->_buttonObjectData.onBlur(button->_eventData);
    }

    window->_setHoverCursor(oldCursor);
}

void H2DE_ObjectManager::handleEvents_buttons_mouseMotion_hoverNewButton(H2DE_ButtonObject* oldHoveredButton) {
    static H2DE_Window* window = engine->_window;

    if (oldHoveredButton != H2DE_NULL_OBJECT) {
        if (oldHoveredButton->_buttonObjectData.onBlur) {
            oldHoveredButton->_buttonObjectData.onBlur(oldHoveredButton->_eventData);
        }
    }

    if (hoveredButton->_buttonObjectData.onHover) {
        hoveredButton->_buttonObjectData.onHover(hoveredButton->_eventData);
    }

    window->_setHoverCursor(hoveredButton->_buttonObjectData.cursor);
}

// ACTIONS
void H2DE_ObjectManager::refreshButtonBuffer(const std::vector<H2DE_Object*>& objects) {
    mouseDownButton = H2DE_NULL_OBJECT;
    hoveredButton = H2DE_NULL_OBJECT;
    refreshBuffer(buttons, objects);
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

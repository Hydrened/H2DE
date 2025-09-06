#include "H2DE/engine/H2DE_object_manager.h"

// EVENTS

// -- -- mouse down
void H2DE_ObjectManager::handleEvents_mouseDown_button(H2DE_ButtonObject* button) {
    bool buttonHasBeenClicked = (button != H2DE_NULL_OBJECT);

    bool disabled = (button->_disabled);
    if (disabled) {
        return;
    }
    
    if (buttonHasBeenClicked) {
        handleEvents_mouseDown_button_mouseDown(button);
    }
}

void H2DE_ObjectManager::handleEvents_mouseDown_button_mouseDown(H2DE_ButtonObject* button) {
    if (button == H2DE_NULL_OBJECT) {
        return;
    }

    if (button->_buttonObjectData.onMouseDown) {
        button->_buttonObjectData.onMouseDown({ button });
    }

    mouseDownButton = button;
}

// -- -- mouse up
void H2DE_ObjectManager::handleEvents_mouseUp_button(SDL_Event event) {
    H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(hoveredObject);
    
    bool buttonIsHovered = (button != H2DE_NULL_OBJECT);
    if (!buttonIsHovered) {
        return;
    }

    bool isCorrectButton = (button == mouseDownButton);
    if (!isCorrectButton) {
        return;
    }

    bool notDisabled = (!button->_disabled);
    bool hasMouseUp = (button->_buttonObjectData.onMouseUp != nullptr);
    bool isValid = (notDisabled && hasMouseUp);
    if (!isValid) {
        return;
    }

    bool isCorrectClick = ((button->_buttonObjectData.mouseButton & H2DE_ObjectManager::getH2DEButton(event.button.button)) != 0);
    if (!isCorrectClick) {
        return;
    }

    handleEvents_mouseUp_button_mouseUp(button);
}

void H2DE_ObjectManager::handleEvents_mouseUp_button_mouseUp(H2DE_ButtonObject* button) {
    if (button == H2DE_NULL_OBJECT) {
        return;
    }

    if (button->_buttonObjectData.onMouseUp) {
        button->_buttonObjectData.onMouseUp({ button });
    }

    mouseDownButton = H2DE_NULL_OBJECT;
}

// -- mouse motion
void H2DE_ObjectManager::handleEvents_mouseMotion_buttons(H2DE_Object* oldHoveredObject) {
    H2DE_ButtonObject* oldHoveredButton = dynamic_cast<H2DE_ButtonObject*>(oldHoveredObject);
    H2DE_ButtonObject* hoveredButton = dynamic_cast<H2DE_ButtonObject*>(hoveredObject);

    bool hoveringNoButton = (hoveredButton == H2DE_NULL_OBJECT);
    if (hoveringNoButton) {
        handleEvents_mouseMotion_buttons_blur(oldHoveredButton);
        return;
    }

    bool hoveringDifferentButton = (hoveredButton != oldHoveredButton);
    if (hoveringDifferentButton) {
        handleEvents_mouseMotion_buttons_blur(oldHoveredButton);
        handleEvents_mouseMotion_buttons_hover(hoveredButton);
    }
}

void H2DE_ObjectManager::handleEvents_mouseMotion_buttons_blur(H2DE_ButtonObject* button) {
    if (button == H2DE_NULL_OBJECT) {
        return;
    }
    
    bool disabled = (button->_disabled);
    if (disabled) {
        return;
    }

    if (button->_buttonObjectData.onBlur) {
        button->_buttonObjectData.onBlur({ button });
    }
}

void H2DE_ObjectManager::handleEvents_mouseMotion_buttons_hover(H2DE_ButtonObject* button) {
    if (button == H2DE_NULL_OBJECT) {
        return;
    }

    bool disabled = (button->_disabled);
    if (disabled) {
        return;
    }

    if (button->_buttonObjectData.onHover) {
        button->_buttonObjectData.onHover({ button });
    }
}

// ACTIONS
void H2DE_ObjectManager::refreshButtonBuffer(const std::vector<H2DE_Object*>& objects) {
    mouseDownButton = H2DE_NULL_OBJECT;
    refreshBuffer(buttons, objects);
    refreshHoverObjectBuffer();
}

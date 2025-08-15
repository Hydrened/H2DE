#include "H2DE/engine/H2DE_object_manager.h"
#include "H2DE/engine/H2DE_geometry.h"

// INIT
H2DE_ObjectManager::H2DE_ObjectManager(H2DE_Engine* e) : engine(e) {
    buttons.reserve(500);
}

// EVENTS
void H2DE_ObjectManager::handleEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            handleMouseDownEvents(event);
            break;

        case SDL_MOUSEBUTTONUP:
            handleMouseUpEvents(event);
            break;

        case SDL_MOUSEMOTION:
            handleBlurEvents(event);
            handleHoverEvents(event);
            break;

        default: return;
    }
}

void H2DE_ObjectManager::handleMouseDownEvents(SDL_Event event) {
    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    H2DE_MouseButton mouseButton = H2DE_ObjectManager::getH2DEButton(event.button.button);

    for (H2DE_ButtonObject* button : getValidButtons()) {
        if (!(mouseButton & button->getMouseButton())) {
            continue;
        }

        const H2DE_Translate mousePos = (button->_objectData.absolute) ? mouseInterfacePos : mouseGamePos;

        for (const auto& [name, hitbox] : button->_hitboxes) {
            const H2DE_LevelRect buttonRect = G::getHitboxRect(button, hitbox);

            if (buttonRect.collides(mousePos)) {

                if (button->_buttonObjectData.onMouseDown) {
                    button->_buttonObjectData.onMouseDown(button->_eventData);
                }
                
                mouseDown = button;
                return;
            }
        }
    }
}

void H2DE_ObjectManager::handleMouseUpEvents(SDL_Event event) {
    if (mouseDown == nullptr) {
        return;
    }

    if (!mouseDown->_buttonObjectData.onMouseUp || mouseDown->_disabled) {
        return;
    }

    H2DE_MouseButton mouseButton = H2DE_ObjectManager::getH2DEButton(event.button.button);

    if (!(mouseButton & mouseDown->getMouseButton())) {
        return;
    }

    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    const H2DE_Translate mousePos = (mouseDown->_objectData.absolute) ? mouseInterfacePos : mouseGamePos;

    for (const auto& [name, hitbox] : mouseDown->_hitboxes) {
        const H2DE_LevelRect buttonRect = G::getHitboxRect(mouseDown, hitbox);

        if (buttonRect.collides(mousePos)) {
            mouseDown->_buttonObjectData.onMouseUp(mouseDown->_eventData);
            mouseDown = nullptr;
            return;
        }
    }
}

void H2DE_ObjectManager::handleHoverEvents(SDL_Event event) {
    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    for (H2DE_ButtonObject* button : getValidButtons()) {
        const H2DE_Translate mousePos = (button->_objectData.absolute) ? mouseInterfacePos : mouseGamePos;

        for (const auto& [name, hitbox] : button->_hitboxes) {
            const H2DE_LevelRect buttonRect = G::getHitboxRect(button, hitbox);

            if (!buttonRect.collides(mousePos)) {
                continue;
            }

            if (hovered != nullptr) {
                if (hovered == button) {
                    continue;
                }

                if (hovered->_objectData.index > button->_objectData.index) {
                    continue;
                }

                if (H2DE_Engine::_isPositionGreater(button, hovered)) {
                    continue;
                }
            }

            if (hovered != nullptr) {
                if (hovered->_buttonObjectData.onBlur) {
                    hovered->_buttonObjectData.onBlur(hovered->_eventData);
                }

                engine->_window->setCursor(oldCursor);
            }

            hovered = button;
            if (button->_buttonObjectData.onHover) {
                button->_buttonObjectData.onHover(button->_eventData);
            }

            engine->_window->_setHoverCursor(button->_buttonObjectData.cursor);

            return;
        }
    }
}

void H2DE_ObjectManager::handleBlurEvents(SDL_Event event) {
    if (hovered == nullptr) {
        return;
    }

    if (hovered->_disabled) {
        return;
    }

    bool stillHovering = false;

    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();
    const H2DE_Translate mousePos = (hovered->_objectData.absolute) ? mouseInterfacePos : mouseGamePos;

    for (const auto& [name, hitbox] : hovered->_hitboxes) {
        const H2DE_LevelRect buttonRect = G::getHitboxRect(hovered, hitbox);

        if (buttonRect.collides(mousePos)) {
            stillHovering = true;
            break;
        }
    }

    if (!stillHovering) {
        if (hovered->_buttonObjectData.onBlur) {
            hovered->_buttonObjectData.onBlur(hovered->_eventData);
        }

        engine->_window->setCursor(oldCursor);
        hovered = nullptr;
    }
}

// ACTIONS
void H2DE_ObjectManager::refreshButtonBuffer(const std::vector<H2DE_Object*>& objects) {
    mouseDown = nullptr;
    hovered = nullptr;

    buttons.clear();

    for (H2DE_Object* object : objects) {
        H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(object);

        if (button != nullptr) {
            buttons.push_back(button);
        }
    }

    std::sort(buttons.begin(), buttons.end(), [](H2DE_ButtonObject* a, H2DE_ButtonObject* b) {
        int indexA = a->getIndex();
        int indexB = b->getIndex();

        if (indexA == indexB) {
            return H2DE_Engine::_isPositionGreater(a, b);
        }

        return (indexA > indexB);
    });
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

H2DE_MouseButton H2DE_ObjectManager::getH2DEButton(Uint8 sdlButton) {
    switch (sdlButton) {
        case SDL_BUTTON_LEFT: return H2DE_MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT: return H2DE_MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE: return H2DE_MOUSE_BUTTON_MIDDLE;
        default: return H2DE_MOUSE_BUTTON_LEFT;
    }
}

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

    for (H2DE_ButtonObject* button : getValidButtons()) {
        const H2DE_Translate mousePos = (button->objectData.absolute) ? mouseInterfacePos : mouseGamePos;

        for (const auto& [name, hitbox] : button->hitboxes) {
            const H2DE_LevelRect buttonRect = G::getHitboxRect(button, hitbox);

            if (buttonRect.collides(mousePos)) {

                if (button->buttonObjectData.onMouseDown) {
                    button->buttonObjectData.onMouseDown({ button, button->currentTimelineID });
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

    if (!mouseDown->buttonObjectData.onMouseUp || mouseDown->disabled) {
        return;
    }

    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    const H2DE_Translate mousePos = (mouseDown->objectData.absolute) ? mouseInterfacePos : mouseGamePos;

    for (const auto& [name, hitbox] : mouseDown->hitboxes) {
        const H2DE_LevelRect buttonRect = G::getHitboxRect(mouseDown, hitbox);

        if (buttonRect.collides(mousePos)) {
            mouseDown->buttonObjectData.onMouseUp({ mouseDown, mouseDown->currentTimelineID });
            mouseDown = nullptr;
            return;
        }
    }
}

void H2DE_ObjectManager::handleHoverEvents(SDL_Event event) {
    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();

    for (H2DE_ButtonObject* button : getValidButtons()) {
        if (!button->buttonObjectData.onHover && !button->buttonObjectData.onBlur) {
            continue;
        }

        const H2DE_Translate mousePos = (button->objectData.absolute) ? mouseInterfacePos : mouseGamePos;

        for (const auto& [name, hitbox] : button->hitboxes) {
            const H2DE_LevelRect buttonRect = G::getHitboxRect(button, hitbox);

            if (!buttonRect.collides(mousePos)) {
                continue;
            }

            if (hovered != nullptr) {
                if (hovered == button) {
                    continue;
                }

                if (hovered->objectData.index > button->objectData.index) {
                    continue;
                }

                if (H2DE_Engine::isPositionGreater(button, hovered)) {
                    continue;
                }
            }

            if (hovered != nullptr) {
                if (hovered->buttonObjectData.onBlur) {
                    hovered->buttonObjectData.onBlur({ hovered, hovered->currentTimelineID });
                }
            }

            hovered = button;
            if (button->buttonObjectData.onHover) {
                button->buttonObjectData.onHover({ button, button->currentTimelineID });
            }

            return;
        }
    }
}

void H2DE_ObjectManager::handleBlurEvents(SDL_Event event) {
    if (hovered == nullptr) {
        return;
    }

    if (hovered->disabled) {
        return;
    }

    bool stillHovering = false;

    const H2DE_Translate mouseGamePos = engine->getMouseGamePos();
    const H2DE_Translate mouseInterfacePos = engine->getMouseInterfacePos();
    const H2DE_Translate mousePos = (hovered->objectData.absolute) ? mouseInterfacePos : mouseGamePos;

    for (const auto& [name, hitbox] : hovered->hitboxes) {
        const H2DE_LevelRect buttonRect = G::getHitboxRect(hovered, hitbox);

        if (buttonRect.collides(mousePos)) {
            stillHovering = true;
            break;
        }
    }

    if (!stillHovering) {
        if (hovered->buttonObjectData.onBlur) {
            hovered->buttonObjectData.onBlur({ hovered, hovered->currentTimelineID });
        }

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
            return H2DE_Engine::isPositionGreater(a, b);
        }

        return (indexA > indexB);
    });
}

// GETTER
const std::vector<H2DE_ButtonObject*> H2DE_ObjectManager::getValidButtons() const {
    std::vector<H2DE_ButtonObject*> res;

    for (H2DE_ButtonObject* button : buttons) {
        if (button->hidden) {
            continue;
        }

        if (button->disabled) {
            continue;
        }

        if (engine->paused && button->buttonObjectData.pauseSensitive) {
            continue;
        }

        res.push_back(button);
    } 

    return res;
}

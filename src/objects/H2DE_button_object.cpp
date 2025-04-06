#include "H2DE/H2DE_object.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData b) : H2DE_Object(engine, od), bod(b) {

}

H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) {
    H2DE_ButtonObject* object = new H2DE_ButtonObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    if (bod.surface) {
        delete bod.surface;
    }
}

// UPDATE
void H2DE_ButtonObject::update() {
    if (!bod.onclick && !bod.onhover && !bod.onout) {
        return;
    }

    H2DE_LevelPos mousePos = H2DE_GetMousePos(engine, od.absolute);
    H2DE_LevelPos pos = od.pos;
    bool clicked = false;
    bool hovered = false;

    for (auto [name, hitbox] : od.hitboxes) {
        if (hitbox.rect.addPos(pos).collides(mousePos)) {
            if (bod.onclick && !clicked && engine->click.has_value()) {
                bod.onclick();
                clicked = true;
            }

            if (bod.onhover && !hover && !hovered) {
                hover = true;
                hovered = true;
                bod.onhover();
            }
        }
    }

    if (!hovered && hover && bod.onout) {
        int nbOut = 0;

        for (auto [name, hitbox] : od.hitboxes) {
            if (!hitbox.rect.addPos(pos).collides(mousePos)) {
                nbOut++;
            }
        }

        if (nbOut >= od.hitboxes.size()) {
            bod.onout();
            hover = false;
        }
    }
}

// CALLS
void H2DE_HoverButton(const H2DE_ButtonObject* button) {
    if (button->bod.onhover) {
        button->bod.onhover();
    }
}

void H2DE_OutButton(const H2DE_ButtonObject* button) {
    if (button->bod.onout) {
        button->bod.onout();
    }
}

void H2DE_ClickButton(const H2DE_ButtonObject* button) {
    if (button->bod.onclick) {
        button->bod.onclick();
    }
}

// GETTER
std::vector<H2DE_SurfaceBuffer> H2DE_ButtonObject::getSurfaceBuffers() const {
    H2DE_LevelPos surfaceOffset = { 0.0f, 0.0f };
    surfaceOffset = surfaceOffset.rotate(od.pivot, od.rotation);

    H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
    buffer.surface = bod.surface;
    buffer.offset = surfaceOffset;
    buffer.size = od.size;
    buffer.rotation = od.rotation;
    buffer.flip = od.flip;
    return { buffer };
}

H2DE_Surface* H2DE_GetButtonSurface(const H2DE_ButtonObject* button) {
    return button->bod.surface;
}

// SETTER
void H2DE_SetButtonHover(H2DE_ButtonObject* button, const std::function<void()>& onhover) {
    button->bod.onhover = onhover;
}

void H2DE_SetButtonOut(H2DE_ButtonObject* button, const std::function<void()>& onout) {
    button->bod.onout = onout;
}

void H2DE_SetButtonClick(H2DE_ButtonObject* button, const std::function<void()>& onclick) {
    button->bod.onclick = onclick;
}

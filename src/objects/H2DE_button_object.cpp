#include "H2DE/objects/H2DE_button_object.h"

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
    // if (!bod.onMouseDown&& !bod.onMouseUp && !bod.onHover && !bod.onBlur) {
    //     return;
    // }

    // H2DE_LevelPos mousePos = H2DE_GetMousePos(engine, od.absolute);
    // H2DE_LevelPos pos = od.pos;
    // bool clicked = false;
    // bool hovered = false;

    // for (auto [name, hitbox] : od.hitboxes) {
    //     if (hitbox.rect.addPos(pos).collides(mousePos)) {
    //         if (bod.onclick && !clicked && engine->click.has_value()) {
    //             bod.onclick();
    //             clicked = true;
    //         }

    //         if (bod.onhover && !hover && !hovered) {
    //             hover = true;
    //             hovered = true;
    //             bod.onhover();
    //         }
    //     }
    // }

    // if (!hovered && hover && bod.onout) {
    //     int nbOut = 0;

    //     for (auto [name, hitbox] : od.hitboxes) {
    //         if (!hitbox.rect.addPos(pos).collides(mousePos)) {
    //             nbOut++;
    //         }
    //     }

    //     if (nbOut >= od.hitboxes.size()) {
    //         bod.onout();
    //         hover = false;
    //     }
    // }
}

// CALLS
void H2DE_ButtonMouseDown(H2DE_ButtonObject* button) {
    if (button->bod.onMouseDown) {
        button->bod.onMouseDown();
    }
}

void H2DE_ButtonMouseUp(H2DE_ButtonObject* button) {
    if (button->bod.onMouseUp) {
        button->bod.onMouseUp();
    }
}

void H2DE_ButtonHover(H2DE_ButtonObject* button) {
    if (button->bod.onHover) {
        button->bod.onHover();
    }
}

void H2DE_ButtonBlur(H2DE_ButtonObject* button) {
    if (button->bod.onBlur) {
        button->bod.onBlur();
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
void H2DE_SetButtonOnMouseDown(H2DE_ButtonObject* button, const std::function<void()>& onMouseDown) {
    button->bod.onMouseDown = onMouseDown;    
}

void H2DE_SetButtonOnMouseUp(H2DE_ButtonObject* button, const std::function<void()>& onMouseUp) {
    button->bod.onMouseUp = onMouseUp;
}

void H2DE_SetButtonOnHover(H2DE_ButtonObject* button, const std::function<void()>& onHover) {
    button->bod.onHover = onHover;
}

void H2DE_SetButtonOnBlur(H2DE_ButtonObject* button, const std::function<void()>& onBlur) {
    button->bod.onBlur = onBlur;
}
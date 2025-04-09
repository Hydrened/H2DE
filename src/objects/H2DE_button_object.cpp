#include "H2DE/H2DE/objects/H2DE_button_object.h"

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

}

// CALLS
void H2DE_ButtonMouseDown(H2DE_ButtonObject* button) {
    if (button->bod.onMouseDown) {
        button->bod.onMouseDown(button);
    }
}

void H2DE_ButtonMouseUp(H2DE_ButtonObject* button) {
    if (button->bod.onMouseUp) {
        button->bod.onMouseUp(button);
    }
}

void H2DE_ButtonHover(H2DE_ButtonObject* button) {
    if (button->bod.onHover) {
        button->bod.onHover(button);
    }
}

void H2DE_ButtonBlur(H2DE_ButtonObject* button) {
    if (button->bod.onBlur) {
        button->bod.onBlur(button);
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
void H2DE_SetButtonOnMouseDown(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseDown) {
    button->bod.onMouseDown = onMouseDown;    
}

void H2DE_SetButtonOnMouseUp(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseUp) {
    button->bod.onMouseUp = onMouseUp;
}

void H2DE_SetButtonOnHover(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onHover) {
    button->bod.onHover = onHover;
}

void H2DE_SetButtonOnBlur(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onBlur) {
    button->bod.onBlur = onBlur;
}
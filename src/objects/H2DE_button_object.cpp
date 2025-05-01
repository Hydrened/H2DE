#include "H2DE/objects/H2DE_button_object.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData b) : H2DE_Object(engine, od), bod(b) {
    resetSurfaceBuffers();
}

H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) {
    H2DE_Error::checkEngine(engine);
    
    H2DE_ButtonObject* object = new H2DE_ButtonObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    H2DE_Object::destroySurfaces(bod.surfaces);
}

// SURFACES
void H2DE_AddSurfaceToButtonObject(H2DE_ButtonObject* button, H2DE_Surface* surface, const std::string& name) {
    H2DE_Error::checkObject(button);

    button->addSurface(button->bod.surfaces, surface, name);
    button->resetSurfaceBuffers();
}

void H2DE_RemoveSurfaceFromButtonObject(H2DE_ButtonObject* button, const std::string& name) {
    H2DE_Error::checkObject(button);
    
    button->removeSurface(button->bod.surfaces, name);
    button->resetSurfaceBuffers();
}

void H2DE_ButtonObject::resetSurfaceBuffers() {
    clearSurfaceBuffers();
    surfaceBuffers.reserve(bod.surfaces.size());

    for (H2DE_Surface* surface : H2DE_Object::getSortedSurfaces(bod.surfaces)) {
        const H2DE_LevelPos surfaceOffset = surface->sd.rect.getPos();

        H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
        buffer.surface = surface;
        buffer.offset = surfaceOffset;
        buffer.size = surface->sd.rect.getSize();
        surfaceBuffers.push_back(buffer);
    }
}

// CALLS
void H2DE_ButtonMouseDown(H2DE_ButtonObject* button) {
    H2DE_Error::checkObject(button);

    if (button->bod.onMouseDown) {
        button->bod.onMouseDown(button);
    }
}

void H2DE_ButtonMouseUp(H2DE_ButtonObject* button) {
    H2DE_Error::checkObject(button);

    if (button->bod.onMouseUp) {
        button->bod.onMouseUp(button);
    }
}

void H2DE_ButtonHover(H2DE_ButtonObject* button) {
    H2DE_Error::checkObject(button);

    if (button->bod.onHover) {
        button->bod.onHover(button);
    }
}

void H2DE_ButtonBlur(H2DE_ButtonObject* button) {
    H2DE_Error::checkObject(button);

    if (button->bod.onBlur) {
        button->bod.onBlur(button);
    }
}

// GETTER
H2DE_Surface* H2DE_GetButtonSurface(const H2DE_ButtonObject* button, const std::string& name) {
    return H2DE_Object::getSurface(button->bod.surfaces, name);
}

// SETTER
void H2DE_SetButtonOnMouseDown(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseDown) {
    H2DE_Error::checkObject(button);
    button->bod.onMouseDown = onMouseDown;
}

void H2DE_SetButtonOnMouseUp(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseUp) {
    H2DE_Error::checkObject(button);
    button->bod.onMouseUp = onMouseUp;
}

void H2DE_SetButtonOnHover(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onHover) {
    H2DE_Error::checkObject(button);
    button->bod.onHover = onHover;
}

void H2DE_SetButtonOnBlur(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onBlur) {
    H2DE_Error::checkObject(button);
    button->bod.onBlur = onBlur;
}

#include "H2DE/objects/H2DE_button_object.h"
#undef max

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) : H2DE_Object(e, od), buttonObjectData(bod), eventData({ this, nullptr }) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    if (eventData.timeline != nullptr) {
        eventData.timeline->stop(false);
    }

    if (textObject != nullptr) {
        if (engine->destroyObject(textObject)) {
            textObject = nullptr;
        }
    }

    H2DE_Object::destroySurfaces(surfaces);
}

// ACTIONS
void H2DE_ButtonObject::refreshTextObject() {
    if (textObject != nullptr) {
        if (engine->destroyObject(textObject)) {
            textObject = nullptr;
        }
    }

    if (buttonObjectData.text.text == "") {
        return;
    }

    H2DE_ObjectData od = objectData;
    od.index++;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    tod.text = buttonObjectData.text;

    textObject = engine->createObject<H2DE_TextObject>(od, tod);
}

void H2DE_ButtonObject::refreshSurfaceBuffers() {
    refreshTextObject();

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::getSortedSurfaces(surfaces);

    surfaceBuffers.clear();
    surfaceBuffers.reserve(sortedSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    rescaleSurfaceBuffers();
}

void H2DE_ButtonObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxSurfaceRadius = getMaxSurfaceRadius(surfaces);
    
    maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_ButtonObject::mouseDown() {
    if (buttonObjectData.onMouseDown && !disabled) {
        buttonObjectData.onMouseDown(eventData);
    }
}

void H2DE_ButtonObject::mouseUp() {
    if (buttonObjectData.onMouseUp && !disabled) {
        buttonObjectData.onMouseUp(eventData);
    }
}

void H2DE_ButtonObject::mouseHover() {
    if (buttonObjectData.onHover && !disabled) {
        buttonObjectData.onHover(eventData);
    }
}

void H2DE_ButtonObject::mouseBlur() {
    if (buttonObjectData.onBlur && !disabled) {
        buttonObjectData.onBlur(eventData);
    }
}

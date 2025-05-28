#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TextObjectData& bod) : H2DE_Object(e, od), textObjectData(bod) {
    updateSurfaceBuffers();
    updateMaxRadius();
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    
}

// ACTIONS
void H2DE_TextObject::updateSurfaceBuffers() {

}

void H2DE_TextObject::updateMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    maxRadius = maxHitboxesRadius;
}

// SETTER

// -- no lerp
void H2DE_TextObject::setText(const std::string& text) {
    textObjectData.text = text;
    updateSurfaceBuffers();
}

void H2DE_TextObject::setFont(const std::string& font) {
    textObjectData.font = font;
    updateSurfaceBuffers();
}

void H2DE_TextObject::setFontSize(const H2DE_Scale& fontSize) {
    textObjectData.fontSize = fontSize;
    updateSurfaceBuffers();
}

void H2DE_TextObject::setSpacing(const H2DE_Scale& spacing) {
    textObjectData.spacing = spacing;
    updateSurfaceBuffers();
}

void H2DE_TextObject::setTextAlign(H2DE_TextAlign textAlign) {
    textObjectData.textAlign = textAlign;
    updateSurfaceBuffers();
}

void H2DE_TextObject::setColor(const H2DE_ColorRGB& color) {
    textObjectData.color = color;
    updateSurfaceBuffers();
}

// -- lerp
unsigned int H2DE_TextObject::setFontSize(const H2DE_Scale& fontSize, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.fontSize, fontSize, duration, easing, [this](H2DE_Scale iv) {
        setFontSize(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_TextObject::setSpacing(const H2DE_Scale& spacing, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.spacing, spacing, duration, easing, [this](H2DE_Scale iv) {
        setSpacing(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_TextObject::setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, textObjectData.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TextObjectData& bod) : H2DE_Object(e, od), textObjectData(bod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    
}

// ACTIONS
void H2DE_TextObject::refreshSurfaceBuffers() {

}

void H2DE_TextObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    maxRadius = maxHitboxesRadius;
}

// SETTER

// -- no lerp
void H2DE_TextObject::setText(const std::string& text) {
    textObjectData.text.text = text;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setFont(const std::string& font) {
    textObjectData.text.font = font;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setFontSize(const H2DE_Scale& fontSize) {
    textObjectData.text.fontSize = fontSize;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setSpacing(const H2DE_Scale& spacing) {
    textObjectData.text.spacing = spacing;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setTextAlign(H2DE_TextAlign textAlign) {
    textObjectData.text.textAlign = textAlign;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setColor(const H2DE_ColorRGB& color) {
    textObjectData.text.color = color;
    refreshSurfaceBuffers();
}

// -- lerp
unsigned int H2DE_TextObject::setFontSize(const H2DE_Scale& fontSize, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.text.fontSize, fontSize, duration, easing, [this](H2DE_Scale iv) {
        setFontSize(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_TextObject::setSpacing(const H2DE_Scale& spacing, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.text.spacing, spacing, duration, easing, [this](H2DE_Scale iv) {
        setSpacing(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_TextObject::setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, textObjectData.text.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);
}

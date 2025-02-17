#include "H2DE/H2DE_surface.h"

// INIT
H2DE_Text::H2DE_Text(H2DE_Engine* e, H2DE_TextData xd) : H2DE_Surface(e, H2DE_TextureData()), textData(xd) {
    textureData.size = getTextSize();
}

H2DE_Text* H2DE_CreateText(H2DE_Engine* engine, H2DE_TextData textData) {
    return new H2DE_Text(engine, textData);
}

// CLEANUP
H2DE_Text::~H2DE_Text() {

}

// UPDATE
void H2DE_Text::update() {

}

// GETTER
H2DE_TextData* H2DE_GetTextData(H2DE_Text* text) {
    return &text->textData;
}

H2DE_LevelSize H2DE_Text::getTextSize() const {
    size_t textLength = textData.text.size();
    return { textData.charSize.w * textLength + (textLength - 1) * textData.spacing, textData.charSize.h };
}

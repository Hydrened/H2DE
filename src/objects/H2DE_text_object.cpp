#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/H2DE_error.h"
#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData t) : H2DE_Object(engine, od), tod(t) {
    resetSurfaceBuffers();
}

H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_TextObjectData& tod) {
    H2DE_Error::checkEngine(engine);

    H2DE_TextObject* object = new H2DE_TextObject(engine, od, tod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    clearSurfaceBuffers();
}

// SURFACES
void H2DE_TextObject::resetSurfaceBuffers() {
    clearSurfaceBuffers();

    const std::unordered_map<std::string, H2DE_Font> fonts = engine->renderer->getFonts();
    const auto itFont = fonts.find(tod.font);
    if (itFont == fonts.end()) {
        return;
    }

    const std::string order = itFont->second.charOrder;
    const std::string textureName = itFont->second.textureName;
    const H2DE_ScaleMode fontScaleMode = itFont->second.scaleMode;

    const H2DE_LevelSize fontSize = tod.fontSize;
    const H2DE_LevelSize fontSpacing = tod.spacing;

    const int charWidth = itFont->second.charSize.x;
    const int charHeight = itFont->second.charSize.y;
    const int charSpacing = itFont->second.spacing;

    const float rotation = od.rotation;
    const H2DE_LevelPos pivot = od.pivot;

    const std::vector<std::vector<std::string>> lines = getLines();

    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        const std::vector<std::string>& line = lines[lineIndex];

        int currentLineLength = 0;
        const float lineStartOffsetX = getLineStartOffsetX(line);
        
        for (int wordIndex = 0; wordIndex < line.size(); wordIndex++) {
            const std::string& word = line[wordIndex];

            for (int charIndex = 0; charIndex < word.length(); charIndex++) {
                const char& c = word[charIndex];

                const auto itOrder = std::find(order.begin(), order.end(), c);
                if (itOrder == order.end() && c != ' ') {
                    continue;
                }

                const int charOrderIndex = order.find(c);

                H2DE_SurfaceData sd = H2DE_SurfaceData();
                sd.scaleMode = fontScaleMode;
                sd.color = tod.color;
                sd.textureName = textureName;

                H2DE_AbsRect srcRect = H2DE_AbsRect();
                srcRect.x = charOrderIndex * (charWidth + charSpacing);
                srcRect.y = 0;
                srcRect.w = charWidth;
                srcRect.h = charHeight;
                
                H2DE_TextureData td = H2DE_TextureData();
                td.srcRect = srcRect;

                const float offsetX = lineStartOffsetX + (charIndex + currentLineLength) * (fontSize.x + fontSpacing.x);
                const float offsetY = lineIndex * (fontSize.y + fontSpacing.y);

                H2DE_LevelPos offset = { offsetX, offsetY };

                H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
                buffer.surface = H2DE_CreateTexture(engine, sd, td);
                buffer.offset = offset;
                buffer.size = fontSize;
                
                surfaceBuffers.push_back(buffer);
            }

            currentLineLength += word.length();
        }
    }
}

// GETTER
std::vector<std::string> H2DE_TextObject::getWords() const {
    std::vector<std::string> words;
    std::string text = tod.text;

    size_t start = 0;
    while (start < text.size()) {
        size_t end = text.find(' ', start);

        if (end == std::string::npos) {
            words.push_back(text.substr(start));
            break;
        }

        words.push_back(text.substr(start, end - start + 1));
        start = end + 1;

        while (start < text.size() && text[start] == ' ') {
            start++;
        }
    }

    return words;
}

std::vector<std::vector<std::string>> H2DE_TextObject::getLines() const {
    std::vector<std::vector<std::string>> lines = {{}};

    const H2DE_LevelSize totalCharSize = tod.fontSize + tod.spacing;
    const int maxLineLength = std::floor(od.rect.w / totalCharSize.x);

    int currentLineIndex = 0;
    int currentLineLength = 0;

    for (const std::string& word : getWords()) {
        const int wordLength = word.length();

        if (currentLineLength + wordLength > maxLineLength) {
            std::vector<std::string>& currentLine = lines[currentLineIndex];

            if (currentLine.size() != 0) {
                std::string& lastWord = currentLine[currentLine.size() - 1];

                if (lastWord[lastWord.length() - 1] == ' ') {
                    lastWord.pop_back();
                }

                currentLineIndex++;
                currentLineLength = 0;

            } else {
                lines.at(currentLineIndex).push_back(word);

                if (currentLineLength == 0) {
                    currentLineIndex++;
                }
                continue;
            }
        }

        currentLineLength += wordLength;

        if (currentLineIndex == lines.size()) {
            lines.push_back({});
        }

        lines.at(currentLineIndex).push_back(word);
    }

    return lines;
}

float H2DE_TextObject::getLineStartOffsetX(const std::vector<std::string>& line) const {
    int nbChars = 0;
    for (const std::string& word : line) {
        nbChars += word.length();
    }

    const float lineLength = static_cast<float>(nbChars) * (tod.fontSize.x + tod.spacing.x);

    switch (tod.textAlign) {
        case H2DE_TEXT_ALIGN_LEFT: return 0.0f;
        case H2DE_TEXT_ALIGN_CENTER: return od.rect.w / 2.0f - lineLength / 2.0f + tod.spacing.x / 2.0f;
        case H2DE_TEXT_ALIGN_RIGHT: return od.rect.w - lineLength + tod.spacing.x;
        default: return 0.0f;
    }
}

// SETTER
void H2DE_SetTextObjectText(H2DE_TextObject* textObject, const std::string& text) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.text = text;
    textObject->resetSurfaceBuffers();
}

void H2DE_SetTextObjectFont(H2DE_TextObject* textObject, const std::string& font) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.font = font;
    textObject->resetSurfaceBuffers();
}

void H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.fontSize = fontSize;
    textObject->resetSurfaceBuffers();
}

unsigned int H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(textObject);

    const H2DE_LevelSize defaultFontSize = textObject->tod.fontSize;
    const H2DE_LevelSize fontSizeToAdd = fontSize - defaultFontSize;

    return H2DE_CreateTimeline(textObject->engine, duration, easing, [textObject, defaultFontSize, fontSizeToAdd](float blend) {
        H2DE_SetTextObjectFontSize(textObject, defaultFontSize + (fontSizeToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.spacing = spacing;
    textObject->resetSurfaceBuffers();
}

unsigned int H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(textObject);

    const H2DE_LevelSize defaultSpacing = textObject->tod.spacing;
    const H2DE_LevelSize spacingToAdd = spacing - defaultSpacing;

    return H2DE_CreateTimeline(textObject->engine, duration, easing, [textObject, defaultSpacing, spacingToAdd](float blend) {
        H2DE_SetTextObjectSpacing(textObject, defaultSpacing + (spacingToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetTextObjectTextAlign(H2DE_TextObject* textObject, H2DE_TextAlign textAlign) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.textAlign = textAlign;
    textObject->resetSurfaceBuffers();
}

void H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color) {
    H2DE_Error::checkObject(textObject);

    textObject->tod.color = color;
    textObject->resetSurfaceBuffers();
}

unsigned int H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(textObject);
    
    const H2DE_ColorRGB defaultColor = textObject->tod.color;
    
    return H2DE_CreateTimeline(textObject->engine, duration, easing, [textObject, defaultColor, color](float blend) {
        H2DE_ColorRGB interpolatedColor = H2DE_ColorRGB();
        interpolatedColor.r = static_cast<Uint8>(defaultColor.r + (color.r - defaultColor.r) * blend);
        interpolatedColor.g = static_cast<Uint8>(defaultColor.g + (color.g - defaultColor.g) * blend);
        interpolatedColor.b = static_cast<Uint8>(defaultColor.b + (color.b - defaultColor.b) * blend);
        interpolatedColor.a = static_cast<Uint8>(defaultColor.a + (color.a - defaultColor.a) * blend);
        
        H2DE_SetTextObjectColor(textObject, interpolatedColor);
    }, nullptr, 0, pauseSensitive);
}

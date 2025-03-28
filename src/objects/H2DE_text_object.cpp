#include "H2DE/H2DE_object.h"
#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData t) : H2DE_Object(engine, od), tod(t) {
    resetSurfaces();
}

H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_TextObjectData& tod) {
    H2DE_TextObject* object = new H2DE_TextObject(engine, od, tod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    destroySurfaces();
}

void H2DE_TextObject::destroySurfaces() {
    for (const H2DE_SurfaceBuffer surfaceBuffer : surfaceBuffers) {
        if (surfaceBuffer.surface) {
            delete surfaceBuffer.surface;
        }
    }

    surfaceBuffers.clear();
}

// UPDATE
void H2DE_TextObject::update() {
    
}

void H2DE_TextObject::resetSurfaces() {
    destroySurfaces();

    const std::unordered_map<std::string, H2DE_Font> fonts = engine->renderer->getFonts();
    auto itFont = fonts.find(tod.font);

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

    const std::vector<std::vector<std::string>> lines = getLines();
    
    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        const std::vector<std::string>& line = lines[lineIndex];

        int currentLineLength = 0;
        float lineStartOffsetX = getLineStartOffsetX(line);
        
        for (int wordIndex = 0; wordIndex < line.size(); wordIndex++) {
            const std::string& word = line[wordIndex];

            for (int charIndex = 0; charIndex < word.length(); charIndex++) {
                const char& c = word[charIndex];

                auto itOrder = std::find(order.begin(), order.end(), c);
                if (itOrder == order.end() && c != ' ') {
                    continue;
                }

                int charOrderIndex = order.find(c);

                H2DE_SurfaceData sd = H2DE_SurfaceData();
                sd.scaleMode = fontScaleMode;
                sd.color = tod.color;
                sd.textureName = textureName;

                H2DE_TextureData td = H2DE_TextureData();
                td.srcRect = H2DE_AbsRect{
                    charOrderIndex * (charWidth + charSpacing),
                    0,
                    charWidth,
                    charHeight,
                };

                float offsetX = lineStartOffsetX + (charIndex + currentLineLength) * (fontSize.x + fontSpacing.x);
                float offsetY = lineIndex * (fontSize.y + fontSpacing.y);

                H2DE_SurfaceBuffer buffer = H2DE_SurfaceBuffer();
                buffer.surface = H2DE_CreateTexture(engine, sd, td);
                buffer.offset.x = offsetX;
                buffer.offset.y = offsetY;
                buffer.size = fontSize;
                
                surfaceBuffers.push_back(buffer);
            }

            currentLineLength += word.length();
        }
    }
}

// GETTER
std::vector<H2DE_SurfaceBuffer> H2DE_TextObject::getSurfaces() const {
    return surfaceBuffers;
}

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
    const int maxLineLength = std::floor(od.size.x / totalCharSize.x);

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

        case H2DE_TEXT_ALIGN_CENTER: return od.size.x / 2.0f - lineLength / 2.0f + tod.spacing.x / 2.0f;

        case H2DE_TEXT_ALIGN_RIGHT: return od.size.x - lineLength + tod.spacing.x;

        default: return 0.0f;
    }
}

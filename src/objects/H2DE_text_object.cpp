#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#undef max

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TextObjectData& bod) : H2DE_Object(e, od), textObjectData(bod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    H2DE_Object::destroySurfaces(surfaceBuffers);
}

// ACTIONS
void H2DE_TextObject::refreshSurfaceBuffers() {
    H2DE_Object::destroySurfaces(surfaceBuffers);
    surfaceBuffers.clear();

    if (isTextNull()) {
        return;
    }

    auto it = engine->fonts.find(textObjectData.text.font);
    if (it == engine->fonts.end()) {
        return;
    }

    const H2DE_Font font = it->second;
    const std::string& fontTextureName = font.textureName;

    const H2DE_PixelSize& fontCharSize = font.charSize;
    const float& fontCharWidth = fontCharSize.x;
    const float& fontCharHeight = fontCharSize.y;
    
    const int& fontCharSpacing = font.spacing;

    const std::string& charOrder = font.charOrder;
    const H2DE_ScaleMode& scaleMode = font.scaleMode;
    const H2DE_BlendMode& blendMode = font.blendMode;

    const std::vector<std::vector<std::string>> lines = getLines();
    const float startingOffsetY = getStartingOffsetY(lines);
    const H2DE_Scale& charScale = textObjectData.text.fontSize;
    const H2DE_Scale totalCharScale = charScale + textObjectData.text.spacing;
    const H2DE_ColorRGB& color = textObjectData.text.color;

    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        const std::vector<std::string>& line = lines[lineIndex];

        const float startingOffsetX = getStartingOffsetX(line);
        int currentLineLength = 0;

        for (int wordIndex = 0; wordIndex < line.size(); wordIndex++) {
            const std::string& word = line[wordIndex];

            for (int charIndex = 0; charIndex < word.length(); charIndex++) {
                const char& c = word[charIndex];
                currentLineLength++;

                auto itOrder = std::find(charOrder.begin(), charOrder.end(), c);
                if (itOrder == charOrder.end()) {
                    continue;
                }

                const int charOrderIndex = charOrder.find(c);

                const float x = startingOffsetX + totalCharScale.x * currentLineLength;
                const float y = startingOffsetY + totalCharScale.y * lineIndex;

                H2DE_SurfaceData sd = H2DE_SurfaceData();
                sd.transform.translate = { x, y };
                sd.transform.scale = charScale;
                sd.scaleMode = scaleMode;
                sd.blendMode = blendMode;

                H2DE_TextureData td = H2DE_TextureData();
                td.textureName = fontTextureName;
                td.color = color;

                H2DE_PixelRect src = H2DE_PixelRect();
                src.x = charOrderIndex * (fontCharWidth + fontCharSpacing);
                src.y = 0;
                src.w = fontCharWidth;
                src.h = fontCharHeight;

                td.srcRect = src;

                H2DE_Texture* texture = new H2DE_Texture(engine, this, sd, td);
                surfaceBuffers.push_back(texture);
            }

            currentLineLength++;
        }

        currentLineLength = 0;
    }
}

void H2DE_TextObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    maxRadius = maxHitboxesRadius;
}

// GETTER
const std::vector<std::string> H2DE_TextObject::getWords() const {
    if (isTextNull()) {
        return {};
    }

    std::vector<std::string> words = {};
    std::string text = textObjectData.text.text;

    size_t start = 0;
    while (start < text.size()) {
        size_t end = text.find(' ', start);

        if (end == std::string::npos) {
            words.push_back(text.substr(start));
            break;
        }

        words.push_back(text.substr(start, end - start));
        start = end + 1;

        while (start < text.size() && text[start] == ' ') {
            start++;
        }
    }

    return words;
}

const std::vector<std::vector<std::string>> H2DE_TextObject::getLines() const {
    if (isTextNull()) {
        return {};
    }

    std::vector<std::vector<std::string>> lines = {{}};

    const H2DE_Scale charTotalScale = textObjectData.text.fontSize + textObjectData.text.spacing;
    const float totalWidthPadding = textObjectData.text.padding.left + textObjectData.text.padding.right;
    const int maxNbCharPerLine = std::floor(objectData.transform.scale.x / charTotalScale.x + textObjectData.text.spacing.x - totalWidthPadding);

    int currentLineIndex = 0;

    for (const std::string& word : getWords()) {
        const int wordLength = word.length();

        if (wordLength > maxNbCharPerLine) {

            const float currentLineLength = H2DE_TextObject::getLineLength(lines[currentLineIndex]);
            if (currentLineLength != 0) {
                lines.push_back({});
                currentLineIndex++;
            }

            lines[currentLineIndex].push_back(word);
            
        } else {
            const float currentLineLength = H2DE_TextObject::getLineLength(lines[currentLineIndex]);
            bool canBeOnCurrentLine = (currentLineLength + wordLength <= maxNbCharPerLine);

            if (!canBeOnCurrentLine) {
                lines.push_back({});
                currentLineIndex++;
            }

            lines[currentLineIndex].push_back(word);
        }
    }

    return lines;
}

int H2DE_TextObject::getLineLength(const std::vector<std::string>& line) {
    int length = std::max(line.size() - 1, (size_t)0);

    for (const std::string& word : line) {
        length += word.length();
    }

    return length;
}

float H2DE_TextObject::getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_TOP       = 0b00001000;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00010000;
    constexpr uint8_t H2DE_TEXT_ALIGN_BOTTOM    = 0b00100000;

    float halfObjectHeight = objectData.transform.scale.y * 0.5f;
    float totalCharHeight = textObjectData.text.fontSize.y + textObjectData.text.spacing.y;
    float halfCharHeight = textObjectData.text.fontSize.y * 0.5f;
    float totalTextHeight = totalCharHeight * lines.size() - textObjectData.text.spacing.y;
    float totalHalfTextHeight = totalTextHeight * 0.5f;

    bool isAlignedTop = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_TOP) != 0);
    if (isAlignedTop) {
        return -halfObjectHeight + textObjectData.text.padding.top + halfCharHeight;
    }

    bool isAlignedCenter = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_CENTER) != 0);
    if (isAlignedCenter) {
        return -totalHalfTextHeight + halfCharHeight;
    }

    bool isAlignedBottom = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_BOTTOM) != 0);
    if (isAlignedBottom) {
        return halfObjectHeight - textObjectData.text.padding.bottom - totalTextHeight + halfCharHeight;
    }

    return 0.0f;
}

float H2DE_TextObject::getStartingOffsetX(const std::vector<std::string>& line) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_LEFT      = 0b00000001;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00000010;
    constexpr uint8_t H2DE_TEXT_ALIGN_RIGHT     = 0b00000100;

    float halfObjectWidth = objectData.transform.scale.x * 0.5f;
    float totalCharWidth = textObjectData.text.fontSize.x + textObjectData.text.spacing.x;
    float halfCharWidth = textObjectData.text.fontSize.x * 0.5f;
    float totalTextWidth = totalCharWidth * getLineLength(line) - textObjectData.text.spacing.x;
    float totalHalfTextWidth = totalTextWidth * 0.5f;

    bool isAlignedLeft = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_LEFT) != 0);
    if (isAlignedLeft) {
        return -halfObjectWidth + textObjectData.text.padding.left - halfCharWidth - textObjectData.text.spacing.x;
    }

    bool isAlignedCenter = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_CENTER) != 0);
    if (isAlignedCenter) {
        return -totalHalfTextWidth - halfCharWidth - textObjectData.text.spacing.x;
    }

    bool isAlignedRight = ((textObjectData.text.textAlign & H2DE_TEXT_ALIGN_RIGHT) != 0);
    if (isAlignedRight) {
        return halfObjectWidth - textObjectData.text.padding.right - totalTextWidth - halfCharWidth - textObjectData.text.spacing.x;
    }

    return 0.0f;
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

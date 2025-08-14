#include "H2DE/objects/H2DE_text_object.h"

#include "H2DE/engine/H2DE_lerp_manager.h"

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

    const auto& fontCharacters = font._characters;
    const H2DE_ScaleMode& scaleMode = font.scaleMode;
    const H2DE_BlendMode& blendMode = font.blendMode;
    
    const float& fontSize = textObjectData.text.fontSize;
    const H2DE_ColorRGB& color = textObjectData.text.color;
    const H2DE_Scale& spacing = textObjectData.text.spacing;

    const float fixedFontSize = getFixedFontSize();

    const std::vector<std::vector<std::string>> lines = getLines();
    float offsetY = getStartingOffsetY(lines) + fontSize * 0.5f;

    const float spaceWidth = getCharWidth(' ');

    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        const std::vector<std::string>& line = lines.at(lineIndex);

        float offsetX = getStartingOffsetX(line);

        for (int wordIndex = 0; wordIndex < line.size(); wordIndex++) {
            const std::string& word = line.at(wordIndex);
            bool isFirstLineWord = (wordIndex == 0);
            bool isLastLineWord = (wordIndex == line.size() - 1);

            for (int charIndex = 0; charIndex < word.length(); charIndex++) {
                const char& c = word.at(charIndex);
                bool isFirstWordFirstChar = (charIndex == 0 && isFirstLineWord);

                auto it = fontCharacters.find(std::string(1, c));
                if (it == fontCharacters.end()) {
                    continue;
                }

                const float charWidth = getCharWidth(c);
                offsetX += charWidth * 0.5f;

                if (!isFirstWordFirstChar) {
                    offsetX += spacing.x;
                }

                H2DE_SurfaceData sd = H2DE_SurfaceData();
                sd.transform.translate = { offsetX, offsetY };
                sd.transform.scale = { charWidth, fixedFontSize };
                sd.scaleMode = scaleMode;
                sd.blendMode = blendMode;

                H2DE_TextureData td = H2DE_TextureData();
                td.textureName = fontTextureName;
                td.color = color;
                td.srcRect = it->second;

                H2DE_Texture* texture = new H2DE_Texture(engine, this, sd, td);
                texture->fromText = true;
                surfaceBuffers.push_back(texture);

                offsetX += charWidth * 0.5f;
            }

            if (!isLastLineWord) {
                offsetX += spaceWidth;
            }
        }

        offsetY += fontSize + spacing.y;
    }

    rescaleSurfaceBuffers();
}

void H2DE_TextObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    maxRadius = maxHitboxesRadius;
}

// GETTER

// -- format
const std::string H2DE_TextObject::getFormatedText() const {
    const std::string txt = textObjectData.text.text;
    std::string res = "";

    for (int i = 0; i < txt.length() - 1; i++) {
        const char& cc = txt.at(i);
        const char& nc = txt.at(i + 1);

        if (nc == '\n' && cc == ' ') {
            continue;
        }

        if (cc != '\n') {
            res += cc;
            continue;
        }

        res += '\n';
        if (nc != ' ') {
            res += ' ';
        }
    }

    return res + txt.substr(txt.length() - 1, 1);
}

const std::string H2DE_TextObject::getFormatedWord(const std::string& word) const {
    std::string res = "";

    for (const char& c : word) {
        if (c == '\n') {
            continue;
        }

        if (getCharWidth(c) == 0.0f) {
            continue;
        }

        res += c;
    }

    return res;
}

// -- lines words
const std::vector<std::string> H2DE_TextObject::getWords() const {
    if (isTextNull()) {
        return {};
    }

    std::vector<std::string> res = {};
    const std::string text = getFormatedText();

    size_t start = 0;
    while (start < text.size()) {
        bool lineBreak = (text.find('\n', start) != std::string::npos);
        size_t end = text.find(' ', start);

        if (end == std::string::npos) {
            const std::string word = text.substr(start) + (lineBreak ? "\n" : "");
            res.push_back(text.substr(start));
            break;
        }

        const std::string word = text.substr(start, end - start) + (lineBreak ? "\n" : "");
        res.push_back(word);
        start = end + 1;

        while (start < text.size() && text[start] == ' ') {
            start++;
        }
    }

    return res;
}

const std::vector<std::vector<std::string>> H2DE_TextObject::getLines() const {
    if (isTextNull()) {
        return {};
    }

    std::vector<std::vector<std::string>> lines = {{}};
    int currentLineIndex = 0;

    const H2DE_Padding& padding = textObjectData.text.padding;
    const float& spacingX = textObjectData.text.spacing.x;

    const float maxWidthPerLine = textObjectData.text.bounds.x - padding.left - padding.right;
    const float spaceWidth = getCharWidth(' ');

    for (const std::string& word : getWords()) {
        bool lineBreak = (word.find('\n') != std::string::npos);
        const std::string formatedWord = getFormatedWord(word);

        const float wordWidth = getWordWidth(formatedWord);
        const float currentLineWidth = getLineWidth(lines[currentLineIndex]) + spaceWidth + spacingX;

        if (wordWidth > maxWidthPerLine) {
            if (currentLineWidth != 0.0f) {
                lines.push_back({});
                currentLineIndex++;
            }

            lines[currentLineIndex].push_back(formatedWord);

        } else {
            bool canBeOnCurrentLine = (currentLineWidth + wordWidth <= maxWidthPerLine);

            if (!canBeOnCurrentLine) {
                lines.push_back({});
                currentLineIndex++;
            }

            lines[currentLineIndex].push_back(formatedWord);
        }

        if (lineBreak) {
            lines.push_back({});
            currentLineIndex++;
        }
    }

    return lines;
}

// -- scales
float H2DE_TextObject::getCharWidth(const char& c) const {
    auto itFont = engine->fonts.find(getFont());
    if (itFont == engine->fonts.end()) {
        return 0.0f;
    }

    const auto& fontCharacters = itFont->second._characters;

    auto itChar = fontCharacters.find(std::string(1, c));
    if (itChar == fontCharacters.end()) {
        return 0.0f;
    }

    const int& characterWidth = itChar->second.w;
    const int& characterHeight = itFont->second._characterHeight;

    return static_cast<float>(characterWidth) / static_cast<float>(characterHeight) * getFixedFontSize();
}

float H2DE_TextObject::getWordWidth(const std::string& word) const {
    float res = 0.0f;

    auto it = engine->fonts.find(textObjectData.text.font);
    if (it == engine->fonts.end()) {
        return res;
    }

    const float& spacingX = textObjectData.text.spacing.x;

    for (const char& c : word) {
        res += getCharWidth(c);
    }

    return res + (word.length() - 1) * spacingX;
}

float H2DE_TextObject::getLineWidth(const std::vector<std::string>& line) const {
    float res = 0.0f;

    const float spaceWidth = getCharWidth(' ');

    for (const std::string& word : line) {
        res += getWordWidth(word);
    }

    return res + H2DE::max<int>(line.size() - 1, 0) * (spaceWidth + textObjectData.text.spacing.x);
}

float H2DE_TextObject::getTextHeight() const {
    int nbLines = getLines().size();
    return (textObjectData.text.fontSize * nbLines) + (textObjectData.text.spacing.y * (nbLines - 1)); 
}

// -- offset
float H2DE_TextObject::getStartingOffsetX(const std::vector<std::string>& line) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_LEFT      = 0b00000001;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00000010;
    constexpr uint8_t H2DE_TEXT_ALIGN_RIGHT     = 0b00000100;

    const H2DE_TextAlign& textAlign = textObjectData.text.textAlign;
    const H2DE_Padding& padding = textObjectData.text.padding;

    const H2DE_Scale halfBounds = textObjectData.text.bounds * 0.5f;
    const float lineWidth = getLineWidth(line);

    bool isAlignedCenter = ((textAlign & H2DE_TEXT_ALIGN_CENTER) != 0);
    if (isAlignedCenter) {
        return -lineWidth * 0.5f;
    }

    bool isAlignedLeft = ((textAlign & H2DE_TEXT_ALIGN_LEFT) != 0);
    if (isAlignedLeft) {
        return -halfBounds.x + padding.left;
    }

    bool isAlignedRight = ((textAlign & H2DE_TEXT_ALIGN_RIGHT) != 0);
    if (isAlignedRight) {
        return halfBounds.x - padding.right - lineWidth;
    }

    return 0.0f;
}

float H2DE_TextObject::getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_TOP       = 0b00001000;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00010000;
    constexpr uint8_t H2DE_TEXT_ALIGN_BOTTOM    = 0b00100000;

    const H2DE_TextAlign& textAlign = textObjectData.text.textAlign;
    const H2DE_Padding& padding = textObjectData.text.padding;

    const H2DE_Scale halfBounds = textObjectData.text.bounds * 0.5f;
    const float textHeight = getTextHeight();

    bool isAlignedCenter = ((textAlign & H2DE_TEXT_ALIGN_CENTER) != 0);
    if (isAlignedCenter) {
        return -textHeight * 0.5f;
    }

    bool isAlignedTop = ((textAlign & H2DE_TEXT_ALIGN_TOP) != 0);
    if (isAlignedTop) {
        return -halfBounds.y + padding.top;
    }

    bool isAlignedBottom = ((textAlign & H2DE_TEXT_ALIGN_BOTTOM) != 0);
    if (isAlignedBottom) {
        return halfBounds.y - padding.bottom - textHeight;
    }

    return 0.0f;
}

float H2DE_TextObject::getFixedFontSize() const {
    auto itFont = engine->fonts.find(textObjectData.text.font);
    if (itFont == engine->fonts.end()) {
        return textObjectData.text.fontSize;
    }

    const int& characterHeight = itFont->second._characterHeight;
    const int& fixedCharacterHeight = itFont->second._fixedCharacterHeight;

    return (characterHeight / static_cast<float>(fixedCharacterHeight)) * textObjectData.text.fontSize;
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

void H2DE_TextObject::setBounds(const H2DE_Scale& bounds) {
    textObjectData.text.bounds = bounds;
    refreshSurfaceBuffers();
}

void H2DE_TextObject::setFontSize(float fontSize) {
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

void H2DE_TextObject::setPadding(const H2DE_Padding& padding) {
    textObjectData.text.padding = padding;
    refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_TextObject::setBounds(const H2DE_Scale& bounds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.text.bounds, bounds, duration, easing, [this](H2DE_Scale iv) {
        setBounds(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setFontSize(float fontSize, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(engine, textObjectData.text.fontSize, fontSize, duration, easing, [this](float iv) {
        setFontSize(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setSpacing(const H2DE_Scale& spacing, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(engine, textObjectData.text.spacing, spacing, duration, easing, [this](H2DE_Scale iv) {
        setSpacing(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(engine, textObjectData.text.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

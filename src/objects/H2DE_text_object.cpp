#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TextObjectData& bod) : H2DE_Object(e, od), _textObjectData(bod) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    H2DE_Object::_destroySurfaces(_surfaceBuffers);
}

// ACTIONS
void H2DE_TextObject::_refreshSurfaceBuffers() {
    H2DE_Object::_destroySurfaces(_surfaceBuffers);
    _surfaceBuffers.clear();

    if (_isTextNull()) {
        return;
    }

    auto it = _engine->_fonts.find(_textObjectData.text.font);
    if (it == _engine->_fonts.end()) {
        return;
    }

    const H2DE_Font font = it->second;
    const std::string& fontTextureName = font.textureName;

    const auto& fontCharacters = font._characters;
    const H2DE_ScaleMode& scaleMode = font.scaleMode;
    const H2DE_BlendMode& blendMode = font.blendMode;
    
    const float& fontSize = _textObjectData.text.fontSize;
    const H2DE_ColorRGB& color = _textObjectData.text.color;
    const H2DE_Scale& spacing = _textObjectData.text.spacing;

    const float fixedFontSize = _getFixedFontSize();

    const std::vector<std::vector<std::string>> lines = _getLines();
    float offsetY = _getStartingOffsetY(lines) + fontSize * 0.5f;

    auto createCharSurface = [this](const H2DE_TextObject::_H2DE_CharacterSurfaceData& data) {
        const float charWidth = _getCharacterWidth(data.c);
        data.offsetX += charWidth * 0.5f;

        if (!data.isFirstWordFirstChar) {
            data.offsetX += data.spacingX;
        }

        H2DE_SurfaceData sd = H2DE_SurfaceData();
        sd.transform.translate = { data.offsetX, data.offsetY };
        sd.transform.scale = { charWidth, data.fixedFontSize };
        sd.scaleMode = data.scaleMode;
        sd.blendMode = data.blendMode;

        H2DE_TextureData td = H2DE_TextureData();
        td.textureName = data.fontTextureName;
        td.color = data.color;
        td.srcRect = data.src;

        H2DE_Texture* texture = new H2DE_Texture(_engine, this, sd, td);
        texture->_fromText = true;
        _surfaceBuffers.push_back(texture);

        data.offsetX += charWidth * 0.5f;
    };

    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        const std::vector<std::string>& line = lines.at(lineIndex);

        float offsetX = _getStartingOffsetX(line);

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

                createCharSurface({ c, offsetX, offsetY, fixedFontSize, spacing.x, isFirstWordFirstChar, fontTextureName, color, it->second, scaleMode, blendMode });
            }

            if (!isLastLineWord) {
                auto it = fontCharacters.find(std::string(1, ' '));
                if (it == fontCharacters.end()) {
                    continue;
                }

                createCharSurface({ ' ', offsetX, offsetY, fixedFontSize, spacing.x, false, fontTextureName, color, it->second, scaleMode, blendMode });
            }
        }

        offsetY += fontSize + spacing.y;
    }

    _rescaleSurfaceBuffers();
}

void H2DE_TextObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    _maxRadius = maxHitboxesRadius;
}

// GETTER

// -- format
const std::string H2DE_TextObject::_getFormatedText() const {
    const std::string& txt = _textObjectData.text.text;
    
    if (txt.length() == 1) {
        return txt;
    }

    std::string res = "";

    for (int i = 1; i < txt.length() - 1; i++) {
        const char& pc = txt.at(i - 1);
        const char& cc = txt.at(i);
        const char& nc = txt.at(i + 1);

        bool isFriendReturn = (pc == '\n' || nc == '\n');
        if (cc == ' ' && isFriendReturn) {
            continue;
        }

        res += cc;
    }

    return txt.substr(0, 1) + res + txt.substr(txt.length() - 1, 1);
}

const std::string H2DE_TextObject::_getFormatedWord(const std::string& word) const {
    std::string res = "";

    for (const char& c : word) {
        if (c == '\n') {
            continue;
        }

        if (_getCharacterWidth(c) == 0.0f) {
            continue;
        }

        res += c;
    }

    return res;
}

// -- lines words
const std::vector<std::string> H2DE_TextObject::_getWords() const {
    if (_isTextNull()) {
        return {};
    }

    std::vector<std::string> res = {};

    const std::string text = _getFormatedText();
    std::string currentWord = "";

    for (const char& c : text) {
        if (c == ' ' || c == '\n') {
            if (c == '\n') {
                currentWord += c;
            }

            res.push_back(currentWord);
            currentWord = "";

        } else {
            currentWord += c;
        }
    }

    res.push_back(currentWord);
    return res;
}

const std::vector<std::vector<std::string>> H2DE_TextObject::_getLines() const {
    if (_isTextNull()) {
        return {};
    }

    std::vector<std::vector<std::string>> res = {{}};

    const H2DE_Padding& padding = _textObjectData.text.padding;
    const float& spacingX = _textObjectData.text.spacing.x;
    const float spaceWidth = _getCharacterWidth(' ');
    const float maxLineWidth = _textObjectData.text.bounds.x - padding.left - padding.right;

    float currentLineWidth = 0.0f;

    for (const std::string& word : _getWords()) {

        bool lineBreak = (word.find('\n') != std::string::npos);
        bool isLineFirstWord = (currentLineWidth == 0.0f);

        const std::string formatedWord = _getFormatedWord(word);
        const float wordWidth = _getWordWidth(formatedWord);

        const float futureLineWidth = (!isLineFirstWord)
            ? currentLineWidth + spacingX + spaceWidth + wordWidth
            : wordWidth;

        bool notEnoughSpaceForWord = (futureLineWidth > maxLineWidth);

        if (notEnoughSpaceForWord && !isLineFirstWord) {
            res.push_back({});
            currentLineWidth = 0.0f;
        }

        res.back().push_back(formatedWord);
        currentLineWidth += (currentLineWidth != 0.0f)
            ? spacingX + spaceWidth + wordWidth
            : wordWidth;

        if (lineBreak) {
            res.push_back({});
            currentLineWidth = 0.0f;
        }
    }

    return res;
}

// -- scales
float H2DE_TextObject::_getCharacterWidth(const char& c) const {
    auto itFont = _engine->_fonts.find(getFont());
    if (itFont == _engine->_fonts.end()) {
        return 0.0f;
    }

    const auto& fontCharacters = itFont->second._characters;

    auto itChar = fontCharacters.find(std::string(1, c));
    if (itChar == fontCharacters.end()) {
        return 0.0f;
    }

    const int& characterWidth = itChar->second.w;
    const int& characterHeight = itFont->second._characterHeight;

    return static_cast<float>(characterWidth) / static_cast<float>(characterHeight) * _getFixedFontSize();
}

float H2DE_TextObject::_getWordWidth(const std::string& word) const {
    if (word.length() == 0) {
        return 0.0f;
    }

    float res = 0.0f;

    auto it = _engine->_fonts.find(_textObjectData.text.font);
    if (it == _engine->_fonts.end()) {
        return res;
    }

    const float& spacingX = _textObjectData.text.spacing.x;

    for (const char& c : word) {
        res += _getCharacterWidth(c);
    }

    return res + (word.length() - 1) * spacingX;
}

float H2DE_TextObject::_getLineWidth(const std::vector<std::string>& line) const {
    float wordsWidth = 0.0f;

    for (const std::string& word : line) {
        wordsWidth += _getWordWidth(word);
    }

    int nbWords = H2DE::max<int>(line.size() - 1, 0);

    const float spacingWidthX = nbWords * _textObjectData.text.spacing.x;
    const float spacesTotalWidth = nbWords * (_getCharacterWidth(' ') + _textObjectData.text.spacing.x);

    return wordsWidth + spacingWidthX + spacesTotalWidth;
}

float H2DE_TextObject::_getTextHeight() const {
    int nbLines = _getLines().size();
    return (_textObjectData.text.fontSize * nbLines) + (_textObjectData.text.spacing.y * (nbLines - 1)); 
}

// -- offset
float H2DE_TextObject::_getStartingOffsetX(const std::vector<std::string>& line) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_LEFT      = 0b00000001;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00000010;
    constexpr uint8_t H2DE_TEXT_ALIGN_RIGHT     = 0b00000100;

    const H2DE_TextAlign& textAlign = _textObjectData.text.textAlign;
    const H2DE_Padding& padding = _textObjectData.text.padding;

    const H2DE_Scale halfBounds = _textObjectData.text.bounds * 0.5f;
    const float lineWidth = _getLineWidth(line);

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

float H2DE_TextObject::_getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const {
    constexpr uint8_t H2DE_TEXT_ALIGN_TOP       = 0b00001000;
    constexpr uint8_t H2DE_TEXT_ALIGN_CENTER    = 0b00010000;
    constexpr uint8_t H2DE_TEXT_ALIGN_BOTTOM    = 0b00100000;

    const H2DE_TextAlign& textAlign = _textObjectData.text.textAlign;
    const H2DE_Padding& padding = _textObjectData.text.padding;

    const H2DE_Scale halfBounds = _textObjectData.text.bounds * 0.5f;
    const float textHeight = _getTextHeight();

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

float H2DE_TextObject::_getFixedFontSize() const {
    auto itFont = _engine->_fonts.find(_textObjectData.text.font);
    if (itFont == _engine->_fonts.end()) {
        return _textObjectData.text.fontSize;
    }

    const int& characterHeight = itFont->second._characterHeight;
    const int& fixedCharacterHeight = itFont->second._fixedCharacterHeight;

    return (characterHeight / static_cast<float>(fixedCharacterHeight)) * _textObjectData.text.fontSize;
}

// SETTER

// -- no lerp
void H2DE_TextObject::setText(const std::string& text) {
    _textObjectData.text.text = text;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setFont(const std::string& font) {
    _textObjectData.text.font = font;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setBounds(const H2DE_Scale& bounds) {
    _textObjectData.text.bounds = bounds;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setFontSize(float fontSize) {
    _textObjectData.text.fontSize = fontSize;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setSpacing(const H2DE_Scale& spacing) {
    _textObjectData.text.spacing = spacing;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setTextAlign(H2DE_TextAlign textAlign) {
    _textObjectData.text.textAlign = textAlign;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setColor(const H2DE_ColorRGB& color) {
    _textObjectData.text.color = color;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setPadding(const H2DE_Padding& padding) {
    _textObjectData.text.padding = padding;
    _refreshSurfaceBuffers();
}

// -- lerp
H2DE_Timeline* H2DE_TextObject::setBounds(const H2DE_Scale& bounds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(_engine, _textObjectData.text.bounds, bounds, duration, easing, [this](H2DE_Scale iv) {
        setBounds(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setFontSize(float fontSize, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<float>(_engine, _textObjectData.text.fontSize, fontSize, duration, easing, [this](float iv) {
        setFontSize(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setSpacing(const H2DE_Scale& spacing, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Scale>(_engine, _textObjectData.text.spacing, spacing, duration, easing, [this](H2DE_Scale iv) {
        setSpacing(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TextObject::setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(_engine, _textObjectData.text.color, color, duration, easing, [this](H2DE_ColorRGB iv) {
        setColor(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

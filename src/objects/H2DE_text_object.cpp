#include "H2DE/objects/H2DE_text_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TextObjectData& bod) : H2DE_Object(e, od), _textObjectData(bod) {
    _textObjectData.text.text = H2DE_TextSurfaceObject::_getFormatedText(_textObjectData.text.text);

    _refreshSurfaceBuffers();
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {
    H2DE_Object::_destroySurfaces(_surfaceBuffers);
}

// ACTIONS
void H2DE_TextObject::_refreshLines() {
    _lines.clear();

    _lines = _H2DE_Lines();
    _lines.push_back(_H2DE_Line());

    const H2DE_Padding& padding = _textObjectData.text.padding;
    const float& spacingX = _textObjectData.text.spacing.x;
    const float maxLineWidth = _textObjectData.text.bounds.x - padding.left - padding.right;

    float currentLineWidth = 0.0f;

    auto createNewLine = [this, &currentLineWidth]() {
        if (!_lines.empty()) {
            _H2DE_Word& lastWord = _lines.back().back();

            if (lastWord.find(' ') != std::string::npos) {
                lastWord = lastWord.substr(0, lastWord.length() - 1);
                lastWord += '\n';
            }
        }

        _lines.push_back(_H2DE_Line());
        currentLineWidth = 0.0f;
    };

    for (const _H2DE_Word& word : _getWords()) {

        bool lineBreak = (word.find('\n') != std::string::npos);
        bool isLineFirstWord = (currentLineWidth == 0.0f);

        const float wordWidth = _getWordWidth(word);

        const float futureLineWidth = (!isLineFirstWord)
            ? currentLineWidth + spacingX + wordWidth
            : wordWidth;

        bool notEnoughSpaceForWord = (futureLineWidth > maxLineWidth);
        if (notEnoughSpaceForWord && !isLineFirstWord) {
            createNewLine();
        }

        _lines.back().push_back(word);
        currentLineWidth += (currentLineWidth != 0.0f)
            ? spacingX + wordWidth
            : wordWidth;

        if (lineBreak) {
            createNewLine();
        }
    }
}

void H2DE_TextObject::_refreshSurfaceBuffers() {
    H2DE_Object::_destroySurfaces(_surfaceBuffers);
    _surfaceBuffers.clear();

    _refreshLines();

    auto it = _engine->_fonts.find(_textObjectData.text.font);
    if (it == _engine->_fonts.end()) {
        return;
    }

    auto createCharSurface = [this](const _H2DE_CharacterSurfaceData& data) {
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

    const H2DE_Font font = it->second;
    const std::string& fontTextureName = font.textureName;
    const auto& fontCharacters = font._characters;
    const H2DE_ScaleMode& scaleMode = font.scaleMode;
    const H2DE_BlendMode& blendMode = font.blendMode;
    
    const float& fontSize = _textObjectData.text.fontSize;
    const H2DE_ColorRGB& color = _textObjectData.text.color;
    const H2DE_Scale& spacing = _textObjectData.text.spacing;

    const float fixedFontSize = _getFixedFontSize();

    float offsetY = _getStartingOffsetY(_lines) + fontSize * 0.5f;

    for (int lineIndex = 0; lineIndex < _lines.size(); lineIndex++) {
        const _H2DE_Line& line = _lines.at(lineIndex);

        float offsetX = _getStartingOffsetX(line);

        for (int wordIndex = 0; wordIndex < line.size(); wordIndex++) {
            const _H2DE_Word& word = line.at(wordIndex);
            bool isFirstLineWord = (wordIndex == 0);

            for (int charIndex = 0; charIndex < word.length(); charIndex++) {
                const char& c = word.at(charIndex);
                bool isFirstWordFirstChar = (charIndex == 0 && isFirstLineWord);

                H2DE_PixelRect src = { 0, 0, 0, 0 };
                
                auto it = fontCharacters.find(std::string(1, c));
                if (it != fontCharacters.end()) {
                    src = it->second;
                }

                createCharSurface({
                    static_cast<unsigned char>(c),
                    offsetX,
                    offsetY,
                    fixedFontSize,
                    spacing.x,
                    isFirstWordFirstChar,
                    fontTextureName,
                    color,
                    src,
                    scaleMode,
                    blendMode
                });
            }
        }

        offsetY += fontSize + spacing.y;
    }

    _rescaleSurfaceBuffers();
}

// GETTER

// -- lines words
const std::vector<H2DE_TextObject::_H2DE_Word> H2DE_TextObject::_getWords() const {
    if (_textObjectData.text.text == "") {
        return {};
    }

    const std::string formatedText = H2DE_TextSurfaceObject::_getFormatedText(_textObjectData.text.text);

    std::vector<_H2DE_Word> res = {};
    _H2DE_Word currentWord = "";

    for (const char& c : formatedText) {

        currentWord += c;

        if (c == ' ' || c == '\n') {
            res.push_back(currentWord);
            currentWord = "";
        } 
    }

    res.push_back(currentWord);
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

float H2DE_TextObject::_getWordWidth(const _H2DE_Word& word) const {
    float res = 0.0f;

    if (word.length() == 0) {
        return res;
    }

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

float H2DE_TextObject::_getLineWidth(const _H2DE_Line& line) const {
    float wordsWidth = 0.0f;

    for (const _H2DE_Word& word : line) {
        wordsWidth += _getWordWidth(word);
    }

    int nbWords = H2DE::max<int>(line.size() - 1, 0);

    const float spacingWidthX = nbWords * _textObjectData.text.spacing.x;

    return wordsWidth + spacingWidthX;
}

float H2DE_TextObject::_getTextHeight() const {
    int nbLines = _lines.size();
    return (_textObjectData.text.fontSize * nbLines) + (_textObjectData.text.spacing.y * (nbLines - 1)); 
}

// -- offset
float H2DE_TextObject::_getStartingOffsetX(const _H2DE_Line& line) const {
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

float H2DE_TextObject::_getStartingOffsetY(const _H2DE_Lines& lines) const {
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
    if (H2DE_TextSurfaceObject::_getFormatedText(text) == _textObjectData.text.text) {
        return;
    }

    _textObjectData.text.text = H2DE_TextSurfaceObject::_getFormatedText(text);
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setFont(const std::string& font) {
    if (_textObjectData.text.font == font) {
        return;
    }

    _textObjectData.text.font = font;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setBounds(const H2DE_Scale& bounds) {
    if (_textObjectData.text.bounds == bounds) {
        return;
    }

    _textObjectData.text.bounds = bounds;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setFontSize(float fontSize) {
    if (_textObjectData.text.fontSize == fontSize) {
        return;
    }

    _textObjectData.text.fontSize = fontSize;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setSpacing(const H2DE_Scale& spacing) {
    if (_textObjectData.text.spacing == spacing) {
        return;
    }

    _textObjectData.text.spacing = spacing;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setTextAlign(H2DE_TextAlign textAlign) {
    if (_textObjectData.text.textAlign == textAlign) {
        return;
    }

    _textObjectData.text.textAlign = textAlign;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setColor(const H2DE_ColorRGB& color) {
    if (_textObjectData.text.color == color) {
        return;
    }

    _textObjectData.text.color = color;
    _refreshSurfaceBuffers();
}

void H2DE_TextObject::setPadding(const H2DE_Padding& padding) {
    if (_textObjectData.text.padding == padding) {
        return;
    }

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

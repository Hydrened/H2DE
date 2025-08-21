#pragma once

/**
 * @file H2DE_text_object.h
 * @brief Definition of the H2DE_TextObject class for 2D text rendering.
 * 
 * This file declares the H2DE_TextObject class, a specialized H2DE_Object
 * responsible for rendering and managing text within the H2DE 2D engine.
 * It supports text properties such as font, size, spacing, alignment, and color,
 * with immediate or animated transitions using the engine's timeline system.
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @class H2DE_TextObject
 * @brief A 2D object for rendering and animating text in the H2DE engine.
 * 
 * H2DE_TextObject extends H2DE_Object to provide rich text display capabilities.
 * You can set text content, font, size, spacing, alignment, and color.
 * It also supports smooth animated transitions of font size, spacing, and color
 * through the built-in timeline animation system, enabling slick visual effects.
 * 
 * This class handles all the internal data for text rendering and exposes
 * getters and setters for easy manipulation.
 */
class H2DE_TextObject : public H2DE_Object {
public:
    /**
     * @brief Get the internal data structure of the text object.
     * 
     * Returns a copy of the `H2DE_TextObjectData` which holds all text-related configuration and state.
     * 
     * @return The full data structure of the text object.
     */
    inline H2DE_TextObjectData getTextData() const noexcept {
        return _textObjectData;
    }
    /**
     * @brief Get the text string displayed by the text object.
     * 
     * Returns the actual string content rendered by the object.
     * 
     * @return The displayed text as a std::string.
     */
    inline std::string getText() const {
        return _textObjectData.text.text;
    }
    /**
     * @brief Get the font name used for rendering the text.
     * 
     * Returns the font family or font file name currently set for the text object.
     * 
     * @return The font name as a std::string.
     */
    inline std::string getFont() const {
        return _textObjectData.text.font;
    }
    /**
     * @brief Get the bounds scale.
     * 
     * Returns the scale of the bounds used for containing the text.
     * 
     * @return The scale of the bounds as an H2DE_Scale value.
     */
    constexpr H2DE_Scale getBounds() const noexcept {
        return _textObjectData.text.bounds;
    }
    /**
     * @brief Get the font size scale.
     * 
     * Returns the scaling factor used for the font size, allowing dynamic resizing.
     * 
     * @return The font size scale as an H2DE_Scale value.
     */
    constexpr float getFontSize() const noexcept {
        return _textObjectData.text.fontSize;
    }
    /**
     * @brief Get the spacing between characters.
     * 
     * Returns the horizontal spacing applied between each character in the text.
     * 
     * @return The character spacing as an H2DE_Scale value.
     */
    constexpr H2DE_Scale getSpacing() const noexcept {
        return _textObjectData.text.spacing;
    }
    /**
     * @brief Get the horizontal alignment of the text.
     * 
     * Returns the text alignment setting (e.g. left, center, right).
     * 
     * @return The text alignment as an H2DE_TextAlign enum value.
     */
    constexpr H2DE_TextAlign getTextAlign() const noexcept {
        return _textObjectData.text.textAlign;
    }
    /**
     * @brief Get the color of the text.
     * 
     * Returns the RGB color currently applied to the rendered text.
     * 
     * @return The text color as an H2DE_ColorRGB struct.
     */
    constexpr H2DE_ColorRGB getColor() const noexcept {
        return _textObjectData.text.color;
    }
    /**
     * @brief Get the padding of the text.
     * 
     * Returns the padding currently applied around the rendered text.
     * Padding defines the spacing between the text content and the edges of its rendering area.
     * 
     * @return The text padding as an H2DE_Padding struct.
     */
    constexpr H2DE_Padding getPadding() const noexcept {
        return _textObjectData.text.padding;
    }

    /**
     * @brief Set the text string to display.
     * 
     * Updates the text content rendered by the object.
     * 
     * @param text The new text string.
     */
    void setText(const std::string& text);
    /**
     * @brief Set the font used for the text.
     * 
     * Changes the font family or font file for rendering.
     * 
     * @param font The font name or path.
     */
    void setFont(const std::string& font);
    /**
     * @brief Set the bounds scale immediately.
     * 
     * Adjusts the bounds scale without animation.
     * 
     * @param bounds The new bounds scale.
     */
    void setBounds(const H2DE_Scale& bounds);
    /**
     * @brief Set the font size scale immediately.
     * 
     * Adjusts the font size scale without animation.
     * 
     * @param fontSize The new font size scale.
     */
    void setFontSize(float fontSize);
    /**
     * @brief Set the spacing between characters immediately.
     * 
     * Adjusts the horizontal space between characters without animation.
     * 
     * @param spacing The new character spacing.
     */
    void setSpacing(const H2DE_Scale& spacing);
    /**
     * @brief Set the horizontal text alignment.
     * 
     * Changes how the text is aligned inside its bounding box.
     * 
     * @param textAlign The new text alignment (left, center, right).
     */
    void setTextAlign(H2DE_TextAlign textAlign);
    /**
     * @brief Set the color of the text immediately.
     * 
     * Changes the RGB color applied to the text without animation.
     * 
     * @param color The new text color.
     */
    void setColor(const H2DE_ColorRGB& color);
    /**
     * @brief Set the padding of the text immediately.
     * 
     * Changes the padding applied around the text without animation.
     * Padding defines the spacing between the text content and the edges of its rendering area.
     * 
     * @param padding The new text padding.
     */
    void setPadding(const H2DE_Padding& padding);
    /**
     * @brief Animate the bounds scale change over time.
     * 
     * Smoothly changes the cotnainer scale from current to target value using timeline animation.
     * 
     * @param bounds The target bounds scale.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setBounds(const H2DE_Scale& bounds, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the font size change over time.
     * 
     * Smoothly changes the font size from current to target value using timeline animation.
     * 
     * @param fontSize The target font size size.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setFontSize(float fontSize, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the spacing change over time.
     * 
     * Smoothly changes the character spacing from current to target value using timeline animation.
     * 
     * @param spacing The target character spacing.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setSpacing(const H2DE_Scale& spacing, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    /**
     * @brief Animate the color change over time.
     * 
     * Smoothly changes the text color from current to target color using timeline animation.
     * 
     * @param color The target text color.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TextObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ButtonObject;
    friend class H2DE_InputObject;

private:
    struct _H2DE_CharacterSurfaceData {
        unsigned char c;
        float& offsetX;
        float& offsetY;
        float fixedFontSize;
        float spacingX;
        bool isFirstWordFirstChar;
        const std::string& fontTextureName;
        const H2DE_ColorRGB& color;
        const H2DE_PixelRect& src;
        H2DE_ScaleMode scaleMode;
        H2DE_BlendMode blendMode;
    };

    H2DE_TextObjectData _textObjectData;

    H2DE_TextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    ~H2DE_TextObject() override;

    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;

    static const std::string _getFormatedText(const std::string& text);
    const std::string _getFormatedWord(const std::string& word) const;

    const std::vector<std::string> _getWords() const;
    const std::vector<std::vector<std::string>> _getLines() const;

    float _getCharacterWidth(const char& c) const;
    float _getWordWidth(const std::string& word) const;
    float _getLineWidth(const std::vector<std::string>& line) const;
    float _getTextHeight() const;

    float _getStartingOffsetX(const std::vector<std::string>& line) const;
    float _getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const;

    float _getFixedFontSize() const;

    inline bool _isTextNull() const {
        return (_textObjectData.text.text == ""); 
    }
};

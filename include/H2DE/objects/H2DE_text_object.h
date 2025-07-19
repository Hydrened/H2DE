#ifndef H2DE_TEXT_OBJECT_H
#define H2DE_TEXT_OBJECT_H

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
        return textObjectData;
    }
    /**
     * @brief Get the text string displayed by the text object.
     * 
     * Returns the actual string content rendered by the object.
     * 
     * @return The displayed text as a std::string.
     */
    inline std::string getText() const {
        return textObjectData.text.text;
    }
    /**
     * @brief Get the font name used for rendering the text.
     * 
     * Returns the font family or font file name currently set for the text object.
     * 
     * @return The font name as a std::string.
     */
    inline std::string getFont() const {
        return textObjectData.text.font;
    }
    /**
     * @brief Get the container scale.
     * 
     * Returns the scale of the container used for containing the text.
     * 
     * @return The scale of the container as an H2DE_Scale value.
     */
    constexpr H2DE_Scale getContainer() const noexcept {
        return textObjectData.text.container;
    }
    /**
     * @brief Get the font size scale.
     * 
     * Returns the scaling factor used for the font size, allowing dynamic resizing.
     * 
     * @return The font size scale as an H2DE_Scale value.
     */
    constexpr H2DE_Scale getFontSize() const noexcept {
        return textObjectData.text.fontSize;
    }
    /**
     * @brief Get the spacing between characters.
     * 
     * Returns the horizontal spacing applied between each character in the text.
     * 
     * @return The character spacing as an H2DE_Scale value.
     */
    constexpr H2DE_Scale getSpacing() const noexcept {
        return textObjectData.text.spacing;
    }
    /**
     * @brief Get the horizontal alignment of the text.
     * 
     * Returns the text alignment setting (e.g. left, center, right).
     * 
     * @return The text alignment as an H2DE_TextAlign enum value.
     */
    constexpr H2DE_TextAlign getTextAlign() const noexcept {
        return textObjectData.text.textAlign;
    }
    /**
     * @brief Get the color of the text.
     * 
     * Returns the RGB color currently applied to the rendered text.
     * 
     * @return The text color as an H2DE_ColorRGB struct.
     */
    constexpr H2DE_ColorRGB getColor() const noexcept {
        return textObjectData.text.color;
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
     * @brief Set the container scale immediately.
     * 
     * Adjusts the container scale without animation.
     * 
     * @param container The new container scale.
     */
    void setContainer(const H2DE_Scale& container);
    /**
     * @brief Set the font size scale immediately.
     * 
     * Adjusts the font size scale without animation.
     * 
     * @param fontSize The new font size scale.
     */
    void setFontSize(const H2DE_Scale& fontSize);
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
     * @brief Animate the container scale change over time.
     * 
     * Smoothly changes the cotnainer scale from current to target value using timeline animation.
     * 
     * @param container The target container scale.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setContainer(const H2DE_Scale& container, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the font size change over time.
     * 
     * Smoothly changes the font size scale from current to target value using timeline animation.
     * 
     * @param fontSize The target font size scale.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setFontSize(const H2DE_Scale& fontSize, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
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
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setSpacing(const H2DE_Scale& spacing, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

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
     * @return Ttimeline controlling this animation.
     */
    H2DE_Timeline* setColor(const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TextObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ButtonObject;

private:
    H2DE_TextObjectData textObjectData;

    H2DE_TextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    ~H2DE_TextObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

    const std::vector<std::string> getWords() const;
    const std::vector<std::vector<std::string>> getLines() const;

    static int getLineLength(const std::vector<std::string>& line);

    float getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const noexcept;
    float getStartingOffsetX(const std::vector<std::string>& line) const;

    inline bool isTextNull() const {
        return (textObjectData.text.text == ""); 
    }
};

#endif

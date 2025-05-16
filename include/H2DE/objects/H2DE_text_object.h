#ifndef H2DE_TEXT_OBJECT_H
#define H2DE_TEXT_OBJECT_H

/**
 * @file H2DE_text_object.h
 * @brief Header file for the `H2DE_TextObject` class.
 * 
 * This file defines the `H2DE_TextObject` class, which represents a text object
 * in the H2DE engine. It allows for the creation and manipulation of text-based
 * objects, including the ability to set text, font, size, spacing, alignment, and color.
 * 
 * The class supports dynamic modifications of the text properties, such as animations
 * for font size, spacing, and color changes, as well as basic object transformations like
 * position, size, rotation, and flip. 
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @brief A text object class within the H2DE engine.
 * 
 * This class extends `H2DE_Object` and represents a text element within the H2DE engine.
 * It provides functionality to set text content, adjust the font, font size, spacing, alignment,
 * and color, with support for animations (e.g., transitioning font size, color, and spacing).
 * 
 * Instances of this class should be created via the `H2DE_CreateTextObject` function.
 */
class H2DE_TextObject : public H2DE_Object {
private:
    H2DE_TextObjectData tod;

    H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData tod);
    ~H2DE_TextObject() override;

    void resetSurfaceBuffers() override;

    std::vector<std::string> getWords() const;
    std::vector<std::vector<std::string>> getLines() const;
    float getLineStartOffsetX(const std::vector<std::string>& line) const;

public:
    /**
     * @brief Creates a text object.
     * 
     * This function creates and initializes a new `H2DE_TextObject` with the provided engine,
     * object data, and text-specific data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param objectData The general object data for initialization.
     * @param textObjectData The specific data for the text object.
     * @return A pointer to the created `H2DE_TextObject`.
     */
    friend H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    /**
     * @brief Sets the text of the text object.
     * 
     * This function sets the text content of the `H2DE_TextObject`.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param text The text content to display.
     */
    friend void H2DE_SetTextObjectText(H2DE_TextObject* textObject, const std::string& text);
    /**
     * @brief Sets the font of the text object.
     * 
     * This function sets the font used by the `H2DE_TextObject`.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param font The name of the font to use.
     */
    friend void H2DE_SetTextObjectFont(H2DE_TextObject* textObject, const std::string& font);
    /**
     * @brief Sets the font size of the text object.
     * 
     * This function sets the font size of the `H2DE_TextObject`, allowing for dynamic changes
     * with optional duration and easing effects.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param fontSize The desired font size.
     */
    friend void H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize);
    /**
     * @brief Sets the font size of the text object with animation.
     * 
     * This function sets the font size of the `H2DE_TextObject` with an optional animation,
     * allowing for smooth transitions between sizes.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param fontSize The desired font size.
     * @param duration The duration of the animation.
     * @param easing The easing function for the animation.
     * @param completed The callback called when the timeline ends
     * @param pauseSensitive Whether the animation should pause when the game is paused.
     * @return The ID of the created timeline.
     */
    friend unsigned int H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
     * @brief Sets the spacing between characters in the text object.
     * 
     * This function adjusts the character spacing in the `H2DE_TextObject`, with the ability to
     * animate the spacing over time.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param spacing The desired character spacing.
     */
    friend void H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing);
    /**
     * @brief Sets the spacing with animation.
     * 
     * This function adjusts the character spacing with an optional animation, enabling smooth
     * transitions between spacing values.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param spacing The desired character spacing.
     * @param duration The duration of the animation.
     * @param easing The easing function for the animation.
     * @param completed The callback called when the timeline ends
     * @param pauseSensitive Whether the animation should pause when the game is paused.
     * @return The ID of the created timeline.
     */
    friend unsigned int H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
     * @brief Sets the text alignment for the text object.
     * 
     * This function sets the horizontal alignment of the text content in the `H2DE_TextObject`,
     * such as left, center, or right alignment.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param textAlign The desired alignment type.
     */
    friend void H2DE_SetTextObjectTextAlign(H2DE_TextObject* textObject, H2DE_TextAlign textAlign);
    /**
     * @brief Sets the color of the text object.
     * 
     * This function sets the color of the text displayed in the `H2DE_TextObject`.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param color The desired color in RGB format.
     */
    friend void H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color);
    /**
     * @brief Sets the text color with animation.
     * 
     * This function sets the text color with an optional animation, allowing smooth transitions
     * between colors over a specified duration.
     * 
     * @param textObject A pointer to the `H2DE_TextObject`.
     * @param color The desired color in RGB format.
     * @param duration The duration of the animation.
     * @param easing The easing function for the animation.
     * @param completed The callback called when the timeline ends
     * @param pauseSensitive Whether the animation should pause when the game is paused.
     * @return The ID of the created timeline.
     */
    friend unsigned int H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    /**
     * @brief Sets the position of the object.
     * 
     * This function sets the position of the `H2DE_Object` (inherited by `H2DE_TextObject`),
     * adjusting its position in the 2D world.
     * 
     * @param object A pointer to the `H2DE_Object`.
     * @param pos The desired position.
     */
    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    /**
     * @brief Sets the size of the object.
     * 
     * This function sets the size of the `H2DE_Object` (inherited by `H2DE_TextObject`).
     * 
     * @param object A pointer to the `H2DE_Object`.
     * @param size The desired size.
     */
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    /**
     * @brief Sets the rotation of the object.
     * 
     * This function sets the rotation angle of the `H2DE_Object` (inherited by `H2DE_TextObject`).
     * 
     * @param object A pointer to the `H2DE_Object`.
     * @param rotation The desired rotation angle (in degrees).
     */
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation);
    /**
     * @brief Sets the pivot point of the object.
     * 
     * This function sets the pivot point for the `H2DE_Object` (inherited by `H2DE_TextObject`).
     * 
     * @param object A pointer to the `H2DE_Object`.
     * @param pivot The desired pivot position.
     */
    friend void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot);
    /**
     * @brief Sets the flip state of the object.
     * 
     * This function sets the flip state (horizontal/vertical) for the `H2DE_Object`
     * (inherited by `H2DE_TextObject`).
     * 
     * @param object A pointer to the `H2DE_Object`.
     * @param flip The desired flip state.
     */
    friend void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip);
};

#endif

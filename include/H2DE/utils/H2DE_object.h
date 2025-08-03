#pragma once

#include <functional>
#include <H2DE/utils/H2DE_utils.h>
class H2DE_Object;
class H2DE_ButtonObject;
class H2DE_Timeline;

/**
 * @struct H2DE_Hitbox
 * @brief Represents a hitbox used for collision detection.
 * 
 * Contains transformation data, visual color, collision identifier,
 * and an optional callback for collision events.
 */
struct H2DE_Hitbox {
    H2DE_Transform transform = H2DE_Transform();                            /** Hitbox transform (position, scale, rotation, pivot). */
    H2DE_ColorRGB color = H2DE_ColorRGB();                                  /** Debug color used when rendering the hitbox. */
    int collisionIndex = 0;                                                 /** Custom collision group index to filter interactions. */
    std::function<void(H2DE_Object*, H2DE_Face)> onCollide = nullptr;       /** Callback triggered when the hitbox collides with another object. */
};

/**
 * @struct H2DE_ButtonEventData
 * @brief Stores event context for button callbacks.
 * 
 * This struct is passed to button event functions to provide access to the button that triggered the event
 * and the timeline in which it exists.
 */
struct H2DE_ButtonEventData {
    H2DE_ButtonObject* button;          /**< Pointer to the button that triggered the event. */
    H2DE_Timeline* timeline;            /**< Reference to the timeline the button is part of. */

    /**
     * @brief Constructs an event data object for button events.
     * @param button Pointer to the button triggering the event.
     * @param id Reference to the timeline where the event occurred.
     */
    constexpr H2DE_ButtonEventData(H2DE_ButtonObject* button, H2DE_Timeline* timeline) noexcept : button(button), timeline(timeline) {}
};

/**
 * @struct H2DE_ObjectData
 * @brief Contains basic data for an object, including its transform, opacity, position mode, and index.
 */
struct H2DE_ObjectData {
    H2DE_Transform transform = H2DE_Transform();    /**< Object's transformation data (position, scale, rotation, pivot). */
    uint8_t opacity = H2DE_OPACITY_MAX;             /**< Opacity value from 0 (transparent) to 255 (fully opaque). */
    bool absolute = false;                          /**< If true, the position is absolute; otherwise relative. */
    int index = 0;                                  /**< Object index identifier. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_ObjectData() noexcept = default;
    /**
     * @brief Constructs a H2DE_Time with specified values.
     * @param hours Number of hours.
     * @param minutes Number of minutes.
     * @param seconds Number of seconds.
     * @param milliseconds Number of milliseconds.
     */
    constexpr H2DE_ObjectData(const H2DE_Transform& transform, uint8_t opacity, bool absolute, int index) noexcept : transform(transform), opacity(opacity), absolute(absolute), index(index) {}
};

/**
 * @struct H2DE_BarObjectData
 * @brief Stores the minimum, maximum, and current value for a bar-like object (e.g., progress bar, health bar).
 */
struct H2DE_BarObjectData {
    float min = 0.0f;       /**< Minimum value of the bar. */
    float max = 100.0f;     /**< Maximum value of the bar. */
    float value = 0.0f;     /**< Current value of the bar. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_BarObjectData() noexcept = default;
    /**
     * @brief Constructs an H2DE_BarObjectData with specified min, max and current value.
     * @param min Minimum value.
     * @param max Maximum value.
     * @param value Current value.
     */
    constexpr H2DE_BarObjectData(float min, float max, float value) noexcept : min(min), max(max), value(value) {}
};

/**
 * @struct H2DE_ButtonObjectData
 * @brief Contains data and event callbacks for a button object.
 */
struct H2DE_ButtonObjectData {
    H2DE_Text text = H2DE_Text();                                       /**< Text displayed on the button. */
    H2DE_MouseButton mouseButton = H2DE_MOUSE_BUTTON_LEFT;              /**< Mouse button(s) that trigger interactions on the button. */
    std::function<void(H2DE_ButtonEventData&)> onMouseDown = nullptr;   /**< Callback triggered when the button is pressed down. */
    std::function<void(H2DE_ButtonEventData&)> onMouseUp = nullptr;     /**< Callback triggered when the button is released. */
    std::function<void(H2DE_ButtonEventData&)> onHover = nullptr;       /**< Callback triggered when the mouse hovers over the button. */
    std::function<void(H2DE_ButtonEventData&)> onBlur = nullptr;        /**< Callback triggered when the mouse stops hovering the button. */
    bool pauseSensitive = true;                                         /**< Whether the button events are sensitive to game pause state. */
};

/**
 * @struct H2DE_TextObjectData
 * @brief Holds text data for a text object.
 */
struct H2DE_TextObjectData {
    H2DE_Text text = H2DE_Text();       /**< Text content and styling for the text object. */
};

/**
 * @struct H2DE_TimerObjectData
 * @brief Stores timer data and display options for a timer object.
 */
struct H2DE_TimerObjectData {
    H2DE_Time time = H2DE_Time();           /**< Current timer time values. */
    H2DE_Text text = H2DE_Text();           /**< Text styling and content for the timer display. */
    char separator = ':';                   /**< Character used as separator between time units. */
    bool displayHours = false;              /**< Whether to display hours. */
    bool displayMinutes = false;            /**< Whether to display minutes. */
    bool displaySeconds = true;             /**< Whether to display seconds. */
    bool displayMilliseconds = false;       /**< Whether to display milliseconds. */
    bool increasing = true;                 /**< Whether the timer is increasing (false for decreasing). */
    bool pauseSensitive = true;             /**< Whether the timer is sensitive to pause state. */
};

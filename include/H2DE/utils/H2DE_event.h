#pragma once

/**
 * @enum H2DE_EventType
 * @brief Enum representing all possible input events handled by the engine.
 * 
 * Used internally to categorize and process user input such as keyboard,
 * mouse button presses, motion, and wheel scrolling.
 */
enum H2DE_EventType {
    H2DE_EVENT_TYPE_NONE,                   /**< No event, default state */
    H2DE_EVENT_TYPE_KEYDOWN,                /**< A keyboard key has been pressed */
    H2DE_EVENT_TYPE_KEYUP,                  /**< A keyboard key has been released */
    H2DE_EVENT_TYPE_MOUSE_BUTTON_DOWN,      /**< A mouse button has been pressed */
    H2DE_EVENT_TYPE_MOUSE_BUTTON_UP,        /**< A mouse button has been released */
    H2DE_EVENT_TYPE_MOUSE_MOTION,           /**< Mouse movement detected */
    H2DE_EVENT_TYPE_MOUSE_WHEEL,            /**< Mouse wheel scrolled (up or down) */
};

/**
 * @enum H2DE_Key
 * @brief Enum representing all keyboard keys supported by the engine.
 * 
 * Provides a unified key mapping based on standard virtual-key codes,
 * allowing consistent handling of keyboard input across platforms.
 */
enum H2DE_Key {
    H2DE_KEY_NONE               = 0x00,     /**< No key pressed (default state) */

    // Control keys
    H2DE_KEY_BACKSPACE          = 0x08,     /**< Backspace key */
    H2DE_KEY_TAB                = 0x09,     /**< Tab key */
    H2DE_KEY_ENTER              = 0x0D,     /**< Enter/Return key */
    H2DE_KEY_SHIFT              = 0x10,     /**< Shift modifier key */
    H2DE_KEY_CONTROL            = 0x11,     /**< Control (Ctrl) modifier key */
    H2DE_KEY_ALT                = 0x12,     /**< Alt modifier key */
    H2DE_KEY_PAUSE              = 0x13,     /**< Pause/Break key */
    H2DE_KEY_CAPSLOCK           = 0x14,     /**< Caps Lock key */
    H2DE_KEY_ESCAPE             = 0x1B,     /**< Escape key */
    H2DE_KEY_SPACE              = 0x20,     /**< Space bar */

    // Navigation keys
    H2DE_KEY_PAGEUP             = 0x21,     /**< Page Up key */
    H2DE_KEY_PAGEDOWN           = 0x22,     /**< Page Down key */
    H2DE_KEY_END                = 0x23,     /**< End key */
    H2DE_KEY_HOME               = 0x24,     /**< Home key */
    H2DE_KEY_LEFT               = 0x25,     /**< Left arrow key */
    H2DE_KEY_UP                 = 0x26,     /**< Up arrow key */
    H2DE_KEY_RIGHT              = 0x27,     /**< Right arrow key */
    H2DE_KEY_DOWN               = 0x28,     /**< Down arrow key */
    H2DE_KEY_INSERT             = 0x2D,     /**< Insert key */
    H2DE_KEY_DELETE             = 0x2E,     /**< Delete key */

    // Number keys (top row)
    H2DE_KEY_0                  = 0x30,     /**< 0 key */
    H2DE_KEY_1                  = 0x31,     /**< 1 key */
    H2DE_KEY_2                  = 0x32,     /**< 2 key */
    H2DE_KEY_3                  = 0x33,     /**< 3 key */
    H2DE_KEY_4                  = 0x34,     /**< 4 key */
    H2DE_KEY_5                  = 0x35,     /**< 5 key */
    H2DE_KEY_6                  = 0x36,     /**< 6 key */
    H2DE_KEY_7                  = 0x37,     /**< 7 key */
    H2DE_KEY_8                  = 0x38,     /**< 8 key */
    H2DE_KEY_9                  = 0x39,     /**< 9 key */

    // Alphabet keys
    H2DE_KEY_A                  = 0x41,     /**< A key */
    H2DE_KEY_B                  = 0x42,     /**< B key */
    H2DE_KEY_C                  = 0x43,     /**< C key */
    H2DE_KEY_D                  = 0x44,     /**< D key */
    H2DE_KEY_E                  = 0x45,     /**< E key */
    H2DE_KEY_F                  = 0x46,     /**< F key */
    H2DE_KEY_G                  = 0x47,     /**< G key */
    H2DE_KEY_H                  = 0x48,     /**< H key */
    H2DE_KEY_I                  = 0x49,     /**< I key */
    H2DE_KEY_J                  = 0x4A,     /**< J key */
    H2DE_KEY_K                  = 0x4B,     /**< K key */
    H2DE_KEY_L                  = 0x4C,     /**< L key */
    H2DE_KEY_M                  = 0x4D,     /**< M key */
    H2DE_KEY_N                  = 0x4E,     /**< N key */
    H2DE_KEY_O                  = 0x4F,     /**< O key */
    H2DE_KEY_P                  = 0x50,     /**< P key */
    H2DE_KEY_Q                  = 0x51,     /**< Q key */
    H2DE_KEY_R                  = 0x52,     /**< R key */
    H2DE_KEY_S                  = 0x53,     /**< S key */
    H2DE_KEY_T                  = 0x54,     /**< T key */
    H2DE_KEY_U                  = 0x55,     /**< U key */
    H2DE_KEY_V                  = 0x56,     /**< V key */
    H2DE_KEY_W                  = 0x57,     /**< W key */
    H2DE_KEY_X                  = 0x58,     /**< X key */
    H2DE_KEY_Y                  = 0x59,     /**< Y key */
    H2DE_KEY_Z                  = 0x5A,     /**< Z key */

    // Symbols & punctuation
    H2DE_KEY_SEMICOLON          = 0xBA,     /**< ; key */
    H2DE_KEY_PLUS               = 0xBB,     /**< + key */
    H2DE_KEY_COMMA              = 0xBC,     /**< , key */
    H2DE_KEY_MINUS              = 0xBD,     /**< - key */
    H2DE_KEY_PERIOD             = 0xBE,     /**< . key */
    H2DE_KEY_SLASH              = 0xBF,     /**< / key */
    H2DE_KEY_GRAVE              = 0xC0,     /**< ` key */
    H2DE_KEY_LEFTBRACKET        = 0xDB,     /**< [ key */
    H2DE_KEY_BACKSLASH          = 0xDC,     /**< \ key */
    H2DE_KEY_RIGHTBRACKET       = 0xDD,     /**< ] key */
    H2DE_KEY_QUOTE              = 0xDE,     /**< ' key */

    // Numpad keys
    H2DE_KEY_NUMPAD0            = 0x60,     /**< Numpad 0 key */
    H2DE_KEY_NUMPAD1            = 0x61,     /**< Numpad 1 key */
    H2DE_KEY_NUMPAD2            = 0x62,     /**< Numpad 2 key */
    H2DE_KEY_NUMPAD3            = 0x63,     /**< Numpad 3 key */
    H2DE_KEY_NUMPAD4            = 0x64,     /**< Numpad 4 key */
    H2DE_KEY_NUMPAD5            = 0x65,     /**< Numpad 5 key */
    H2DE_KEY_NUMPAD6            = 0x66,     /**< Numpad 6 key */
    H2DE_KEY_NUMPAD7            = 0x67,     /**< Numpad 7 key */
    H2DE_KEY_NUMPAD8            = 0x68,     /**< Numpad 8 key */
    H2DE_KEY_NUMPAD9            = 0x69,     /**< Numpad 9 key */
    H2DE_KEY_MULTIPLY           = 0x6A,     /**< Numpad * key */
    H2DE_KEY_ADD                = 0x6B,     /**< Numpad + key */
    H2DE_KEY_SEPARATOR          = 0x6C,     /**< Numpad separator key */
    H2DE_KEY_SUBTRACT           = 0x6D,     /**< Numpad - key */
    H2DE_KEY_DECIMAL            = 0x6E,     /**< Numpad . key */
    H2DE_KEY_DIVIDE             = 0x6F,     /**< Numpad / key */

    // Function keys
    H2DE_KEY_F1                 = 0x70,     /**< F1 key */
    H2DE_KEY_F2                 = 0x71,     /**< F2 key */
    H2DE_KEY_F3                 = 0x72,     /**< F3 key */
    H2DE_KEY_F4                 = 0x73,     /**< F4 key */
    H2DE_KEY_F5                 = 0x74,     /**< F5 key */
    H2DE_KEY_F6                 = 0x75,     /**< F6 key */
    H2DE_KEY_F7                 = 0x76,     /**< F7 key */
    H2DE_KEY_F8                 = 0x77,     /**< F8 key */
    H2DE_KEY_F9                 = 0x78,     /**< F9 key */
    H2DE_KEY_F10                = 0x79,     /**< F10 key */
    H2DE_KEY_F11                = 0x7A,     /**< F11 key */
    H2DE_KEY_F12                = 0x7B,     /**< F12 key */
};

/**
 * @enum H2DE_MouseButton
 * @brief Enum representing mouse buttons recognized by the engine.
 * 
 * Used to identify which mouse button(s) are pressed during input events.
 */
enum H2DE_MouseButton {
    H2DE_MOUSE_BUTTON_NONE              = 0x00,     /**< No button pressed */
    H2DE_MOUSE_BUTTON_LEFT              = 0x01,     /**< Left mouse button */
    H2DE_MOUSE_BUTTON_RIGHT             = 0x02,     /**< Right mouse button */
    H2DE_MOUSE_BUTTON_MIDDLE            = 0x04,     /**< Middle mouse button (wheel click) */
    H2DE_MOUSE_BUTTON_X1                = 0x08,     /**< First extra mouse button */
    H2DE_MOUSE_BUTTON_X2                = 0x10,     /**< Second extra mouse button */
    H2DE_MOUSE_BUTTON_LEFT_OR_RIGHT     = 0x03,     /**< Left or right button pressed */
    H2DE_MOUSE_BUTTON_X1_OR_X2          = 0x18,     /**< Either extra button (X1 or X2) pressed */
    H2DE_MOUSE_BUTTON_ALL               = 0x1F,     /**< Any mouse button pressed */
};

/**
 * @enum H2DE_WheelDirection
 * @brief Enum representing the scroll direction of the mouse wheel.
 * 
 * Used to determine the user's scrolling action in input events.
 */
enum H2DE_WheelDirection {
    H2DE_WHEEL_DIRECTION_NONE       = 0x02,     /**< No wheel movement */
    H2DE_WHEEL_DIRECTION_DOWN       = 0x00,     /**< Wheel scrolled down */
    H2DE_WHEEL_DIRECTION_UP         = 0x01,     /**< Wheel scrolled up */
};

/**
 * @struct H2DE_Event
 * @brief Represents an input event captured by the engine.
 * 
 * This structure stores all relevant data for a single input event,
 * such as keyboard presses, mouse button actions, mouse movement, 
 * and wheel scrolling. It also provides mouse position in both
 * game and interface coordinates for flexible handling.
 */
struct H2DE_Event {
public:
    H2DE_EventType type = H2DE_EVENT_TYPE_NONE;                         /** Type of the event (keyboard, mouse button, motion, wheel, etc.). */
    H2DE_Key key = H2DE_KEY_NONE;                                       /** Key code if the event is related to keyboard input. */
    H2DE_MouseButton button = H2DE_MOUSE_BUTTON_NONE;                   /** Mouse button pressed/released if applicable. */
    H2DE_WheelDirection wheelDirection = H2DE_WHEEL_DIRECTION_NONE;     /** Scroll direction if the event is a mouse wheel action. */
    H2DE_Translate mouseGameTranslate = H2DE_Translate();               /** Mouse position translated into game world coordinates. */
    H2DE_Translate mouseInterfaceTranslate = H2DE_Translate();          /** Mouse position translated into interface (UI) coordinates. */

private:
    constexpr H2DE_Event() = default;

    friend class H2DE_EventManager;
};

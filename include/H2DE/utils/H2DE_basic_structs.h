#pragma once

#include <H2DE/utils/H2DE_utils.h>

/**
 * @struct H2DE_Padding
 * @brief Defines padding values for each side of a rectangle.
 */
struct H2DE_Padding {
    float top = 0.0f;       /** Padding on the top side. */
    float right = 0.0f;     /** Padding on the right side. */
    float bottom = 0.0f;    /** Padding on the bottom side. */
    float left = 0.0f;      /** Padding on the left side. */

    /**
     * @brief Default constructor. Initializes all paddings to 0.
     */
    constexpr H2DE_Padding() noexcept = default;
    /**
     * @brief Uniform padding constructor. Applies the same value to all sides.
     * @param all The padding to apply to top, right, bottom, and left.
     */
    constexpr H2DE_Padding(float all) noexcept : top(all), right(all), bottom(all), left(all) {}
    /**
     * @brief Block/inline padding constructor.
     * 
     * Applies the first value to top & bottom (block), and the second to left & right (inline).
     * 
     * @param block Padding for top and bottom.
     * @param inLine Padding for left and right.
     */
    constexpr H2DE_Padding(float block, float inLine) noexcept : top(block), right(inLine), bottom(block), left(inLine) {}
    /**
     * @brief Full custom padding constructor.
     * @param top Top padding.
     * @param right Right padding.
     * @param bottom Bottom padding.
     * @param left Left padding.
     */
    constexpr H2DE_Padding(float top, float right, float bottom, float left) noexcept : top(top), right(right), bottom(bottom), left(left) {}

    /**
     * @brief Outputs the H2DE_Padding to an output stream in a readable format.
     * 
     * This operator allows H2DE_Padding instances to be streamed (e.g., to std::cout)
     * with a format like: "top: 10, right: 5, bottom: 10, left: 5".
     *
     * Useful for debugging or logging layout information.
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Padding& padding) {
        os << "[top: " << padding.top;
        os << ", right: " << padding.right;
        os << ", bottom: " << padding.bottom;
        os << ", left: " << padding.left << "]";
        return os;
    }
};

/**
 * @struct H2DE_Transform
 * @brief Represents a 2D transformation for positioning, scaling, and rotating objects.
 * 
 * This structure is used to apply transformations to entities in a 2D space,
 * such as their position, scale, rotation, and pivot point.
 */
struct H2DE_Transform {
public:
    H2DE_Translate translate = { 0.0f, 0.0f };      /** Object position in the 2D space. */
    H2DE_Scale scale = { 1.0f, 1.0f };              /** Object scale on both axes. */
    float rotation = 0.0f;                          /** Object rotation in degrees. */
    H2DE_Pivot pivot = { 0.0f, 0.0f };              /** Pivot point for rotation and scaling. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_Transform() noexcept = default;
    /**
     * @brief Initializes a transform with custom values.
     * 
     * @param translate The position of the object.
     * @param scale The scaling factors.
     * @param rotation The rotation in degrees.
     * @param pivot The pivot point.
     */
    constexpr H2DE_Transform(const H2DE_Translate& translate, const H2DE_Scale& scale, float rotation, const H2DE_Pivot& pivot) noexcept : translate(translate), defaultTranslate(translate), scale(scale), defaultScale(scale), rotation(rotation), pivot(pivot), defaultPivot(pivot) {}

    /**
     * @brief Outputs the H2DE_Transform to an output stream in a readable format.
     * 
     * This operator allows H2DE_Transform instances to be streamed (e.g., to std::cout)
     * 
     * Useful for debugging transformation data such as position, scale, rotation, and pivot.
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Transform& transform) {
        os << "[translate: " << transform.translate;
        os << ", scale: " << transform.scale;
        os << ", rotation: " << transform.rotation;
        os << ", pivot: " << transform.pivot << "]";
        return os;
    }

private:
    H2DE_Translate defaultTranslate = { 0.0f, 0.0f };
    H2DE_Scale defaultScale = { 1.0f, 1.0f };
    H2DE_Pivot defaultPivot = { 0.0f, 0.0f };

    friend class H2DE_Object;
    friend class H2DE_Surface;
};

/**
 * @struct H2DE_Text
 * @brief Represents all the properties to render a text element.
 * 
 * Contains the text content, font settings, size, spacing,
 * alignment, color, and padding around the text box.
 */
struct H2DE_Text {
    std::string text = "";                                          /** Text content to display. */
    std::string font = "";                                          /** Font name or path to use for rendering the text. */
    H2DE_Scale container = { 10.0f, 10.0f };                        /** Zone for the text to be written in. */
    H2DE_Scale fontSize = { 1.0f, 1.0f };                           /** Font size scale for X and Y axes. */
    H2DE_Scale spacing = { 0.1f, 0.3f };                            /** Spacing between characters (X) and lines (Y). */
    H2DE_TextAlign textAlign = H2DE_TEXT_ALIGN_CENTER_CENTER;       /** Text alignment (horizontal and vertical). */
    H2DE_ColorRGB color = H2DE_ColorRGB();                          /** Color of the text. */
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };              /** Padding around the text box (top, right, bottom, left). */
};

/**
 * @struct H2DE_Time
 * @brief Represents a time duration or timestamp with hours, minutes, seconds, and milliseconds.
 */
struct H2DE_Time {
    uint8_t hours = 0;          /** Number of hours. */
    uint8_t minutes = 0;        /** Number of minutes. */
    uint8_t seconds = 0;        /** Number of seconds. */
    uint16_t milliseconds = 0;  /** Number of milliseconds. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_Time() noexcept = default;
    /**
     * @brief Constructs a H2DE_Time with specified values.
     * @param hours Number of hours.
     * @param minutes Number of minutes.
     * @param seconds Number of seconds.
     * @param milliseconds Number of milliseconds.
     */
    constexpr H2DE_Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept : hours(hours), minutes(minutes), seconds(seconds), milliseconds(milliseconds) {}

    /**
     * @brief Converts the time to a float representing the total elapsed time in seconds.
     * 
     * This function converts the current time represented by the H2DE_Time object into a float value.
     * The result is the total number of seconds, including the fractional part from milliseconds.
     * 
     * @return Elapsed time in seconds as a float.
     */
    constexpr float toElapsed() const noexcept {
        return (hours * 3600.0f) + (minutes * 60.0f) + (seconds) + (milliseconds * 0.001f);
    }

    /**
     * @brief Converts a float elapsed time in seconds into an H2DE_Time object.
     * 
     * This static function takes a float representing a duration in seconds, and decomposes it
     * into hours, minutes, seconds, and milliseconds. It uses `H2DE::floor` and `H2DE::round` internally
     * to compute each part precisely. The result is clamped to valid ranges for each time unit.
     * 
     * @param elapsed The total elapsed time in seconds.
     * @return An H2DE_Time object representing the decomposed time.
     */
    static constexpr H2DE_Time toTime(float elapsed) noexcept {
        return H2DE_Time(
            static_cast<int>(elapsed) / 3600,
            H2DE::clamp((static_cast<int>(elapsed) % 3600) / 60, 0, 59),
            H2DE::clamp(static_cast<int>(elapsed) % 60, 0, 59),
            H2DE::clamp(H2DE::round((elapsed - H2DE::floor(elapsed)) * 1000.0f), 0, 999)
        );
    }

    /**
     * @brief Outputs the H2DE_Time to an output stream in a readable format.
     * 
     * This operator allows H2DE_Time instances to be streamed (e.g., to std::cout)
     * with a format like: "h: 1, m: 30, s: 45, ms: 250".
     */
    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Time& time) {
        os << "[hours: " << static_cast<int>(time.hours);
        os << ", minutes: " << static_cast<int>(time.minutes);
        os << ", seconds: " << static_cast<int>(time.seconds);
        os << ", milliseconds: " << static_cast<int>(time.milliseconds) << "]";
        return os;
    }
};

/**
 * @struct H2DE_Font
 * @brief Represents font data including texture, character sizing, spacing, and rendering modes.
 */
struct H2DE_Font {
    std::string textureName = "";                           /**< Name of the texture used for the font */ 
    H2DE_PixelSize charSize = { 0, 0 };                     /**< Size of each character in pixels */
    int spacing = 0;                                        /**< Spacing between characters */
    std::string charOrder = "";                             /**< Order of characters in the texture */
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;      /**< Scaling mode for rendering */
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;       /**< Blending mode for rendering */
};

/**
 * @struct H2DE_WindowData
 * @brief Contains configuration data for initializing an H2DE window.
 * 
 * This structure includes all necessary parameters to define the window's 
 * default state such as size, position, title, framerate, and other behaviors.
 */
struct H2DE_WindowData {
    uint16_t fps = 60;                                                              /** Target frames per second of the window. */
    const char* title = "H2DE Window";                                              /** Title text shown in the window's title bar. */
    H2DE_PixelPos pos = { H2DE_WINDOW_POS_CENTERED, H2DE_WINDOW_POS_CENTERED };     /** Initial position of the window on the screen. */
    H2DE_PixelSize size = { 1280, 720 };                                            /** Initial dimensions of the window in pixels. */
    bool fullscreen = false;                                                        /** Whether the window should launch in fullscreen mode. */
    bool resizable = false;                                                         /** Whether the window can be resized by the user. */
    bool saveState = false;                                                         /** Whether to save window state (position, size, etc.) on exit. */
    H2DE_WindowRatio ratio = H2DE_WINDOW_RATIO_NO_RATIO;                            /** Defines the window's aspect ratio behavior. */
};

/**
 * @struct H2DE_CameraData
 * @brief Contains settings for configuring the in-game camera.
 * 
 * This structure defines how the camera behaves in the game world, including
 * translation, dimensions, smoothing, padding, origin, and debug options.
 */
struct H2DE_CameraData {
    H2DE_Translate translate = { 0.0f, 0.0f };              /** Initial camera offset in world units. */
    float gameWidth = 20.0f;                                /** Width of the game world visible by the camera. */
    float interfaceWidth = 20.0f;                           /** Width used for UI scaling relative to the interface. */
    float smoothing = 0.0f;                                 /** Smooth transition factor when moving the camera. */
    H2DE_Padding padding = { 0.0f, 0.0f, 0.0f, 0.0f };      /** Padding around the camera view. */
    H2DE_Face xOrigin = H2DE_FACE_LEFT;                     /** Horizontal alignment of the camera's origin. */
    H2DE_Face yOrigin = H2DE_FACE_TOP;                      /** Vertical alignment of the camera's origin. */
    bool grid = true;                                       /** Whether to display a grid overlay (usually for debugging). */
};

/**
 * @struct H2DE_EngineData
 * @brief Holds the engine's core configuration.
 * 
 * This structure aggregates the essential components required to initialize
 * and manage the game engine, including window and camera settings.
 */
struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();     /** Settings related to the main application window. */
    H2DE_CameraData camera = H2DE_CameraData();     /** Settings related to the main in-game camera. */
};

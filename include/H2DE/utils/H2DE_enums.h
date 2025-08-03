#pragma once

#include <H2DE/utils/H2DE_utils.h>

/**
 * @enum H2DE_WindowRatio
 * @brief Enum representing common window aspect ratios for the engine.
 * 
 * Used to specify how the game window scales or adapts to different screen shapes.
 */
enum H2DE_WindowRatio {
    H2DE_WINDOW_RATIO_NO_RATIO,     /**< No fixed ratio, free scaling */
    H2DE_WINDOW_RATIO_CUSTOM,       /**< Custom ratio defined by user */
    H2DE_WINDOW_RATIO_4_3,          /**< Classic 4:3 aspect ratio */
    H2DE_WINDOW_RATIO_3_2,          /**< 3:2 aspect ratio */
    H2DE_WINDOW_RATIO_5_4,          /**< 5:4 aspect ratio */
    H2DE_WINDOW_RATIO_16_10,        /**< 16:10 aspect ratio */
    H2DE_WINDOW_RATIO_16_9,         /**< Widescreen 16:9 aspect ratio */
    H2DE_WINDOW_RATIO_21_9,         /**< Ultra-wide 21:9 aspect ratio */
    H2DE_WINDOW_RATIO_32_9,         /**< Super ultra-wide 32:9 aspect ratio */
};

/**
 * @enum H2DE_Easing
 * @brief Types of easing functions for animations and transitions.
 * 
 * These easing modes define how values interpolate over time,
 * controlling the acceleration and deceleration of animations.
 */
enum H2DE_Easing {
    H2DE_EASING_LINEAR,             /**< Constant speed interpolation */
    H2DE_EASING_EASE_IN,            /**< Accelerating from zero velocity */
    H2DE_EASING_EASE_OUT,           /**< Decelerating to zero velocity */
    H2DE_EASING_EASE_IN_OUT,        /**< Acceleration until halfway, then deceleration */
    H2DE_EASING_BACK_IN,            /**< Overshoots slightly before accelerating */
    H2DE_EASING_BACK_OUT,           /**< Overshoots slightly before decelerating */
    H2DE_EASING_BACK_IN_OUT,        /**< Overshoot at both start and end */
    H2DE_EASING_ELASTIC_IN,         /**< Elastic bounce effect at start */
    H2DE_EASING_ELASTIC_OUT,        /**< Elastic bounce effect at end */
    H2DE_EASING_ELASTIC_IN_OUT,     /**< Elastic bounce at start and end */
    H2DE_EASING_BOUNCE_IN,          /**< Bounce effect at start */
    H2DE_EASING_BOUNCE_OUT,         /**< Bounce effect at end */
    H2DE_EASING_BOUNCE_IN_OUT,      /**< Bounce effect at start and end */
    H2DE_EASING_SINE_IN,            /**< Sinusoidal acceleration at start */
    H2DE_EASING_SINE_OUT,           /**< Sinusoidal deceleration at end */
    H2DE_EASING_SINE_IN_OUT,        /**< Sinusoidal acceleration and deceleration */
    H2DE_EASING_EXPO_IN,            /**< Exponential acceleration at start */
    H2DE_EASING_EXPO_OUT,           /**< Exponential deceleration at end */
    H2DE_EASING_EXPO_IN_OUT,        /**< Exponential acceleration and deceleration */
};

/**
 * @enum H2DE_Face
 * @brief Represents directional faces or orientations using 2-bit flags.
 * 
 * Useful for indicating the facing direction of an object, sprite, or tile.
 */
enum H2DE_Face {
    H2DE_FACE_TOP       = 0b00,     /**< Facing upwards (top) */
    H2DE_FACE_RIGHT     = 0b01,     /**< Facing right */
    H2DE_FACE_BOTTOM    = 0b10,     /**< Facing downwards (bottom) */
    H2DE_FACE_LEFT      = 0b11,     /**< Facing left */
};

/**
 * @enum H2DE_TextAlign
 * @brief Defines text alignment options combining vertical and horizontal positions.
 * 
 * Bits encode vertical and horizontal alignment to position text within a rectangle:
 * - Vertical: Top (0b00001000), Center (0b00010000), Bottom (0b00100000)
 * - Horizontal: Left (0b00000001), Center (0b00000010), Right (0b00000100)
 * 
 * Allows specifying precise alignment like top-left, center-center, bottom-right, etc.
 */
enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_TOP_LEFT        = 0b00001001,   /**< Align top vertically and left horizontally */
    H2DE_TEXT_ALIGN_TOP_CENTER      = 0b00001010,   /**< Align top vertically and center horizontally */
    H2DE_TEXT_ALIGN_TOP_RIGHT       = 0b00001100,   /**< Align top vertically and right horizontally */
    H2DE_TEXT_ALIGN_CENTER_LEFT     = 0b00010001,   /**< Align center vertically and left horizontally */
    H2DE_TEXT_ALIGN_CENTER_CENTER   = 0b00010010,   /**< Align center vertically and center horizontally */
    H2DE_TEXT_ALIGN_CENTER_RIGHT    = 0b00010100,   /**< Align center vertically and right horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_LEFT     = 0b00100001,   /**< Align bottom vertically and left horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_CENTER   = 0b00100010,   /**< Align bottom vertically and center horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_RIGHT    = 0b00100100,   /**< Align bottom vertically and right horizontally */
};

/**
 * @enum H2DE_BorderType
 * @brief Represents the type of border shape.
 * 
 * - H2DE_BORDER_TYPE_RECTANGLE: A standard rectangular border.
 * - H2DE_BORDER_TYPE_CIRCLE: A circular border.
 */
enum H2DE_BorderType {
    H2DE_BORDER_TYPE_RECTANGLE, /**< A standard rectangular border */
    H2DE_BORDER_TYPE_CIRCLE     /**< A circular border */
};

/**
 * @enum H2DE_BorderPlacement
 * @brief Represents the placement of the border relative to the shape's outline.
 * 
 * - H2DE_BORDER_PLACEMENT_CENTER: Border is centered on the shape's edge (half inside, half outside).
 * - H2DE_BORDER_PLACEMENT_INNER: Border is fully inside the shape's boundary.
 * - H2DE_BORDER_PLACEMENT_OUTER: Border is fully outside the shape's boundary.
 */
enum H2DE_BorderPlacement {
    H2DE_BORDER_PLACEMENT_CENTER,   /**< Border centered on the edge */
    H2DE_BORDER_PLACEMENT_INNER,    /**< Border fully inside the shape */
    H2DE_BORDER_PLACEMENT_OUTER,    /**< Border fully outside the shape */
};

/**
 * @enum H2DE_MouseButton
 * @brief Represents mouse buttons that can be used to trigger interactions.
 * 
 * This enum allows you to specify which mouse button(s) should be detected
 * for an interaction.
 * You can combine multiple values using bitwise OR to allow several buttons.
 * 
 * - H2DE_MOUSE_BUTTON_LEFT: Left mouse button.
 * - H2DE_MOUSE_BUTTON_RIGHT: Right mouse button.
 * - H2DE_MOUSE_BUTTON_MIDDLE: Middle mouse button (usually the scroll wheel).
 * - H2DE_MOUSE_BUTTON_LEFT_AND_RIGHT: Both left and right buttons.
 * - H2DE_MOUSE_BUTTON_ALL: All mouse buttons (left, right, and middle).
 */
enum H2DE_MouseButton {
    H2DE_MOUSE_BUTTON_LEFT              = 0b0001,   /**< Left mouse button */
    H2DE_MOUSE_BUTTON_RIGHT             = 0b0010,   /**< Right mouse button */
    H2DE_MOUSE_BUTTON_MIDDLE            = 0b0100,   /**< Middle mouse button (scroll wheel) */
    H2DE_MOUSE_BUTTON_LEFT_AND_RIGHT    = 0b0011,   /**< Both left and right buttons */
    H2DE_MOUSE_BUTTON_ALL               = 0b0111,   /**< All mouse buttons */
};

/**
 * @enum H2DE_ScaleMode
 * @brief Defines how textures or sprites are scaled.
 * 
 * - NEAREST: Pixelated scaling, preserves hard edges (nearest neighbor).
 * - LINEAR: Smooth scaling with linear interpolation.
 * - BEST: Chooses the best available filtering method (may vary by platform).
 */
enum H2DE_ScaleMode {   
    H2DE_SCALE_MODE_NEAREST,    /**< Fast pixelated scaling */
    H2DE_SCALE_MODE_LINEAR,     /**< Smooth linear interpolation */
    H2DE_SCALE_MODE_BEST,       /**< Best quality scaling available */
};

/**
 * @enum H2DE_BlendMode
 * @brief Defines blending modes for rendering surfaces.
 * 
 * - BLEND: Standard alpha blending.
 * - ADD: Additive blending (lightens colors).
 * - MOD: Modulate (multiplicative) blending.
 * - MUL: Multiply blending.
 * - INVALID: Invalid or unsupported blend mode.
 * - NONE: No blending, opaque rendering.
 */
enum H2DE_BlendMode {
    H2DE_BLEND_MODE_BLEND,      /**< Alpha blend */
    H2DE_BLEND_MODE_ADD,        /**< Additive blend */
    H2DE_BLEND_MODE_MOD,        /**< Modulate blend */
    H2DE_BLEND_MODE_MUL,        /**< Multiply blend */
    H2DE_BLEND_MODE_INVALID,    /**< Invalid mode */
    H2DE_BLEND_MODE_NONE,       /**< No blending */
};

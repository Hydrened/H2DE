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
    H2DE_FACE_TOP       = 0x00,     /**< Facing upwards (top) */
    H2DE_FACE_RIGHT     = 0x01,     /**< Facing right */
    H2DE_FACE_BOTTOM    = 0x02,     /**< Facing downwards (bottom) */
    H2DE_FACE_LEFT      = 0x03,     /**< Facing left */
};

/**
 * @enum H2DE_TextAlign
 * @brief Defines text alignment options combining vertical and horizontal positions.
 * 
 * Bits encode vertical and horizontal alignment to position text within a rectangle:
 * - Vertical: Top, Center, Bottom
 * - Horizontal: Left, Center, Right
 * 
 * Allows specifying precise alignment like top-left, center-center, bottom-right, etc.
 */
enum H2DE_TextAlign {
    H2DE_TEXT_ALIGN_TOP_LEFT        = 0x09,     /**< Align top vertically and left horizontally */
    H2DE_TEXT_ALIGN_TOP_CENTER      = 0x0A,     /**< Align top vertically and center horizontally */
    H2DE_TEXT_ALIGN_TOP_RIGHT       = 0x0C,     /**< Align top vertically and right horizontally */
    H2DE_TEXT_ALIGN_CENTER_LEFT     = 0x11,     /**< Align center vertically and left horizontally */
    H2DE_TEXT_ALIGN_CENTER_CENTER   = 0x12,     /**< Align center vertically and center horizontally */
    H2DE_TEXT_ALIGN_CENTER_RIGHT    = 0x14,     /**< Align center vertically and right horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_LEFT     = 0x21,     /**< Align bottom vertically and left horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_CENTER   = 0x22,     /**< Align bottom vertically and center horizontally */
    H2DE_TEXT_ALIGN_BOTTOM_RIGHT    = 0x24,     /**< Align bottom vertically and right horizontally */
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

/**
 * @enum H2DE_Cursor
 * @brief Defines the available system cursors for rendering.
 * 
 * - ARROW: Default arrow pointer.
 * - IBEAM: Text selection (I-beam) cursor.
 * - WAIT: Wait or loading indicator.
 * - CROSSHAIR: Crosshair cursor for precision selection.
 * - WAITARROW: Combined wait and arrow cursor.
 * - SIZENWSE: Diagonal resize from top-left to bottom-right.
 * - SIZENESW: Diagonal resize from top-right to bottom-left.
 * - SIZEWE: Horizontal resize.
 * - SIZENS: Vertical resize.
 * - SIZEALL: All-direction move cursor.
 * - NO: "Not allowed" or unavailable action cursor.
 * - HAND: Hand cursor, typically used for clickable elements.
 */
enum H2DE_Cursor {
    H2DE_CURSOR_ARROW,          /**< Default arrow pointer */
    H2DE_CURSOR_IBEAM,          /**< Text selection (I-beam) */
    H2DE_CURSOR_WAIT,           /**< Wait or loading */
    H2DE_CURSOR_CROSSHAIR,      /**< Precision selection crosshair */
    H2DE_CURSOR_WAITARROW,      /**< Wait + arrow */
    H2DE_CURSOR_SIZENWSE,       /**< Diagonal resize */
    H2DE_CURSOR_SIZENESW,       /**< Diagonal resize */
    H2DE_CURSOR_SIZEWE,         /**< Horizontal resize */
    H2DE_CURSOR_SIZENS,         /**< Vertical resize */
    H2DE_CURSOR_SIZEALL,        /**< Move in all directions */
    H2DE_CURSOR_NO,             /**< Not allowed / unavailable */
    H2DE_CURSOR_HAND,           /**< Clickable element pointer */
};

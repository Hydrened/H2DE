#pragma once

#include <H2DE/utils/H2DE_utils.h>

/**
 * @struct H2DE_SurfaceData
 * @brief Contains transformation and rendering parameters for a surface.
 */
struct H2DE_SurfaceData {
    H2DE_Transform transform = H2DE_Transform();            /**< Transformation data (position, scale, rotation, pivot). */
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;      /**< Scaling interpolation mode. */
    H2DE_BlendMode blendMode = H2DE_BLEND_MODE_BLEND;       /**< Blending mode used for rendering. */
    int index = 0;                                          /**< Identifier or order index of the surface. */

    /**
     * @brief Default constructor.
     */
    constexpr H2DE_SurfaceData() noexcept = default;
    /**
     * @brief Constructs an H2DE_SurfaceData with specified transform, scale mode, blend mode, and index.
     * @param transform The transform data (position, scale, rotation, pivot).
     * @param scaleMode The scaling mode used for interpolation.
     * @param blendMode The blending mode for rendering.
     * @param index The index or order of the surface.
     */
    constexpr H2DE_SurfaceData(const H2DE_Transform& transform, H2DE_ScaleMode scaleMode, H2DE_BlendMode blendMode, int index) noexcept : transform(transform), scaleMode(scaleMode), blendMode(blendMode), index(index) {}
};

/**
 * @struct H2DE_TextureData
 * @brief Holds texture data including texture name, color tint, and optional source rectangle.
 */
struct H2DE_TextureData {
    std::string textureName = "";                               /**< Name or identifier of the texture */
    H2DE_ColorRGB color = H2DE_ColorRGB();                      /**< Color tint applied to the texture */
    std::optional<H2DE_PixelRect> srcRect = std::nullopt;       /**< Optional source rectangle to use a part of the texture */
};

/**
 * @struct H2DE_SpriteData
 * @brief Stores data for a sprite including texture info, position, size, animation and other settings.
 */
struct H2DE_SpriteData {
    std::string textureName = "";                   /**< Name or ID of the texture used for the sprite */
    H2DE_ColorRGB color = H2DE_ColorRGB();          /**< Color tint applied to the sprite */
    H2DE_PixelPos startingPos = { 0, 0 };           /**< Starting position of the sprite within the texture */
    H2DE_PixelSize size = { 1, 1 };                 /**< Size of the sprite frame */
    int spacing = 0;                                /**< Spacing between frames in the texture */
    uint16_t nbFrame = 0;                           /**< Number of frames for animation */
    uint32_t delay = 200;                           /**< Delay in milliseconds between animation frames */
    bool pauseSensitive = true;                     /**< Whether the sprite animation respects pause state */
};

/**
 * @struct H2DE_ColorData
 * @brief Holds RGB color data for an object.
 */
struct H2DE_ColorData {
    H2DE_ColorRGB color = H2DE_ColorRGB();      /**< The RGB color value */

    /** 
     * @brief Default constructor, initializes with default color.
     */
    constexpr H2DE_ColorData() noexcept = default;
    /**
     * @brief Constructs H2DE_ColorData with a given RGB color.
     * @param color The RGB color to initialize with.
     */
    constexpr H2DE_ColorData(const H2DE_ColorRGB& color) noexcept : color(color) {}
};

/**
 * @struct H2DE_BorderData
 * @brief Describes the border style used for border object.
 * 
 * Contains data such as border type, color, thickness, and fill status.
 */
struct H2DE_BorderData {
    H2DE_BorderType type = H2DE_BORDER_TYPE_RECTANGLE;                  /**< Shape of the border (rectangle or circle) */
    H2DE_BorderPlacement placement = H2DE_BORDER_PLACEMENT_CENTER;      /**< Placement of the border relative to the shape's edge */
    H2DE_ColorRGB color = H2DE_ColorRGB();                              /**< Color of the border */
    uint16_t thickness = 1;                                             /**< Thickness of the border in pixels */
    bool filled = false;                                                /**< Whether the shape is filled (true) or just outlined (false) */

    /** 
     * @brief Default constructor. 
     */
    constexpr H2DE_BorderData() noexcept = default;

    /**
     * @brief Custom constructor to initialize all fields.
     * @param type Border shape type
     * @param placement Border placement
     * @param color Border color
     * @param thickness Border thickness in pixels
     * @param filled Whether the shape is filled or not
     */
    constexpr H2DE_BorderData(H2DE_BorderType type, H2DE_BorderPlacement placement, const H2DE_ColorRGB& color, uint16_t thickness, bool filled) noexcept : type(type), placement(placement), color(color), thickness(thickness), filled(filled) {}
};

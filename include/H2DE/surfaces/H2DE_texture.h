#ifndef H2DE_TEXTURE_H
#define H2DE_TEXTURE_H

/**
 * @file H2DE_texture.h
 * @brief Defines the `H2DE_Texture` class for managing textures in the engine.
 * 
 * The `H2DE_Texture` class is a derived class of `H2DE_Surface`, specifically handling texture surfaces.
 * It provides functionality to manipulate texture-specific properties like source rectangles.
 */

#include <H2DE/surfaces/H2DE_surface.h>

/**
 * @brief Class representing a texture surface.
 * 
 * The `H2DE_Texture` class inherits from `H2DE_Surface` and is used for handling textures within the engine.
 * It allows setting a source rectangle for texture mapping and managing texture-specific properties.
 */
class H2DE_Texture : public H2DE_Surface {
private:
    H2DE_TextureData ted;

    H2DE_Texture(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_TextureData& ted);
    ~H2DE_Texture() override;

    inline std::optional<H2DE_AbsRect> getSrcRect() const override { return ted.srcRect; };
    
public:
    /**
     * @brief Creates a new texture surface.
     * 
     * This function creates a new `H2DE_Texture` with the provided engine, surface data, and texture data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param surfaceData The surface data to initialize the surface with.
     * @param textureData The texture data to initialize the texture with.
     * @return A pointer to the created `H2DE_Texture`.
     */
    friend H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
    /**
     * @brief Sets the source rectangle for the texture.
     * 
     * This function sets the source rectangle for the texture, determining which portion of the texture to use.
     * 
     * @param texture A pointer to the `H2DE_Surface` (which is a texture).
     * @param srcRect The optional source rectangle to set.
     */
    friend void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect);
};

H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
void H2DE_SetTextureSrcRect(H2DE_Surface* texture, const std::optional<H2DE_AbsRect>& srcRect);

#endif

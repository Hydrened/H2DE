#ifndef H2DE_TEXTURE_H
#define H2DE_TEXTURE_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Texture : public H2DE_Surface {
private:
    H2DE_TextureData textureData;

    H2DE_Texture(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
    ~H2DE_Texture() override;

public:
    inline H2DE_TextureData getTextureData() const { return textureData; }
    inline std::optional<H2DE_PixelRect> getSrcRect() const override { return textureData.srcRect; };

    inline void setSrcRect(const std::optional<H2DE_PixelRect>& srcRect) { textureData.srcRect = srcRect; }

    friend class H2DE_Object;
};

#endif

#ifndef H2DE_TEXTURE_H
#define H2DE_TEXTURE_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Texture : public H2DE_Surface {
private:
    H2DE_TextureData textureData;

    H2DE_Texture(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
    ~H2DE_Texture() override;

    bool isVisible() const override;

public:
    inline H2DE_TextureData getTextureData() const { return textureData; }
    inline std::string getTextureName() const override { return textureData.textureName; }
    inline H2DE_ColorRGB getColor() const { return textureData.color; }
    inline std::optional<H2DE_PixelRect> getSrcRect() const override { return textureData.srcRect; };

    void setTextureName(const std::string& textureName);
    void setColor(const H2DE_ColorRGB& color);
    void setSrcRect(const std::optional<H2DE_PixelRect>& srcRect);

    unsigned int setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Object;
    friend class H2DE_TextObject;
};

#endif

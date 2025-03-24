#ifndef H2DE_TEXTURE_H
#define H2DE_TEXTURE_H

#include <H2DE/H2DE_engine.h>

class H2DE_Texture : public H2DE_Surface {
private:
    H2DE_TextureData ted;

    H2DE_Texture(H2DE_Engine* engine, const H2DE_SurfaceData& sd, const H2DE_TextureData& ted);
    ~H2DE_Texture() override;

    std::optional<H2DE_AbsRect> getSrcRect() const override;
    
public:
    friend H2DE_Surface* H2DE_CreateTexture(H2DE_Engine* engine, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData);
};

#endif

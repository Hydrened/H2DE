#ifndef H2DE_SURFACE_SPRITE_H
#define H2DE_SURFACE_SPRITE_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Sprite : public H2DE_Surface {
private:
    H2DE_SpriteData spriteData;

    int delayId = -1;
    uint16_t currentFrame = 0;

    H2DE_Sprite(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData);
    ~H2DE_Sprite() override;

    void initDelay();
    void nextFrame();

    std::optional<H2DE_PixelRect> getSrcRect() const override;
    bool isVisible() const override;

public:
    inline H2DE_SpriteData getSpriteData() const { return spriteData; }
    inline std::string getTextureName() const override { return spriteData.textureName; }
    inline H2DE_ColorRGB getColor() const override { return spriteData.color; }
    inline H2DE_PixelPos getStartingPos() const { return spriteData.startingPos; }
    inline H2DE_PixelSize getSize() const { return spriteData.size; }
    inline int getSpacing() const { return spriteData.spacing; }
    inline uint16_t getNbFrame() const { return spriteData.nbFrame; }
    inline uint32_t getDelay() const { return spriteData.delay; }
    inline bool isPauseSensitive() const { return spriteData.pauseSensitive; }

    void setTextureName(const std::string& textureName);
    void setColor(const H2DE_ColorRGB& color);
    void setStartingPos(const H2DE_PixelPos& startingPos);
    void setSize(const H2DE_PixelSize& size);
    void setSpacing(int spacing);
    void setNbFrame(uint16_t nbFrame);
    void setDelay(uint32_t delay);
    void setPauseSensitive(bool pauseSensitive);

    H2DE_TimelineID setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_SpriteData;

    friend class H2DE_Object;
};

#endif

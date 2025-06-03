#ifndef H2DE_SPRITE_H
#define H2DE_SPRITE_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Sprite : public H2DE_Surface {
private:
    H2DE_SpriteData spriteData;

    int delayId = -1;
    unsigned int currentFrame = 0;

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
    inline unsigned int getNbFrame() const { return spriteData.nbFrame; }
    inline unsigned int getDelay() const { return spriteData.delay; }
    inline bool isPauseSensitive() const { return spriteData.pauseSensitive; }

    void setTextureName(const std::string& textureName);
    void setColor(const H2DE_ColorRGB& color);
    void setStartingPos(const H2DE_PixelPos& startingPos);
    void setSize(const H2DE_PixelSize& size);
    void setSpacing(int spacing);
    void setNbFrame(unsigned int nbFrame);
    void setDelay(unsigned int delay);
    void setPauseSensitive(bool pauseSensitive);

    unsigned int setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    friend class H2DE_Object;
};

#endif

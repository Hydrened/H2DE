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

public:
    inline H2DE_SpriteData getSpriteData() const { return spriteData; }
    inline H2DE_PixelPos getStartingPos() const { return spriteData.startingPos; }
    inline H2DE_PixelSize getSize() const { return spriteData.size; }
    inline int getSpacing() const { return spriteData.spacing; }
    inline unsigned int getNbFrame() const { return spriteData.nbFrame; }
    inline unsigned int getDelay() const { return spriteData.delay; }
    inline bool isPauseSensitive() const { return spriteData.pauseSensitive; }

    inline void setStartingPos(const H2DE_PixelPos& startingPos) { spriteData.startingPos = startingPos; }
    inline void setSize(const H2DE_PixelSize& size) { spriteData.size = size; }
    inline void setSpacing(int spacing) { spriteData.spacing = spacing; }
    inline void setNbFrame(unsigned int nbFrame) { spriteData.nbFrame = nbFrame; }
    inline void setDelay(unsigned int delay) { spriteData.delay = delay; }
    inline void setPauseSensitive(bool pauseSensitive) { spriteData.pauseSensitive = pauseSensitive; }

    friend class H2DE_Object;
};

#endif

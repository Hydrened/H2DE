#ifndef H2DE_COLOR_H
#define H2DE_COLOR_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Color : public H2DE_Surface {
private:
    H2DE_ColorData colorData;

    H2DE_Color(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData);
    ~H2DE_Color() override;

    inline std::optional<H2DE_PixelRect> getSrcRect() const override { return std::nullopt; };

public:
    inline H2DE_ColorData getColorData() const { return colorData; }
    inline H2DE_ColorRGB getColor() const { return colorData.color; }

    inline void setColor(const H2DE_ColorRGB& color) { colorData.color = color; }

    unsigned int setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Object;
};

#endif

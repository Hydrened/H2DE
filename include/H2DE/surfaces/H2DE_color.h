#ifndef H2DE_SURFACE_COLOR_H
#define H2DE_SURFACE_COLOR_H

#include <H2DE/surfaces/H2DE_surface.h>

class H2DE_Color : public H2DE_Surface {
public:
    constexpr H2DE_ColorData getColorData() const { return colorData; }
    inline H2DE_ColorRGB getColor() const override { return colorData.color; }

    inline void setColor(const H2DE_ColorRGB& color) { colorData.color = color; }

    H2DE_TimelineID setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_ColorData;
    
    friend class H2DE_Object;

private:
    H2DE_ColorData colorData;

    H2DE_Color(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData);
    ~H2DE_Color() override = default;

    inline std::string getTextureName() const override { return "/"; }
    inline std::optional<H2DE_PixelRect> getSrcRect() const override { return std::nullopt; };
    bool isVisible() const override;
};

#endif

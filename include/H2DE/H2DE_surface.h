#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Surface {
private:
    H2DE_Engine* engine;
    H2DE_SurfaceData sd;

    virtual std::optional<H2DE_AbsRect> getSrcRect() const = 0;

protected:
    H2DE_Surface(H2DE_Engine* engine, const H2DE_SurfaceData& sd);
    ~H2DE_Surface();

public:
    friend class H2DE_Engine;
    friend class H2DE_BarObject;
    friend class H2DE_BasicObject;
    friend class H2DE_ButtonObject;
    friend class H2DE_TextObject;
};

#endif

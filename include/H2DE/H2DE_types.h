#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>

/**
 * `H2DE_GraphicObject` types
 * \since H2DE-1.0.0
 */
enum H2DE_GraphicObjectType {
    IMAGE,
    CIRCLE,
    POLYGON,
    TEXT,
};

/**
 * `H2DE_Timeline` effects
 * \since H2DE-1.0.9
 */
enum H2DE_TimelineEffect {
    LINEAR,
    EASE_IN_OUT,
    EASE_IN,
    EASE_OUT,
};

/**
 * H2DE position
 * \since H2DE-1.0.0
 */
struct H2DE_Pos {
    int x;
    int y;

    explicit operator SDL_Point() const;
};

/**
 * H2DE size
 * \since H2DE-1.0.0
 */
struct H2DE_Size {
    int w;
    int h;
};

/**
 * H2DE rect
 * \since H2DE-1.2.4
 */
struct H2DE_Rect {
    H2DE_Pos pos;
    H2DE_Size size;

    operator SDL_Rect() const;
};

struct H2DE_HSV;
/**
 * H2DE rgb color
 * \since H2DE-1.0.0
 */
struct H2DE_RGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    explicit operator Uint32() const;
    operator SDL_Color() const;
    explicit operator H2DE_HSV() const;
};

/**
 * H2DE rgb hsv
 * \since H2DE-1.2.0
 */
struct H2DE_HSV {
    float h;
    float s;
    float v;
    float a;

    explicit operator H2DE_RGB() const;
};

/**
 * H2DE scale
 * \since H2DE-1.0.0
 */
struct H2DE_Scale {
    float x;
    float y;
};

/**
 * H2DE click
 * \since H2DE-1.1.0
 */
struct H2DE_SClick {
    int x;
    int y;
};

#endif

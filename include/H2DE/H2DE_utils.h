#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <SDL2/SDL_video.h>

template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x;
    H2DE_Vector2D_T y;
};

using H2DE_AbsPos = H2DE_Vector2D<int>;
using H2DE_AbsSize = H2DE_Vector2D<int>;
using H2DE_LevelPos = H2DE_Vector2D<float>;
using H2DE_LevelSize = H2DE_Vector2D<float>;
using H2DE_LevelVelocity = H2DE_Vector2D<float>;

template<typename H2DE_Rect_T>
struct H2DE_Rect {
    H2DE_Rect_T x;
    H2DE_Rect_T y;
    H2DE_Rect_T w;
    H2DE_Rect_T h;
};

using H2DE_AbsRect = H2DE_Rect<int>;
using H2DE_LevelRect = H2DE_Rect<float>;








struct H2DE_WindowData {
    unsigned int fps = 60;
    const char* title = "H2DE window";
    H2DE_AbsPos pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
    H2DE_AbsSize size = { 640, 360 };
    bool fullscreen = false;
    bool resizable = false;
    bool saveState = false;
};

struct H2DE_EngineData {
    H2DE_WindowData window = H2DE_WindowData();
};

#endif

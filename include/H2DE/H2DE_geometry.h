#ifndef H2DE_GEOMETRY_H
#define H2DE_GEOMETRY_H

#include <H2DE/H2DE_engine.h>
class H2DE_Object;

class H2DE_Geometry {
public:
    static H2DE_LevelRect getHitboxRect(const H2DE_Object* object, const H2DE_Hitbox& hitbox);

    static H2DE_LevelRect getWorldFlipedRect(const H2DE_LevelRect& world_parentRect, const H2DE_LevelRect& local_childRect, SDL_RendererFlip flip);
    static SDL_RendererFlip getFlipFromScale(const H2DE_Scale& scale);
    static SDL_RendererFlip addFlip(SDL_RendererFlip flip1, SDL_RendererFlip flip2);
    static float flipRotation(float rotation, SDL_RendererFlip flip);
    static float rotationCausedByFlip(SDL_RendererFlip flip1, SDL_RendererFlip flip2);
};

using G = H2DE_Geometry;

#endif

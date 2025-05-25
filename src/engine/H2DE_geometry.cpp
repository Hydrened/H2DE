#include "H2DE/H2DE_geometry.h"

// HITBOX
H2DE_LevelRect H2DE_Geometry::getHitboxRect(const H2DE_Object* object, const H2DE_Hitbox& hitbox) {
    const H2DE_Transform objTransform = object->getTransform();
    const H2DE_Transform hitTransform = hitbox.transform;

    const H2DE_LevelRect world_objRect = objTransform.translate.makeRect(objTransform.scale);
    const H2DE_LevelRect local_hitRect = hitTransform.translate.makeRect(hitTransform.scale);
    SDL_RendererFlip local_objFlip = G::getFlipFromScale(objTransform.scale);

    const H2DE_LevelRect world_objFliped_hitRect = G::getWorldFlipedRect(world_objRect, local_hitRect, local_objFlip);

    SDL_RendererFlip objFlip = G::getFlipFromScale(objTransform.scale);
    SDL_RendererFlip hitFlip = G::getFlipFromScale(hitTransform.scale);

    float rotationCausedByFlip = G::rotationCausedByFlip(objFlip, hitFlip);

    float world_objFliped_objRotation = G::flipRotation(objTransform.rotation, objFlip);
    float local_objFliped_hitFliped_hitRotation = G::flipRotation(hitTransform.rotation, G::addFlip(objFlip, hitFlip));
    float world_objFliped_hitFliped_hitRotation = local_objFliped_hitFliped_hitRotation + world_objFliped_objRotation + rotationCausedByFlip;
    float world_snaped_objFliped_hitFliped_hitRotation = std::round(world_objFliped_hitFliped_hitRotation / 90.0f) * 90.0f;

    if (world_snaped_objFliped_hitFliped_hitRotation != 0.0f) {
        // flip hitbox pivot from object
        // flip hitbox pivot from hitbox
        



        // apply rotation on rect
    }
    
    return world_objFliped_hitRect;
}

// FLIP
H2DE_LevelRect H2DE_Geometry::getWorldFlipedRect(const H2DE_LevelRect& world_parentRect, const H2DE_LevelRect& local_childRect, SDL_RendererFlip flip) {
    H2DE_LevelRect res = local_childRect;

    if (flip == SDL_FLIP_HORIZONTAL) {
        res.x = world_parentRect.getScale().x - local_childRect.getTranslate().x + local_childRect.getScale().x;
    }

    if (flip == SDL_FLIP_VERTICAL) {
        res.y = world_parentRect.getScale().y - local_childRect.getTranslate().y + local_childRect.getScale().y;
    }

    return res.addTranslate(world_parentRect.getTranslate());
}

SDL_RendererFlip H2DE_Geometry::getFlipFromScale(const H2DE_Scale& scale) {
    SDL_RendererFlip res = SDL_FLIP_NONE;

    if (scale.x < 0.0f) {
        res = static_cast<SDL_RendererFlip>(res | SDL_FLIP_HORIZONTAL);
    }

    if (scale.y < 0.0f) {
        res = static_cast<SDL_RendererFlip>(res | SDL_FLIP_VERTICAL);
    }

    return res;
}

SDL_RendererFlip H2DE_Geometry::addFlip(SDL_RendererFlip flip1, SDL_RendererFlip flip2) {
    if (flip1 == flip2) {
        return SDL_FLIP_NONE;
    }

    return static_cast<SDL_RendererFlip>(flip1 | flip2);
}

float H2DE_Geometry::flipRotation(float rotation, SDL_RendererFlip flip) {
    float res = rotation;

    switch (flip) {
        case SDL_FLIP_HORIZONTAL: res = -res; break;
        case SDL_FLIP_VERTICAL: res = 180.0f - res; break;
        // case H2DE_FLIP_XY: res = 180.0f + res; break;
        default: break;
    }

    while (res < 0.0f) {
        res += 360.0f;
    }
    while (res >= 360.0f) {
        res -= 360.0f;
    }

    return res;
}

float H2DE_Geometry::rotationCausedByFlip(SDL_RendererFlip flip1, SDL_RendererFlip flip2) {
    if (flip1 == SDL_FLIP_NONE || flip2 == SDL_FLIP_NONE) {
        return 0.0f;
    }

    return ((flip1 != flip2) ? 180.0f : 0.0f);
}

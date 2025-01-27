#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include "H2DE_engine.h"
#include <optional>

struct H2DE_Texture {
    std::string name = "";
    std::optional<H2DE_AbsRect> srcRect = std::nullopt;
    H2DE_ColorRGB color = { 255, 255, 255, 255 };
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
};

struct H2DE_LevelObjectTransform {
    H2DE_LevelPos origin = { 0.0f, 0.0f }; 

    float rotation = 0.0f;
    H2DE_Scale scale = { 1.0f, 1.0f };
    H2DE_Flip flip = H2DE_NO_FLIP;
};

struct H2DE_LevelObject {
    H2DE_LevelHitbox rect = { 0.0f, 0.0f, 1.0f, 1.0f };
    std::optional<H2DE_LevelHitbox> hitbox = std::nullopt;

    H2DE_Texture texture = H2DE_Texture();

    int collisionIndex = 0;
    bool absolute = false;
    bool gravity = false;

    H2DE_LevelObjectTransform transform = H2DE_LevelObjectTransform();

    std::optional<H2DE_LevelObject*> parent = std::nullopt;
    int index = 0;
};

extern H2DE_LevelObject* H2DE_CreateLevelObject();
extern H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_LevelObject g);
extern void H2DE_DestroyLevelObject(H2DE_LevelObject* g);

#endif

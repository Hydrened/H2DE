#ifndef H2DE_TRANSFORM_H
#define H2DE_TRANSFORM_H

#include <H2DE/H2DE_utils.h>

class H2DE_Transform {
public:
    static H2DE_LevelRect flipRect(const H2DE_LevelRect& W_parentRect, const H2DE_LevelRect& L_childRect, H2DE_Flip flip);
    static float flipRotation(float rotation, H2DE_Flip flip);
    static H2DE_LevelPos flipPivot(const H2DE_LevelRect& W_parentRect, const H2DE_LevelPos& L_pivot, H2DE_Flip flip);

    static H2DE_LevelPos applyRotationOnPos(const H2DE_LevelPos& W_pos, const H2DE_LevelPos& W_pivot, float rotation);
    static H2DE_LevelRect applyRotationOnRect(const H2DE_LevelRect& W_rect, const H2DE_LevelPos& W_pivot, float rotation);

    static inline bool isRotationInverted(H2DE_Flip flip) { return (flip == H2DE_FLIP_X || flip == H2DE_FLIP_Y); }

    static H2DE_LevelRect getHitboxWorldDestRect(const H2DE_LevelRect& W_objectRect, const H2DE_LevelPos& L_objectPivot, float objectRotation, H2DE_Flip objectFlip, const H2DE_LevelRect& L_hitboxRect);
};

#endif

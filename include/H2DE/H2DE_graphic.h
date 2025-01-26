#ifndef H2DE_GRAPHIC_H
#define H2DE_GRAPHIC_H

#include "H2DE_engine.h"
#include <optional>

enum H2DE_GraphicObjectType {
    IMAGE,
    POLYGON,
    CIRCLE,
};

struct H2DE_GraphicObjectTransform {
    H2DE_LevelPos origin = { 0.0f, 0.0f }; 

    float rotation = 0.0f;
    H2DE_Scale scale = { 1.0f, 1.0f };
    H2DE_Flip flip = H2DE_NO_FLIP;
};

struct H2DE_Image {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    H2DE_LevelSize size = { 0.0f, 0.0f };

    std::string texture = "";
    std::optional<H2DE_AbsRect> srcRect = std::nullopt;
    H2DE_ScaleMode scaleMode = H2DE_SCALE_MODE_LINEAR;
};

struct H2DE_Polygon {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    std::vector<H2DE_LevelPos> points;
    bool filled = false;
};

struct H2DE_Circle {
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    float radius = 0.0f;
    bool filled = false;
};

struct H2DE_GraphicObject {
    H2DE_GraphicObjectType type;

    H2DE_Image image;
    H2DE_Polygon polygon;
    H2DE_Circle circle;

    H2DE_GraphicObjectTransform transform = H2DE_GraphicObjectTransform();
    H2DE_ColorRGB color = { 255, 255, 255, 255 };

    bool absolute = false;
    std::optional<H2DE_GraphicObject*> parent = std::nullopt;
    int index = 0;
};

extern H2DE_GraphicObject* H2DE_CreateGraphicObject();
extern H2DE_GraphicObject* H2DE_CreateGraphicObject(H2DE_GraphicObject g);
extern void H2DE_DestroyGraphicObject(H2DE_GraphicObject* g);

#endif

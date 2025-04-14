#ifndef H2DE_BAR_OBJECT_H
#define H2DE_BAR_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData bod;
    float percentage = 0.0f;
    float value;

    H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData bod);
    ~H2DE_BarObject() override;

    void refreshPercentage();
    void resetSurfaceBuffers() override;

public:
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);

    friend H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar, const std::string& name);
    friend H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar, const std::string& name);
    friend void H2DE_AddSurfaceToBarObjectFront(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name);
    friend void H2DE_AddSurfaceToBarObjectBackground(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name);
    friend void H2DE_RemoveSurfaceFromBarObjectFront(H2DE_BarObject* bar, const std::string& name);
    friend void H2DE_RemoveSurfaceFromBarObjectBackground(H2DE_BarObject* bar, const std::string& name);

    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value);
    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetBarMin(H2DE_BarObject* bar, float min);
    friend void H2DE_SetBarMax(H2DE_BarObject* bar, float max);
};

#endif

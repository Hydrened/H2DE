#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_ButtonObject : public H2DE_Object {
private:
    H2DE_ButtonObjectData bod;

    bool hover = false;

    H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData bod);
    ~H2DE_ButtonObject() override;

    void resetSurfaceBuffers() override;

public:
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    friend H2DE_Surface* H2DE_GetButtonSurface(const H2DE_ButtonObject* button, const std::string& name);

    friend void H2DE_AddSurfaceToButtonObject(H2DE_ButtonObject* button, H2DE_Surface* surface, const std::string& name);
    friend void H2DE_RemoveSurfaceFromButtonObject(H2DE_ButtonObject* button, const std::string& name);
    
    friend void H2DE_ButtonMouseDown(H2DE_ButtonObject* button);
    friend void H2DE_ButtonMouseUp(H2DE_ButtonObject* button);
    friend void H2DE_ButtonHover(H2DE_ButtonObject* button);
    friend void H2DE_ButtonBlur(H2DE_ButtonObject* button);

    friend void H2DE_SetButtonOnMouseDown(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseDown);
    friend void H2DE_SetButtonOnMouseUp(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseUp);
    friend void H2DE_SetButtonOnHover(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onHover);
    friend void H2DE_SetButtonOnBlur(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onBlur);

    friend class H2DE_Engine;
};

#endif

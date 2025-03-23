#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

#include <H2DE/H2DE_engine.h>

class H2DE_ButtonObject : public H2DE_Object {
private:
    H2DE_ButtonObjectData bod;

    H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData bod);
    ~H2DE_ButtonObject();

    std::vector<H2DE_Surface*> getSurfaces() const override;

public:
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    friend class H2DE_Engine;
};

#endif

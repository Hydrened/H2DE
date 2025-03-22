#ifndef H2DE_TEXT_OBJECT_H
#define H2DE_TEXT_OBJECT_H

#include <H2DE/H2DE_engine.h>

class H2DE_TextObject : public H2DE_Object {
private:
    H2DE_TextObjectData tod;

    H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData tod);
    ~H2DE_TextObject();

    void update() override;

public:
    friend H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_TextObjectData& tod);
};

#endif

#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Object {
private:
    H2DE_Engine* engine;
    H2DE_ObjectData data;

protected:
    H2DE_Object(H2DE_Engine* engine, H2DE_ObjectData data);
    ~H2DE_Object();

public:

};

#endif

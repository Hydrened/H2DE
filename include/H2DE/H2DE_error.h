#ifndef H2DE_ERROR_H
#define H2DE_ERROR_H

#include <iostream>
#include <string>
class H2DE_Engine;
class H2DE_Object;
class H2DE_Surface;

class H2DE_Error {
private:
    static void log(const std::string& message, int color);

public:
    static void logWarning(const std::string& message);
    static void logError(const std::string& message);
    static void throwError(const std::string& message);

    static void checkEngine(const H2DE_Engine* engine);
    static void checkObject(const H2DE_Object* object);
    static void checkSurface(const H2DE_Surface* surface);
};

#endif

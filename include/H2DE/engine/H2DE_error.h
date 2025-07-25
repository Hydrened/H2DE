#ifndef H2DE_ERROR_H
#define H2DE_ERROR_H

#include <string>

class H2DE_Error {
public:
    static void logWarning(const std::string& message);
    static void logError(const std::string& message);
    static void throwError(const std::string& message);

private:
    static void log(const std::string& message, int color);
};

#endif

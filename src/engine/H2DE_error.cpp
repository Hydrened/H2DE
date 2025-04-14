#include "H2DE/H2DE_error.h"

// LOG
void H2DE_Error::log(const std::string& message, int color) {
    const std::string type = color == 1 ? "ERROR" : "Waring";
    std::string colorCode = "\033[3" + std::to_string(color) + "m";
    std::cerr << "H2DE => " << colorCode << type << "\033[0m: " << message << std::endl;
}

void H2DE_Error::logWarning(const std::string& message) {
    H2DE_Error::log(message, 3);
}

void H2DE_Error::logError(const std::string& message) {
    H2DE_Error::log(message, 1);
}

// THROW
void H2DE_Error::throwError(const std::string& message) {
    throw std::runtime_error(message);
}

// POINTERS
void H2DE_Error::checkEngine(const H2DE_Engine* engine) {
    if (engine == nullptr) {
        H2DE_Error::logError("Engine is null");
    }
}

void H2DE_Error::checkObject(const H2DE_Object* object) {
    if (object == nullptr) {
        H2DE_Error::logError("Object is null");
    }
}

void H2DE_Error::checkSurface(const H2DE_Surface* surface) {
    if (surface == nullptr) {
        H2DE_Error::logError("Object is null");
    }
}

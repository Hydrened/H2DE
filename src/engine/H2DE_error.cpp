#include "H2DE/engine/H2DE_error.h"

#include "H2DE/utils/H2DE_utils.h"

// LOG
void H2DE_Error::log(const std::string& message, int color) {
    const std::string type = color == 1 ? "ERROR" : "Waring";
    std::string colorCode = "\033[3" + std::to_string(color) + "m";

    H2DE::print("H2DE => " + colorCode + type + "\033[0m: " + message); 
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

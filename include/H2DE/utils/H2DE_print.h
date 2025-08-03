#pragma once

#include <H2DE/utils/H2DE_utils.h>

namespace H2DE {
    /**
     * @brief Prints a value to std::cout followed by a newline.
     * 
     * Generic template for any type that supports operator<<.
     * 
     * @tparam T Type of the value to print.
     * @param value The value to print.
     */
    template<typename T>
    inline void print(const T& value) {
        std::cout << value << std::endl;
    }
    /**
     * @brief Prints a boolean value as "true" or "false" to std::cout followed by a newline.
     * 
     * Specialized template for bool type.
     * 
     * @tparam T Must be bool.
     * @param value Boolean value to print.
     */
    template<typename T>
    requires (std::same_as<T, bool>)
    inline void print(const T& value) {
        std::cout << (value ? "true" : "false") << std::endl;
    }
    /**
     * @brief Prints an unsigned integral value casted to long long to std::cout followed by a newline.
     * 
     * This avoids printing unsigned char or unsigned short as characters.
     * 
     * @tparam T Unsigned integral type except bool.
     * @param value Unsigned value to print.
     */
    template<typename T>
    requires (std::is_unsigned_v<T> && !std::same_as<T, bool>)
    inline void print(const T& value) {
        std::cout << static_cast<long long>(value) << std::endl;
    }
};

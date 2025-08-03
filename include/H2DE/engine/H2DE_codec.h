#pragma once

#include <base64/base64.h>
#include <string>

struct H2DE_Codec {
public:
    static inline std::string encode(const std::string& s) {
        return xorEncryptDecrypt(base64_encode(s));
    }
    static inline std::string decode(const std::string& s) {
        return base64_decode(xorEncryptDecrypt(s));
    }

private:
    static std::string xorEncryptDecrypt(const std::string& s);
};

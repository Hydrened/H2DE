#include "H2DE/engine/H2DE_codec.h"

std::string H2DE_Codec::xorEncryptDecrypt(const std::string& s) {
    static const std::string key = "9L7ZpR2KqWsMvXoD1AjTbHiCng0EUfGlzVYkXwSjmFQyNPx3ecHr4OaJdIB85tuv6";

    std::string res = s;

    for (size_t i = 0; i < s.size(); ++i) {
        res[i] = s[i] ^ key[i % key.size()];
    }

    return res;
}

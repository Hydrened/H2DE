#include <H2DE/H2DE_types.h>

H2DE_Pos::operator SDL_Point() const {
    return SDL_Point{ x, y };
}

H2DE_Rect::operator SDL_Rect() const {
    return { pos.x, pos.y, size.w, size.h };
}

H2DE_RGB::operator Uint32() const {
    return (a << 24) | (b << 16) | (g << 8) | r;
}

H2DE_RGB::operator SDL_Color() const {
    return { r, g, b, a };
}

H2DE_RGB::operator H2DE_HSV() const {
    H2DE_HSV hsv;

    float r_f = r / 255.0f;
    float g_f = g / 255.0f;
    float b_f = b / 255.0f;

    float max = std::max({r_f, g_f, b_f});
    float min = std::min({r_f, g_f, b_f});
    float delta = max - min;

    hsv.v = max;
    hsv.a = a / 255.0f;

    if (delta == 0) {
        hsv.h = 0;
        hsv.s = 0;
    } else {
        hsv.s = delta / max;

        if (max == r_f) hsv.h = 60 * (fmod(((g_f - b_f) / delta), 6));
        else if (max == g_f) hsv.h = 60 * (((b_f - r_f) / delta) + 2);
        else hsv.h = 60 * (((r_f - g_f) / delta) + 4);
    }

    if (hsv.h < 0) hsv.h += 360;

    return hsv;
}

H2DE_HSV::operator H2DE_RGB() const {
    H2DE_RGB rgb;

    float c = v * s;
    float x = c * (1 - std::fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float r_f, g_f, b_f;

    if (h >= 0 && h < 60) {
        r_f = c;
        g_f = x;
        b_f = 0;
    } else if (h >= 60 && h < 120) {
        r_f = x;
        g_f = c;
        b_f = 0;
    } else if (h >= 120 && h < 180) {
        r_f = 0;
        g_f = c;
        b_f = x;
    } else if (h >= 180 && h < 240) {
        r_f = 0;
        g_f = x;
        b_f = c;
    } else if (h >= 240 && h < 300) {
        r_f = x;
        g_f = 0;
        b_f = c;
    } else {
        r_f = c;
        g_f = 0;
        b_f = x;
    }

    rgb.r = static_cast<Uint8>((r_f + m) * 255);
    rgb.g = static_cast<Uint8>((g_f + m) * 255);
    rgb.b = static_cast<Uint8>((b_f + m) * 255);
    rgb.a = static_cast<Uint8>(a * 255);

    return rgb;
}

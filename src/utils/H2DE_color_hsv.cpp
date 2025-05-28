#include "H2DE/utils/H2DE_utils.h"
#include "H2DE/engine/H2DE_geometry.h"

// CAST
H2DE_ColorHSV::operator H2DE_ColorRGB() const {
    H2DE_ColorRGB rgb;

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

// ADD
H2DE_ColorHSV H2DE_ColorHSV::addHue(float hue) const {
    H2DE_ColorHSV hsv = *this;
    hsv.h = G::normalizeRotation(hsv.h + hue);

    return hsv;
}

H2DE_ColorHSV H2DE_ColorHSV::addSaturation(float saturation) const {
    H2DE_ColorHSV hsv = *this;
    hsv.s += saturation;

    if (hsv.s > 1.0f) {
        hsv.s = 1.0f;

    } else if (hsv.s < 0.0f) {
        hsv.s = 0.0f;
    }

    return hsv;
}

H2DE_ColorHSV H2DE_ColorHSV::addValue(float value) const {
    H2DE_ColorHSV hsv = *this;
    hsv.v += value;

    if (hsv.v > 1.0f) {
        hsv.v = 1.0f;
    
    } else if (hsv.v < 0.0f) {
        hsv.v = 0.0f;
    }

    return hsv;
}

// SUBTRACT
H2DE_ColorHSV H2DE_ColorHSV::subtractHue(float hue) const {
    return addHue(hue * -1);
}

H2DE_ColorHSV H2DE_ColorHSV::subtractSaturation(float saturation) const {
    return addSaturation(saturation * -1);
}

H2DE_ColorHSV H2DE_ColorHSV::subtractValue(float value) const {
    return addValue(value * -1);
}

// MULTIPLY
H2DE_ColorHSV H2DE_ColorHSV::multiplyHue(float multiplier) const {
    H2DE_ColorHSV hsv = *this;
    hsv.h = G::normalizeRotation(hsv.h * multiplier);

    return hsv;
}

H2DE_ColorHSV H2DE_ColorHSV::multiplySaturation(float multiplier) const {
    H2DE_ColorHSV hsv = *this;
    hsv.s *= multiplier;

    if (hsv.s > 1.0f) {
        hsv.s = 1.0f;

    } else if (hsv.s < 0.0f) {
        hsv.s = 0.0f;
    }

    return hsv;
}

H2DE_ColorHSV H2DE_ColorHSV::multiplyValue(float multiplier) const {
    H2DE_ColorHSV hsv = *this;
    hsv.v *= multiplier;

    if (hsv.v > 1.0f) {
        hsv.v = 1.0f;

    } else if (hsv.v < 0.0f) {
        hsv.v = 0.0f;
    }
    
    return hsv;
}

// DIVIDE
H2DE_ColorHSV H2DE_ColorHSV::divideHue(float divider) const {
    return multiplyHue(1 / divider);
}

H2DE_ColorHSV H2DE_ColorHSV::divideSaturation(float divider) const {
    return multiplySaturation(1 / divider);
}

H2DE_ColorHSV H2DE_ColorHSV::divideValue(float divider) const {
    return multiplyValue(1 / divider);
}

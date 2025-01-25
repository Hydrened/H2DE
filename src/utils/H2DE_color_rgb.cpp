#include "H2DE_utils.h"

// CONVERT
H2DE_ColorRGB::operator H2DE_ColorHSV() const {
    H2DE_ColorHSV hsv;

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

// COMPARISON
bool H2DE_ColorRGB::operator==(const H2DE_ColorRGB& other) const {
    return (r == other.r && g == other.g && b == other.b, a == other.a);
}

// ADD
H2DE_ColorRGB H2DE_ColorRGB::addHue(const float& hue) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).addHue(hue));
}

H2DE_ColorRGB H2DE_ColorRGB::addSaturation(const float& saturation) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).addSaturation(saturation));
}

H2DE_ColorRGB H2DE_ColorRGB::addValue(const float& value) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).addValue(value));
}

// SUBTRACT
H2DE_ColorRGB H2DE_ColorRGB::subtractHue(const float& hue) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).subtractHue(hue));
}

H2DE_ColorRGB H2DE_ColorRGB::subtractSaturation(const float& saturation) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).subtractSaturation(saturation));
}

H2DE_ColorRGB H2DE_ColorRGB::subtractValue(const float& value) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).subtractValue(value));
}

// MULTIPLY
H2DE_ColorRGB H2DE_ColorRGB::multiplyHue(const float& multiplier) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).multiplyHue(multiplier));
}

H2DE_ColorRGB H2DE_ColorRGB::multiplySaturation(const float& multiplier) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).multiplySaturation(multiplier));
}

H2DE_ColorRGB H2DE_ColorRGB::multiplyValue(const float& multiplier) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).multiplyValue(multiplier));
}

// DIVIDE
H2DE_ColorRGB H2DE_ColorRGB::divideHue(const float& divider) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).divideHue(divider));
}

H2DE_ColorRGB H2DE_ColorRGB::divideSaturation(const float& divider) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).divideSaturation(divider));
}

H2DE_ColorRGB H2DE_ColorRGB::divideValue(const float& divider) const {
    return static_cast<H2DE_ColorRGB>(static_cast<H2DE_ColorHSV>(*this).divideValue(divider));
}

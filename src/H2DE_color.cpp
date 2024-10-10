#include <H2DE/H2DE_color.h>

// ADD
H2DE_RGB H2DE_AddHueToRGB(H2DE_RGB rgb, float hue) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.h += hue;

    while (hsv.h >= 360) hsv.h -= 360.0f;
    while (hsv.h < 0.0f) hsv.h += 360.0f;

    return static_cast<H2DE_RGB>(hsv);
}

H2DE_RGB H2DE_AddSaturationToRGB(H2DE_RGB rgb, float saturation) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.s += saturation;

    if (hsv.s > 1.0f) hsv.s = 1.0f;
    else if (hsv.s < 0.0f) hsv.s = 0.0f;

    return static_cast<H2DE_RGB>(hsv);
}

H2DE_RGB H2DE_AddValueToRGB(H2DE_RGB rgb, float value) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.v += value;

    if (hsv.v > 1.0f) hsv.v = 1.0f;
    else if (hsv.v < 0.0f) hsv.v = 0.0f;

    return static_cast<H2DE_RGB>(hsv);
}



// SUBSTRACT
H2DE_RGB H2DE_SubstractHueToRGB(H2DE_RGB rgb, float hue) {
    return H2DE_AddHueToRGB(rgb, hue * -1);
}

H2DE_RGB H2DE_SubstractSaturationToRGB(H2DE_RGB rgb, float saturation) {
    return H2DE_AddSaturationToRGB(rgb, saturation * -1);
}

H2DE_RGB H2DE_SubstractValueToRGB(H2DE_RGB rgb, float value) {
    return H2DE_AddValueToRGB(rgb, value * -1);
}



// MULTIPLY
H2DE_RGB H2DE_MultiplyRGBHue(H2DE_RGB rgb, float multiplier) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.h *= multiplier;

    while (hsv.h >= 360) hsv.h -= 360.0f;
    while (hsv.h < 0.0f) hsv.h += 360.0f;

    return static_cast<H2DE_RGB>(hsv);
}

H2DE_RGB H2DE_MultiplyRGBSaturation(H2DE_RGB rgb, float multiplier) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.s *= multiplier;

    if (hsv.s > 1.0f) hsv.s = 1.0f;
    else if (hsv.s < 0.0f) hsv.s = 0.0f;

    return static_cast<H2DE_RGB>(hsv);
}

H2DE_RGB H2DE_MultiplyRGBValue(H2DE_RGB rgb, float multiplier) {
    H2DE_HSV hsv = static_cast<H2DE_HSV>(rgb);
    hsv.v *= multiplier;

    if (hsv.v > 1.0f) hsv.v = 1.0f;
    else if (hsv.v < 0.0f) hsv.v = 0.0f;

    return static_cast<H2DE_RGB>(hsv);
}



// DIVIDE
H2DE_RGB H2DE_DivideRGBHue(H2DE_RGB rgb, float divider) {
    return H2DE_MultiplyRGBHue(rgb, 1 / divider);
}

H2DE_RGB H2DE_DivideRGBSaturation(H2DE_RGB rgb, float divider) {
    return H2DE_MultiplyRGBSaturation(rgb, 1 / divider);
}

H2DE_RGB H2DE_DivideRGBValue(H2DE_RGB rgb, float divider) {
    return H2DE_MultiplyRGBValue(rgb, 1 / divider);
}

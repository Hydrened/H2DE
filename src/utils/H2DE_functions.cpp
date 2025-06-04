#include "H2DE/utils/H2DE_utils.h"

int H2DE_RandomIntegerInRange(int min, int max) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(e);
}

float H2DE_RandomFloatInRange(float min, float max) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(e);
}

bool H2DE_RandomBool() {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    return (dist(e) == 0);
}

float H2DE_Lerp(float min, float max, float blend, H2DE_Easing easing = H2DE_EASING_LINEAR) {
    constexpr float PI = 3.14159265359f;

    blend = std::clamp(blend, 0.0f, 1.0f);

    switch (easing) {
        case H2DE_EASING_EASE_IN: {
            blend = blend * blend;
            break;
        }

        case H2DE_EASING_EASE_OUT: {
            blend = 1.0f - (1.0f - blend) * (1.0f - blend);
            break;
        }

        case H2DE_EASING_EASE_IN_OUT: {
            blend = (blend < 0.5f) ? 2.0f * blend * blend : 1.0f - powf(-2.0f * blend + 2.0f, 2) * 0.5f;
            break;
        }

        case H2DE_EASING_BACK_IN: {
            blend = blend * blend * (2.70158f * blend - 1.70158f);
            break;
        }

        case H2DE_EASING_BACK_OUT: {
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            blend -= 1.0f;
            blend = 1.0f + blend * blend * (c3 * blend + c1);
            break;
        }

        case H2DE_EASING_BACK_IN_OUT: {
            float c1 = 1.70158f;
            float c2 = c1 * 1.525f;
        
            if (blend < 0.5f) {
                float t = blend * 2.0f;
                blend = (t * t * ((c2 + 1.0f) * t - c2)) * 0.5f;
                
            } else {
                float t = (blend * 2.0f) - 2.0f;
                blend = (t * t * ((c2 + 1.0f) * t + c2) + 2.0f) * 0.5f;
            }
            break;
        }

        case H2DE_EASING_ELASTIC_IN: {
            if (blend == 0.0f || blend == 1.0f) {
                break;
            }

            float amplitude = powf(2.0f, 10.0f * (blend - 1.0f));
            float oscillation = sinf((blend - 1.075f) * (2.0f * PI) / 0.3f);
        
            blend = -amplitude * oscillation;
            break;
        }

        case H2DE_EASING_ELASTIC_OUT: {
            if (blend == 0.0f || blend == 1.0f) {
                break;
            }
        
            float amplitude = powf(2.0f, -10.0f * blend);
            float oscillation = sinf((blend - 0.075f) * (2.0f * PI) / 0.3f);
        
            blend = amplitude * oscillation + 1.0f;
            break;
        }

        case H2DE_EASING_ELASTIC_IN_OUT: {
            if (blend == 0.0f || blend == 1.0f) {
                break;
            }
        
            float sineFactor = (20.0f * blend - 11.125f);
            float amplitude, oscillation;
        
            if (blend < 0.5f) {
                amplitude = powf(2.0f, 20.0f * blend - 10.0f);
                oscillation = sinf(sineFactor * (2.0f * PI) / 4.5f);
                blend = -(amplitude * oscillation) * 0.5f;

            } else {
                amplitude = powf(2.0f, -20.0f * blend + 10.0f);
                oscillation = sinf(sineFactor * (2.0f * PI) / 4.5f);
                blend = (amplitude * oscillation) * 0.5f + 1.0f;
            }
        
            break;
        }

        case H2DE_EASING_BOUNCE_IN: {
            blend = 1.0f - H2DE_Lerp(0.0f, 1.0f, 1.0f - blend, H2DE_EASING_BOUNCE_OUT);
            break;
        }

        case H2DE_EASING_BOUNCE_OUT: {
            if (blend < 1.0f / 2.75f) {
                blend = 7.5625f * blend * blend;

            } else if (blend < 2.0f / 2.75f) {
                blend = 7.5625f * (blend -= 1.5f / 2.75f) * blend + 0.75f;

            } else if (blend < 2.5f / 2.75f) {
                blend = 7.5625f * (blend -= 2.25f / 2.75f) * blend + 0.9375f;

            } else {
                blend = 7.5625f * (blend -= 2.625f / 2.75f) * blend + 0.984375f;
            }
            break;
        }

        case H2DE_EASING_BOUNCE_IN_OUT: {
            if (blend < 0.5f) {
                float bounceBlend = 1.0f - 2.0f * blend;
                float bounceInValue = H2DE_Lerp(0.0f, 1.0f, bounceBlend, H2DE_EASING_BOUNCE_OUT);
                blend = (1.0f - bounceInValue) * 0.5f;

            } else {
                float bounceBlend = 2.0f * blend - 1.0f;
                float bounceOutValue = H2DE_Lerp(0.0f, 1.0f, bounceBlend, H2DE_EASING_BOUNCE_OUT);
                blend = (1.0f + bounceOutValue) * 0.5f;
            }
            break;
        }

        case H2DE_EASING_SINE_IN: {
            blend = 1.0f - cosf((blend * PI) * 0.5f);
            break;
        }

        case H2DE_EASING_SINE_OUT: {
            blend = sinf((blend * PI) * 0.5f);
            break;
        }

        case H2DE_EASING_SINE_IN_OUT: {
            blend = -(cosf(PI * blend) - 1.0f) * 0.5f;
            break;
        }

        case H2DE_EASING_EXPO_IN: {
            blend = (blend == 0.0f) ? 0.0f : powf(2.0f, 10.0f * (blend - 1.0f));
            break;
        }

        case H2DE_EASING_EXPO_OUT: {
            blend = (blend == 1.0f) ? 1.0f : 1.0f - powf(2.0f, -10.0f * blend);
            break;
        }

        case H2DE_EASING_EXPO_IN_OUT: {
            blend = (blend == 0.0f)
                ? 0.0f
                : (blend == 1.0f)
                    ? 1.0f
                    : (blend < 0.5f)
                        ? powf(2.0f, 20.0f * blend - 10.0f) * 0.5f
                        : (2.0f - powf(2.0f, -20.0f * blend + 10.0f)) * 0.5f;
            break;
        }

        default: break;
    }

    return min + (max - min) * blend;
}

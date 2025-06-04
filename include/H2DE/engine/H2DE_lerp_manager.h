#ifndef H2DE_LERP_MANAGER_H
#define H2DE_LERP_MANAGER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_LerpManager {
public:
    template<typename H2DE_Lerp_T>
    static H2DE_TimelineID lerp(H2DE_Engine* engine, H2DE_Lerp_T origin, H2DE_Lerp_T destination, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void(H2DE_Lerp_T)>& update, const std::function<void()>& completed, bool pauseSensitive = true) {
        const H2DE_Lerp_T valueToAdd = destination - origin;

        return engine->createTimeline(duration, easing, [update, origin, valueToAdd](float blend) {
            if (update) {
                H2DE_Lerp_T interpolatedValue = origin + (valueToAdd * blend);
                update(interpolatedValue);
            }
        }, completed, 0, pauseSensitive);
    }

    static H2DE_TimelineID lerp(H2DE_Engine* engine, const H2DE_ColorRGB& origin, const H2DE_ColorRGB& destination, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void(H2DE_ColorRGB)>& update, const std::function<void()>& completed, bool pauseSensitive = true) {
        const uint8_t redToAdd = destination.r - origin.r;
        const uint8_t greenToAdd = destination.g - origin.g;
        const uint8_t blueToAdd = destination.b - origin.b;
        const uint8_t alphaToAdd = destination.a - origin.a;

        return engine->createTimeline(duration, easing, [update, origin, redToAdd, greenToAdd, blueToAdd, alphaToAdd](float blend) {
            if (update) {
                H2DE_ColorRGB interpolatedColor = H2DE_ColorRGB();
                interpolatedColor.r = static_cast<uint8_t>(origin.r + (redToAdd * blend));
                interpolatedColor.g = static_cast<uint8_t>(origin.g + (greenToAdd * blend));
                interpolatedColor.b = static_cast<uint8_t>(origin.b + (blueToAdd * blend));
                interpolatedColor.a = static_cast<uint8_t>(origin.a + (alphaToAdd * blend));

                update(interpolatedColor);
            }
        }, completed, 0, pauseSensitive);
    }
};

#endif

#ifndef H2DE_LERP_MANAGER_H
#define H2DE_LERP_MANAGER_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_LerpManager {
public:
    template<typename H2DE_Lerp_T>
    static unsigned int lerp(H2DE_Engine* engine, H2DE_Lerp_T origin, H2DE_Lerp_T destination, unsigned int duration, H2DE_Easing easing, const std::function<void(H2DE_Lerp_T)>& update, const std::function<void()>& completed, bool pauseSensitive) {
        const H2DE_Lerp_T valueToAdd = destination - origin;

        return engine->createTimeline(duration, easing, [update, origin, valueToAdd](float blend) {
            if (update) {
                H2DE_Lerp_T interpolatedValue = origin + (valueToAdd * blend);
                update(interpolatedValue);
            }
        }, completed, 0, pauseSensitive);
    }

    static unsigned int lerp(H2DE_Engine* engine, const H2DE_ColorRGB& origin, const H2DE_ColorRGB& destination, unsigned int duration, H2DE_Easing easing, const std::function<void(H2DE_ColorRGB)>& update, const std::function<void()>& completed, bool pauseSensitive) {
        const Uint8 redToAdd = destination.r - origin.r;
        const Uint8 greenToAdd = destination.g - origin.g;
        const Uint8 blueToAdd = destination.b - origin.b;
        const Uint8 alphaToAdd = destination.a - origin.a;

        return engine->createTimeline(duration, easing, [update, origin, redToAdd, greenToAdd, blueToAdd, alphaToAdd](float blend) {
            if (update) {
                H2DE_ColorRGB interpolatedColor = H2DE_ColorRGB();
                interpolatedColor.r = static_cast<Uint8>(origin.r + (redToAdd * blend));
                interpolatedColor.g = static_cast<Uint8>(origin.g + (greenToAdd * blend));
                interpolatedColor.b = static_cast<Uint8>(origin.b + (blueToAdd * blend));
                interpolatedColor.a = static_cast<Uint8>(origin.a + (alphaToAdd * blend));

                update(interpolatedColor);
            }
        }, completed, 0, pauseSensitive);
    }
};

#endif

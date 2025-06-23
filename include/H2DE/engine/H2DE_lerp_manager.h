#ifndef H2DE_LERP_MANAGER_H
#define H2DE_LERP_MANAGER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_LerpManager {
public:
    static H2DE_TimelineID lerp(H2DE_Engine* engine, const H2DE_ColorRGB& origin, const H2DE_ColorRGB& destination, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void(H2DE_ColorRGB)>& update, const std::function<void()>& completed, bool pauseSensitive = true) {
        const int redToAdd = destination.r - origin.r;
        const int greenToAdd = destination.g - origin.g;
        const int blueToAdd = destination.b - origin.b;
        const int alphaToAdd = destination.a - origin.a;

        return engine->createTimeline(duration, easing, [update, origin, redToAdd, greenToAdd, blueToAdd, alphaToAdd](float blend) {
            if (update) {
                H2DE_ColorRGB interpolatedColor = H2DE_ColorRGB();
                interpolatedColor.r = static_cast<uint8_t>(H2DE::round(origin.r + (redToAdd * blend)));
                interpolatedColor.g = static_cast<uint8_t>(H2DE::round(origin.g + (greenToAdd * blend)));
                interpolatedColor.b = static_cast<uint8_t>(H2DE::round(origin.b + (blueToAdd * blend)));
                interpolatedColor.a = static_cast<uint8_t>(H2DE::round(origin.a + (alphaToAdd * blend)));

                update(interpolatedColor);
            }
        }, completed, 0, pauseSensitive);
    }

    static H2DE_TimelineID lerp(H2DE_Engine* engine, const H2DE_Time& origin, const H2DE_Time& destination, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void(H2DE_Time)>& update, const std::function<void()>& completed, bool pauseSensitive = true) {
        const int hrToAdd = destination.hours - origin.hours;
        const int minToAdd = destination.minutes - origin.minutes;
        const int secToAdd = destination.seconds - origin.seconds;
        const int msToAdd = destination.milliseconds - origin.milliseconds;

        return engine->createTimeline(duration, easing, [update, origin, hrToAdd, minToAdd, secToAdd, msToAdd](float blend) {
            H2DE_Time interpolatedTime = H2DE_Time();
            interpolatedTime.hours = static_cast<uint8_t>(H2DE::round(origin.hours + (hrToAdd * blend)));
            interpolatedTime.minutes = static_cast<uint8_t>(H2DE::round(origin.minutes + (minToAdd * blend)));
            interpolatedTime.seconds = static_cast<uint8_t>(H2DE::round(origin.seconds + (secToAdd * blend)));
            interpolatedTime.milliseconds = static_cast<uint16_t>(H2DE::round(origin.milliseconds + (msToAdd * blend)));

            update(interpolatedTime);

        }, completed, 0, pauseSensitive);
    }

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
};

#endif

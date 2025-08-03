#pragma once

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

template<typename H2DE_Lerp_T>
concept HasNormalLerp = (!std::same_as<H2DE_Lerp_T, H2DE_ColorRGB> && !std::same_as<H2DE_Lerp_T, H2DE_Time>);

class H2DE_LerpManager {
public:
    static H2DE_Timeline* lerp(H2DE_Engine* engine, const H2DE_ColorRGB& origin, const H2DE_ColorRGB& destination, uint32_t duration, H2DE_Easing easing, const std::function<void(H2DE_ColorRGB)>& update, const std::function<void()>& completed, bool pauseSensitive);
    static H2DE_Timeline* lerp(H2DE_Engine* engine, const H2DE_Time& origin, const H2DE_Time& destination, uint32_t duration, H2DE_Easing easing, const std::function<void(H2DE_Time)>& update, const std::function<void()>& completed, bool pauseSensitive);

    template<typename H2DE_Lerp_T>
    requires (HasNormalLerp<H2DE_Lerp_T>)
    static H2DE_Timeline* lerp(H2DE_Engine* engine, H2DE_Lerp_T origin, H2DE_Lerp_T destination, uint32_t duration, H2DE_Easing easing, const std::function<void(H2DE_Lerp_T)>& update, const std::function<void()>& completed, bool pauseSensitive) {
        const H2DE_Lerp_T valueToAdd = destination - origin;

        return engine->createTimeline(duration, easing, [update, origin, valueToAdd](float blend) {
            if (update) {
                H2DE_Lerp_T interpolatedValue = origin + (valueToAdd * blend);
                update(interpolatedValue);
            }
        }, completed, 0, pauseSensitive);
    }
};

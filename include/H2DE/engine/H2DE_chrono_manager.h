#pragma once

#include <H2DE/engine/H2DE_engine.h>
#include <H2DE/engine/H2DE_chrono.h>
class H2DE_Engine;

class H2DE_ChronoManager {
private:
    H2DE_Engine* engine;

    std::vector<H2DE_Chrono*> chronos;

    H2DE_ChronoManager(H2DE_Engine* engine);
    ~H2DE_ChronoManager();

    void destroyChrono(H2DE_Chrono* chrono);

    void update();

    H2DE_Chrono* create(const H2DE_Time& start, bool increasing = true, bool pauseSensitive = true);

    static H2DE_Time elapsedToTime(float elapsed) noexcept;
    static constexpr double getElapsed(const H2DE_Time& time) noexcept {
        return (time.hours * 3600.0) + (time.minutes * 60.0) + (time.seconds) + (time.milliseconds * 0.001);
    }

    friend class H2DE_Engine;
    friend class H2DE_Chrono;
};

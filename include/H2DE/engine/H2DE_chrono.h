#ifndef H2DE_CHRONO_H
#define H2DE_CHRONO_H

#include <H2DE/utils/H2DE_utils.h>
class H2DE_Engine;
class H2DE_ChronoManager;

class H2DE_Chrono {
public:
    inline void pause() noexcept { paused = true; }
    inline void resume() noexcept { paused = false; }
    inline void togglePause() noexcept { paused = !paused; }
    inline void reset() noexcept { current = H2DE_Time(); }
    void stop() noexcept;

    void onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once = true);

    constexpr H2DE_Time getTime() const noexcept { return current; }
    constexpr uint8_t getHours() const noexcept { return current.hours; }
    constexpr uint8_t getMinutes() const noexcept { return current.minutes; }
    constexpr uint8_t getSeconds() const noexcept { return current.seconds; }
    constexpr uint16_t getMilliseconds() const noexcept { return current.milliseconds; }
    constexpr bool isPaused() const noexcept { return paused; }
    constexpr bool isIncreasing() const noexcept { return increasing; }
    constexpr bool isPauseSensitive() const noexcept { return pauseSensitive; }

    inline void setTime(const H2DE_Time& time) noexcept { current = time; }
    inline void setHours(uint8_t hours) noexcept { current.hours = hours; }
    inline void setMinutes(uint8_t minutes) noexcept { current.minutes = minutes; }
    inline void setSeconds(uint8_t seconds) noexcept { current.seconds = seconds; }
    inline void setMilliseconds(uint16_t milliseconds) noexcept { current.milliseconds = milliseconds; }
    inline void setSetIncreasing(bool increasing) noexcept { this->increasing = increasing; }
    inline void setPauseSensitive(bool pauseSensitive) noexcept { this->pauseSensitive = pauseSensitive; }

    friend class H2DE_ChronoManager;

private:
    struct H2DE_OnReachEvent {
        H2DE_Time target = H2DE_Time();
        std::function<void()> callback = nullptr;
        bool once = true;
    };

private:
    H2DE_Engine* engine;
    H2DE_ChronoManager* manager;

    H2DE_Time current = H2DE_Time();
    bool increasing = true;
    bool pauseSensitive = true;
    bool paused = false;

    std::vector<H2DE_OnReachEvent> onReachEvents = {};

    H2DE_Chrono(H2DE_Engine* engine, H2DE_ChronoManager* manager, const H2DE_Time& start, bool increasing = true, bool pauseSensitive = true);
    ~H2DE_Chrono() noexcept = default;

    void update();
    void updateCurrentTime();
    void updateOnReachEvents();

    // H2DE_TimelineID setHours(uint8_t hours, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    // H2DE_TimelineID setMinutes(uint8_t minutes, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    // H2DE_TimelineID setSeconds(uint8_t seconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    // H2DE_TimelineID setMilliseconds(uint16_t milliseconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
};


#endif

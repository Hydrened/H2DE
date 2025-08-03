#pragma once

/**
 * @file H2DE_chrono.h
 * @brief Chrono system used for time tracking and scheduled callbacks.
 * 
 * Provides a system to measure elapsed time in hours, minutes, seconds,
 * and milliseconds. Allows triggering actions when a specific time is reached.
 * Includes support for pause/resume, direction (increasing/decreasing), and
 * pause sensitivity.
 */

#include <H2DE/utils/H2DE_utils.h>
class H2DE_Engine;
class H2DE_ChronoManager;

/**
 * @class H2DE_Chrono
 * @brief Tracks elapsed time and triggers callbacks when specific times are reached.
 * 
 * The H2DE_Chrono class is designed to measure elapsed time during gameplay
 * or internal engine operations. It can:
 * 
 * - Be paused/resumed/reset manually.
 * - Track time with `H2DE_Time` precision.
 * - Trigger callbacks when the chrono reaches specific time values (via `onReach`).
 * - Be set to increase or decrease over time.
 * - Respect or ignore global pause states, if desired.
 * 
 * Chronos are managed by the `H2DE_ChronoManager`, and can be destroyed automatically
 * via `stop()` or manually through the manager.
 */
class H2DE_Chrono {
public:
    /**
     * @brief Pauses the chrono.
     */
    inline void pause() noexcept {
        paused = true;
    }
    /**
     * @brief Resumes the chrono.
     */
    inline void resume() noexcept {
        paused = false;
    }
    /**
     * @brief Toggles the pause state.
     */
    inline void togglePause() noexcept {
        paused = !paused;
    }
    /**
     * @brief Resets the current time to 0.
     */
    inline void reset() noexcept {
        current = H2DE_Time();
    }
    /**
     * @brief Stops and destroys this chrono instance via the manager.
     */
    void stop() noexcept;

    /**
     * @brief Triggers a callback when the chrono reaches the given time.
     * 
     * The callback will be executed once or repeatedly, depending on the `once` flag.
     * 
     * @param target Time to reach (H2DE_Time).
     * @param callback Function to call when time is reached.
     * @param once If true, callback is triggered once. Otherwise, it's triggered every time it's matched.
     */
    void onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once = true);

    /**
     * @brief Gets the current time of the chrono.
     * 
     * @return A copy of the current H2DE_Time object.
     */
    constexpr H2DE_Time getTime() const noexcept {
        return current;
    }
    /**
     * @brief Gets the current number of hours.
     * 
     * @return Hours component (0–255).
     */
    constexpr uint8_t getHours() const noexcept {
        return current.hours;
    }
    /**
     * @brief Gets the current number of minutes.
     * 
     * @return Minutes component (0–59).
     */
    constexpr uint8_t getMinutes() const noexcept {
        return current.minutes;
    }
    /**
     * @brief Gets the current number of seconds.
     * 
     * @return Seconds component (0–59).
     */
    constexpr uint8_t getSeconds() const noexcept {
        return current.seconds;
    }
    /**
     * @brief Gets the current number of milliseconds.
     * 
     * @return Milliseconds component (0–999).
     */
    constexpr uint16_t getMilliseconds() const noexcept {
        return current.milliseconds;
    }
    /**
     * @brief Checks if the chrono is paused.
     */
    constexpr bool isPaused() const noexcept {
        return paused;
    }
    /**
     * @brief Checks if the chrono is increasing (vs decreasing).
     */
    constexpr bool isIncreasing() const noexcept {
        return increasing;
    }
    /**
     * @brief Checks if the chrono is affected by engine pause.
     */
    constexpr bool isPauseSensitive() const noexcept {
        return pauseSensitive;
    }

    /**
     * @brief Sets the current time of the chrono.
     * 
     * @param time The new time to assign.
     */
    inline void setTime(const H2DE_Time& time) noexcept {
        current = time;
    }
    /**
     * @brief Sets the hours component of the current time.
     * 
     * @param hours New hour value (0–255).
     */
    inline void setHours(uint8_t hours) noexcept {
        current.hours = hours;
    }
    /**
     * @brief Sets the minutes component of the current time.
     * 
     * @param minutes New minute value (0–59).
     */
    inline void setMinutes(uint8_t minutes) noexcept {
        current.minutes = minutes;
    }
    /**
     * @brief Sets the seconds component of the current time.
     * 
     * @param seconds New second value (0–59).
     */
    inline void setSeconds(uint8_t seconds) noexcept {
        current.seconds = seconds;
    }
    /**
     * @brief Sets the milliseconds component of the current time.
     * 
     * @param milliseconds New millisecond value (0–999).
     */
    inline void setMilliseconds(uint16_t milliseconds) noexcept {
        current.milliseconds = milliseconds;
    }
    /**
     * @brief Sets whether the chrono should count up or down.
     * 
     * @param increasing If true, the chrono increases over time. If false, it decreases.
     */
    inline void setSetIncreasing(bool increasing) noexcept {
        this->increasing = increasing;
    }
    /**
     * @brief Sets whether the chrono is affected by engine pause.
     * 
     * @param pauseSensitive If true, the chrono pauses when the engine is paused.
     */
    inline void setPauseSensitive(bool pauseSensitive) noexcept {
        this->pauseSensitive = pauseSensitive;
    }

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
};

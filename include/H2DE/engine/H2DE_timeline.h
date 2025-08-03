#pragma once

/**
 * @file H2DE_timeline.h
 * @brief Timeline animation system used for handling interpolated updates over time.
 * 
 * Provides a high-level system to create and manage animations or value interpolations
 * over a specified duration. Includes support for looping, delays, easing types,
 * pause/resume, and callbacks for updates and completion.
 */

#include <H2DE/utils/H2DE_utils.h>
class H2DE_Engine;
class H2DE_TimelineManager;

/**
 * @class H2DE_Timeline
 * @brief Represents a time-based animation or value interpolation.
 * 
 * The H2DE_Timeline class manages time-based progression from 0.0 to 1.0,
 * invoking update callbacks at each tick. It supports features such as:
 * 
 * - Duration and delay configuration.
 * - Looping (with optional repeat count or infinite loop).
 * - Easing types (see H2DE_Easing).
 * - Manual control (pause, resume, reset, stop).
 * - Callbacks for update and completion.
 * 
 * Timelines are updated by the H2DE_TimelineManager and are destroyed
 * automatically when completed or manually stopped.
 */
class H2DE_Timeline {
public:
    /**
     * @brief Pauses the timeline.
     * 
     * After calling this function, the timeline will no longer update
     * until `resume()` is called.
     */
    inline void pause() noexcept {
        paused = true;
    }
    /**
     * @brief Resumes the timeline if it was paused.
     * 
     * Calling this function allows the timeline to continue updating.
     */
    inline void resume() noexcept {
        paused = false;
    }
    /**
     * @brief Toggles the pause state of the timeline.
     * 
     * If the timeline is paused, it will resume. If it's running, it will be paused.
     */
    inline void togglePause() noexcept {
        paused = !paused;
    }
    /**
     * @brief Resets the timeline to its initial state.
     *  
     * This sets the current time and loop count to zero,
     * but does not pause or stop the timeline.
     */
    void reset() noexcept;
    /**
     * @brief Stops and destroys the timeline.
     * 
     * This will remove the timeline from the manager and delete it.
     * Optionally, it can trigger the final update and completed callbacks.
     * 
     * @param callCompleted Whether to call the update (with 1.0f) and completed callbacks before deletion.
     */
    void stop(bool callCompleted) noexcept;

    /**
     * @brief Checks if the timeline is currently paused.
     * 
     * @return true if the timeline is paused, false otherwise.
     */
    constexpr bool isPaused() const noexcept {
        return paused;
    }
    
private:
    H2DE_Engine* engine;
    H2DE_TimelineManager* manager;

    float current = 0.0f;
    float duration = 0.0f;
    uint32_t loops = 0;
    uint32_t currentLoop = 0;
    H2DE_Easing easing = H2DE_EASING_LINEAR;
    std::function<void(float)> updateCall = nullptr;
    std::function<void()> completedCall = nullptr;
    bool pauseSensitive = true;
    bool paused = false;

    H2DE_Timeline(H2DE_Engine* engine, H2DE_TimelineManager* manager, uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);
    ~H2DE_Timeline() noexcept = default;

    bool update();

    friend class H2DE_TimelineManager;
};

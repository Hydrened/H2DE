#pragma once

/**
 * @file H2DE_timer_object.h
 * @brief Definition of the H2DE_TimerObject class for handling timer display and animation.
 * 
 * This file contains the declaration of the H2DE_TimerObject class, which extends the basic
 * object class (H2DE_Object) with timer-specific functionality. It supports managing
 * time components (hours, minutes, seconds, milliseconds), surfaces for graphical
 * representation, and animations through timelines with easing.
 * 
 * The timer can be paused, resumed, reset, and animated smoothly. It also links to a
 * text object for rendering the current time.
 */

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;
class H2DE_Chrono;

/**
 * @class H2DE_TimerObject
 * @brief A specialized object to manage, display and animate a timer in the H2DE engine.
 * 
 * H2DE_TimerObject inherits from H2DE_Object and adds timer-specific data and functionality.
 * It manages hours, minutes, seconds, and milliseconds with direct setters and getters.
 * 
 * Key features:
 * - Add, remove and retrieve graphical surfaces representing the timer (textures, sprites, colors).
 * - Pause, resume, toggle pause state and reset the timer.
 * - Animate timer changes smoothly using timelines and easing functions.
 * - Link to a text object to render the current time as a string.
 * 
 * This class is suitable for UI elements, gameplay timers, or any time-based visual feedback
 * within games or applications built on H2DE.
 */
class H2DE_TimerObject : public H2DE_Object {
public:
    /**
     * @brief Add a surface to the object.
     * 
     * Adds a new surface (texture, sprite, or color) to the object.
     * The surface is stored and identified by a unique name.
     * 
     * @tparam H2DE_Surface_T The type of surface to add (e.g. Texture, Sprite, Color).
     * @param name The unique name to associate with the surface.
     * @param surfaceData Common parameters such as position, size, etc.
     * @param specificData Specific data for the surface type.
     * @return A pointer to the created surface, or nullptr if it failed.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(surfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a surface from the object.
     * 
     * Removes a surface previously added by its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was found and removed, false otherwise.
     */
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::removeSurface(surfaces, name);
    }

    /**
     * @brief Registers a callback to be triggered when the internal timer reaches a specific time.
     * 
     * The callback will be invoked once the elapsed time matches the specified `target` time.
     * 
     * @param target The H2DE_Time at which the callback should be triggered.
     * @param callback The function to execute when the target time is reached.
     * @param once If true, the callback is called only once and removed after execution. 
     *             If false, the callback remains and is triggered every time the timer hits the target.
     */
    void onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once);

    /**
     * @brief Reset the timer to zero.
     * 
     * Sets all time components (hours, minutes, seconds, milliseconds) back to 0.
     */
    inline void reset() noexcept { 
        timerObjectData.time = { 0, 0, 0, 0 };
    }
    /**
     * @brief Pause the timer.
     * 
     * Stops the timer from running, freezing its current time.
     */
    void pause();
    /**
     * @brief Resume the timer if it was paused.
     * 
     * Restarts the timer from its paused state.
     */
    void resume();
    /**
     * @brief Toggle the timer's paused state.
     * 
     * If the timer is running, pauses it; if paused, resumes it.
     */
    void togglePause();
    /**
     * @brief Check if the timer is currently paused.
     * 
     * @return true if the timer is paused, false otherwise.
     */
    bool isPaused() const; 

    /**
     * @brief Get the full timer data structure.
     * 
     * Returns a copy of the `H2DE_TimerObjectData` containing all timer-related info.
     * 
     * @return The complete timer data.
     */
    inline H2DE_TimerObjectData getTimerData() const {
        return timerObjectData;
    }
    /**
     * @brief Get the current time of the timer.
     * 
     * Returns the `H2DE_Time` struct holding hours, minutes, seconds, and milliseconds.
     * 
     * @return The current time.
     */
    constexpr H2DE_Time getTime() const noexcept {
        return timerObjectData.time; 
    }
    /**
     * @brief Get the hours component of the timer.
     * 
     * @return The hours as an 8-bit unsigned integer.
     */
    constexpr uint8_t getHours() const noexcept {
        return timerObjectData.time.hours;
    }
    /**
     * @brief Get the minutes component of the timer.
     * 
     * @return The minutes as an 8-bit unsigned integer.
     */
    constexpr uint8_t getMinutes() const noexcept {
        return timerObjectData.time.minutes;
    }
    /**
     * @brief Get the seconds component of the timer.
     * 
     * @return The seconds as an 8-bit unsigned integer.
     */
    constexpr uint8_t getSeconds() const noexcept {
        return timerObjectData.time.seconds;
    }
    /**
     * @brief Get the milliseconds component of the timer.
     * 
     * @return The milliseconds as a 16-bit unsigned integer.
     */
    constexpr uint16_t getMilliseconds() const noexcept {
        return timerObjectData.time.milliseconds;
    }
    /**
     * @brief Checks if the timer is currently increasing.
     * 
     * @return true if the timer is increasing, false if it is decreasing.
     */
    constexpr bool isIncreasing() const noexcept {
        return timerObjectData.increasing;
    }
    /**
     * @brief Check if the timer is sensitive to the game's pause state.
     * 
     * Returns true if the timer is paused when the engine is paused,
     * false otherwise.
     * 
     * @return true if pause sensitive, false if not.
     */
    constexpr bool isPauseSensitive() const noexcept {
        return timerObjectData.pauseSensitive;
    }

    /**
     * @brief Get all surfaces of the object.
     * 
     * Returns a map of all currently attached surfaces. Each entry maps the surface name
     * to its base `H2DE_Surface` pointer.
     * 
     * @return A map of surface names to their respective surface pointers.
     */
    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const noexcept {
        return surfaces; 
    }
    /**
     * @brief Get a surface by name and cast it to a specific type.
     * 
     * Retrieves a surface of a specific type from the object. If the name is not found or
     * the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface cast to the specified type, or nullptr.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const {
        return H2DE_Object::getSurface<H2DE_Surface_T>(surfaces, name);
    }
    /**
     * @brief Get the text object associated with the timer.
     * 
     * Returns a pointer to the `H2DE_TextObject` used for rendering
     * timer's current time
     * 
     * @return Pointer to the timer's text object, or nullptr if none.
     */
    inline H2DE_TextObject* getTextObject() const noexcept {
        return textObject; 
    }
    
    /**
     * @brief Set the full time immediately.
     * 
     * Updates the timer to the given `H2DE_Time` value without animation.
     * 
     * @param time The new time to set.
     */
    void setTime(const H2DE_Time& time);
    /**
     * @brief Set the hours component immediately.
     * 
     * Updates only the hours of the timer without animation.
     * 
     * @param hours The new hours value (0-23).
     */
    void setHours(uint8_t hours);
    /**
     * @brief Set the minutes component immediately.
     * 
     * Updates only the minutes of the timer without animation.
     * 
     * @param minutes The new minutes value (0-59).
     */
    void setMinutes(uint8_t minutes);
    /**
     * @brief Set the seconds component immediately.
     * 
     * Updates only the seconds of the timer without animation.
     * 
     * @param seconds The new seconds value (0-59).
     */
    void setSeconds(uint8_t seconds);
    /**
     * @brief Set the milliseconds component immediately.
     * 
     * Updates only the milliseconds of the timer without animation.
     * 
     * @param milliseconds The new milliseconds value (0-999).
     */
    void setMilliseconds(uint16_t milliseconds);
    /**
     * @brief Sets whether the timer is increasing or decreasing.
     * 
     * @param increasing True if the timer should count up, false to count down.
     */
    void setIncreasing(bool increasing);
    /**
     * @brief Set whether the timer is sensitive to the engine's pause state.
     * 
     * When set to true, the timer wil be paused if the engine is paused.
     * 
     * @param pauseSensitive True to make the timer sensitive to pause, false otherwise.
     */
    void setPauseSensitive(bool pauseSensitive);
    
    /**
     * @brief Animate the full time change over a duration.
     * 
     * Smoothly transitions the timer from current time to the target `H2DE_Time` using timeline animation.
     * 
     * @param time The target time value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setTime(const H2DE_Time& time, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the hours change over a duration.
     * 
     * Smoothly animates hours from current to target value.
     * 
     * @param hours Target hours value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setHours(uint8_t hours, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the minutes change over a duration.
     * 
     * Smoothly animates minutes from current to target value.
     * 
     * @param minutes Target minutes value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setMinutes(uint8_t minutes, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the seconds change over a duration.
     * 
     * Smoothly animates seconds from current to target value.
     * 
     * @param seconds Target seconds value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setSeconds(uint8_t seconds, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the milliseconds change over a duration.
     * 
     * Smoothly animates milliseconds from current to target value.
     * 
     * @param milliseconds Target milliseconds value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setMilliseconds(uint16_t milliseconds, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TimerObjectData;
    
    friend class H2DE_Engine;

private:
    H2DE_TimerObjectData timerObjectData;

    H2DE_TextObject* textObject = nullptr;
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    H2DE_Chrono* chrono = nullptr;

    H2DE_TimerObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TimerObjectData& timerObjectData);
    ~H2DE_TimerObject() override;

    void update() override;

    void initChrono();
    void destroyChrono();

    void refreshTextObject();
    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

    static std::string intToStr(int value, int nbDigits);
    const std::string getStringifiedTime() const;
};

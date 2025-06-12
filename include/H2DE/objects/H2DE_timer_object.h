#ifndef H2DE_TIMER_OBJECT_H
#define H2DE_TIMER_OBJECT_H

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

class H2DE_TimerObject : public H2DE_Object {
public:
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(surfaces, name, surfaceData, specificData);
    }
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::removeSurface(surfaces, name);
    }

    inline void reset() { timerObjectData.time = { 0, 0, 0, 0 }; }
    void pause();
    void resume();
    void togglePause();
    bool isPaused() const; 

    inline H2DE_TimerObjectData getTimerData() const { return timerObjectData; }
    constexpr H2DE_Time getTime() const { return timerObjectData.time; }
    constexpr uint8_t getHours() { return timerObjectData.time.hours; }
    constexpr uint8_t getMinutes() { return timerObjectData.time.minutes; }
    constexpr uint8_t getSeconds() { return timerObjectData.time.seconds; }
    constexpr uint16_t getMilliseconds() { return timerObjectData.time.milliseconds; }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const { return surfaces; }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const { return H2DE_Object::getSurface<H2DE_Surface_T>(surfaces, name); }
    inline H2DE_TextObject* getTextObject() const { return textObject; }
    
    void setTime(const H2DE_Time& time);
    void setHours(uint8_t hours);
    void setMinutes(uint8_t minutes);
    void setSeconds(uint8_t seconds);
    void setMilliseconds(uint16_t milliseconds);

    H2DE_TimelineID setTime(const H2DE_Time& time, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setHours(uint8_t hours, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setMinutes(uint8_t minutes, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setSeconds(uint8_t seconds, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setMilliseconds(uint16_t milliseconds, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TimerObjectData;
    
    friend class H2DE_Engine;

private:
    H2DE_TimerObjectData timerObjectData;

    H2DE_TextObject* textObject = nullptr;
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    H2DE_TimelineID timelineId = 0;
    float elapsed = 0.0f;

    H2DE_TimerObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TimerObjectData& timerObjectData);
    ~H2DE_TimerObject() override;

    void initElapsedTime();
    void initTimeline();
    void destroyTimeline();

    void refreshTextObject();
    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

    static H2DE_Time elapsedToTime(float elapsed);
    static std::string intToStr(int value, int nbDigits);
    const std::string getStringifiedTime() const;
};

#endif

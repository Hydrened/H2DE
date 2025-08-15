#include "H2DE/objects/H2DE_timer_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TimerObject::H2DE_TimerObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TimerObjectData& tod) : H2DE_Object(e, od), _timerObjectData(tod) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
    _initChrono();
}

void H2DE_TimerObject::_initChrono() {
    _chrono = _engine->createChrono(_timerObjectData.time, _timerObjectData.increasing, _timerObjectData.pauseSensitive);
}

// CLEANUP
H2DE_TimerObject::~H2DE_TimerObject() {
    _destroyChrono();

    if (_textObject != nullptr) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = nullptr;
        }
    }

    H2DE_Object::_destroySurfaces(_surfaces);
}

void H2DE_TimerObject::_destroyChrono() {
    _chrono->stop();
}

// UPDATE
void H2DE_TimerObject::_update() {
    _timerObjectData.time = _chrono->getTime();
    _textObject->setText(_getStringifiedTime());
}

// ACTIONS
void H2DE_TimerObject::_refreshTextObject() {
    if (_textObject != nullptr) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = nullptr;
        }
    }

    H2DE_ObjectData od = _objectData;
    od.index++;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    _timerObjectData.text.text = _getStringifiedTime();
    tod.text = _timerObjectData.text;

    _textObject = _engine->createObject<H2DE_TextObject>(od, tod);
}

void H2DE_TimerObject::_refreshSurfaceBuffers() {
    _refreshTextObject();

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::_getSortedSurfaces(_surfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    _rescaleSurfaceBuffers();
}

void H2DE_TimerObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius(_surfaces);
    
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_TimerObject::onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once) {
    _chrono->onReach(target, callback, once);
}

void H2DE_TimerObject::pause() {
    _chrono->pause();
}

void H2DE_TimerObject::resume() {
    _chrono->resume();
}

void H2DE_TimerObject::togglePause() {
    _chrono->togglePause();
}

// GETTER
std::string H2DE_TimerObject::_intToStr(int value, int nbDigits) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(nbDigits) << value;
    return oss.str();
}

const std::string H2DE_TimerObject::_getStringifiedTime() const {
    std::string res = "";

    if (_timerObjectData.displayHours) {
        res += std::to_string(_timerObjectData.time.hours) + _timerObjectData.separator;
    }

    if (_timerObjectData.displayMinutes) {
        res += H2DE_TimerObject::_intToStr(_timerObjectData.time.minutes, 2) + _timerObjectData.separator;
    }

    if (_timerObjectData.displaySeconds) {
        res += H2DE_TimerObject::_intToStr(_timerObjectData.time.seconds, 2) + _timerObjectData.separator;
    }

    if (_timerObjectData.displayMilliseconds) {
        res += H2DE_TimerObject::_intToStr(_timerObjectData.time.milliseconds, 3) + _timerObjectData.separator;
    }

    res.pop_back();
    return res;
}

bool H2DE_TimerObject::isPaused() const {
    return _chrono->isPaused();
}

// SETTER

// -- non lerp
void H2DE_TimerObject::setTime(const H2DE_Time& time) {
    _timerObjectData.time = time;
    _chrono->setTime(time);
    _refreshSurfaceBuffers();
}

void H2DE_TimerObject::setHours(uint8_t hours) {
    _timerObjectData.time.hours = hours;
    _chrono->setHours(hours);
    _refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMinutes(uint8_t minutes) {
    _timerObjectData.time.minutes = minutes;
    _chrono->setMinutes(minutes);
    _refreshSurfaceBuffers();
}

void H2DE_TimerObject::setSeconds(uint8_t seconds) {
    _timerObjectData.time.seconds = seconds;
    _chrono->setSeconds(seconds);
    _refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMilliseconds(uint16_t milliseconds) {
    _timerObjectData.time.milliseconds = milliseconds;
    _chrono->setMilliseconds(milliseconds);
    _refreshSurfaceBuffers();
}

void H2DE_TimerObject::setIncreasing(bool increasing) {
    _timerObjectData.increasing = increasing;
    _chrono->setSetIncreasing(increasing);
}

void H2DE_TimerObject::setPauseSensitive(bool pauseSensitive) {
    _timerObjectData.pauseSensitive = pauseSensitive;
    _chrono->setPauseSensitive(pauseSensitive);
}

// lerp
H2DE_Timeline* H2DE_TimerObject::setTime(const H2DE_Time& time, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(_engine, _timerObjectData.time, time, duration, easing, [this](H2DE_Time iv) {
        setTime(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setHours(uint8_t hours, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(_engine, _timerObjectData.time.hours, hours, duration, easing, [this](uint8_t iv) {
        setHours(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setMinutes(uint8_t minutes, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(_engine, _timerObjectData.time.minutes, minutes, duration, easing, [this](uint8_t iv) {
        setMinutes(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setSeconds(uint8_t seconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(_engine, _timerObjectData.time.seconds, seconds, duration, easing, [this](uint8_t iv) {
        setSeconds(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setMilliseconds(uint16_t milliseconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint16_t>(_engine, _timerObjectData.time.milliseconds, milliseconds, duration, easing, [this](uint8_t iv) {
        setMilliseconds(iv);
    }, completed, pauseSensitive);

    _addTimelineToTimelines(timeline);
    return timeline;
}

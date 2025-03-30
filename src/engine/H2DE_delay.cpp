#include "H2DE/H2DE_delay.h"

// INIT
H2DE_Engine::H2DE_Delay::H2DE_Delay(H2DE_Engine* e) : engine(e) {

}

// CLEANUP
H2DE_Engine::H2DE_Delay::~H2DE_Delay() {
    delays.clear();
}

// UPDATE
void H2DE_Engine::H2DE_Delay::update() {
    for (auto it = delays.begin(); it != delays.end(); ) {
        auto& [delayId, delay] = *it;
    
        if (delay.pauseSensitive && H2DE_IsPaused(engine)) {
            ++it;
            continue;
        }
    
        delay.current++;
    
        if (delay.current >= delay.max) {
            if (delay.callback) {
                delay.callback();
            }

            it = delays.erase(it);

        } else {
            ++it;
        }
    }
}

// EVENTS
unsigned int H2DE_Delay(const H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, bool pauseSensitive) {
    H2DE_Engine::H2DE_Delay::SpecificDelay delay = H2DE_Engine::H2DE_Delay::SpecificDelay();
    delay.max = (ms == 0) ? 1 : engine->fps / (1000.0f / ms);
    delay.callback = callback;
    delay.pauseSensitive = pauseSensitive;

    engine->delay->id++;
    engine->delay->delays[engine->delay->id] = delay;

    return engine->delay->id;
}

void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id) {
    auto it = engine->delay->delays.find(id);

    if (it != engine->delay->delays.end()) {
        it->second.current = 0;
    }
}

void H2DE_StopDelay(const H2DE_Engine* engine, unsigned int id, bool call) {
    auto it = engine->delay->delays.find(id);

    if (it != engine->delay->delays.end()) {
        if (call && it->second.callback) {
            it->second.callback();
        }
        
        engine->delay->delays.erase(it);
    }
}

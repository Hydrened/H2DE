#include "H2DE/H2DE_delay_manager.h"

// INIT
H2DE_Engine::H2DE_DelayManager::H2DE_DelayManager(H2DE_Engine* e) : engine(e) {

}

// CLEANUP
H2DE_Engine::H2DE_DelayManager::~H2DE_DelayManager() {
    delays.clear();
}

// UPDATE
void H2DE_Engine::H2DE_DelayManager::update() {
    for (auto& [delayId, delay] : delays) {
        if ((delay.pauseSensitive && H2DE_IsPaused(engine)) || delay.finished) {
            continue;
        }

        delay.current++;

        if (delay.current >= delay.max) {
            if (delay.loop >= 0) {
                delay.currentLoop--;

                if (delay.currentLoop < 0) {
                    delay.finished = true;
                }

            } else H2DE_ResetDelay(engine, delayId);

            if (delay.callback) {
                delay.callback();
            }
        }
    }
}

// EVENTS
unsigned int H2DE_Delay(H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, int loop, bool pauseSensitive) {
    H2DE_Engine::H2DE_DelayManager::Delay delay = H2DE_Engine::H2DE_DelayManager::Delay();
    delay.max = (ms == 0) ? 1 : engine->fps / (1000.0f / ms);
    delay.callback = callback;
    delay.loop = loop;
    delay.currentLoop = loop;
    delay.pauseSensitive = pauseSensitive;

    engine->delayManager->id++;
    engine->delayManager->delays[engine->delayManager->id] = delay;

    return engine->delayManager->id;
}

void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id) {
    auto it = engine->delayManager->delays.find(id);

    if (it != engine->delayManager->delays.end()) {
        it->second.current = 0;
        it->second.currentLoop = it->second.loop;
        it->second.finished = false;
    }
}

void H2DE_StopDelay(H2DE_Engine* engine, unsigned int id, bool call) {
    auto it = engine->delayManager->delays.find(id);

    if (it != engine->delayManager->delays.end()) {
        if (call && it->second.callback) {
            it->second.callback();
        }
        
        it->second.current = it->second.max;
        it->second.currentLoop = it->second.loop;
        it->second.finished = true;
    }
}

#ifndef H2DE_DELAY_MANAGER_H
#define H2DE_DELAY_MANAGER_H

#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_DelayManager {
private:
    H2DE_Engine* engine;

    struct Delay {
        unsigned int current = 0;
        unsigned int max;
        std::function<void()> callback;
        int loop;
        int currentLoop;
        bool pauseSensitive;
        bool finished = false;
    };

    std::unordered_map<int, Delay> delays;
    unsigned int id = 0;

public:
    H2DE_DelayManager(H2DE_Engine* engine);
    ~H2DE_DelayManager();

    void update();

    friend unsigned int H2DE_Delay(H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, int loop, bool pauseSensitive);
    friend void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id);
    friend void H2DE_StopDelay(H2DE_Engine* engine, unsigned int id, bool call);
};

#endif

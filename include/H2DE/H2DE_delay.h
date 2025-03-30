#ifndef H2DE_DELAY__H
#define H2DE_DELAY__H

#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Delay {
private:
    H2DE_Engine* engine;

    struct SpecificDelay {
        unsigned int current = 0;
        unsigned int max;
        std::function<void()> callback;
        bool pauseSensitive;
    };

    std::unordered_map<int, SpecificDelay> delays;
    unsigned int id = 0;

public:
    H2DE_Delay(H2DE_Engine* engine);
    ~H2DE_Delay();

    void update();

    friend unsigned int H2DE_Delay(const H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, bool pauseSensitive);
    friend void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id);
    friend void H2DE_StopDelay(const H2DE_Engine* engine, unsigned int id, bool call);
};

#endif

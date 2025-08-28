#pragma once

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_EventManager {
private:
    using VK = uint8_t;

    H2DE_Engine* engine;
    SDL_Event eventSDL;

    std::unordered_map<VK, bool> vkStates;
    H2DE_PixelPos oldCursorPos = H2DE_PixelPos();

    std::vector<H2DE_Event> events;

    H2DE_EventManager(H2DE_Engine* engine) noexcept;
    ~H2DE_EventManager() noexcept = default;

    void initVkStates();
    void initCursorPos();
    
    void handle();
    void handleSDL();

    void handleH2DE();
    void handleH2DE_createEventsFromVk();
    void handleH2DE_createEventsFromMouseMotion();
    void handleH2DE_createEventsFromMouseWheel(int delta);
    void handleH2DE_callback();

    static inline bool isKey(VK vk) {
        return (vk >= 0x08);
    }
    static inline bool isMouse(VK vk) {
        return (vk > 0x00 && vk != 0x03 && vk < 0x07);
    }
    static H2DE_PixelPos getCursorPos();
    static H2DE_MouseButton getH2DEMouseButton(VK vk);

    friend class H2DE_Engine;
};

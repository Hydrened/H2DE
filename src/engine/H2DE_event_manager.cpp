#include "H2DE/engine/H2DE_event_manager.h"
#include <windows.h>

// INIT
H2DE_EventManager::H2DE_EventManager(H2DE_Engine* engine) : engine(engine) {
    initVkStates();
    initCursorPos();
}

void H2DE_EventManager::initVkStates() {
    constexpr VK VK_MIN = VK_LBUTTON;
    constexpr VK VK_MAX = VK_OEM_CLEAR;
    constexpr VK VK_RANGE = VK_MAX - VK_MIN;

    vkStates.reserve(VK_RANGE);

    for (VK vk = VK_MIN; vk <= VK_MAX; vk++) {
        vkStates[vk] = ((GetAsyncKeyState(vk) & 0x8000) != 0);
    }
}

void H2DE_EventManager::initCursorPos() {
    oldCursorPos = getCursorPos();
}

// EVENTS
void H2DE_EventManager::handle() {
    events.clear();

    handleSDL();
    handleH2DE();
}

// -- SDL
void H2DE_EventManager::handleSDL() {
    while (SDL_PollEvent(&eventSDL)) {
        switch (eventSDL.type) {
            
            case SDL_QUIT:
                engine->_isRunning = false;
                break;

            case SDL_WINDOWEVENT:
                if (eventSDL.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    engine->_window->setSize(H2DE_PixelSize{ eventSDL.window.data1, eventSDL.window.data2 });
                    engine->_camera->_updateGrid();
                }
                break;

            case SDL_MOUSEMOTION:
                engine->_mousePos = { eventSDL.button.x, eventSDL.button.y };
                break;

            case SDL_MOUSEWHEEL:
                handleH2DE_createEventsFromMouseWheel(eventSDL.wheel.y);
                break;
            
            default: break;
        }

        engine->_objectManager->handleEvents(eventSDL);
    }
}

// -- H2DE
void H2DE_EventManager::handleH2DE() {
    if (engine->_handleEventsCall) {
        handleH2DE_createEventsFromVk();
        handleH2DE_createEventsFromMouseMotion();
        handleH2DE_callback();
    }
}

void H2DE_EventManager::handleH2DE_createEventsFromVk() {
    constexpr VK VK_MIN = VK_LBUTTON;
    constexpr VK VK_MAX = VK_OEM_CLEAR;

    const H2DE_Translate mouseGameTranslate = engine->getMouseGameTranslate();
    const H2DE_Translate mouseInterfaceTranslate = engine->getMouseInterfaceTranslate();

    for (VK vk = VK_MIN; vk <= VK_MAX; vk++) {
        bool wasDown = vkStates.at(vk);
        bool isDown = ((GetAsyncKeyState(vk) & 0x8000) != 0);

        vkStates[vk] = isDown;

        bool downEvent = (!wasDown && isDown);
        bool upEvent = (wasDown && !isDown);

        H2DE_Key key = static_cast<H2DE_Key>(vk);
        H2DE_MouseButton button = H2DE_EventManager::getH2DEMouseButton(vk);

        bool isEventValid = (downEvent || upEvent);
        if (!isEventValid) {
            continue;
        }

        bool isKey = H2DE_EventManager::isKey(vk);
        bool isMouse = H2DE_EventManager::isMouse(vk);

        bool isEventTypeNull = (!isMouse && !isKey);
        if (isEventTypeNull) {
            continue;
        }

        H2DE_Event event = H2DE_Event();
        event.mouseGameTranslate = mouseGameTranslate;
        event.mouseInterfaceTranslate = mouseInterfaceTranslate;

        if (isKey) {
            event.type = (downEvent) ? H2DE_EVENT_TYPE_KEYDOWN : H2DE_EVENT_TYPE_KEYUP;
            event.key = key;

        } else {
            event.type = (downEvent) ? H2DE_EVENT_TYPE_MOUSE_BUTTON_DOWN : H2DE_EVENT_TYPE_MOUSE_BUTTON_UP;
            event.button = button;
        }

        events.push_back(event);
    }
}

void H2DE_EventManager::handleH2DE_createEventsFromMouseMotion() {
    H2DE_PixelPos cursorPos = getCursorPos();

    bool mouseHaveMoved = (cursorPos != oldCursorPos);
    if (!mouseHaveMoved) {
        return;
    }

    H2DE_Event event = H2DE_Event();
    event.type = H2DE_EVENT_TYPE_MOUSE_MOTION;
    event.mouseGameTranslate = engine->getMouseGameTranslate();
    event.mouseInterfaceTranslate = engine->getMouseInterfaceTranslate();
    events.push_back(event);

    oldCursorPos = cursorPos;
}

void H2DE_EventManager::handleH2DE_createEventsFromMouseWheel(int delta) {
    H2DE_Event event = H2DE_Event();
    event.type = H2DE_EVENT_TYPE_MOUSE_WHEEL;
    event.wheelDirection = (delta > 0) ? H2DE_WHEEL_DIRECTION_UP : H2DE_WHEEL_DIRECTION_DOWN;
    event.mouseGameTranslate = engine->getMouseGameTranslate();
    event.mouseInterfaceTranslate = engine->getMouseInterfaceTranslate();
    events.push_back(event);
}

void H2DE_EventManager::handleH2DE_callback() {
    for (const H2DE_Event& event : events) {
        engine->_handleEventsCall(event);
    }
}

// GETTER
H2DE_PixelPos H2DE_EventManager::getCursorPos() {
    POINT p;
    GetCursorPos(&p);

    return { p.x, p.y };
}

H2DE_MouseButton H2DE_EventManager::getH2DEMouseButton(VK vk) {
    VK res;

    switch (vk) {
        case VK_LBUTTON:
        case VK_RBUTTON:
        case VK_MBUTTON:
            res = vk;
            break;

        case VK_XBUTTON1:
            res = H2DE_MOUSE_BUTTON_X1;
            break;

        case VK_XBUTTON2:
            res = H2DE_MOUSE_BUTTON_X2;
            break;

        default:
            res = H2DE_MOUSE_BUTTON_NONE;
            break;
    }

    return static_cast<H2DE_MouseButton>(res);
}

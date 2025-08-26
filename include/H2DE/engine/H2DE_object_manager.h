#pragma once

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_ObjectManager {
private:
    H2DE_Engine* engine;

    std::vector<H2DE_Object*> hoverObjects;
    std::vector<H2DE_ButtonObject*> buttons;
    std::vector<H2DE_InputObject*> inputs;

    H2DE_Object* hoveredObject = H2DE_NULL_OBJECT;
    H2DE_ButtonObject* mouseDownButton = H2DE_NULL_OBJECT;
    H2DE_InputObject* focusedInput = H2DE_NULL_OBJECT;

    H2DE_Cursor oldCursor = H2DE_CURSOR_ARROW;

    H2DE_ObjectManager(H2DE_Engine* engine);
    ~H2DE_ObjectManager() noexcept = default;

    void handleEvents(SDL_Event event);

    void handleEvents_mouseDown(SDL_Event event);
    H2DE_Object* handleEvents_mouseDown_getClickedObject(SDL_Event event);
    void handleEvents_mouseDown_button(H2DE_ButtonObject* button);
    void handleEvents_mouseDown_button_mouseDown(H2DE_ButtonObject* button);
    void handleEvents_mouseDown_input(H2DE_InputObject* input);
    void handleEvents_mouseDown_input_focus(H2DE_InputObject* input);
    void handleEvents_mouseDown_input_blur(H2DE_InputObject* input);
    int handleEvents_mouseDown_input_getFocusedInputLetterIndex();
    H2DE_Translate handleEvents_mouseDown_input_getFixedMouseTranslate();

    void handleEvents_mouseUp(SDL_Event event);
    void handleEvents_mouseUp_button(SDL_Event event);
    void handleEvents_mouseUp_button_mouseUp(H2DE_ButtonObject* button);

    void handleEvents_mouseMotion();
    void handleEvents_mouseMotion_hoveringObject(H2DE_Object* object);
    void handleEvents_mouseMotion_notHoveringObject();
    H2DE_Object* handleEvents_mouseMotion_getHoveredObject();
    void handleEvents_mouseMotion_buttons(H2DE_Object* oldHoveredObject);
    void handleEvents_mouseMotion_buttons_blur(H2DE_ButtonObject* button);
    void handleEvents_mouseMotion_buttons_hover(H2DE_ButtonObject* button);

    void handleEvents_keydown_input(SDL_Event event);
    void handleEvents_keydown_input_modifyText(SDL_Event event, const std::function<void(std::string, int, unsigned char)>& tasks);
    void handleEvents_keydown_input_default(SDL_Event event);
    void handleEvents_keydown_input_delete(SDL_Event event);
    void handleEvents_keydown_input_suppr(SDL_Event event);
    void handleEvents_keydown_input_enter(SDL_Event event);
    void handleEvents_keydown_input_inlineArrow(SDL_Event event);
    void handleEvents_keydown_input_blockArrow(SDL_Event event);

    void refreshHoverObjectBuffer();
    template<typename H2DE_ObjectType>
    void refreshBuffer(std::vector<H2DE_ObjectType*>& buffer, const std::vector<H2DE_Object*>& objects);
    void refreshButtonBuffer(const std::vector<H2DE_Object*>& objects);
    void refreshInputBuffer(const std::vector<H2DE_Object*>& objects);
    template<typename H2DE_ObjectType>
    void sortBuffer(std::vector<H2DE_ObjectType*>& buffer);

    void focusInput(H2DE_InputObject* input);
    void blurInput(H2DE_InputObject* input);
    void submitInput(H2DE_InputObject* input);

    bool isMouseCollidingObject(H2DE_Object* object) const;
    bool isCursorPositionValid() const;

    static H2DE_MouseButton getH2DEButton(Uint8 sdlButton);
    static size_t getFirstSpaceIndex(const std::string& text);
    static size_t getLastSpaceIndex(const std::string& text);

    static inline bool isCtrlDown(SDL_Event event) {
        return (event.key.keysym.mod & KMOD_CTRL);
    }
    static inline bool isAltDown(SDL_Event event) {
        return (event.key.keysym.mod & KMOD_ALT);
    }
    static inline bool isASCII(char c) {
        return (static_cast<unsigned char>(c) < 127);
    }

    friend class H2DE_Engine;
    friend class H2DE_Window;
    friend class H2DE_InputObject;
};

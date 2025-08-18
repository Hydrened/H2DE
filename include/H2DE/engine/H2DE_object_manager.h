#pragma once

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;
class H2DE_ButtonObject;
class H2DE_InputObject;

class H2DE_ObjectManager {
private:
    H2DE_Engine* engine;
    std::vector<H2DE_ButtonObject*> buttons;
    std::vector<H2DE_InputObject*> inputs;

    H2DE_ButtonObject* mouseDownButton = nullptr;
    H2DE_ButtonObject* hoveredButton = nullptr;

    H2DE_InputObject* focusedInput = nullptr;
    H2DE_InputObject* hoveredInput = nullptr;

    H2DE_Cursor oldCursor = H2DE_CURSOR_ARROW;

    H2DE_ObjectManager(H2DE_Engine* engine);
    ~H2DE_ObjectManager() noexcept = default;

    void handleEvents(SDL_Event event);

    void handleEvents_buttons_mouseDown(SDL_Event event);
    H2DE_ButtonObject* handleEvents_buttons_mouseDown_getMouseDownButton(SDL_Event event);
    void handleEvents_buttons_mouseDown_mouseDownButton();

    void handleEvents_buttons_mouseUp(SDL_Event event);
    bool handleEvents_buttons_mouseUp_isOnHoveredButton(SDL_Event event);
    void handleEvents_buttons_mouseDown_mouseUpButton();

    void handleEvents_buttons_mouseMotion();
    H2DE_ButtonObject* handleEvents_buttons_mouseMotion_getHoveredButton();
    void handleEvents_buttons_mouseMotion_blurButton(H2DE_ButtonObject* button);
    void handleEvents_buttons_mouseMotion_hoverNewButton(H2DE_ButtonObject* oldHoveredButton);

    void handleEvents_inputs_mouseDown(SDL_Event event);
    bool handleEvents_inputs_mouseDown_isMouseCollidingInput();
    void handleEvents_inputs_mouseDown_blurFocusedInput();
    void handleEvents_inputs_mouseDown_focusNewInput(H2DE_InputObject* oldFocusedInput);
    int handleEvents_inputs_mouseDown_getFocusedInputLetterIndex();

    void handleEvents_inputs_mouseMotion();
    H2DE_InputObject* handleEvents_inputs_mouseMotion_getHoveredInput();

    void handleEvents_inputs_keydown(SDL_Event event);
    void handleEvents_inputs_keydown_normalCharacter(SDL_Event event);
    void handleEvents_inputs_keydown_deleteCharacter();

    template<typename H2DE_ObjectType>
    void refreshBuffer(std::vector<H2DE_ObjectType*>& buffer, const std::vector<H2DE_Object*>& objects);
    void refreshButtonBuffer(const std::vector<H2DE_Object*>& objects);
    void refreshInputBuffer(const std::vector<H2DE_Object*>& objects);

    const std::vector<H2DE_ButtonObject*> getValidButtons() const;
    const std::vector<H2DE_InputObject*> getValidInputs() const;

    static H2DE_MouseButton getH2DEButton(Uint8 sdlButton);

    bool isMouseCollidingObject(H2DE_Object* object) const;

    friend class H2DE_Engine;
    friend class H2DE_Window;
};

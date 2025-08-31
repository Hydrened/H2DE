#include "H2DE/engine/H2DE_object_manager.h"

// EVENTS

// -- mouse down
void H2DE_ObjectManager::handleEvents_mouseDown_checkbox(H2DE_CheckboxObject* checkbox) {
    bool noCheckboxClicked = (checkbox == H2DE_NULL_OBJECT);
    if (noCheckboxClicked) {
        return;
    }

    checkbox->toggleCheck();
}

// ACTIONS
void H2DE_ObjectManager::refreshCheckboxBuffer(const std::vector<H2DE_Object*>& objects) {
    refreshBuffer(checkboxes, objects);
    refreshHoverObjectBuffer();
}

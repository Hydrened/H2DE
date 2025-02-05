#include "H2DE_button.h"

// INIT
H2DE_Button::H2DE_Button(H2DE_Engine* e, H2DE_ButtonData d) : engine(e), data(d) {

}

// CLEANUP
H2DE_Button::~H2DE_Button() {

}

// GETTER
H2DE_ButtonData* H2DE_GetButtonData(H2DE_Button* button) {
    return &(button->data);
}

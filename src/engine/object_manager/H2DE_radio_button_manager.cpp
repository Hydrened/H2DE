#include "H2DE/engine/H2DE_object_manager.h"

// EVENTS

// -- mouse down
void H2DE_ObjectManager::handleEvents_mouseDown_radioButton(H2DE_RadioButtonObject* radioButton) {
    bool noRadioButtonClicked = (radioButton == H2DE_NULL_OBJECT);
    if (noRadioButtonClicked) {
        return;
    }

    checkRadioButton(radioButton);
}

// ACTIONS
void H2DE_ObjectManager::refreshRadioButtonBuffer(const std::vector<H2DE_Object*>& objects) {
    refreshBuffer(radioButtons, objects);
    refreshHoverObjectBuffer();
}

void H2DE_ObjectManager::checkRadioButton(H2DE_RadioButtonObject* radioButton) {
    bool radioButtonIsNull = (radioButton == H2DE_NULL_OBJECT);
    if (radioButtonIsNull) {
        return;
    }

    bool hasBeenChecked = (!radioButton->_radioButtonObjectData.checked);

    radioButton->_setCheck(true);

    if (!hasBeenChecked) {
        return;
    }

    const H2DE_RadioButtonID id = radioButton->_radioButtonObjectData.id;

    for (H2DE_RadioButtonObject* otherRadioButton : radioButtons) {
        bool isSameRadioButton = (radioButton == otherRadioButton);
        if (isSameRadioButton) {
            continue;
        }

        const H2DE_RadioButtonID otherID = otherRadioButton->_radioButtonObjectData.id;

        bool sameID = (id == otherID);
        if (!sameID) {
            continue;
        }

        otherRadioButton->uncheck();
    }
}

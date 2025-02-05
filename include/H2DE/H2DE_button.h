#ifndef H2DE_BUTTON_H
#define H2DE_BUTTON_H

#include <H2DE_engine.h>
class H2DE_Engine;

/**
 * Type used to identify a button
 * \since H2DE-2.0.10
 */
class H2DE_Button {
private:
    H2DE_Engine* engine;
    H2DE_ButtonData data;

public:
    H2DE_Button(H2DE_Engine* engine, H2DE_ButtonData data);
    ~H2DE_Button();
    
    /**
     * Gets a button's data
     * \param button pointer to a button
     * \returns button's data
     * \since H2DE-2.0.10
     */
    friend H2DE_ButtonData* H2DE_GetButtonData(H2DE_Button* button);
};

#endif

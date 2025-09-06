#pragma once

#include <H2DE/objects/parents/H2DE_object.h>

class H2DE_ToggleableObject : virtual public H2DE_Object {
public:
    inline void enable() {
        _disabled = false;
    }
    inline void disable() {
        _disabled = true;
    }
    inline void toggleEnable() {
        _disabled = !_disabled;
    }

    constexpr bool isEnabled() {
        return !_disabled;
    }
    constexpr bool isDisabled() {
        return _disabled;
    }

    friend class H2DE_Engine;

protected:
    H2DE_ToggleableObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData) {}
    ~H2DE_ToggleableObject() override = default;
};

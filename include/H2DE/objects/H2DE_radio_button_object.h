#pragma once

#include <H2DE/objects/parents/H2DE_dual_surface_object.h>
#include <H2DE/objects/parents/H2DE_toggleable_object.h>

class H2DE_RadioButtonObject : public H2DE_DualSurfaceObject, public H2DE_ToggleableObject {
public:
    void check();
    void uncheck();
    void toggleCheck();

    inline H2DE_RadioButtonObjectData getradioButtonData() const noexcept {
        return _radioButtonObjectData;
    }
    constexpr H2DE_RadioButtonID getID() const noexcept {
        return _radioButtonObjectData.id;
    }
    constexpr bool isChecked() const noexcept {
        return _radioButtonObjectData.checked;
    }
    inline void setOnChange(const std::function<void(H2DE_RadioButtonEventData)>& onChange) noexcept {
        _radioButtonObjectData.onChange = onChange;
    }

    using H2DE_DataType = H2DE_RadioButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    static constexpr bool _isParentObject = false;

    H2DE_RadioButtonObjectData _radioButtonObjectData;

    H2DE_RadioButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_RadioButtonObjectData& radioButtonObjectData) : H2DE_Object(engine, objectData), H2DE_DualSurfaceObject(engine, objectData), H2DE_ToggleableObject(engine, objectData), _radioButtonObjectData(radioButtonObjectData) {}
    ~H2DE_RadioButtonObject() override = default;

    void _refreshSurfaceBuffers() override;
    void _check();

    void _setCheck(bool checked);
};

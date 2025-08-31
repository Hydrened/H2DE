#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_CheckboxObject : public H2DE_DualSurfaceObject {
public:
    /**
     * @brief Enable the checkbox (makes it active).
     */
    inline void enable() { 
        _disabled = false;
    }
    /**
     * @brief Disable the checkbox (makes it inactive).
     */
    inline void disable() {
        _disabled = true;
    }

    void check();
    void uncheck();
    void toggleCheck();

    /**
     * @brief Get the data specific to the checkbox object.
     * 
     * Returns a copy of the internal `H2DE_CheckboxObjectData` structure
     * which contains all configuration and state info related to the checkbox.
     * 
     * @return A copy of the checkbox's data.
     */
    inline H2DE_CheckboxObjectData getCheckboxData() const noexcept {
        return _checkboxObjectData;
    }
    constexpr bool isChecked() const noexcept {
        return _checkboxObjectData.checked;
    }
    /**
     * @brief Check if the checkbox is disabled.
     * @return true if disabled, false otherwise.
     */
    constexpr bool isDisabled() const noexcept {
        return _disabled; 
    }

    inline void setOnChange(const std::function<void(H2DE_CheckboxEventData)>& onChange) noexcept {
        _checkboxObjectData.onChange = onChange;
    }

    using H2DE_DataType = H2DE_CheckboxObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_CheckboxObjectData _checkboxObjectData;

    H2DE_CheckboxObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_CheckboxObjectData& checkboxObjectData) : H2DE_DualSurfaceObject(engine, objectData), _checkboxObjectData(checkboxObjectData) {}
    ~H2DE_CheckboxObject() override = default;

    void _refreshSurfaceBuffers() override;
};

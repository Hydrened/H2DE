#pragma once

template<typename H2DE_Object_T, typename... H2DE_SpecificObjectData_T>
H2DE_Object_T* H2DE_Engine::createObject(const H2DE_ObjectData& objectData, H2DE_SpecificObjectData_T&&... specificObjectData) {
    H2DE_Object_T* object = H2DE_NULL_OBJECT;

    constexpr int nbArgs = sizeof...(H2DE_SpecificObjectData_T);
    constexpr bool hasDataType = _hasH2DE_DataType<H2DE_Object_T>::value;

    if (nbArgs > 1) {
        static_assert((hasDataType && nbArgs == 1) || (!hasDataType && nbArgs == 0), "Invalid arguments passed to createObject");
    }
    
    constexpr bool objectHasNoSpecificDataType = (!hasDataType && nbArgs == 0);
    constexpr bool objectHasSpecificDataType = (hasDataType && nbArgs == 1);

    if constexpr (objectHasNoSpecificDataType) {
        object = new H2DE_Object_T(this, objectData);

    } else if constexpr (objectHasSpecificDataType) {
        object = new H2DE_Object_T(this, objectData, std::forward<H2DE_SpecificObjectData_T>(specificObjectData)...);
    
    } else {
        return H2DE_NULL_OBJECT;
    }

    _objects.push_back(object);

    constexpr bool isButtonObject = (std::is_same_v<H2DE_Object_T, H2DE_ButtonObject>);
    if constexpr (isButtonObject) {
        _refreshObjectManagerButtons();
    }

    constexpr bool isCheckboxObject = (std::is_same_v<H2DE_Object_T, H2DE_CheckboxObject>);
    if constexpr (isCheckboxObject) {
        _refreshObjectManagerCheckboxes();
    }

    constexpr bool isInputObject = (std::is_same_v<H2DE_Object_T, H2DE_InputObject>);
    if constexpr (isInputObject) {
        _refreshObjectManagerInputs();
    }

    return object;
}

template<typename H2DE_Module>
void H2DE_Engine::destroyModule(H2DE_Module* module) {
    if (module != nullptr) {
        delete module;
        module = nullptr;
    }
}

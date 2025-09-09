#pragma once

template<typename H2DE_Object_T, typename... H2DE_SpecificObjectData_T>
H2DE_Object_T* H2DE_Engine::createObject(const H2DE_ObjectData& objectData, H2DE_SpecificObjectData_T&&... specificObjectData) {
    constexpr bool isObject = _isObject<H2DE_Object_T>::value;
    static_assert(isObject, "Invalid type passed to createObject");

    constexpr bool isParent = H2DE_Object_T::_isParentObject;
    static_assert(!isParent, "Invalid object type passed to createObject");

    constexpr int nbArgs = sizeof...(H2DE_SpecificObjectData_T);
    constexpr bool hasDataType = _hasH2DE_DataType<H2DE_Object_T>::value;

    constexpr bool objectHasNoSpecificDataType = (!hasDataType && nbArgs == 0);
    constexpr bool objectHasSpecificDataType = (hasDataType && nbArgs == 1);

    static_assert(objectHasSpecificDataType || objectHasNoSpecificDataType, "Invalid arguments passed to createObject");

    H2DE_Object_T* object = H2DE_NULL_OBJECT;

    if constexpr (objectHasNoSpecificDataType) {
        object = new H2DE_Object_T(this, objectData);

    } else if constexpr (objectHasSpecificDataType) {
        object = new H2DE_Object_T(this, objectData, std::forward<H2DE_SpecificObjectData_T>(specificObjectData)...);
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

    constexpr bool isRadioButtonObject = (std::is_same_v<H2DE_Object_T, H2DE_RadioButtonObject>);
    if constexpr (isRadioButtonObject) {
        _refreshObjectManagerRadioButtons();
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

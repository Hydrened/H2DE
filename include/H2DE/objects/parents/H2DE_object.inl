#pragma once

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::_addSurface(H2DE_SurfaceMap& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
    H2DE_Surface_T* surface = new H2DE_Surface_T(_engine, this, surfaceData, specificData);
    surfaces[name] = surface;
    _refreshSurfaceBuffers();
    return surface;
}

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::_getSurface(const H2DE_SurfaceMap& surfaces, const std::string& name) {
    auto it = surfaces.find(name);
    if (it == surfaces.end()) {
        return H2DE_NULL_SURFACE;
    }

    return dynamic_cast<H2DE_Surface_T*>(it->second);
}

// template<typename H2DE_Property_T>
// bool H2DE_Object::_setProperty(H2DE_Property_T& property, const H2DE_Property_T& value) {
//     if (property != value) {
//         property = value;
//         _refreshSurfaceBuffers();
//         return true;
//     }

//     return false;
// }

// template<typename H2DE_HitboxProperty_T>
// bool H2DE_Object::_setHitboxProperty(H2DE_HitboxProperty_T H2DE_Transform::* property, H2DE_HitboxProperty_T H2DE_Transform::* defaultProperty, const std::string& name, const H2DE_HitboxProperty_T& value) {
//     if (!hasHitbox(name)) {
//         return false;
//     }

//     _hitboxes[name].transform.*property = value;
//     if (defaultProperty != nullptr) {
//         _hitboxes[name].transform.*defaultProperty = value;
//     }

//     _refreshMaxRadius();
//     return true;
// }

// template<typename H2DE_HitboxProperty_T>
// bool H2DE_Object::_setHitboxProperty(H2DE_HitboxProperty_T H2DE_Hitbox::* property, const std::string& name, const H2DE_HitboxProperty_T& value) {
//     if (!hasHitbox(name)) {
//         return false;
//     }

//     _hitboxes[name].*property = value;
//     return true;
// }

// template<typename H2DE_Property_T>
// H2DE_Timeline* H2DE_Object::_setProperty(H2DE_Property_T H2DE_Transform::* property, void (H2DE_Object::*setter)(const H2DE_Property_T& v), const H2DE_Property_T& value, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
//     H2DE_Timeline* timeline = H2DE_LerpManager::lerp<H2DE_Property_T>(_engine, _objectData.transform.*property, value, duration, easing, [this, setter](H2DE_Property_T iv) {
//         (this->*setter)(iv);
//     }, completed, pauseSensitive);

//     _addTimelineToTimelines(timeline);
//     return timeline;
// }

#pragma once

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::_addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
    H2DE_Surface_T* surface = new H2DE_Surface_T(_engine, this, surfaceData, specificData);
    surfaces[name] = surface;
    _refreshMaxRadius();
    _refreshSurfaceBuffers();
    return surface;
}

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::_getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    auto it = surfaces.find(name);
    if (it == surfaces.end()) {
        return H2DE_NULL_SURFACE;
    }

    return dynamic_cast<H2DE_Surface_T*>(it->second);
}

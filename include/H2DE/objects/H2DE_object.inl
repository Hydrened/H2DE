#pragma once

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
    H2DE_Surface_T* surface = new H2DE_Surface_T(engine, this, surfaceData, specificData);
    surfaces[name] = surface;
    refreshMaxRadius();
    refreshSurfaceBuffers();
    return surface;
}

template<typename H2DE_Surface_T>
H2DE_Surface_T* H2DE_Object::getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    auto it = surfaces.find(name);
    if (it == surfaces.end()) {
        return nullptr;
    }

    return dynamic_cast<H2DE_Surface_T*>(it->second);
}

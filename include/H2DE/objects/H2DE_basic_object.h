#ifndef H2DE_BASIC_OBJECT_H
#define H2DE_BASIC_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BasicObject : public H2DE_Object {
public:
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(surfaces, name, surfaceData, specificData);
    }
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::removeSurface(surfaces, name);
    }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const { return surfaces; }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const { return H2DE_Object::getSurface<H2DE_Surface_T>(surfaces, name); }

    friend class H2DE_Engine;

private:
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    H2DE_BasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    ~H2DE_BasicObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;
};

#endif

#ifndef H2DE_BAR_OBJECT_H
#define H2DE_BAR_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData barObjectData;

    std::unordered_map<std::string, H2DE_Surface*> frontSurfaces = {};
    std::unordered_map<std::string, H2DE_Surface*> backgroundSurfaces = {};

    H2DE_BarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    ~H2DE_BarObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

public:
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToFront(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(frontSurfaces, name, surfaceData, specificData);
    }
    inline void removeSurfaceFromFront(const std::string& name) {
        H2DE_Object::removeSurface(frontSurfaces, name);
    }

    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(backgroundSurfaces, name, surfaceData, specificData);
    }
    inline void removeSurfaceFromBackground(const std::string& name) {
        H2DE_Object::removeSurface(backgroundSurfaces, name);
    }

    inline H2DE_BarObjectData getBarData() const { return barObjectData; }
    inline float getMin() const { return barObjectData.min; }
    inline float getMax() const { return barObjectData.max; }
    inline float getValue() const { return barObjectData.value; }

    inline std::unordered_map<std::string, H2DE_Surface*> getFrontSurfaces() const { return frontSurfaces; }
    inline std::unordered_map<std::string, H2DE_Surface*> getBackgroundSurfaces() const { return backgroundSurfaces; }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getFrontSurface(const std::string& name) const { return H2DE_Object::getSurface<H2DE_Surface_T>(frontSurfaces, name); }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getBackgroundSurface(const std::string& name) const { return H2DE_Object::getSurface<H2DE_Surface_T>(backgroundSurfaces, name); }

    void setMin(float min);
    void setMax(float max);
    void setValue(float value);

    H2DE_TimelineID setMin(float min, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setMax(float max, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setValue(float value, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_BarObjectData;

    friend class H2DE_Engine;
};

#endif

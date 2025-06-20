#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

class H2DE_ButtonObject : public H2DE_Object {
public:
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(surfaces, name, surfaceData, specificData);
    }
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::removeSurface(surfaces, name);
    }

    inline void enable() { disabled = false; }
    inline void disable() { disabled = true; }

    void mouseDown();
    void mouseUp();
    void mouseHover();
    void mouseBlur();

    bool stopTimeline();

    inline H2DE_ButtonObjectData getButtonData() const { return buttonObjectData; }
    constexpr bool isPauseSensitive() const { return buttonObjectData.pauseSensitive; }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const { return surfaces; }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const { return H2DE_Object::getSurface<H2DE_Surface_T>(surfaces, name); }
    inline H2DE_TextObject* getTextObject() const { return textObject; }

    inline void setMouseDown(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onMouseDown) { buttonObjectData.onMouseDown = onMouseDown; }
    inline void setMouseUp(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onMouseUp) { buttonObjectData.onMouseUp = onMouseUp; }
    inline void setMouseHover(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onHover) { buttonObjectData.onHover = onHover; }
    inline void setMouseBlur(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onBlur) { buttonObjectData.onBlur = onBlur; }
    inline void setPauseSensitive(bool pauseSensitive) { buttonObjectData.pauseSensitive = pauseSensitive; }

    using H2DE_DataType = H2DE_ButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_ButtonObjectData buttonObjectData;

    H2DE_TextObject* textObject = nullptr;
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    bool disabled = false;
    H2DE_TimelineID currentTimelineID = H2DE_INVALID_TIMELINE_ID;

    H2DE_ButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    ~H2DE_ButtonObject() override;

    void refreshTextObject();
    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;
};

#endif

#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <sstream>
#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Object {
private:
    bool hidden = false;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData od;
    
    H2DE_Object(H2DE_Engine* engine, H2DE_ObjectData od);
    virtual ~H2DE_Object();

    virtual void update();
    void updateCollision();
    void snap(const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face);
    virtual std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const = 0;

public:
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);

    friend void H2DE_AddHitboxToObject(H2DE_Object* object, const std::string& name, const H2DE_Hitbox& hitbox);
    friend void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& name);

    friend H2DE_LevelPos H2DE_GetObjectPos(const H2DE_Object* object);
    friend H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object);
    friend float H2DE_GetObjectRotation(const H2DE_Object* object);
    friend H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object);
    friend H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object);
    friend std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object);
    friend int H2DE_GetObjectIndex(const H2DE_Object* object);
    friend bool H2DE_IsObjectAbsolute(const H2DE_Object* object);
    friend bool H2DE_IsObjectHidden(const H2DE_Object* object);

    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectIndex(H2DE_Object* object, int index);
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation);
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot);
    friend void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip);
    friend void H2DE_ShowObject(H2DE_Object* object);
    friend void H2DE_HideObject(H2DE_Object* object);

    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object);

    friend class H2DE_Engine;
};



class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData bod;
    float percentage = 0.0f;
    float value;

    H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData bod);
    ~H2DE_BarObject() override;

    void update() override;
    void refreshPercentage();
    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const override;

public:
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    friend H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar);
    friend H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar);
    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value);
    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetBarMin(H2DE_BarObject* bar, float min);
    friend void H2DE_SetBarMax(H2DE_BarObject* bar, float max);
};



class H2DE_BasicObject : public H2DE_Object {
private:
    H2DE_BasicObjectData bod;

    H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData bod);
    ~H2DE_BasicObject() override;

    void update() override;
    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const override;

public:
    friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
    friend H2DE_Surface* H2DE_GetBasicObjectSurface(const H2DE_BasicObject* basicObject);
};



class H2DE_ButtonObject : public H2DE_Object {
private:
    H2DE_ButtonObjectData bod;

    bool hover = false;

    H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData bod);
    ~H2DE_ButtonObject() override;

    void update() override;
    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const override;

public:
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    friend H2DE_Surface* H2DE_GetButtonSurface(const H2DE_ButtonObject* button);
    friend void H2DE_HoverButton(const H2DE_ButtonObject* button);
    friend void H2DE_OutButton(const H2DE_ButtonObject* button);
    friend void H2DE_ClickButton(const H2DE_ButtonObject* button);
    friend void H2DE_SetButtonHover(H2DE_ButtonObject* button, const std::function<void()>& onhover);
    friend void H2DE_SetButtonOut(H2DE_ButtonObject* button, const std::function<void()>& onout);
    friend void H2DE_SetButtonClick(H2DE_ButtonObject* button, const std::function<void()>& onclick);
};



class H2DE_TextObject : public H2DE_Object {
private:
    H2DE_TextObjectData tod;

    std::vector<H2DE_SurfaceBuffer> surfaceBuffers = {};

    H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData tod);
    ~H2DE_TextObject() override;

    void update() override;
    void destroySurfaces();
    void resetSurfaces();
    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const override;
    std::vector<std::string> getWords() const;
    std::vector<std::vector<std::string>> getLines() const;
    float getLineStartOffsetX(const std::vector<std::string>& line) const;

public:
    friend H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    friend void H2DE_SetTextObjectText(H2DE_TextObject* textObject, const std::string& text);
    friend void H2DE_SetTextObjectFont(H2DE_TextObject* textObject, const std::string& font);
    friend void H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize);
    friend void H2DE_SetTextObjectFontSize(H2DE_TextObject* textObject, const H2DE_LevelSize& fontSize, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing);
    friend void H2DE_SetTextObjectSpacing(H2DE_TextObject* textObject, const H2DE_LevelSize& spacing, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetTextObjectTextAlign(H2DE_TextObject* textObject, H2DE_TextAlign textAlign);
    friend void H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color);
    friend void H2DE_SetTextObjectColor(H2DE_TextObject* textObject, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);

    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation);
    friend void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot);
    friend void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip);
};

#endif

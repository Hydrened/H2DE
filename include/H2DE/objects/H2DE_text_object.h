#ifndef H2DE_TEXT_OBJECT_H
#define H2DE_TEXT_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_TextObject : public H2DE_Object {
private:
    H2DE_TextObjectData tod;

    H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData tod);
    ~H2DE_TextObject() override;

    void resetSurfaceBuffers() override;

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

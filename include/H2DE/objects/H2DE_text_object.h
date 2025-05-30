#ifndef H2DE_TEXT_OBJECT_H
#define H2DE_TEXT_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_TextObject : public H2DE_Object {
private:
    H2DE_TextObjectData textObjectData;

    H2DE_TextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    ~H2DE_TextObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

public:
    inline H2DE_TextObjectData getTextData() const { return textObjectData; }
    inline std::string getText() const { return textObjectData.text.text; }
    inline std::string getFont() const { return textObjectData.text.font; }
    inline H2DE_Scale getFontSize() const { return textObjectData.text.fontSize; }
    inline H2DE_Scale getSpacing() const { return textObjectData.text.spacing; }
    inline H2DE_TextAlign getTextAlign() const { return textObjectData.text.textAlign; }
    inline H2DE_ColorRGB getColor() const { return textObjectData.text.color; }

    void setText(const std::string& text);
    void setFont(const std::string& font);
    void setFontSize(const H2DE_Scale& fontSize);
    void setSpacing(const H2DE_Scale& spacing);
    void setTextAlign(H2DE_TextAlign textAlign);
    void setColor(const H2DE_ColorRGB& color);

    unsigned int setFontSize(const H2DE_Scale& fontSize, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setSpacing(const H2DE_Scale& spacing, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Engine;
};

#endif

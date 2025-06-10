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

    const std::vector<std::string> getWords() const;
    const std::vector<std::vector<std::string>> getLines() const;

    static int getLineLength(const std::vector<std::string>& line);

    float getStartingOffsetY(const std::vector<std::vector<std::string>>& lines) const;
    float getStartingOffsetX(const std::vector<std::string>& line) const;

    inline bool isTextNull() const { return (textObjectData.text.text == ""); }

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

    H2DE_TimelineID setFontSize(const H2DE_Scale& fontSize, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setSpacing(const H2DE_Scale& spacing, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setColor(const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_TextObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ButtonObject;
};

#endif

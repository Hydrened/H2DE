#pragma once

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

class H2DE_InputObject : public H2DE_Object {
public:
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::_addSurface<H2DE_Surface_T>(_surfaces, name, surfaceData, specificData);
    }

    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::_removeSurface(_surfaces, name);
    }

    inline void enable() { 
        _disabled = false;
    }
    inline void disable() {
        _disabled = true;
    }

    void input(unsigned char c);
    void focus();
    void blur();
    void submit();

    inline H2DE_InputObjectData getInputData() const noexcept {
        return _inputObjectData;
    }
    inline std::string getText() const noexcept {
        return _inputObjectData.text.text;
    }
    constexpr bool isDisabled() const noexcept {
        return _disabled; 
    }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const noexcept {
        return _surfaces; 
    }
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const {
        return H2DE_Object::_getSurface<H2DE_Surface_T>(_surfaces, name); 
    }
    inline bool hasSurface(const std::string& name) const {
        return H2DE_Object::_hasSurface(_surfaces, name);
    }
    inline H2DE_TextObject* getTextObject() const noexcept {
        return _textObject; 
    }

    void setText(const std::string& text);
    inline void setOnInput(const std::function<void(H2DE_InputEventData)>& onInput) noexcept {
        _inputObjectData.onInput = onInput;
    }
    inline void setOnFocus(const std::function<void(H2DE_InputEventData)>& onFocus) noexcept {
        _inputObjectData.onFocus = onFocus;
    }
    inline void setOnBlur(const std::function<void(H2DE_InputEventData)>& onBlur) noexcept {
        _inputObjectData.onBlur = onBlur;
    }
    inline void setOnSubmit(const std::function<void(H2DE_InputEventData)>& onSubmit) noexcept {
        _inputObjectData.onSubmit = onSubmit;
    }

    using H2DE_DataType = H2DE_InputObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_InputObjectData _inputObjectData;

    H2DE_TextObject* _textObject = H2DE_NULL_OBJECT;
    int _cursorPosition = -1;
    H2DE_Color* _cursor = H2DE_NULL_SURFACE;
    std::unordered_map<std::string, H2DE_Surface*> _surfaces = {};
    H2DE_InputEventData _eventData;

    bool _disabled = false;

    H2DE_InputObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_InputObjectData& inputObjectData);
    ~H2DE_InputObject() override;

    void _initCursor();

    void _refreshTextObject();
    void _refreshCursor();
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;

    inline bool _isInputValid(unsigned char c) const {
        return (std::isdigit(c))
            ? ((_inputObjectData.type & H2DE_INPUT_TYPE_NUMBER) != 0)
            : ((_inputObjectData.type & H2DE_INPUT_TYPE_TEXT) != 0);
    }

    constexpr bool _isFull() const {
        return (_inputObjectData.text.text.length() >= _inputObjectData.maxLength);
    }

    void _setCursorPosition(int position);
};

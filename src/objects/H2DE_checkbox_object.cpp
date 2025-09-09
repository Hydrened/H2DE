#include "H2DE/objects/H2DE_checkbox_object.h"

// ACTIONS
void H2DE_CheckboxObject::_refreshSurfaceBuffers() {
    H2DE_Object::_refreshSurfaceBuffers();

    size_t surfaceBufferSize = _backgroundSurfaces.size() + ((_checkboxObjectData.checked) ? _fillSurfaces.size() : 0);
    _surfaceBuffers.reserve(surfaceBufferSize);

    const std::vector<H2DE_Surface*> sortedBackgroundSurfaces = H2DE_Object::_getSortedSurfaces(_backgroundSurfaces);
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedBackgroundSurfaces.begin(), sortedBackgroundSurfaces.end());

    if (_checkboxObjectData.checked) {
        const std::vector<H2DE_Surface*> sortedFillSurfaces = H2DE_Object::_getSortedSurfaces(_fillSurfaces);
        _surfaceBuffers.insert(_surfaceBuffers.end(), sortedFillSurfaces.begin(), sortedFillSurfaces.end());
    }

    _surfaceBuffers = H2DE_Object::_getSortedSurfaces(_surfaceBuffers);
    _rescaleSurfaceBuffers();
    _refreshMaxRadius();
}

void H2DE_CheckboxObject::check() {
    _setCheck(true);
}

void H2DE_CheckboxObject::uncheck() {
    _setCheck(false);
}

void H2DE_CheckboxObject::toggleCheck() {
    _setCheck(!_checkboxObjectData.checked);
}

// SETTER
void H2DE_CheckboxObject::_setCheck(bool checked) {
    if (_disabled) {
        return;
    }

    if (_checkboxObjectData.checked == checked) {
        return;
    }

    if (_checkboxObjectData.onChange) {
        _checkboxObjectData.onChange({ this, checked });
    }

    _checkboxObjectData.checked = checked;
    _refreshSurfaceBuffers();
}

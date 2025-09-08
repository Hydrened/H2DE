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
    if (_checkboxObjectData.checked) {
        return;
    }

    if (_checkboxObjectData.onChange) {
        _checkboxObjectData.onChange({ this, true });
    }

    _checkboxObjectData.checked = true;
    _refreshSurfaceBuffers();
}

void H2DE_CheckboxObject::uncheck() {
    if (!_checkboxObjectData.checked) {
        return;
    }

    if (_checkboxObjectData.onChange) {
        _checkboxObjectData.onChange({ this, false });
    }

    _checkboxObjectData.checked = false;
    _refreshSurfaceBuffers();
}

void H2DE_CheckboxObject::toggleCheck() {
    (_checkboxObjectData.checked) ? uncheck() : check();
}

#include "H2DE/objects/H2DE_radio_button_object.h"
#include "H2DE/engine/H2DE_engine.h"

// ACTIONS
void H2DE_RadioButtonObject::_refreshSurfaceBuffers() {
    H2DE_Object::_refreshSurfaceBuffers();

    size_t surfaceBufferSize = _backgroundSurfaces.size() + ((_radioButtonObjectData.checked) ? _fillSurfaces.size() : 0);
    _surfaceBuffers.reserve(surfaceBufferSize);

    const std::vector<H2DE_Surface*> sortedBackgroundSurfaces = H2DE_Object::_getSortedSurfaces(_backgroundSurfaces);
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedBackgroundSurfaces.begin(), sortedBackgroundSurfaces.end());

    if (_radioButtonObjectData.checked) {
        const std::vector<H2DE_Surface*> sortedFillSurfaces = H2DE_Object::_getSortedSurfaces(_fillSurfaces);
        _surfaceBuffers.insert(_surfaceBuffers.end(), sortedFillSurfaces.begin(), sortedFillSurfaces.end());
    }

    _surfaceBuffers = H2DE_Object::_getSortedSurfaces(_surfaceBuffers);
    _rescaleSurfaceBuffers();
    _refreshMaxRadius();
}

void H2DE_RadioButtonObject::_check() {
    if (_disabled) {
        return;
    }

    _engine->_objectManager->checkRadioButton(this);
}

void H2DE_RadioButtonObject::check() {
    _check();
}

void H2DE_RadioButtonObject::uncheck() {
    _setCheck(false);
}

void H2DE_RadioButtonObject::toggleCheck() {
    (!_radioButtonObjectData.checked) ? check() : uncheck();
}

// SETTER
void H2DE_RadioButtonObject::_setCheck(bool checked) {
    if (_disabled) {
        return;
    }

    if (_radioButtonObjectData.checked == checked) {
        return;
    }

    if (_radioButtonObjectData.onChange) {
        _radioButtonObjectData.onChange({ this, _radioButtonObjectData.id, checked });
    }

    _radioButtonObjectData.checked = checked;
    _refreshSurfaceBuffers();
}

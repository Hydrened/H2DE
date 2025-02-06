#include "H2DE/H2DE_renderer.h"

H2DE_AbsPos H2DE_Renderer::lvlToAbs(H2DE_LevelPos pos, bool absolute) const {
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    H2DE_LevelPos camPos = H2DE_GetCameraPos(camera);
    int blockSize = getBlockSize();

    return {
        static_cast<int>((pos.x - ((absolute) ? 0.0f : camPos.x)) * blockSize),
        static_cast<int>((pos.y - ((absolute) ? 0.0f : camPos.y)) * blockSize),
    };
}

H2DE_AbsSize H2DE_Renderer::lvlToAbs(H2DE_LevelSize size) const {
    int blockSize = getBlockSize();

    return {
        static_cast<int>(size.w * blockSize),
        static_cast<int>(size.h * blockSize),
    };
}

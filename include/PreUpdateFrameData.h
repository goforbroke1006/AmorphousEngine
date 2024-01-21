//
// Created by goforbroke on 1/20/24.
//

#ifndef AMORPHOUS_ENGINE_PRE_UPDATE_FRAME_DATA_H
#define AMORPHOUS_ENGINE_PRE_UPDATE_FRAME_DATA_H

#include <string>
#include <map>
#include "Core/GameObject.h"
#include "Core/KeyCode.h"

namespace AmE {
    class PreUpdateFrameData {
    public:
        std::map<KeyCode, bool> keysPressed;
        std::map<KeyCode, bool> keysReleased;
    };
}

#endif //AMORPHOUS_ENGINE_PRE_UPDATE_FRAME_DATA_H

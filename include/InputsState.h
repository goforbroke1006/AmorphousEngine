//
// Created by goforbroke on 1/20/24.
//

#ifndef AMORPHOUS_ENGINE_INPUTS_STATE_H
#define AMORPHOUS_ENGINE_INPUTS_STATE_H

#include <string>
#include <map>
#include "Core/GameObject.h"
#include "Core/KeyCode.h"

namespace AmE {
    /**
     * InputsState is an DTO to transfer keyboard and mouse state.
     */
    class InputsState {
    public:
        std::map<KeyCode, bool> pressed;
        std::map<KeyCode, bool> released;
    };
}

#endif //AMORPHOUS_ENGINE_INPUTS_STATE_H

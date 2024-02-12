//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_H
#define AMORPHOUS_ENGINE_INPUT_READER_H

#include "InputsState.h"

namespace AmE {
    /**
     * InputReader use OIS to fill InputsState with actual keyboard and mouse buttons state.
     */
    class InputReader {
    public:
        virtual void collectCodes() = 0;

        [[nodiscard]] virtual InputsState *getState() const  = 0;
    };
}

#endif //AMORPHOUS_ENGINE_INPUT_READER_H

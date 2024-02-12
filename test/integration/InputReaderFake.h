//
// Created by goforbroke on 2/11/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_FAKE_H
#define AMORPHOUS_ENGINE_INPUT_READER_FAKE_H

#include "../../include/InputReader.h"
#include "../../include/InputsState.h"

class InputReaderFake : public AmE::InputReader {
public:
    explicit InputReaderFake() {
        mInputState = new AmE::InputsState();
    }

    ~InputReaderFake() {
        delete mInputState;
    }

    void collectCodes() override {}

    [[nodiscard]] AmE::InputsState *getState() const override { return mInputState; }

private:
    AmE::InputsState *mInputState;
};

#endif //AMORPHOUS_ENGINE_INPUT_READER_FAKE_H

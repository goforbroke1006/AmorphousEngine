//
// Created by goforbroke on 2/11/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_FAKE_H
#define AMORPHOUS_ENGINE_INPUT_READER_FAKE_H

#include "../../../include/InputReader.h"
#include "../../../include/InputsState.h"

/**
 * There is two reasons to create fake input reader:
 * 1. Emulate key events from code
 * 2. Usage OIS in debug mode locks application and all OS
 */
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

    void fakePress(const AmE::KeyCode &code) {
        mInputState->pressed[code] = true;
    }

    void fakeRelease(const AmE::KeyCode &code) {
        mInputState->released[code] = true;
    }

    void fakeClear() {
        mInputState->pressed.clear();
        mInputState->released.clear();
    }

private:
    AmE::InputsState *mInputState;
};

#endif //AMORPHOUS_ENGINE_INPUT_READER_FAKE_H

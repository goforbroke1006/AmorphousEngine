//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_OIS_H
#define AMORPHOUS_ENGINE_INPUT_READER_OIS_H

#include <map>

#include <ois/OIS.h>

#include "InputsState.h"
#include "InputReader.h"
#include "Core/KeyCode.h"

namespace AmE {
    /**
     * InputReader use OIS to fill InputsState with actual keyboard and mouse buttons state.
     */
    class InputReaderOIS : public InputReader, OIS::KeyListener, OIS::MouseListener {
    public:
        explicit InputReaderOIS(size_t windowHnd);

        ~InputReaderOIS() override;

        void collectCodes() override;

        [[nodiscard]] InputsState *getState() const override;

    private:
        InputsState *mInputState;

        OIS::InputManager *mInputManager;
        OIS::Keyboard *mKeyboard;
        OIS::Mouse *mMouse;

    private:
        static std::map<OIS::KeyCode, KeyCode> kbOisToAME;
        static std::map<OIS::MouseButtonID, KeyCode> msOisToAME;

        // OIS::KeyListener implementation
    public:
        bool keyPressed(const OIS::KeyEvent &arg) override;

        bool keyReleased(const OIS::KeyEvent &arg) override;

        // OIS::MouseListener implementation
    public:
        bool mouseMoved(const OIS::MouseEvent &arg) override;

        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;

        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
    };
}

#endif //AMORPHOUS_ENGINE_INPUT_READER_OIS_H

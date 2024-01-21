//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_H
#define AMORPHOUS_ENGINE_INPUT_READER_H

#include <map>

#include <ois/OIS.h>

#include "InputsState.h"
#include "Core/KeyCode.h"

namespace AmE {
    /**
     * InputReader use OIS to fill InputsState with actual keyboard and mouse buttons state.
     */
    class InputReader : public OIS::KeyListener, OIS::MouseListener {
    public:
        explicit InputReader(size_t windowHnd, InputsState *const preUpdateFrameData);

        ~InputReader() override;

        void collectCodes();

    private:
        InputsState * mPreUpdateFrameData;

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

#endif //AMORPHOUS_ENGINE_INPUT_READER_H

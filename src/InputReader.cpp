//
// Created by goforbroke on 1/13/24.
//

#include "../include/InputReader.h"

#include <sstream>

AmE::InputReader::InputReader(const size_t windowHnd, PreUpdateFrameData *const preUpdateFrameData) {
    mPreUpdateFrameData = preUpdateFrameData;

    std::ostringstream windowHndStr;
    windowHndStr << windowHnd;

    OIS::ParamList pl;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem(pl);

    if (mInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        mKeyboard = dynamic_cast<OIS::Keyboard *>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setEventCallback(this);
    }

    if (mInputManager->getNumberOfDevices(OIS::OISMouse) > 0) {
        mMouse = dynamic_cast<OIS::Mouse *>(mInputManager->createInputObject(OIS::OISMouse, false));
        mMouse->setEventCallback(this);
    }

    // put all keys for keyboard
    for (const auto &pair: InputReader::kbOisToAME) {
        mPreUpdateFrameData->keysPressed[pair.second] = false;
        mPreUpdateFrameData->keysReleased[pair.second] = false;
    }
    // put all keys for mouse
    for (const auto &pair: InputReader::msOisToAME) {
        mPreUpdateFrameData->keysPressed[pair.second] = false;
        mPreUpdateFrameData->keysReleased[pair.second] = false;
    }
}

AmE::InputReader::~InputReader() {
    if (mMouse) {
        mInputManager->destroyInputObject(mMouse);
        mMouse = nullptr;
    }

    if (mKeyboard) {
        mInputManager->destroyInputObject(mKeyboard);
        mKeyboard = nullptr;
    }

    mInputManager->destroyInputSystem(mInputManager);
    mInputManager = nullptr;
}

void AmE::InputReader::collectCodes() {
    // reset all states
    for (auto &[_, v]: mPreUpdateFrameData->keysPressed) v = false;
    for (auto &[_, v]: mPreUpdateFrameData->keysReleased) v = false;

    // https://wiki.ogre3d.org/Using+OIS

    if (mKeyboard != nullptr) mKeyboard->capture();
    if (mMouse != nullptr) mMouse->capture();
}

std::map<OIS::KeyCode, AmE::KeyCode> AmE::InputReader::kbOisToAME = {
        {OIS::KC_W,        KeyCode_W},
        {OIS::KC_A,        KeyCode_A},
        {OIS::KC_S,        KeyCode_S},
        {OIS::KC_D,        KeyCode_D},

        {OIS::KC_UP,       KeyCode_UpArrow},
        {OIS::KC_DOWN,     KeyCode_DownArrow},
        {OIS::KC_LEFT,     KeyCode_LeftArrow},
        {OIS::KC_RIGHT,    KeyCode_RightArrow},

        {OIS::KC_ESCAPE,   KeyCode_Escape},
        {OIS::KC_LCONTROL, KeyCode_Ctrl},
        {OIS::KC_SPACE,    KeyCode_Space},
};

std::map<OIS::MouseButtonID, AmE::KeyCode> AmE::InputReader::msOisToAME = {
        {OIS::MB_Left,   KeyCode_Mouse0},
        {OIS::MB_Right,  KeyCode_Mouse1},
        {OIS::MB_Middle, KeyCode_Mouse2},
};

bool AmE::InputReader::keyPressed(const OIS::KeyEvent &arg) {
    if (InputReader::kbOisToAME.find(arg.key) == kbOisToAME.end())
        return true;

    mPreUpdateFrameData->keysPressed[kbOisToAME[arg.key]] = true;

    return true;
}

bool AmE::InputReader::keyReleased(const OIS::KeyEvent &arg) {
    if (InputReader::kbOisToAME.find(arg.key) == kbOisToAME.end())
        return true;

    mPreUpdateFrameData->keysReleased[kbOisToAME[arg.key]] = true;

    return true;
}

bool AmE::InputReader::mouseMoved(const OIS::MouseEvent &arg) {
    // TODO: implement me

    return true;
}

bool AmE::InputReader::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (InputReader::msOisToAME.find(id) == msOisToAME.end())
        return true;

    mPreUpdateFrameData->keysPressed[msOisToAME[id]] = true;

    return true;
}

bool AmE::InputReader::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (InputReader::msOisToAME.find(id) == msOisToAME.end())
        return true;

    mPreUpdateFrameData->keysReleased[msOisToAME[id]] = true;

    return true;
}

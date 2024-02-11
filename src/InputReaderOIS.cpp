//
// Created by goforbroke on 1/13/24.
//

#include "../include/InputReaderOIS.h"

#include <sstream>

AmE::InputReaderOIS::InputReaderOIS(const size_t windowHnd) {
    mInputState = new InputsState();

    mInputManager = nullptr;
    mKeyboard = nullptr;
    mMouse = nullptr;

    std::ostringstream windowHndStr;
    windowHndStr << windowHnd;

    OIS::ParamList pl;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem(pl);

    if (mInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        Logger::Trace("Initialize OIS keyboard");
        mKeyboard = dynamic_cast<OIS::Keyboard *>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setEventCallback(this);
    }

    if (mInputManager->getNumberOfDevices(OIS::OISMouse) > 0) {
        Logger::Trace("Initialize OIS mouse");
        mMouse = dynamic_cast<OIS::Mouse *>(mInputManager->createInputObject(OIS::OISMouse, true));
        mMouse->setEventCallback(this);
    }

    // put all keys for keyboard
    for (const auto &pair: InputReaderOIS::kbOisToAME) {
        mInputState->pressed[pair.second] = false;
        mInputState->released[pair.second] = false;
    }
    // put all keys for mouse
    for (const auto &pair: InputReaderOIS::msOisToAME) {
        mInputState->pressed[pair.second] = false;
        mInputState->released[pair.second] = false;
    }
}

AmE::InputReaderOIS::~InputReaderOIS() {
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

void AmE::InputReaderOIS::collectCodes() {
    // reset all states
    for (auto &[_, v]: mInputState->pressed) v = false;
    for (auto &[_, v]: mInputState->released) v = false;

    // https://wiki.ogre3d.org/Using+OIS

    if (mKeyboard != nullptr) mKeyboard->capture();
    if (mMouse != nullptr) mMouse->capture();
}

AmE::InputsState *AmE::InputReaderOIS::getState() const {
    return mInputState;
}

std::map<OIS::KeyCode, AmE::KeyCode> AmE::InputReaderOIS::kbOisToAME = {
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

std::map<OIS::MouseButtonID, AmE::KeyCode> AmE::InputReaderOIS::msOisToAME = {
        {OIS::MB_Left,   KeyCode_Mouse0},
        {OIS::MB_Right,  KeyCode_Mouse1},
        {OIS::MB_Middle, KeyCode_Mouse2},
};

bool AmE::InputReaderOIS::keyPressed(const OIS::KeyEvent &arg) {
    if (InputReaderOIS::kbOisToAME.find(arg.key) == kbOisToAME.end())
        return true;

    Logger::Trace("Press keyboard: " + kbOisToAME[arg.key].toString());
    mInputState->pressed[kbOisToAME[arg.key]] = true;

    return true;
}

bool AmE::InputReaderOIS::keyReleased(const OIS::KeyEvent &arg) {
    if (InputReaderOIS::kbOisToAME.find(arg.key) == kbOisToAME.end())
        return true;

    Logger::Trace("Release keyboard: " + kbOisToAME[arg.key].toString());
    mInputState->released[kbOisToAME[arg.key]] = true;

    return true;
}

bool AmE::InputReaderOIS::mouseMoved(const OIS::MouseEvent &arg) {
    // TODO: implement me

    return true;
}

bool AmE::InputReaderOIS::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (InputReaderOIS::msOisToAME.find(id) == msOisToAME.end())
        return true;

    Logger::Trace("Press mouse: " + msOisToAME[id].toString());
    mInputState->pressed[msOisToAME[id]] = true;

    return true;
}

bool AmE::InputReaderOIS::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (InputReaderOIS::msOisToAME.find(id) == msOisToAME.end())
        return true;

    Logger::Trace("Release mouse: " + msOisToAME[id].toString());
    mInputState->released[msOisToAME[id]] = true;

    return true;
}



//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_WINDOW_EVENT_LISTENER_H
#define AMORPHOUS_ENGINE_WINDOW_EVENT_LISTENER_H

#include <OGRE/OgreWindowEventUtilities.h>

class WindowEventListener : public Ogre::WindowEventListener {
public:
    WindowEventListener() : mShouldQuit(false) {}

    void windowClosed(Ogre::Window *window) override {
        printf("Window closed!\n");
        mShouldQuit = true;
    }

    [[nodiscard]] bool shouldQuit() const {
        return mShouldQuit;
    }

private:
    bool mShouldQuit;
};

#endif //AMORPHOUS_ENGINE_WINDOW_EVENT_LISTENER_H

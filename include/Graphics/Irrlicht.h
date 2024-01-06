//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUS_ENGINE_IRRLICHT_H
#define AMORPHOUS_ENGINE_IRRLICHT_H

#include "../GraphicsEngine.h"

#include <irrlicht.h>

#define TO_COLOR 255.0

class Irrlicht : public GraphicsEngine {
public:
    Irrlicht();

    virtual ~Irrlicht();

    void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

    bool update(const std::map<std::string, GameObject *> &gameObjects) override;

private:
    bool mQuit = false;

    irr::IrrlichtDevice *mDevice;
    irr::video::IVideoDriver *mVideoDriver;
    irr::scene::ISceneManager *mSceneManager;
    irr::gui::IGUIEnvironment *mGuiEnv;

    Color mBackgroundColor;
};

#endif //AMORPHOUS_ENGINE_IRRLICHT_H

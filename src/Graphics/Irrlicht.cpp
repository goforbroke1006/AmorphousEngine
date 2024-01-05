//
// Created by goforbroke on 1/5/24.
//

#include "../../include/Graphics/Irrlicht.h"

#include <irrlicht.h>
#include <stdexcept>

Irrlicht::Irrlicht() {
    mDevice = irr::createDevice(
            irr::video::EDT_SOFTWARE,
            irr::core::dimension2d<irr::u32>(800, 600),
            16, false, false, false, 0);
    if (!mDevice)
        throw std::runtime_error("device initialization failed");

    mDevice->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

    mVideoDriver = mDevice->getVideoDriver();
    mSceneManager = mDevice->getSceneManager();
    mGuiEnv = mDevice->getGUIEnvironment();

    mGuiEnv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
                           irr::core::rect<irr::s32>(10, 10, 260, 22), true);
}

void Irrlicht::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    for (const auto &goPair: gameObjects) {
        GameObject *pGO = goPair.second;

        auto *tr = pGO->mTransform;

        auto &pos = tr->mPosition;
        auto &rot = tr->mRotation;
        auto &scale = tr->mLocalScale;

        if (pGO->isCamera()) {
            mSceneManager->addCameraSceneNode(
                    nullptr,
                    irr::core::vector3df(pos.mX, pos.mY, pos.mZ),
                    irr::core::vector3df(0, 5, 0) // TODO:
            );

            // TODO: parse composite value
//            mBackgroundColor = pGO->mComponents["Camera"].mProperties["backgroundColor"].mValue;
        } else {
            //
        }
    }
}

bool Irrlicht::update(const std::map<std::string, GameObject *> &gameObjects) {
    mQuit |= !mDevice->run();
    if (mQuit)
        return false;

    mVideoDriver->beginScene(
            true,
            true,
            irr::video::SColor(255, 100, 101, 140) // TODO: set real color
    );

    mSceneManager->drawAll();
    mGuiEnv->drawAll();

    mVideoDriver->endScene();

    return true;
}



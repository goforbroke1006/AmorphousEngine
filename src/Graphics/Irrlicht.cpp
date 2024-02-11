//
// Created by goforbroke on 1/5/24.
//

#include "../../include/Graphics/Irrlicht.h"

#include <irrlicht.h>
#include <stdexcept>

AmE::Irrlicht::Irrlicht() {
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

    mBackgroundColor = AmE::grey;
}

AmE::Irrlicht::~Irrlicht() {
    GraphicsEngine::~GraphicsEngine();

//    delete mGuiEnv;
//    delete mSceneManager;
//    delete mVideoDriver;
//    delete mDevice;
}

size_t AmE::Irrlicht::getWindowHnd() const {
    return 0;
}

void AmE::Irrlicht::initialize(const SceneState * const sceneState) {
    for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
        auto *tr = pGameObj->getTransform();

        auto &pos = tr->mPosition;
//        auto &rot = tr->mRotation;
//        auto &scale = tr->mLocalScale;

        if (pGameObj->isCamera()) {
            mBackgroundColor = std::any_cast<Color>(
                    pGameObj->getComponent("Component/Camera")->mProperties["backgroundColor"].mValue);

            mSceneManager->addCameraSceneNode(
                    nullptr,
                    irr::core::vector3df(pos.getX(), pos.getY(), pos.getZ()),
                    irr::core::vector3df(0, 5, 0) // TODO:
            );

            // TODO: parse composite value
//            mBackgroundColor = pGO->mComponents["Camera"].mProperties["backgroundColor"].mValue;
        } else {
            //
        }
    }
}

bool AmE::Irrlicht::update( SceneState * const sceneState) {
    mQuit |= !mDevice->run();
    if (mQuit)
        return false;

    mVideoDriver->beginScene(
            true,
            true,
            irr::video::SColor((irr::u32) (mBackgroundColor.mA * TO_COLOR),
                               (irr::u32) (mBackgroundColor.mR * TO_COLOR),
                               (irr::u32) (mBackgroundColor.mG * TO_COLOR),
                               (irr::u32) (mBackgroundColor.mB * TO_COLOR)
            )
    );

    mSceneManager->drawAll();
    mGuiEnv->drawAll();

    mVideoDriver->endScene();

    return true;
}

void AmE::Irrlicht::stop() {
    mDevice->drop();
}



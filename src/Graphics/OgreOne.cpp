//
// Created by goforbroke on 2/6/24.
//

#include "../../include/Graphics/OgreOne.h"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/OgreViewport.h>


AmE::OgreOne::OgreOne(
        const std::string &pluginsCfgPathname,
        const std::string &projectRoot
) {
    //OgreBites::ApplicationContext::setup();
    mRoot = new Ogre::Root(pluginsCfgPathname);
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog(nullptr))
        throw std::runtime_error("Ogre config broken");


    mWindow = mRoot->initialise(true, "");
    mWindowHnd = -1;
    mWindow->getCustomAttribute("WINDOW", &mWindowHnd);

    mSceneManager = mRoot->createSceneManager();

    //

    Ogre::Light *light = mSceneManager->createLight("MainLight");
    Ogre::SceneNode *lightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    //

    Ogre::ResourceGroupManager::getSingleton()
            .addResourceLocation(
                    projectRoot,
                    "FileSystem",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton()
            .addResourceLocation(
                    projectRoot + "/Models",
                    "FileSystem",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    AmE::Logger::Debug("Load resources from " + projectRoot);
    AmE::Logger::Debug("Load resources from " + projectRoot + "/Models");

    // Initialise resources groups
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

AmE::OgreOne::~OgreOne() {
    GraphicsEngine::~GraphicsEngine();

//    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mWinListener);
    delete mRoot;
}

size_t AmE::OgreOne::getWindowHnd() const {
    return mWindowHnd;
}

void AmE::OgreOne::initialize(const AmE::SceneState *const sceneState) {
    for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
        if (pGameObj->isCamera()) {
            createCameraNode(pGameObj);
        } else if (pGameObj->isLight()) {
            auto color = pGameObj->getComponent("Light")->mProperties["color"].asColor();

            auto *light = mSceneManager->createLight();
            light->setType(Ogre::Light::LT_POINT);
            light->setDiffuseColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
            light->setSpecularColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);

            auto *spotLightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
            spotLightNode->attachObject(light);
            spotLightNode->setDirection(-1, -1, 0);
            spotLightNode->setPosition(Ogre::Vector3(200, 200, 0));

            mSceneNodes[pGameObj->getID()] = spotLightNode;
        } else {
            createSceneNode(pGameObj);
        }
    }
}

bool AmE::OgreOne::update(const AmE::SceneState *const sceneState) {
    if (mWindow->isVisible() && !mQuit) {
        for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
            if (pGameObj->isCamera()) {
                bool isNewObj = mSceneNodes.find(pGameObj->getID()) == mSceneNodes.end();

                if (isNewObj)
                    createCameraNode(pGameObj);
                else
                    updateCameraNode(pGameObj);
            } else if (pGameObj->isLight()) {
                // TODO: ???
            } else {
                bool isNewObj = mSceneNodes.find(pGameObj->getID()) == mSceneNodes.end();

                if (isNewObj)
                    createSceneNode(pGameObj);
                else
                    updateSceneNode(pGameObj);
            }
        }

        mQuit |= !mRoot->renderOneFrame();
    }

    return !mQuit;
}

void AmE::OgreOne::stop() {
    mRoot->saveConfig();
}

void AmE::OgreOne::createCameraNode(const GameObject *const gameObjectPtr) {
    mSceneNodes[gameObjectPtr->getID()] = mSceneManager->getRootSceneNode()->createChildSceneNode();

    if (gameObjectPtr->getComponent("Camera")->isEnabled()) {
//        auto backgroundColor = std::any_cast<Color>(
//                gameObjectPtr->getComponent("Camera")->mProperties.at("backgroundColor").mValue);
//
//        const Ogre::ColourValue backgroundColour(
//                (float) backgroundColor.mR,
//                (float) backgroundColor.mG,
//                (float) backgroundColor.mB,
//                (float) backgroundColor.mA);
//        mWindow->getViewport(0)->setBackgroundColour(backgroundColour);

        auto nearClipPlane = gameObjectPtr->getComponent("Camera")->mProperties.at("nearClipPlane").asDouble();
        auto farClipPlane = gameObjectPtr->getComponent("Camera")->mProperties.at("farClipPlane").asDouble();

        Ogre::Camera *cam = mSceneManager->createCamera(gameObjectPtr->getName());
        cam->setNearClipDistance((Ogre::Real) nearClipPlane);
        cam->setFarClipDistance((Ogre::Real) farClipPlane);
        cam->setAutoAspectRatio(true);

        mWindow->addViewport(cam);

        mSceneNodes[gameObjectPtr->getID()]->attachObject(cam);
    }

    updateCameraNode(gameObjectPtr);
}

void AmE::OgreOne::updateCameraNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->getTransform();
    auto pos = GraphicsEngine::convertPositionLeftToRightHand(tr->mPosition);
    auto rot = GraphicsEngine::convertRotationLeftToRightHand(tr->mRotation);

    mSceneNodes[gameObjectPtr->getID()]->setPosition(
            (Ogre::Real) pos.getX(),
            (Ogre::Real) pos.getY(),
            (Ogre::Real) pos.getZ()
    );

    mSceneNodes[gameObjectPtr->getID()]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.getW(),
                    (Ogre::Real) rot.getX(),
                    (Ogre::Real) rot.getY(),
                    (Ogre::Real) rot.getZ()
            )
    );

    if (nullptr != mWindow->getViewport(0)) {
        auto backgroundColor = std::any_cast<Color>(
                gameObjectPtr->getComponent("Camera")->mProperties.at("backgroundColor").mValue);

        const Ogre::ColourValue backgroundColour(
                (float) backgroundColor.mR,
                (float) backgroundColor.mG,
                (float) backgroundColor.mB,
                (float) backgroundColor.mA);
        mWindow->getViewport(0)->setBackgroundColour(backgroundColour);
    }
}

void AmE::OgreOne::createSceneNode(const GameObject *const gameObjectPtr) {
    auto *sceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(gameObjectPtr->getName());
    mSceneNodes[gameObjectPtr->getID()] = sceneNode;

    if (!gameObjectPtr->getMeshPathname().empty()) {
        Ogre::Entity *ent = mSceneManager->createEntity(gameObjectPtr->getMeshPathname());
        sceneNode->attachObject((Ogre::MovableObject *) ent);
    }

    updateSceneNode(gameObjectPtr);
}

void AmE::OgreOne::updateSceneNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->getTransform();

    auto pos = GraphicsEngine::convertPositionLeftToRightHand(tr->mPosition);
    auto rot = GraphicsEngine::convertRotationLeftToRightHand(tr->mRotation);
    auto &scale = tr->mLocalScale;

    mSceneNodes[gameObjectPtr->getID()]->setPosition(
            Ogre::Vector3(
                    (Ogre::Real) pos.getX(),
                    (Ogre::Real) pos.getY(),
                    (Ogre::Real) pos.getZ()
            )
    );
    mSceneNodes[gameObjectPtr->getID()]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.getW(),
                    (Ogre::Real) rot.getX(),
                    (Ogre::Real) rot.getY(),
                    (Ogre::Real) rot.getZ()
            )
    );
    mSceneNodes[gameObjectPtr->getID()]->setScale(
            Ogre::Vector3(
                    (Ogre::Real) scale.getX(),
                    (Ogre::Real) scale.getY(),
                    (Ogre::Real) scale.getZ()
            )
    );
}

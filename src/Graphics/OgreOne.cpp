//
// Created by goforbroke on 2/6/24.
//

#include "../../include/Graphics/OgreOne.h"

#include <string>

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/RTShaderSystem/OgreShaderRenderState.h>

#include "../../include/Core/LIght.h"


AmE::OgreOne::OgreOne(
        const std::string &pluginsCfgPathname,
        const std::string &projectRoot
) {
    //OgreBites::ApplicationContext::setup();
    mRoot = new Ogre::Root(pluginsCfgPathname);
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog(nullptr))
        throw std::runtime_error("Ogre config broken");

    mSceneManager = mRoot->createSceneManager();

    // register our scene with the RTSS
//    if (Ogre::RTShader::ShaderGenerator::initialize()) {
//        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
//
//        // Add the shader libs resource location.
//		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("libpath", "FileSystem");
//
//		// Set shader cache path.
//		mShaderGenerator->setShaderCachePath("cache");
//
//        // Set the scene manager.
//		mShaderGenerator->addSceneManager(mSceneManager);
//
//		// Add a specialized sub-render (per-pixel lighting) state to the default scheme render state
//		Ogre::RTShader::RenderState* pMainRenderState =
//			mShaderGenerator->createOrRetrieveRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME).first;
//		pMainRenderState->resetToBuiltinSubRenderStates();
//    }

    mWindow = mRoot->initialise(true, "");
    mWindowHnd = -1;
    mWindow->getCustomAttribute("WINDOW", &mWindowHnd);

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
//        if (pGameObj->isCamera()) {
//            createCameraNode(pGameObj);
//        } else if (pGameObj->isLight()) {
//            auto color = pGameObj->getComponent("Light")->mProperties["color"].asColor();
//
//            auto *light = mSceneManager->createLight();
//            light->setType(Ogre::Light::LT_POINT);
//            light->setDiffuseColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
//            light->setSpecularColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
//
//            auto *spotLightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
//            spotLightNode->attachObject(light);
//            spotLightNode->setDirection(-1, -1, 0);
//            spotLightNode->setPosition(Ogre::Vector3(0, 0, 0));
//
//            mSceneNodes[pGameObj->getID()] = spotLightNode;
//        } else {
//            createSceneNode(pGameObj);
//        }

        initOrUpdateNode(pGameObj);
    }
}

bool AmE::OgreOne::update(const AmE::SceneState *const sceneState) {
    if (mWindow->isVisible() && !mQuit) {
        for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
//            if (pGameObj->isCamera()) {
//                bool isNewObj = mSceneNodes.find(pGameObj->getID()) == mSceneNodes.end();
//
//                if (isNewObj)
//                    createCameraNode(pGameObj);
//                else
//                    updateCameraNode(pGameObj);
//            } else if (pGameObj->isLight()) {
//                // TODO: ???
//            } else {
//                bool isNewObj = mSceneNodes.find(pGameObj->getID()) == mSceneNodes.end();
//
//                if (isNewObj)
//                    createSceneNode(pGameObj);
//                else
//                    updateSceneNode(pGameObj);
//            }
            initOrUpdateNode(pGameObj);
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

void AmE::OgreOne::initOrUpdateNode(const AmE::GameObject *const pGameObject) {
    bool isNew = false;

    if (mSceneNodes.find(pGameObject->getID()) == mSceneNodes.end()) {
        std::string nodeName = "Node: " + pGameObject->getName();

        mSceneNodes[pGameObject->getID()] =
                mSceneManager
                        ->getRootSceneNode()
                        ->createChildSceneNode(nodeName);
        isNew = true;

        Logger::Trace("Create new node: " + pGameObject->getName());
    }

    auto *tr = pGameObject->getTransform();
    auto pos = GraphicsEngine::convertPositionLeftToRightHand(tr->mPosition);
    auto rot = GraphicsEngine::convertRotationLeftToRightHand(tr->mRotation);

    mSceneNodes[pGameObject->getID()]->setPosition(
            (Ogre::Real) pos.getX(),
            (Ogre::Real) pos.getY(),
            (Ogre::Real) pos.getZ()
    );

    mSceneNodes[pGameObject->getID()]->setOrientation(
            (Ogre::Real) rot.getW(),
            (Ogre::Real) rot.getX(),
            (Ogre::Real) rot.getY(),
            (Ogre::Real) rot.getZ()
    );

    if (!pGameObject->isCamera() && !pGameObject->isLight()) {
        auto &scale = tr->mLocalScale;

        mSceneNodes[pGameObject->getID()]->setScale(
                (Ogre::Real) scale.getX(),
                (Ogre::Real) scale.getY(),
                (Ogre::Real) scale.getZ()
        );
    }

    if (pGameObject->isCamera() && pGameObject->getComponent("Camera")->isEnabled()) {
        std::string cameraName = "Camera: " + pGameObject->getName();

        if (isNew) {
            Ogre::Camera *cam = mSceneManager->createCamera(cameraName);
            mWindow->addViewport(cam);
            mSceneNodes[pGameObject->getID()]->attachObject(cam);
        }

        Ogre::Camera *cam = dynamic_cast<Ogre::Camera *>(
                mSceneNodes[pGameObject->getID()]->getAttachedObject(cameraName)
        );

        auto nearClipPlane = pGameObject->getComponent("Camera")->mProperties.at("nearClipPlane").asDouble();
        auto farClipPlane = pGameObject->getComponent("Camera")->mProperties.at("farClipPlane").asDouble();

        cam->setNearClipDistance((Ogre::Real) nearClipPlane);
        cam->setFarClipDistance((Ogre::Real) farClipPlane);
        cam->setAutoAspectRatio(true);

        const unsigned short defaultViewport = 0;
        if (nullptr != mWindow->getViewport(defaultViewport)) {
            auto backgroundColor =
                    pGameObject->getComponent("Camera")->mProperties.at("backgroundColor").asColor();

            const Ogre::ColourValue backgroundColour(
                    (float) backgroundColor.mR,
                    (float) backgroundColor.mG,
                    (float) backgroundColor.mB,
                    (float) backgroundColor.mA);

            mWindow->getViewport(defaultViewport)->setBackgroundColour(backgroundColour);
        }

        return;
    }

    if (pGameObject->isLight() && pGameObject->getComponent("Light")->isEnabled()) {
        std::string lightName = "Light: " + pGameObject->getName();

        if (isNew) {
            auto *light = mSceneManager->createLight(lightName);

            auto *lightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
            lightNode->attachObject(light);
            mSceneNodes[pGameObject->getID()] = lightNode;

            Logger::Trace("Create new light: " + pGameObject->getName());
        }

        Ogre::Light *light = dynamic_cast<Ogre::Light *>(
                mSceneNodes[pGameObject->getID()]->getAttachedObject(lightName)
        );

        auto color = pGameObject->getComponent("Light")->mProperties["color"].asColor();
        light->setDiffuseColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
        light->setSpecularColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);

        auto type = Light::parse(pGameObject->getComponent("Light")->mProperties["type"].asString());
        switch (type) {
            case LightType::Point: {
                light->setType(Ogre::Light::LT_POINT);
                // TODO: set point light radius; can't find in Ogre docs how to do it
                light->setPowerScale(500);
                light->setSourceSize(500, 500);
                break;
            }
            case Directional:
                light->setType(Ogre::Light::LT_DIRECTIONAL);
                break;
            case Spot:
                light->setType(Ogre::Light::LT_SPOTLIGHT);
                light->setSpotlightRange(Ogre::Radian(30), Ogre::Radian(30), 1.0); // TODO: init with real values
                break;
        }


        return;
    }

    if (!pGameObject->getMeshPathname().empty()) {
        if (isNew) {
            Ogre::Entity *ent = mSceneManager->createEntity(pGameObject->getMeshPathname());
            mSceneNodes[pGameObject->getID()]->attachObject((Ogre::MovableObject *) ent);
        }
    }
}

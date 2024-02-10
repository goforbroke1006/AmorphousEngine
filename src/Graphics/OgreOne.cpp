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
        initOrUpdateNode(pGameObj);
    }
}

bool AmE::OgreOne::update(const AmE::SceneState *const sceneState) {
    if (mWindow->isVisible() && !mQuit) {
        for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
            initOrUpdateNode(pGameObj);
        }

        mQuit |= !mRoot->renderOneFrame();
    }

    return !mQuit;
}

void AmE::OgreOne::stop() {
    mRoot->saveConfig();
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

    Component *pCameraComponent = pGameObject->getComponent("Component/Camera");
    if (nullptr != pCameraComponent && pCameraComponent->isEnabled()) {
        std::string cameraName = "Camera: " + pGameObject->getName();

        if (isNew) {
            Ogre::Camera *cam = mSceneManager->createCamera(cameraName);
            mWindow->addViewport(cam);
            mSceneNodes[pGameObject->getID()]->attachObject(cam);
        }

        Ogre::Camera *cam = dynamic_cast<Ogre::Camera *>(
                mSceneNodes[pGameObject->getID()]->getAttachedObject(cameraName)
        );

        auto nearClipPlane = pCameraComponent->mProperties.at("nearClipPlane").asDouble();
        auto farClipPlane = pCameraComponent->mProperties.at("farClipPlane").asDouble();

        cam->setNearClipDistance((Ogre::Real) nearClipPlane);
        cam->setFarClipDistance((Ogre::Real) farClipPlane);
        cam->setAutoAspectRatio(true);

        const unsigned short defaultViewport = 0;
        if (nullptr != mWindow->getViewport(defaultViewport)) {
            auto backgroundColor =
                    pCameraComponent->mProperties.at("backgroundColor").asColor();

            const Ogre::ColourValue backgroundColour(
                    (float) backgroundColor.mR,
                    (float) backgroundColor.mG,
                    (float) backgroundColor.mB,
                    (float) backgroundColor.mA);

            mWindow->getViewport(defaultViewport)->setBackgroundColour(backgroundColour);
        }

        return;
    }

    Component *pLightComponent = pGameObject->getComponent("Component/Light");
    if (nullptr != pLightComponent && pLightComponent->isEnabled()) {
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

        auto color = pLightComponent->mProperties["color"].asColor();
        light->setDiffuseColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
        light->setSpecularColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);

        auto type = Light::parse(pLightComponent->mProperties["type"].asString());
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

    if (pGameObject->hasMeshRender()) {
        if (isNew) {
            Ogre::Entity *ent = mSceneManager->createEntity(
                    pGameObject->getComponent("Component/MeshRender")->mProperties["path"].asString()
            );
            mSceneNodes[pGameObject->getID()]->attachObject((Ogre::MovableObject *) ent);
        }
    }
}

//
// Created by goforbroke on 1/1/24.
//

#include "../../../include/Graphics/OgreNext/OgreNext.h"
#include "../../../include/Graphics/OgreNext/hlms.h"
#include "../../../include/Graphics/OgreNext/resources.h"

#include <OGRE/OgreItem.h>
#include <OGRE/OgreMeshManager2.h>

AmE::OgreNext::OgreNext(
        const std::string &pluginsCfgPathname,
        const std::string &projectRootPathname
) {
    mRoot = OGRE_NEW Ogre::Root(pluginsCfgPathname, "ogre.cfg", "Ogre.log");
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw std::runtime_error("Ogre config broken");

    mRoot->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
    mWindow = mRoot->initialise(true, "Hello Ogre-next");
    mWindowHnd = -1;
    mWindow->getCustomAttribute("WINDOW", &mWindowHnd);

    registerHlmsForEngine();

    //const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, "Hello Ogre-next SM");

    loadResourcesForProject(projectRootPathname);

    mWinListener = new WindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mWinListener);

    mCompositorManager = nullptr;
}

AmE::OgreNext::~OgreNext() {
    GraphicsEngine::~GraphicsEngine();

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mWinListener);
    delete mRoot;
}

void AmE::OgreNext::initialize(const std::map<GameObjectInstanceID, GameObject *> &gameObjects) {
    for (const auto &goPair: gameObjects) {
        GameObject *pGO = goPair.second;

        if (pGO->isCamera()) {
            createCameraNode(pGO);
        } else if (pGO->isLight()) {
            auto color = pGO->getComponents()["Light"].mProperties["color"].asColor();

            auto *light = mSceneManager->createLight();
            light->setType(Ogre::Light::LT_POINT);
            light->setDiffuseColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);
            light->setSpecularColour((Ogre::Real) color.mR, (Ogre::Real) color.mG, (Ogre::Real) color.mB);

            auto *spotLightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
            spotLightNode->attachObject(light);
            spotLightNode->setDirection(-1, -1, 0);
            spotLightNode->setPosition(Ogre::Vector3(200, 200, 0));

            mSceneNodes[pGO->getID()] = spotLightNode;
        } else {
            createSceneNode(pGO);
        }
    }
}

bool AmE::OgreNext::update(const std::map<GameObjectInstanceID, GameObject *> &gameObjects) {
    Ogre::WindowEventUtilities::messagePump();

    mQuit |= mWinListener->shouldQuit();

    if (mWindow->isVisible() && !mQuit) {
        for (const auto &goPair: gameObjects) {
            GameObject *pGO = goPair.second;

            if (pGO->isCamera()) {
                bool isNewObj = mCameraNodes.find(pGO->getID()) == mCameraNodes.end();

                if (isNewObj)
                    createCameraNode(pGO);
                else
                    updateCameraNode(pGO);
            } else if (pGO->isLight()) {
                // TODO: ???
            } else {
                bool isNewObj = mSceneNodes.find(pGO->getID()) == mSceneNodes.end();

                if (isNewObj)
                    createSceneNode(pGO);
                else
                    updateSceneNode(pGO);
            }
        }

        mQuit |= !mRoot->renderOneFrame();
    }

    return !mQuit;
}


void AmE::OgreNext::stop() {
    mRoot->saveConfig();

    mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->removeAndDestroyAllChildren();

    mSceneNodes.clear();
    mCameraNodes.clear();
}

void AmE::OgreNext::createCameraNode(const GameObject *const gameObjectPtr) {
    mCameraNodes[gameObjectPtr->getID()] = mSceneManager->createCamera(gameObjectPtr->getName());

    if (gameObjectPtr->getComponent("Camera").isEnabled()) {
        auto backgroundColor = std::any_cast<Color>(
                gameObjectPtr->getComponent("Camera").mProperties.at("backgroundColor").mValue);

        mCompositorManager = mRoot->getCompositorManager2();
        const Ogre::String workspaceName("Demo Workspace");
        const Ogre::ColourValue backgroundColour(
                (float) backgroundColor.mR,
                (float) backgroundColor.mG,
                (float) backgroundColor.mB,
                (float) backgroundColor.mA
        );
        mCompositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
        mCompositorManager->addWorkspace(
                mSceneManager,
                mWindow->getTexture(),
                mCameraNodes[gameObjectPtr->getID()],
                workspaceName,
                true);
    }

    updateCameraNode(gameObjectPtr);
}

void AmE::OgreNext::updateCameraNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->getTransform();

    auto pos = GraphicsEngine::convertPositionLeftToRightHand(tr->mPosition);
    auto rot = GraphicsEngine::convertRotationLeftToRightHand(tr->mRotation);

    mCameraNodes[gameObjectPtr->getID()]->setPosition(
            Ogre::Vector3(
                    (Ogre::Real) pos.mX,
                    (Ogre::Real) pos.mY,
                    (Ogre::Real) pos.mZ
            )
    );
    mCameraNodes[gameObjectPtr->getID()]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.mW,
                    (Ogre::Real) rot.mX,
                    (Ogre::Real) rot.mY,
                    (Ogre::Real) rot.mZ
            )
    );

    auto camProps = gameObjectPtr->getComponent("Camera").mProperties;

    auto nearClipPlane = std::any_cast<double>(camProps.at("nearClipPlane").mValue);
    auto farClipPlane = std::any_cast<double>(camProps.at("farClipPlane").mValue);

    mCameraNodes[gameObjectPtr->getID()]->setNearClipDistance((Ogre::Real) nearClipPlane);
    mCameraNodes[gameObjectPtr->getID()]->setFarClipDistance((Ogre::Real) farClipPlane);
    mCameraNodes[gameObjectPtr->getID()]->setAutoAspectRatio(true);

//    if (nullptr != mRoot->getRenderSystem()->getCurrentPassDescriptor()) {
//        // TODO: can't solve - getCurrentPassDescriptor returns NULL
//        // https://forums.ogre3d.org/viewtopic.php?p=552706
//
//        auto backgroundColor = std::any_cast<Color>(camProps.at("backgroundColor").mValue);
//        const Ogre::ColourValue backgroundColour(
//                (float) backgroundColor.mR,
//                (float) backgroundColor.mG,
//                (float) backgroundColor.mB,
//                (float) backgroundColor.mA);
//        mRoot->getRenderSystem()->getCurrentPassDescriptor()->setClearColour(backgroundColour);
//    }
}

void AmE::OgreNext::createSceneNode(const GameObject *const gameObjectPtr) {
    auto *sceneNode = mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
            ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
    sceneNode->setName(gameObjectPtr->getName());

    sceneNode->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
//    sceneNode->setOrientation(
//            Ogre::Quaternion(
//                    (Ogre::Real) rot.mW,
//                    (Ogre::Real) rot.mX,
//                    (Ogre::Real) rot.mY,
//                    (Ogre::Real) rot.mZ
//            )
//    );
    sceneNode->setScale(Ogre::Vector3(1.0, 1.0, 1.0));

    mSceneNodes[gameObjectPtr->getID()] = sceneNode;

    if (!gameObjectPtr->getMeshPathname().empty()) {
        std::string meshV2Pathname = gameObjectPtr->getMeshPathname() + " Imported";

        if (mMeshes.find(meshV2Pathname) == mMeshes.end()) {
            Ogre::v1::MeshPtr v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
                    gameObjectPtr->getMeshPathname(), Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                    Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);
            //Create a v2 mesh to import to, with a different name.
            Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().createByImportingV1(
                    meshV2Pathname, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    v1Mesh.get(), true, true, true);
            v2Mesh->load();
            v1Mesh->unload();

            mMeshes.insert(meshV2Pathname);
        }

        Ogre::Item *item = mSceneManager->createItem(
                meshV2Pathname,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                Ogre::SCENE_DYNAMIC);

        sceneNode->attachObject(item);
    }

//    updateSceneNode(gameObjectPtr);
}

void AmE::OgreNext::updateSceneNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->getTransform();

    auto pos = GraphicsEngine::convertPositionLeftToRightHand(tr->mPosition);
    auto rot = GraphicsEngine::convertRotationLeftToRightHand(tr->mRotation);
    auto &scale = tr->mLocalScale;

    mSceneNodes[gameObjectPtr->getID()]->setPosition(
            Ogre::Vector3(
                    (Ogre::Real) pos.mX,
                    (Ogre::Real) pos.mY,
                    (Ogre::Real) pos.mZ
            )
    );
    mSceneNodes[gameObjectPtr->getID()]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.mW,
                    (Ogre::Real) rot.mX,
                    (Ogre::Real) rot.mY,
                    (Ogre::Real) rot.mZ
            )
    );
    mSceneNodes[gameObjectPtr->getID()]->setScale(
            Ogre::Vector3(
                    (Ogre::Real) scale.mX,
                    (Ogre::Real) scale.mY,
                    (Ogre::Real) scale.mZ
            )
    );
}

size_t AmE::OgreNext::getWindowHnd() const {
    return mWindowHnd;
}

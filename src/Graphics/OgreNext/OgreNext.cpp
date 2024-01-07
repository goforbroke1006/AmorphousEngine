//
// Created by goforbroke on 1/1/24.
//

#include "../../../include/Graphics/OgreNext/OgreNext.h"
#include "../../../include/Graphics/OgreNext/hlms.h"
#include "../../../include/Graphics/OgreNext/resources.h"

#include <OGRE/OgreItem.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreMeshManager2.h>

#define POS_Z_CORRECTION (-1)

OgreNext::OgreNext(
        const std::string &pluginsCfgPathname,
        const std::string &resourcesCfgPathname
) {
    mRoot = OGRE_NEW Ogre::Root(pluginsCfgPathname, "ogre.cfg", "Ogre.log");
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw std::runtime_error("Ogre config broken");

    mRoot->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
    mWindow = mRoot->initialise(true, "Hello Ogre-next");

    registerHlms(resourcesCfgPathname);

    //const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, "Hello Ogre-next SM");

    loadResources(resourcesCfgPathname);

    mWinListener = new WindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mWinListener);

    mCompositorManager = nullptr;
}

OgreNext::~OgreNext() {
    GraphicsEngine::~GraphicsEngine();

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mWinListener);
    delete mRoot;
}

void OgreNext::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    for (const auto &goPair: gameObjects) {
        GameObject *pGO = goPair.second;

        if (pGO->isCamera()) {
            createCameraNode(pGO);
        } else {
            createSceneNode(pGO);
        }
    }
}

bool OgreNext::update(const std::map<std::string, GameObject *> &gameObjects) {
    Ogre::WindowEventUtilities::messagePump();

    mQuit |= mWinListener->shouldQuit();

    if (mWindow->isVisible() && !mQuit) {
        for (const auto &goPair: gameObjects) {
            GameObject *pGO = goPair.second;

            if (pGO->isCamera()) {
                bool isNewObj = mCameraNodes.find(pGO->mID) == mCameraNodes.end();

                if (isNewObj)
                    createCameraNode(pGO);
                else
                    updateCameraNode(pGO);
            } else {
                bool isNewObj = mSceneNodes.find(pGO->mID) == mSceneNodes.end();

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


void OgreNext::stop() {
    // TODO: implement me
    mRoot->saveConfig();
}

void OgreNext::createCameraNode(const GameObject *const gameObjectPtr) {
    mCameraNodes[gameObjectPtr->mID] = mSceneManager->createCamera(gameObjectPtr->mName);

    if (gameObjectPtr->mComponents.at("Camera").isEnabled()) {
        auto backgroundColor = std::any_cast<Color>(
                gameObjectPtr->mComponents.at("Camera").mProperties.at("backgroundColor").mValue);

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
                mCameraNodes[gameObjectPtr->mID],
                workspaceName,
                true);
    }

    updateCameraNode(gameObjectPtr);
}

void OgreNext::updateCameraNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->mTransform;

    auto &pos = tr->mPosition;
    auto &rot = tr->mRotation;

    mCameraNodes[gameObjectPtr->mID]->setPosition(
            Ogre::Vector3(
                    (Ogre::Real) pos.mX,
                    (Ogre::Real) pos.mY,
                    (Ogre::Real) pos.mZ * POS_Z_CORRECTION
            )
    );
    mCameraNodes[gameObjectPtr->mID]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.mW,
                    (Ogre::Real) rot.mX,
                    (Ogre::Real) rot.mY,
                    (Ogre::Real) rot.mZ
            )
    );

    auto &camProps = gameObjectPtr->mComponents.at("Camera").mProperties;

    auto nearClipPlane = std::any_cast<double>(camProps.at("nearClipPlane").mValue);
    auto farClipPlane = std::any_cast<double>(camProps.at("farClipPlane").mValue);

    mCameraNodes[gameObjectPtr->mID]->setNearClipDistance((Ogre::Real) nearClipPlane);
    mCameraNodes[gameObjectPtr->mID]->setFarClipDistance((Ogre::Real) farClipPlane);
    mCameraNodes[gameObjectPtr->mID]->setAutoAspectRatio(true);

    if (nullptr != mRoot->getRenderSystem()->getCurrentPassDescriptor()) {
        // TODO: can't solve - getCurrentPassDescriptor returns NULL
        // https://forums.ogre3d.org/viewtopic.php?p=552706

        auto backgroundColor = std::any_cast<Color>(
                gameObjectPtr->mComponents.at("Camera").mProperties.at("backgroundColor").mValue);
        const Ogre::ColourValue backgroundColour(
                (float) backgroundColor.mR,
                (float) backgroundColor.mG,
                (float) backgroundColor.mB,
                (float) backgroundColor.mA);
        mRoot->getRenderSystem()->getCurrentPassDescriptor()->setClearColour(backgroundColour);
    }
}

void OgreNext::createSceneNode(const GameObject *const gameObjectPtr) {
    auto *sceneNode = mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
            ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
    sceneNode->setName(gameObjectPtr->mID);
    mSceneNodes[gameObjectPtr->mID] = sceneNode;

    if (!gameObjectPtr->mMeshPathname.empty()) {
        std::string meshV2Pathname = gameObjectPtr->mMeshPathname + " Imported";

        Ogre::v1::MeshPtr v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
                gameObjectPtr->mMeshPathname, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);
        //Create a v2 mesh to import to, with a different name.
        Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().createByImportingV1(
                meshV2Pathname, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                v1Mesh.get(), true, true, true);
        v2Mesh->load();
        v1Mesh->unload();

        Ogre::Item *item = mSceneManager->createItem(
                meshV2Pathname,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                Ogre::SCENE_DYNAMIC);

        sceneNode->attachObject(item);
    }

    updateSceneNode(gameObjectPtr);
}

void OgreNext::updateSceneNode(const GameObject *const gameObjectPtr) {
    auto *tr = gameObjectPtr->mTransform;

    auto &pos = tr->mPosition;
    auto &rot = tr->mRotation;
    auto &scale = tr->mLocalScale;

    mSceneNodes[gameObjectPtr->mID]->setPosition(
            Ogre::Vector3(
                    (Ogre::Real) pos.mX,
                    (Ogre::Real) pos.mY,
                    (Ogre::Real) pos.mZ * POS_Z_CORRECTION
            )
    );
    mSceneNodes[gameObjectPtr->mID]->setOrientation(
            Ogre::Quaternion(
                    (Ogre::Real) rot.mW,
                    (Ogre::Real) rot.mX,
                    (Ogre::Real) rot.mY,
                    (Ogre::Real) rot.mZ
            )
    );
    mSceneNodes[gameObjectPtr->mID]->setScale(
            Ogre::Vector3(
                    (Ogre::Real) scale.mX,
                    (Ogre::Real) scale.mY,
                    (Ogre::Real) scale.mZ
            )
    );
}

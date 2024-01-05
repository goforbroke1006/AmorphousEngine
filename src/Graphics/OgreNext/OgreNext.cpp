//
// Created by goforbroke on 1/1/24.
//

#include "../../../include/Graphics/OgreNext/OgreNext.h"
#include "../../../include/Graphics/OgreNext/hlms.h"
#include "../../../include/Graphics/OgreNext/resources.h"

#include <OGRE/OgreItem.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreMeshManager2.h>

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
}

OgreNext::~OgreNext() {
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mWinListener);
    delete mRoot;
}

void OgreNext::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    for (const auto &goPair: gameObjects) {
        GameObject *pGO = goPair.second;

        auto *tr = pGO->mTransform;

        auto &pos = tr->mPosition;
        auto &rot = tr->mRotation;
        auto &scale = tr->mLocalScale;

        if (pGO->isCamera()) {
            mCamera = mSceneManager->createCamera(pGO->mName);
            mCamera->setPosition(Ogre::Vector3(pos.mX, pos.mY, pos.mZ));
            mCamera->lookAt(0, 0, 0); // TODO:
//            mCamera->setOrientation(Ogre::Quaternion(0.0, rot.mX, rot.mY, rot.mZ));
            mCamera->setNearClipDistance(0.2f);
            mCamera->setFarClipDistance(1000.0f);
            mCamera->setAutoAspectRatio(true);

            // The setup for a basic compositor with a blue clear colour
            Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();
            const Ogre::String workspaceName("Demo Workspace");
            const Ogre::ColourValue backgroundColour(0.2f, 0.4f, 0.6f); // TODO: set real color
            compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
            compositorManager->addWorkspace(mSceneManager, mWindow->getTexture(), mCamera, workspaceName, true);

//            mSceneNodes[pGO->mID] = mCamera;
            // TODO: ???
        } else {
            auto *sceneNode = mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
                    ->createChildSceneNode(Ogre::SCENE_DYNAMIC);

            sceneNode->setName(pGO->mID);
            sceneNode->setPosition(Ogre::Vector3(pos.mX, pos.mY, pos.mZ));
            sceneNode->setOrientation(Ogre::Quaternion(1.0, rot.mX, rot.mY, rot.mZ));
            sceneNode->setScale(Ogre::Vector3(scale.mX, scale.mY, scale.mZ));

            if (!pGO->mMeshPathname.empty()) {
                Ogre::v1::MeshPtr v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
                        pGO->mMeshPathname, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                        Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);
                //Create a v2 mesh to import to, with a different name.
                Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().createByImportingV1(
                        pGO->mMeshPathname + " Imported", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                        v1Mesh.get(), true, true, true);
                v2Mesh->load();
                v1Mesh->unload();

                Ogre::Item *item = mSceneManager
                        ->createItem(pGO->mMeshPathname + " Imported",
                                     Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                                     Ogre::SCENE_DYNAMIC);

                sceneNode->attachObject(item);
            }

            mSceneNodes[pGO->mID] = sceneNode;
        }
    }
}

bool OgreNext::update(const std::map<std::string, GameObject *> &gameObjects) {
    Ogre::WindowEventUtilities::messagePump();

    mQuit |= mWinListener->shouldQuit();

    if (mWindow->isVisible() && !mQuit) {
        for (const auto &goPair: gameObjects) {
            GameObject *pGO = goPair.second;

            auto *tr = pGO->mTransform;

            auto &pos = tr->mPosition;
            auto &rot = tr->mRotation;
            auto &scale = tr->mLocalScale;

            if (mSceneNodes.find(pGO->mID) == mSceneNodes.end())
                continue; // TODO: ???

            mSceneNodes[pGO->mID]->setPosition(Ogre::Vector3(pos.mX, pos.mY, pos.mZ));
            mSceneNodes[pGO->mID]->setOrientation(Ogre::Quaternion(1.0, rot.mX, rot.mY, rot.mZ));
            mSceneNodes[pGO->mID]->setScale(Ogre::Vector3(scale.mX, scale.mY, scale.mZ));
        }

        mQuit |= !mRoot->renderOneFrame();
    }

    return !mQuit;
}

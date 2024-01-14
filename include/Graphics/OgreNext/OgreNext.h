//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_OGRE_NEXT_H
#define AMORPHOUS_ENGINE_OGRE_NEXT_H

#include <string>
#include <map>
#include <set>

#include "../../Core/GameObject.h"
#include "../../GraphicsEngine.h"
#include "WindowEventListener.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreWindow.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgrePlatformInformation.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>
#include <OGRE/OgreMesh2.h>

namespace AmE {
    class OgreNext : public GraphicsEngine {
    public:
        explicit OgreNext(
                const std::string &pluginsCfgPathname,
                const std::string &resourcesCfgPathname);

        virtual ~OgreNext();

        [[nodiscard]] size_t getWindowHnd() const override;

        void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

        bool update(const std::map<std::string, GameObject *> &gameObjects) override;

        void stop() override;

    private:
        void createCameraNode(const GameObject *gameObjectPtr);

        void updateCameraNode(const GameObject *gameObjectPtr);

        void createSceneNode(const GameObject *gameObjectPtr);

        void updateSceneNode(const GameObject *gameObjectPtr);

    private:
        bool mQuit = false;

        Ogre::Root *mRoot;
        Ogre::Window *mWindow;
        size_t mWindowHnd;
        Ogre::SceneManager *mSceneManager;
        Ogre::CompositorManager2 *mCompositorManager;

        WindowEventListener *mWinListener;

        std::map<std::string, Ogre::Camera *> mCameraNodes;
        std::map<std::string, Ogre::SceneNode *> mSceneNodes;
        std::set<std::string> mMeshes;
    };
}

#endif //AMORPHOUS_ENGINE_OGRE_NEXT_H

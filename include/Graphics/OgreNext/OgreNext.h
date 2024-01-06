//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_OGRE_NEXT_H
#define AMORPHOUS_ENGINE_OGRE_NEXT_H

#include <string>
#include <map>

#include "../../types.h"
#include "../../GameObject.h"
#include "../../GraphicsEngine.h"
#include "WindowEventListener.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreWindow.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgrePlatformInformation.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>

class OgreNext : public GraphicsEngine {
public:
    explicit OgreNext(
            const std::string &pluginsCfgPathname,
            const std::string &resourcesCfgPathname);

    virtual ~OgreNext() ;

    void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

    bool update(const std::map<std::string, GameObject *> &gameObjects) override;

private:
    bool mQuit = false;

    Ogre::Root *mRoot;
    Ogre::Window *mWindow;
    Ogre::SceneManager *mSceneManager;
    Ogre::Camera *mCamera;

    WindowEventListener *mWinListener;

    std::map<std::string, Ogre::SceneNode *> mSceneNodes;
};

#endif //AMORPHOUS_ENGINE_OGRE_NEXT_H

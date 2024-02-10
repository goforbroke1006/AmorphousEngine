//
// Created by goforbroke on 2/6/24.
//

#ifndef AMORPHOUSENGINE_OGREONE_H
#define AMORPHOUSENGINE_OGREONE_H

#include "../GraphicsEngine.h"

#include <string>

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreShaderGenerator.h>

namespace AmE {
    class OgreOne : public GraphicsEngine {
    public:
        OgreOne(const std::string &pluginsCfgPathname,
                const std::string &projectRoot);

        virtual ~OgreOne();

        size_t getWindowHnd() const override;

        void initialize(const SceneState *const sceneState) override;

        bool update(const SceneState *const sceneState) override;

        void stop() override;

    private:
        void initOrUpdateNode(const AmE::GameObject *const pGameObject);

        void createCameraNode(const GameObject *gameObjectPtr);

        void updateCameraNode(const GameObject *gameObjectPtr);

        void createSceneNode(const GameObject *gameObjectPtr);

        void updateSceneNode(const GameObject *gameObjectPtr);

    private:
        Ogre::Root *mRoot;
        Ogre::RenderWindow *mWindow;
        size_t mWindowHnd;
        Ogre::SceneManager *mSceneManager;
        Ogre::RTShader::ShaderGenerator *mShaderGenerator;

//        std::map<GameObjectInstanceID, Ogre::Camera *> mCameraNodes;
        std::map<GameObjectInstanceID, Ogre::SceneNode *> mSceneNodes;

        bool mQuit;
    };
}

#endif //AMORPHOUSENGINE_OGREONE_H

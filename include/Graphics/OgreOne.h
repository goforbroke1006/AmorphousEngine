//
// Created by goforbroke on 2/6/24.
//

#ifndef AMORPHOUS_ENGINE_OGRE_ONE_H
#define AMORPHOUS_ENGINE_OGRE_ONE_H

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

        [[nodiscard]] size_t getWindowHnd() const override;

        void initialize(const SceneState *sceneState) override;

        bool update(SceneState *sceneState) override;

        void stop() override;

    private:
        void initOrUpdateNode(const AmE::GameObject *pGameObject);

        void removeNode(const AmE::GameObject *pGameObject);

    private:
        Ogre::Root *mRoot;
        Ogre::RenderWindow *mWindow;
        size_t mWindowHnd;
        Ogre::SceneManager *mSceneManager;
        Ogre::RTShader::ShaderGenerator *mShaderGenerator;

        std::map<GameObjectInstanceID, Ogre::SceneNode *> mSceneNodes;

        bool mQuit;
    };
}

#endif //AMORPHOUS_ENGINE_OGRE_ONE_H

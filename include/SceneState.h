//
// Created by goforbroke on 1/20/24.
//

#ifndef AMORPHOUS_ENGINE_SCENE_STATE_H
#define AMORPHOUS_ENGINE_SCENE_STATE_H

#include <string>
#include <map>
#include <utility>
#include <set>

#include "Core/GameObject.h"
#include "Core/KeyCode.h"

namespace AmE {
    /**
     * SceneState is an DTO to share scene content between LUA and graphics engine.
     */
    class SceneState {
    public:
        explicit SceneState();

        virtual ~SceneState();

        [[nodiscard]] const std::map<std::string, Component *> &getComponents() const;

        [[nodiscard]]
        const std::map<GameObjectInstanceID, GameObject *> &
        getSceneGameObjects() const;

        [[nodiscard]]
        GameObject *
        getSceneGameObject(GameObjectInstanceID id) const;

        void addSceneGameObject(GameObject *pGameObj);

        void markSceneGameObjectAsRemoved(GameObjectInstanceID id);

        [[nodiscard]]
        const std::map<GameObjectInstanceID, GameObject *> &
        getSceneObjectForRemove() const;

        void removeSceneObjectFinally(GameObjectInstanceID id);

        [[nodiscard]]
        const std::map<std::string, GameObject *> &
        getPrefabGameObjects() const;

        [[nodiscard]]
        bool isAppQuit() const;

        void setAppQuit(bool appQuit);

        static SceneState *loadFromFile(
                std::string projectRoot,
                const std::string &jsonFilepath
        );

    private:
        std::map<std::string, Component *> mComponents;
        std::map<GameObjectInstanceID, GameObject *> mSceneGameObjectsActive;
        std::map<std::string, GameObject *> mPrefabGameObjects;
        bool mAppQuit;
        std::map<GameObjectInstanceID, GameObject *> mSceneObjectRemoved;
    };
}

#endif //AMORPHOUS_ENGINE_SCENE_STATE_H

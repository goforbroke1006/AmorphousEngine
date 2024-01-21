//
// Created by goforbroke on 1/20/24.
//

#ifndef AMORPHOUS_ENGINE_SCENE_STATE_H
#define AMORPHOUS_ENGINE_SCENE_STATE_H

#include <string>
#include <map>
#include <utility>
#include "Core/GameObject.h"
#include "Core/KeyCode.h"

namespace AmE {
    /**
     * SceneState is an DTO to share scene content between LUA and graphics engine.
     */
    class SceneState {
    public:
        explicit SceneState(std::map<GameObjectInstanceID, GameObject *> gameObjects) {
            this->gameObjects = std::move(gameObjects);
            this->appQuit = false;
        }

        std::map<GameObjectInstanceID, GameObject *> gameObjects;
        bool appQuit;
    };
}

#endif //AMORPHOUS_ENGINE_SCENE_STATE_H

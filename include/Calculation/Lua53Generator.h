//
// Created by goforbroke on 1/25/24.
//

#ifndef AMORPHOUS_ENGINE_LUA53GENERATOR_H
#define AMORPHOUS_ENGINE_LUA53GENERATOR_H

#include <string>

#include "../SceneState.h"

namespace AmE {
    class Lua53Generator {
    public:
        static std::string buildInitLuaCode(
                const SceneState *sceneState
        );

        static std::string generateInitCode(
                const std::string &luaRef,
                const GameObject *pGameObj,
                bool isSceneObject
        );

        static std::string simplePropValToLuaCode(const Property &prop);
    };
}

#endif //AMORPHOUS_ENGINE_LUA53GENERATOR_H

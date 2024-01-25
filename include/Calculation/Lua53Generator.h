//
// Created by goforbroke on 1/25/24.
//

#ifndef AMORPHOUS_ENGINE_LUA53GENERATOR_H
#define AMORPHOUS_ENGINE_LUA53GENERATOR_H

#include <string>

#include "../SceneState.h"

#define LUA53_G_VAR_GO_T "__all_game_objects"
#define LUA53_G_VAR_CMP_T "__all_components"
#define LUA53_G_VAR_BTN_P_T "__global_buttons_pressed"
#define LUA53_G_VAR_BTN_R_T "__global_buttons_released"
#define LUA53_G_VAR_APP_QUIT "__application_quit"
#define LUA53_G_VAR_PREFABS "__global_prefab_game_objects"

namespace AmE {
    class Lua53Generator {
    public:
        static std::string buildInitLuaCode(
                const SceneState *const sceneState
        );

        static std::string generateInitCode(
                const std::string &luaRef,
                const GameObject *const pGameObj,
                bool isSceneObject
        );

        static std::string simplePropValToLuaCode(const Property &prop);
    };
}

#endif //AMORPHOUS_ENGINE_LUA53GENERATOR_H

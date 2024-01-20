//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_LUA53_H
#define AMORPHOUS_ENGINE_LUA53_H

#include <vector>
#include <string>

#include <LuaCpp/Engine/LuaState.hpp>
#include <LuaCpp/Engine/LuaTTable.hpp>
#include <LuaCpp/Engine/LuaTBoolean.hpp>

#include "../Core/GameObject.h"
#include "../Core/KeyCode.h"
#include "../CalculationEngine.h"

#define LUA53_G_VAR_GO_T "__all_game_objects"
#define LUA53_G_VAR_CMP_T "__all_components"
#define LUA53_G_VAR_BTN_P_T "__global_buttons_pressed"
#define LUA53_G_VAR_BTN_R_T "__global_buttons_released"
#define LUA53_G_VAR_APP_QUIT "__application_quit"

namespace AmE {
    class Lua53 : public CalculationEngine {
    public:
        explicit Lua53(const std::string &projectRoot);

        virtual ~Lua53();

        void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

        void update(
                std::map<std::string, GameObject *> &gameObjects,
                const std::map<KeyCode, bool> &keysPressed,
                const std::map<KeyCode, bool> &keysReleased,
                bool &appQuit
        ) override;

        static std::string buildInitLuaCode(const std::map<std::string, GameObject *> &gameObjects);

        static std::string propValToLuaCode(const Property &prop);

        static std::any parsePropValFromLua(const PropType::Kind &kind, LuaCpp::Engine::LuaType *rawLuaVal);

    private:
        std::unique_ptr<LuaCpp::Engine::LuaState> L;

        LuaCpp::Engine::LuaTTable mGameObjectsTbl;
        LuaCpp::Engine::LuaTTable mComponentsTbl;
        LuaCpp::Engine::LuaTTable mBtnPressedTbl;
        LuaCpp::Engine::LuaTTable mBtnReleasedTbl;
        LuaCpp::Engine::LuaTBoolean *mAppQuit;
    };
}

#endif //AMORPHOUS_ENGINE_LUA53_H

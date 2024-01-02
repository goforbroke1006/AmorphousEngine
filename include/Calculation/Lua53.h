//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_LUA53_H
#define AMORPHOUS_ENGINE_LUA53_H

#include <vector>
#include <string>

#include <LuaCpp.hpp>

#include "../types.h"
#include "../GameObject.h"
#include "../CalculationEngine.h"

#define LUA53_G_VAR_GO_T "allGameObjects"
#define LUA53_G_VAR_CMP_T "allComponents"

class Lua53 : public CalculationEngine {
public:
    explicit Lua53(const std::string &projectRoot);

    virtual ~Lua53();

    void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

    void update(std::map<std::string, GameObject *> &gameObjects) override;

    std::string buildInitLuaCode(const std::map<std::string, GameObject *> &gameObjects);

private:
    std::unique_ptr<LuaCpp::Engine::LuaState> L;

    LuaCpp::Engine::LuaTTable mGameObjectsTbl;
    LuaCpp::Engine::LuaTTable mComponentsTbl;
};

#endif //AMORPHOUS_ENGINE_LUA53_H

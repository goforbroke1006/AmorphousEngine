//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_LUA53_H
#define OGRITY_LUA53_H

#include <vector>
#include <string>

#include <LuaCpp.hpp>

#include "../types.h"
#include "../GameObject.h"
#include "../CalculationEngine.h"

class Lua53 : public CalculationEngine {
public:
    void initialize(const std::vector<GameObject *> &gameObjects) override;

    std::vector<GameObject *> update() override;

    std::string buildInitLuaCode(const std::vector<GameObject *> &gameObjects);

private:
    LuaCpp::LuaContext ctx;
    std::unique_ptr<LuaCpp::Engine::LuaState> L;

    LuaCpp::Engine::LuaTTable mGameObjectsTbl;
    LuaCpp::Engine::LuaTTable mComponentsTbl;

    std::string mEngineRoot;
};

#endif //OGRITY_LUA53_H

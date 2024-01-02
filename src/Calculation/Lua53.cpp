//
// Created by goforbroke on 1/1/24.
//

#include "../../include/Logger.h"
#include "../../include/Calculation/Lua53.h"

#include <string>

using LETKey = LuaCpp::Engine::Table::Key;
using LENum = LuaCpp::Engine::LuaTNumber;
using LEString = LuaCpp::Engine::LuaTString;
using LETable = LuaCpp::Engine::LuaTTable;

void Lua53::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    L = ctx.newState();

    mGameObjectsTbl.PushGlobal(*L, LUA53_G_VAR_GO_T);
    mComponentsTbl.PushGlobal(*L, LUA53_G_VAR_CMP_T);

    // 1) Include LUA modules from 'Core'
    // 2) Create all game objects (set name, set transform)
    // 3) Create all components (set gameObject, set transform)
    // 4) Set each component with args (static values, references to gameObject or transforms)
    // 5) Run 'Start()' method for each component


    const std::string &initCode = buildInitLuaCode(gameObjects);
    luaL_loadstring(*L, initCode.c_str());

    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
//        Logger::Error(lua_tostring(*L, 1));
//        std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
//                  << lua_error(*L) << std::endl;
//        L->PrintStack(std::cerr); // TODO:

        throw std::runtime_error(lua_tostring(*L, 1));
    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);

    Logger::Debug("Scene initialization finished...");
    Logger::Debug("Game objects: " + std::to_string(mGameObjectsTbl.getValues().size()));
    Logger::Debug("Components:   " + std::to_string(mComponentsTbl.getValues().size()));
}

void Lua53::update(std::map<std::string, GameObject *> &gameObjects) {
    std::string updateFrameCode
            = std::string() +
              "for _, cmpInstance in pairs(" + LUA53_G_VAR_CMP_T + ") do \n"
                                                                             "     cmpInstance:Update() \n"
                                                                             "end \n";
    luaL_loadstring(*L, updateFrameCode.c_str());

    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
        throw std::runtime_error(lua_tostring(*L, 1));
    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);

    for (const auto &goState: mGameObjectsTbl.getValues()) {
        const std::string goID = goState.first.ToString();

        auto &name = (LEString &) ((LETable &) goState.second).getValue(LETKey("name"));
        auto &transform = (LETable &) ((LETable &) goState.second).getValue(LETKey("transform"));
        auto &position = (LETable &) transform.getValue(LETKey("position"));

        if (gameObjects.find(goID) == gameObjects.end()) {
            gameObjects[goID] = GameObject::build(name.getValue());
        }

        gameObjects[goID]->mTransform->mPosition.mX = ((LENum &) position.getValue(LETKey("x"))).getValue();
        gameObjects[goID]->mTransform->mPosition.mY = ((LENum &) position.getValue(LETKey("y"))).getValue();
        gameObjects[goID]->mTransform->mPosition.mZ = ((LENum &) position.getValue(LETKey("z"))).getValue();
    }
}

std::string Lua53::buildInitLuaCode(const std::map<std::string, GameObject *> &gameObjects) {
    if (gameObjects.empty())
        return "";

    std::string initCode;
    initCode += "require 'Core'\n"
                "\n";

    for (const auto &goPair: gameObjects) {
        auto *go = goPair.second;

        auto pos = go->mTransform->mPosition;
        auto rot = go->mTransform->mRotation;

        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'] = GameObject:new('" + go->mName + "')\n";
        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.position:Set("
                    + std::to_string(pos.mX)
                    + ", "
                    + std::to_string(pos.mY)
                    + ", "
                    + std::to_string(pos.mZ) + ")\n";
        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.rotation:Set("
                    + std::to_string(rot.mX)
                    + ", "
                    + std::to_string(rot.mY)
                    + ", "
                    + std::to_string(rot.mZ)
                    + ", 0.0)\n";
        initCode += "\n";
    }

    for (const auto &goPair: gameObjects) {
        auto *go = goPair.second;

        for (const auto &cmp: go->mComponents) {
            initCode += "require '" + cmp.mPathname + "'\n";
            initCode += "\n";
            initCode += "local cmp = " + cmp.mName + "\n";
            initCode += std::string()
                        + "cmp.gameObject = " + LUA53_G_VAR_GO_T + "['" + go->mID + "']\n";
            initCode += std::string()
                        + "cmp.transform  = " + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + go->mID + " :: " + cmp.mName + "'] = cmp\n";
            initCode += "\n";

            for (const auto &prop: cmp.mProperties) {
                initCode += std::string()
                            + LUA53_G_VAR_CMP_T + "['" + go->mID + " :: " + cmp.mName + "']." + prop.mName +
                            " = " + prop.mValue +
                            "\n";
            }
            initCode += "\n";
        }
    }

    initCode += std::string()
                + "for _, cmpInstance in pairs(" + LUA53_G_VAR_CMP_T + ") do\n"
                                                                                 "    cmpInstance:Start()\n"
                                                                                 "end\n";

    return initCode;
}
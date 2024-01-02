//
// Created by goforbroke on 1/1/24.
//

#include "../../include/Calculation/Lua53.h"

#include <string>

using LETKey = LuaCpp::Engine::Table::Key;
using LENum = LuaCpp::Engine::LuaTNumber;
using LETable = LuaCpp::Engine::LuaTTable;

void Lua53::initialize(const std::vector<GameObject *> &gameObjects) {
    L = ctx.newState();

    mGameObjectsTbl.PushGlobal(*L, "allGameObjects");
    mComponentsTbl.PushGlobal(*L, "allComponents");

    // 1) Include LUA modules from 'Core'
    // 2) Create all game objects (set name, set transform)
    // 3) Create all components (set gameObject, set transform)
    // 4) Set each component with args (static values, references to gameObject or transforms)
    // 5) Run 'Start()' method for each component


    const std::string &initCode = this->buildInitLuaCode(gameObjects);
    luaL_loadstring(*L, initCode.c_str());

    int res = lua_pcall(*L, 0, 0, 0);
//        if (res != LUA_OK) {
//            std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
//                      << lua_error(*L) << std::endl;
//            L->PrintStack(std::cout);
//        }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);

//        std::cout << "Scene initialization finished..." << std::endl;
//        std::cout << "Game objects: " << allGameObjectsTbl.getValues().size() << std::endl;
//        std::cout << "Components:   " << allComponentsTbl.getValues().size() << std::endl;
}

std::vector<GameObject *> Lua53::update() {
    luaL_loadstring(*L,

                    "for cmpName, cmpInstance in pairs(allComponents) do \n"
                    "     cmpInstance:Update() \n"
                    "end \n"
    );

    int res = lua_pcall(*L, 0, 0, 0);
//    if (res != LUA_OK) {
//        std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
//                  << lua_error(*L) << std::endl;
//        L->PrintStack(std::cout);
//
//        continue;
//    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);

    // extract transforms

    std::vector<GameObject *> result;

    // TODO:
//    for (const auto &goState: mGameObjectsTbl.getValues()) {
//        auto *gameObject = new GameObject(0, goState.first.ToString()); // TODO:
//
//        auto &transform = (LETable &) ((LETable &) goState.second).getValue(LETKey("transform"));
//        auto &position = (LETable &) transform.getValue(LETKey("position"));
//
//        gameObject->mTransform->mPosition.mX = ((LENum &) position.getValue(LETKey("x"))).getValue();
//        gameObject->mTransform->mPosition.mY = ((LENum &) position.getValue(LETKey("y"))).getValue();
//        gameObject->mTransform->mPosition.mZ = ((LENum &) position.getValue(LETKey("z"))).getValue();
//    }

    return result;
}

std::string Lua53::buildInitLuaCode(const std::vector<GameObject *> &gameObjects) {
    if (gameObjects.empty())
        return "";

    std::string initCode;
    initCode += "require 'Core'\n"
                "\n";

    for (const auto &go: gameObjects) {
        initCode += "allGameObjects['" + go->mID + "'] = GameObject:new('" + go->mName + "')\n";
        initCode += "allGameObjects['" + go->mID + "'].transform.position:Set(0.0, 0.0, 0.0)\n";
        initCode += "allGameObjects['" + go->mID + "'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0)\n";
        initCode += "\n";
    }

    for (const auto &go: gameObjects) {
        for (const auto &cmp: go->mComponents) {
            initCode += "require '" + mEngineRoot + "/" + cmp.mPathname + "'\n";
            initCode += "\n";
            initCode += "local cmp = " + cmp.mName + "\n";
            initCode += "cmp.gameObject = allGameObjects['" + go->mID + "']\n";
            initCode += "cmp.transform  = allGameObjects['" + go->mID + "'].transform\n";
            initCode += "allComponents['" + go->mID + " :: " + cmp.mName + "'] = cmp\n";
            initCode += "\n";

            for (const auto &prop: cmp.mProperties) {
                initCode += "allComponents['" + go->mID + " :: " + cmp.mName + "']." + prop.mName + " = " + prop.mValue + "\n";
            }
            initCode += "\n";
        }
    }

    initCode += "for cmpName, cmpInstance in pairs(allComponents) do\n"
                "    cmpInstance:Start()\n"
                "end\n";

    return initCode;
}
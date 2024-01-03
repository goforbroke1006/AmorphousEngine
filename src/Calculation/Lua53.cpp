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

Lua53::Lua53(const std::string &projectRoot) {
    LuaCpp::LuaContext ctx;
    L = ctx.newState();

    mGameObjectsTbl.PushGlobal(*L, LUA53_G_VAR_GO_T);
    mComponentsTbl.PushGlobal(*L, LUA53_G_VAR_CMP_T);

    std::string setLuaPathCode;
    setLuaPathCode += "package.path = package.path .. ';" + projectRoot + "?.lua'\n";
    setLuaPathCode += "print(package.path)\n";

    luaL_loadstring(*L, setLuaPathCode.c_str());
    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
        throw std::runtime_error(lua_tostring(*L, 1));
    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);
}

Lua53::~Lua53() {
//    lua_close(*L);
}

void Lua53::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    // 1) Include LUA modules from 'Core'
    // 2) Create all game objects (set name, set transform)
    // 3) Create all components (set gameObject, set transform)
    // 4) Set each component with args (static values, references to gameObject or transforms)
    // 5) Run 'Start()' method for each component

    const std::string &initCode = buildInitLuaCode(gameObjects);
    luaL_loadstring(*L, initCode.c_str());
    Logger::Trace("\n" + initCode);

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
        auto *goTbl = (LETable *) goState.second.get();

        auto &idVal = goTbl->getValue(LETKey("id"));
        auto id = ((LEString &) idVal).getValue();

        auto &transform = (LETable &) goTbl->getValue(LETKey("transform"));

        auto &position = (LETable &) transform.getValue(LETKey("position"));
        auto &rotation = (LETable &) transform.getValue(LETKey("rotation"));
        auto &localScale = (LETable &) transform.getValue(LETKey("localScale"));

        if (gameObjects.find(id) == gameObjects.end()) {
            auto &nameVal = ((LEString &) goTbl->getValue(LETKey("name")));

            gameObjects[id] = new GameObject(id, nameVal.getValue());
        }

        gameObjects[id]->mTransform->mPosition.Set(
                ((LENum &) position.getValue(LETKey("x"))).getValue(),
                ((LENum &) position.getValue(LETKey("y"))).getValue(),
                ((LENum &) position.getValue(LETKey("z"))).getValue()
        );
        gameObjects[id]->mTransform->mRotation.Set(
                ((LENum &) rotation.getValue(LETKey("x"))).getValue(),
                ((LENum &) rotation.getValue(LETKey("y"))).getValue(),
                ((LENum &) rotation.getValue(LETKey("z"))).getValue()
        );
        gameObjects[id]->mTransform->mLocalScale.Set(
                ((LENum &) localScale.getValue(LETKey("x"))).getValue(),
                ((LENum &) localScale.getValue(LETKey("y"))).getValue(),
                ((LENum &) localScale.getValue(LETKey("z"))).getValue()
        );
    }
}

std::string Lua53::buildInitLuaCode(const std::map<std::string, GameObject *> &gameObjects) {
    if (gameObjects.empty())
        return "";

    std::string initCode;
    initCode += "require 'Core'\n"
                "\n";

//    initCode += std::string()
//                + LUA53_G_VAR_GO_T + " = {}\n"
//                + LUA53_G_VAR_CMP_T + " = {}\n"
//                                      "\n";

    for (const auto &goPair: gameObjects) {
        auto *go = goPair.second;

        auto pos = go->mTransform->mPosition;
        auto rot = go->mTransform->mRotation;
        auto scale = go->mTransform->mLocalScale;

        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'] = GameObject:new('" + go->mID + "', '" + go->mName +
                    "')\n";
        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.position:Set("
                    + std::to_string(pos.mX)
                    + ", "
                    + std::to_string(pos.mY)
                    + ", "
                    + std::to_string(pos.mZ)
                    + ")\n";
        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.rotation:Set("
                    + std::to_string(rot.mX)
                    + ", "
                    + std::to_string(rot.mY)
                    + ", "
                    + std::to_string(rot.mZ)
                    + ", 1.0)\n";
        initCode += std::string()
                    + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.localScale:Set("
                    + std::to_string(scale.mX)
                    + ", "
                    + std::to_string(scale.mY)
                    + ", "
                    + std::to_string(scale.mZ)
                    + ")\n";
        initCode += "\n";
    }

    for (const auto &goPair: gameObjects) {
        auto *go = goPair.second;

        for (const auto &cmp: go->mComponents) {
            std::string cmpID = go->mID + " :: " + cmp.mName;

            initCode += "require '" + cmp.mPathname + "'\n";
            initCode += "\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'] = " + cmp.mName + "\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].gameObject = " + LUA53_G_VAR_GO_T + "['" + go->mID +
                        "']\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].transform  = " + LUA53_G_VAR_GO_T + "['" + go->mID +
                        "'].transform\n";
            initCode += "\n";

            for (const auto &prop: cmp.mProperties) {
                initCode += std::string()
                            + LUA53_G_VAR_CMP_T + "['" + cmpID + "']." + prop.mName +
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

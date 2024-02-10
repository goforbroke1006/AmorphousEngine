//
// Created by goforbroke on 1/1/24.
//

#include "../../include/Calculation/Lua53.h"

#include <string>
#include <map>

#include <LuaCpp/LuaContext.hpp>
#include <LuaCpp/Engine/LuaTNumber.hpp>
#include <LuaCpp/Engine/LuaTString.hpp>
#include <LuaCpp/Engine/LuaTBoolean.hpp>

#include "../../include/Calculation/Lua53Generator.h"

using LETKey = LuaCpp::Engine::Table::Key;
using LENum = LuaCpp::Engine::LuaTNumber;
using LEString = LuaCpp::Engine::LuaTString;
using LEBoolean = LuaCpp::Engine::LuaTBoolean;
using LETable = LuaCpp::Engine::LuaTTable;

AmE::Lua53::Lua53(const std::string &engineRoot, const std::string &projectRoot) {
    LuaCpp::LuaContext ctx;
    L = ctx.newState();

    mAppQuit = new LEBoolean(false);
    mTimeDelta = new LENum(0.0);

    mGameObjectsTbl.PushGlobal(*L, LUA53_G_VAR_GO_T);
    mComponentsTbl.PushGlobal(*L, LUA53_G_VAR_CMP_T);
    mBtnPressedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_P_T);
    mBtnReleasedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_R_T);
    mAppQuit->PushGlobal(*L, LUA53_G_VAR_APP_QUIT);
    mTimeDelta->PushGlobal(*L, LUA53_G_VAR_TIME_DELTA);

    std::string setLuaPathCode;
    setLuaPathCode += "package.path = package.path .. ';" + engineRoot + "/?.lua;" + projectRoot + "/?.lua'\n";
//    setLuaPathCode += "package.path = package.path .. ';" + projectRoot + "?.lua'\n";
    setLuaPathCode += "print(package.path)\n";

    luaL_loadstring(*L, setLuaPathCode.c_str());
    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
        throw std::runtime_error(lua_tostring(*L, 1));
    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);
}

AmE::Lua53::~Lua53() {
    delete mAppQuit;
//    lua_close(*L);
}

void AmE::Lua53::initialize(const SceneState *const sceneState) {
    // 1) Include LUA modules from 'Core'
    // 2) Create all game objects (set name, set transform)
    // 3) Create all components (set gameObject, set transform)
    // 4) Set each component with args (static values, references to gameObject or transforms)
    // 5) Run 'Start()' method for each component

    const std::string &initCode = Lua53Generator::buildInitLuaCode(sceneState);
    luaL_loadstring(*L, initCode.c_str());
//    Logger::Trace("\n" + initCode);

    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
//        Logger::Error(lua_tostring(*L, 1));
//        std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
//                  << lua_error(*L) << std::endl;
        L->PrintStack(std::cerr); // TODO:

        throw std::runtime_error(lua_tostring(*L, 1));
    }

//    {
//        std::string prefabsLoadingCode;
//
//        for (const auto &[path, pGO]: sceneState->getPrefabGameObjects()) {
//            prefabsLoadingCode +=
//                    "__global_prefab_game_objects['" + path + "'] = GameObject:new(-1, '" + pGO->getName() + "')\n"
//                    + "__global_prefab_game_objects['" + path + "'].transform.position";
//        }
//
//        luaL_loadstring(*L, prefabsLoadingCode.c_str());
//        int res = lua_pcall(*L, 0, 0, 0);
//        if (res != LUA_OK) {
//            L->PrintStack(std::cerr);
//            throw std::runtime_error(lua_tostring(*L, 1));
//        }
//    }

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);
    mBtnPressedTbl.PopGlobal(*L);
    mBtnReleasedTbl.PopGlobal(*L);

    Logger::Debug("Scene initialization finished...");
    Logger::Debug("Game objects: " + std::to_string(mGameObjectsTbl.getValues().size()));
    Logger::Debug("Components:   " + std::to_string(mComponentsTbl.getValues().size()));
}

void AmE::Lua53::update(
        const InputsState *const inputsState,
        SceneState *const sceneState,
        double timeDelta
) {
    mTimeDelta->setValue(timeDelta);
    mTimeDelta->PushGlobal(*L, LUA53_G_VAR_TIME_DELTA);

    // push info about keyboard and mouse state
    for (const auto &kp: inputsState->pressed) {
        auto &val = (LEBoolean &) mBtnPressedTbl.getValue(LETKey(kp.first.toString()));
        val.setValue(kp.second);
    }
    for (const auto &kp: inputsState->released) {
        auto &val = (LEBoolean &) mBtnReleasedTbl.getValue(LETKey(kp.first.toString()));
        val.setValue(kp.second);
    }
    mBtnPressedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_P_T);
    mBtnReleasedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_R_T);

    // calculate new frame in LUA
    std::string updateFrameCode
            = "__before_update_frame();\n"
              "__on_update_frame();\n"
              "__after_update_frame();\n";
    luaL_loadstring(*L, updateFrameCode.c_str());
    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
        throw std::runtime_error(lua_tostring(*L, 1));
    }

    // extract scene state
    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);
    for (const auto &[_, luaGameObjTbl]: mGameObjectsTbl.getValues()) {
        auto *goTbl = (LETable *) luaGameObjTbl.get();

        auto &idVal = goTbl->getValue(LETKey(GO_PROP_INSTANCE_ID));
        auto id = (GameObjectInstanceID) ((LENum &) idVal).getValue();

        GameObject *pGameObj = sceneState->getSceneGameObject(id);

        // register new GO, that was created with user scripts
        if (nullptr == pGameObj) {
            auto &nameVal = ((LEString &) goTbl->getValue(LETKey(GO_PROP_NAME)));
            pGameObj = new GameObject(id, nameVal.getValue());
            sceneState->addSceneGameObject(pGameObj);
        }

        {
            auto &transform = (LETable &) goTbl->getValue(LETKey(CMP_PROP_TR));

            auto &position = (LETable &) transform.getValue(LETKey(CMP_PROP_TR_POS));
            auto &rotation = (LETable &) transform.getValue(LETKey("rotation"));
            auto &localScale = (LETable &) transform.getValue(LETKey("localScale"));

            pGameObj->getTransform()->mPosition.Set(
                    ((LENum &) position.getValue(LETKey("x"))).getValue(),
                    ((LENum &) position.getValue(LETKey("y"))).getValue(),
                    ((LENum &) position.getValue(LETKey("z"))).getValue()
            );
            pGameObj->getTransform()->mRotation.Set(
                    ((LENum &) rotation.getValue(LETKey("x"))).getValue(),
                    ((LENum &) rotation.getValue(LETKey("y"))).getValue(),
                    ((LENum &) rotation.getValue(LETKey("z"))).getValue(),
                    ((LENum &) rotation.getValue(LETKey("w"))).getValue()
            );
            pGameObj->getTransform()->mLocalScale.Set(
                    ((LENum &) localScale.getValue(LETKey("x"))).getValue(),
                    ((LENum &) localScale.getValue(LETKey("y"))).getValue(),
                    ((LENum &) localScale.getValue(LETKey("z"))).getValue()
            );
        }
    }

    for (const auto &cmpState: mComponentsTbl.getValues()) {
        auto *cmpTbl = (LETable *) cmpState.second.get();

        LETable &goVal = (LETable &) cmpTbl->getValue(LETKey(CMP_PROP_GO));
        const auto &goID = (GameObjectInstanceID) ((LENum &) goVal.getValue(LETKey(GO_PROP_INSTANCE_ID))).getValue();

        const std::string &cmpName = cmpTbl->getValue(LETKey("__name")).ToString();

        auto *pCmp = sceneState->getSceneGameObject(goID)->getComponent(cmpName);

        for (const auto &luaAttrState: cmpTbl->getValues()) {
            std::string luaAttrName = luaAttrState.first.ToString();
            std::string luaAttrVal = luaAttrState.second->ToString();

            // skip "Start", "Update" and another functions of component
            if (luaAttrVal == "function")
                continue;

            if (luaAttrName == "__name")
                continue;
            if (luaAttrName == CMP_PROP_GO)
                continue;
            if (luaAttrName == CMP_PROP_TR)
                continue;

            // skip private fields of component
            if (pCmp->mProperties.find(luaAttrName) == pCmp->mProperties.end())
                continue;

            PropType::Kind kind = pCmp->mProperties[luaAttrName].mType;
            if (kind == PropType::PropTypeGameObject)
                continue;
            if (kind == PropType::PropTypeGameObjectTransform)
                continue;

            pCmp->mProperties[luaAttrName].mValue = parsePropValFromLua(kind, luaAttrState.second.get());
        }
    }

    mAppQuit->PopGlobal(*L);
    sceneState->setAppQuit(mAppQuit->getValue());
}


std::any AmE::Lua53::parsePropValFromLua(const PropType::Kind &kind, LuaCpp::Engine::LuaType *rawLuaVal) {
    switch (kind) {
        case PropType::PropTypeBoolean: {
            return ((LEBoolean *) rawLuaVal)->getValue();
        }
        case PropType::PropTypeDouble: {
            return ((LENum *) rawLuaVal)->getValue();
        }
        case PropType::PropTypeString: {
            return ((LEString *) rawLuaVal)->getValue();
        }
        case PropType::PropTypeColor: {
            auto colorTable = ((LETable *) rawLuaVal);
            auto color = Color{
                    ((LENum &) colorTable->getValue(LETKey("r"))).getValue(),
                    ((LENum &) colorTable->getValue(LETKey("g"))).getValue(),
                    ((LENum &) colorTable->getValue(LETKey("b"))).getValue(),
                    ((LENum &) colorTable->getValue(LETKey("a"))).getValue(),
            };
            return color;
        }
        case PropType::PropTypeVector3: {
            auto colorTable = ((LETable *) rawLuaVal);
            auto vec = Vector3{
                    ((LENum &) colorTable->getValue(LETKey("x"))).getValue(),
                    ((LENum &) colorTable->getValue(LETKey("y"))).getValue(),
                    ((LENum &) colorTable->getValue(LETKey("z"))).getValue(),
            };
            return vec;
        }
        case PropType::PropTypeGameObject:
            return nullptr;
        case PropType::PropTypeGameObjectTransform:
            return nullptr;
        case PropType::PropTypePrefabPath:
            return nullptr;
    }

    throw std::runtime_error("AmE::Lua53::parsePropValFromLua: unexpected property type " + PropType::asString(kind));
}



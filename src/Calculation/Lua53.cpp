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

    Logger::Debug("\n" + setLuaPathCode);

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

    std::string initCode = Lua53Generator::buildInitLuaCode(sceneState);
    initCode += "__before_scene_components_awake();\n"
                "__before_scene_components_start();\n"
                "\n";
    luaL_loadstring(*L, initCode.c_str());
    Logger::Trace("\n" + initCode);

    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
//        Logger::Error(lua_tostring(*L, 1));
//        std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
//                  << lua_error(*L) << std::endl;
        L->PrintStack(std::cerr); // TODO:

        throw std::runtime_error(lua_tostring(*L, 1));
    }

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
              "__check_all_collisions();\n"
              "__after_update_frame();\n";
    luaL_loadstring(*L, updateFrameCode.c_str());
    int res = lua_pcall(*L, 0, 0, 0);
    if (res != LUA_OK) {
        throw std::runtime_error(lua_tostring(*L, 1));
    }

    // extract scene state
    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);

    mUpdatedIDs.clear();
    for (const auto &[luaTblIdx, luaGameObjTbl]: mGameObjectsTbl.getValues()) {
        try {
            GameObjectInstanceID goID = luaTblIdx.getIntValue();
            mUpdatedIDs.insert(goID);

            auto *goTbl = (LETable *) luaGameObjTbl.get();

            auto &idVal = goTbl->getValue(LETKey(GO_PROP_INSTANCE_ID));
            auto id = (GameObjectInstanceID) ((LENum &) idVal).getValue();

            GameObject *pGameObj = sceneState->getSceneGameObject(id);

            // register new GO, that was created with user scripts
            if (nullptr == pGameObj) {
                auto &nameVal = ((LEString &) goTbl->getValue(LETKey(GO_PROP_NAME)));
                pGameObj = new GameObject(id, nameVal.getValue());
                sceneState->addSceneGameObject(pGameObj);

                Logger::Trace("Lua53: Create new game object " + std::to_string(goID));
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
        } catch (std::out_of_range &ex) {
            Logger::Error("Lua53: GameObject parsing failed: " + luaTblIdx.ToString());
        }
    }

    for (auto &[goID, _]: sceneState->getSceneGameObjects()) {
        if (mUpdatedIDs.find(goID) == mUpdatedIDs.end()) {
            sceneState->markSceneGameObjectAsRemoved(goID);
            Logger::Trace("Lua53: Start to remove " + std::to_string(goID));
        }
    }

    for (const auto &[luaGoIdx, luaCmpList]: mComponentsTbl.getValues()) {
        for (const auto &[luaIdx, luaCmp]: ((LETable *)luaCmpList.get())->getValues()) {
            try {
                auto *cmpTbl = (LETable *) luaCmp.get();
                if (nullptr == cmpTbl) {
                    continue;
                }

                LETable &goVal = (LETable &) cmpTbl->getValue(LETKey(CMP_PROP_GO));
                const auto &goID = (GameObjectInstanceID) ((LENum &) goVal.getValue(
                        LETKey(GO_PROP_INSTANCE_ID))).getValue();

                const std::string &cmpName = cmpTbl->getValue(LETKey("__type_filepath")).ToString();

                GameObject *pGameObject = sceneState->getSceneGameObject(goID);
                if (nullptr == pGameObject) {
                    continue;
                }

                if (!pGameObject->hasComponent(cmpName)) {
                    Logger::Trace("Create new component " + cmpName + " for " + std::to_string(goID));
                    auto *pCmp = new Component(cmpName, {});

                    auto *protoCmp = sceneState->getComponents().at(cmpName);
                    for (auto [key, val]: protoCmp->mProperties) {
                        pCmp->mProperties[key] = val;
                    }

                    pGameObject->addComponent(pCmp);
                }
                auto *pCmp = pGameObject->getComponent(cmpName);

                for (const auto &[luaAttrName, luaAttrVal]: cmpTbl->getValues()) {
                    std::string attrName = luaAttrName.ToString();
                    std::string attrVal = luaAttrVal->ToString();

                    // skip "Start", "Update" and another functions of component
                    if (attrName == "function")
                        continue;

                    if (attrName == "__type_name") continue;
                    if (attrName == "__type_filepath") continue;

                    if (attrName == CMP_PROP_GO)
                        continue;
                    if (attrName == CMP_PROP_TR)
                        continue;

                    // skip private fields of component
                    if (pCmp->mProperties.find(attrName) == pCmp->mProperties.end())
                        continue;

                    PropType::Kind kind = pCmp->mProperties[attrName].mType;
                    if (kind == PropType::PropTypeGameObject)
                        continue;
                    if (kind == PropType::PropTypeGameObjectTransform)
                        continue;

                    pCmp->mProperties[attrName].mValue = parsePropValFromLua(kind, luaAttrVal.get());
                }
            } catch (std::out_of_range &ex) {
                Logger::Error(
                        std::string() + "Lua53: Component parsing failed: " + ex.what() + ": " + luaIdx.ToString());
            }
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
            return (double) ((LENum *) rawLuaVal)->getValue();
        }
        case PropType::PropTypeInteger: {
            return (int) ((LENum *) rawLuaVal)->getValue();
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
            auto vecTable = ((LETable *) rawLuaVal);
            auto vec = Vector3{
                    ((LENum &) vecTable->getValue(LETKey("x"))).getValue(),
                    ((LENum &) vecTable->getValue(LETKey("y"))).getValue(),
                    ((LENum &) vecTable->getValue(LETKey("z"))).getValue(),
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

    throw std::runtime_error("Lua53::parsePropValFromLua: unexpected property type " + PropType::asString(kind));
}



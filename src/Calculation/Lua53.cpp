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

using LETKey = LuaCpp::Engine::Table::Key;
using LENum = LuaCpp::Engine::LuaTNumber;
using LEString = LuaCpp::Engine::LuaTString;
using LEBoolean = LuaCpp::Engine::LuaTBoolean;
using LETable = LuaCpp::Engine::LuaTTable;

AmE::Lua53::Lua53(const std::string &projectRoot) {
    LuaCpp::LuaContext ctx;
    L = ctx.newState();

    mAppQuit = new LEBoolean(false);

    mGameObjectsTbl.PushGlobal(*L, LUA53_G_VAR_GO_T);
    mComponentsTbl.PushGlobal(*L, LUA53_G_VAR_CMP_T);
    mBtnPressedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_P_T);
    mBtnReleasedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_R_T);
    mAppQuit->PushGlobal(*L, LUA53_G_VAR_APP_QUIT);

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

AmE::Lua53::~Lua53() {
    delete mAppQuit;
//    lua_close(*L);
}

void AmE::Lua53::initialize(const std::map<std::string, GameObject *> &gameObjects) {
    // 1) Include LUA modules from 'Core'
    // 2) Create all game objects (set name, set transform)
    // 3) Create all components (set gameObject, set transform)
    // 4) Set each component with args (static values, references to gameObject or transforms)
    // 5) Run 'Start()' method for each component

    const std::string &initCode = buildInitLuaCode(gameObjects);
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

    mGameObjectsTbl.PopGlobal(*L);
    mComponentsTbl.PopGlobal(*L);
    mBtnPressedTbl.PopGlobal(*L);
    mBtnReleasedTbl.PopGlobal(*L);

    Logger::Debug("Scene initialization finished...");
    Logger::Debug("Game objects: " + std::to_string(mGameObjectsTbl.getValues().size()));
    Logger::Debug("Components:   " + std::to_string(mComponentsTbl.getValues().size()));
}

void AmE::Lua53::update(
        std::map<std::string, GameObject *> &gameObjects,
        const std::map<KeyCode, bool> &keysPressed,
        const std::map<KeyCode, bool> &keysReleased,
        bool &appQuit
) {
    for (const auto &kp: keysPressed) {
        auto &val = (LEBoolean &) mBtnPressedTbl.getValue(LETKey(kp.first.toString()));
        val.setValue(kp.second);
    }
    for (const auto &kp: keysReleased) {
        auto &val = (LEBoolean &) mBtnReleasedTbl.getValue(LETKey(kp.first.toString()));
        val.setValue(kp.second);
    }
    mBtnPressedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_P_T);
    mBtnReleasedTbl.PushGlobal(*L, LUA53_G_VAR_BTN_R_T);

    std::string updateFrameCode
            = "__before_update_frame();\n"
              "__on_update_frame();\n"
              "__after_update_frame();\n";
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
                ((LENum &) rotation.getValue(LETKey("z"))).getValue(),
                ((LENum &) rotation.getValue(LETKey("w"))).getValue()
        );
        gameObjects[id]->mTransform->mLocalScale.Set(
                ((LENum &) localScale.getValue(LETKey("x"))).getValue(),
                ((LENum &) localScale.getValue(LETKey("y"))).getValue(),
                ((LENum &) localScale.getValue(LETKey("z"))).getValue()
        );
    }

    for (const auto &cmpState: mComponentsTbl.getValues()) {
        auto *cmpTbl = (LETable *) cmpState.second.get();

        LETable &goVal = (LETable &) cmpTbl->getValue(LETKey("gameObject"));
        const std::string &goID = goVal.getValue(LETKey("id")).ToString();

        const std::string &cmpName = cmpTbl->getValue(LETKey("__name")).ToString();

        Component &cmp = gameObjects[goID]->mComponents[cmpName];

        for (const auto &luaAttrState: cmpTbl->getValues()) {
            std::string luaAttrName = luaAttrState.first.ToString();
            std::string luaAttrVal = luaAttrState.second->ToString();

            // skip "Start", "Update" and another functions of component
            if (luaAttrVal == "function")
                continue;

            if (luaAttrName == "__name")
                continue;
            if (luaAttrName == "gameObject")
                continue;
            if (luaAttrName == "transform")
                continue;

            // skip private fields of component
            if (cmp.mProperties.find(luaAttrName) == cmp.mProperties.end())
                continue;

            PropType::Kind kind = cmp.mProperties[luaAttrName].mType;
            if (kind == PropType::PropTypeGameObject)
                continue;
            if (kind == PropType::PropTypeGameObjectTransform)
                continue;

            cmp.mProperties[luaAttrName].mValue = parsePropValFromLua(kind, luaAttrState.second.get());
        }
    }

    mAppQuit->PopGlobal(*L);
    appQuit = mAppQuit->getValue();
}

std::string AmE::Lua53::buildInitLuaCode(const std::map<std::string, GameObject *> &gameObjects) {
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
                    + ", "
                    + std::to_string(rot.mW)
                    + ")\n";
        if (!go->isCamera()) {
            initCode += std::string()
                        + LUA53_G_VAR_GO_T + "['" + go->mID + "'].transform.localScale:Set("
                        + std::to_string(scale.mX)
                        + ", "
                        + std::to_string(scale.mY)
                        + ", "
                        + std::to_string(scale.mZ)
                        + ")\n";
        }
        initCode += "\n";
    }

    // TODO:
    std::map<std::string, std::string> cmpNameToPath;
    for (const auto &goPair: gameObjects) {
        for (const auto &cmpPair: goPair.second->mComponents) {
            cmpNameToPath[cmpPair.second.mName] = cmpPair.second.mPathname;
        }
    }
    initCode += "require 'Core/LuaBehaviour'\n\n";
    for (const auto &cmpPair: cmpNameToPath) {
        initCode += "require '" + cmpPair.second + "'\n";
        initCode += "\n";

        initCode += std::string()
                    + "function " + cmpPair.first + ":new()\n"
                    + "    local instance = LuaBehaviour:new()\n"
                    + "    setmetatable(instance, self)\n"
                    + "    self.__index = self\n"
                    + "    return instance\n"
                    + "end\n\n";
    }

    for (const auto &goPair: gameObjects) {
        auto *go = goPair.second;

        for (const auto &cmpPair: go->mComponents) {
            std::string cmpID = go->mID + " :: " + cmpPair.second.mName;

            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'] = " + cmpPair.second.mName + ":new()\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].__name = '" + cmpPair.second.mName + "'\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].gameObject = " + LUA53_G_VAR_GO_T + "['" + go->mID +
                        "']\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].transform = " + LUA53_G_VAR_GO_T + "['" + go->mID +
                        "'].transform\n";
            initCode += "\n";

            for (const auto &propPair: cmpPair.second.mProperties) {
                initCode += std::string()
                            + LUA53_G_VAR_CMP_T + "['" + cmpID + "']." + propPair.second.mName + " = " +
                            propValToLuaCode(propPair.second) + "\n";
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

std::string AmE::Lua53::propValToLuaCode(const Property &prop) {
    switch (prop.mType) {
        case PropType::PropTypeBoolean:
            return std::any_cast<bool>(prop.mValue) ? "true" : "false";
        case PropType::PropTypeDouble:
            return std::to_string(std::any_cast<double>(prop.mValue));
        case PropType::PropTypeString:
            return "'" + std::any_cast<std::string>(prop.mValue) + "'";
        case PropType::PropTypeColor: {
            auto colorVal = std::any_cast<Color>(prop.mValue);
            return "Color:new("
                   + std::to_string(colorVal.mR)
                   + ", "
                   + std::to_string(colorVal.mG)
                   + ", "
                   + std::to_string(colorVal.mB)
                   + ", "
                   + std::to_string(colorVal.mA)
                   + ")";
        }
        case PropType::PropTypeVector3: {
            auto vecVal = std::any_cast<Vector3>(prop.mValue);
            return "Vector:new("
                   + std::to_string(vecVal.mX)
                   + ", "
                   + std::to_string(vecVal.mY)
                   + ", "
                   + std::to_string(vecVal.mZ)
                   + ")";
        }
        case PropType::PropTypeGameObject: {
            try {
                auto goID = std::any_cast<std::string>(prop.mValue);
                return std::string()
                       + LUA53_G_VAR_GO_T + "['" + goID + "']";
            } catch (const std::bad_any_cast &e) {
                //Logger::Error(e.what());
            }

            try {
                auto goID = std::any_cast<const char *>(prop.mValue);
                return std::string()
                       + LUA53_G_VAR_GO_T + "['" + goID + "']";
            } catch (const std::bad_any_cast &e) {
                //Logger::Error(e.what());
            }
        }
        case PropType::PropTypeGameObjectTransform: {
            try {
                auto goID = std::any_cast<std::string>(prop.mValue);
                return std::string()
                       + LUA53_G_VAR_GO_T + "['" + goID + "'].transform";
            } catch (const std::bad_any_cast &e) {
                //Logger::Error(e.what());
            }

            try {
                auto goID = std::any_cast<const char *>(prop.mValue);
                return std::string()
                       + LUA53_G_VAR_GO_T + "['" + goID + "'].transform";
            } catch (const std::bad_any_cast &e) {
                //Logger::Error(e.what());
            }
        }
    }

    throw std::runtime_error("unexpected type " + PropType::asString(prop.mType));
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
            // TODO:
            break;
        case PropType::PropTypeGameObjectTransform:
            // TODO:
            break;
    }

    throw std::runtime_error("unexpected type " + PropType::asString(kind));
}

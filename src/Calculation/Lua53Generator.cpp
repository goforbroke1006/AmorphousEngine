//
// Created by goforbroke on 1/25/24.
//

#include "../../include/Calculation/Lua53Generator.h"
#include "../../include/Core/Property.h"

std::string
AmE::Lua53Generator::buildInitLuaCode(
        const SceneState *const sceneState
) {
    if (sceneState->getSceneGameObjects().empty())
        return "";

    std::string initCode;
    initCode += "require 'Core'\n"
                "require 'Component'\n"
                "\n";

    for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
        initCode += generateInitCode(
                std::string() + LUA53_G_VAR_GO_T + "[" + std::to_string(pGameObj->getID()) + "]",
                pGameObj,
                true
        );
    }

    // TODO:
    std::map<std::string, std::string> cmpNameToPath;
    for (const auto &goPair: sceneState->getSceneGameObjects()) {
        for (const auto &cmpPair: goPair.second->getComponents()) {
            cmpNameToPath[cmpPair.second->mName] = cmpPair.second->mPathname;
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

    // inject components to lua context
    for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
        const auto &idStr = std::to_string(pGameObj->getID());

        for (const auto &[_, pCmp]: pGameObj->getComponents()) {
            std::string cmpID = idStr + " :: " + pCmp->mName;

            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'] = " + pCmp->mName + ":new()\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].__name = '" + pCmp->mName + "'\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].gameObject = "
                        + LUA53_G_VAR_GO_T + "[" + idStr + "]\n";
            initCode += std::string()
                        + LUA53_G_VAR_CMP_T + "['" + cmpID + "'].transform = "
                        + LUA53_G_VAR_GO_T + "[" + idStr + "].transform\n";
            initCode += "\n";

            for (const auto &[_, prop]: pCmp->mProperties) {
                const std::string &val = simplePropValToLuaCode(prop);
                if (val.empty())
                    continue;

                initCode += std::string()
                            + LUA53_G_VAR_CMP_T + "['" + cmpID + "']." + prop.mName + " = " + val + "\n";
            }

            initCode += "\n";
        }
    }

    initCode += std::string()
                + "for _, cmpInstance in pairs(" + LUA53_G_VAR_CMP_T + ") do\n"
                + "    cmpInstance:Start()\n"
                + "end\n";

    return initCode;
}

std::string
AmE::Lua53Generator::generateInitCode(
        const std::string &luaRef,
        const AmE::GameObject *const pGameObj,
        bool isSceneObject
) {
    auto pos = pGameObj->getTransform()->mPosition;

    const std::string &posX = std::to_string(pos.getX());
    const std::string &posY = std::to_string(pos.getY());
    const std::string &posZ = std::to_string(pos.getZ());

    auto rot = pGameObj->getTransform()->mRotation;

    const std::string &rotX = std::to_string(rot.getX());
    const std::string &rotY = std::to_string(rot.getY());
    const std::string &rotZ = std::to_string(rot.getZ());
    const std::string &rotW = std::to_string(rot.getW());


    std::string code =
            std::string()
            + luaRef + " = GameObject:new(" + std::to_string(pGameObj->getID()) + ", '" + pGameObj->getName() + "')\n"
            + luaRef + ".transform.position:Set(" + posX + ", " + posY + ", " + posZ + ")\n"
            + luaRef + ".transform.rotation:Set(" + rotX + ", " + rotY + ", " + rotZ + ", " + rotW + ")\n";

    if (!pGameObj->isCamera() && !pGameObj->isLight()) {
        auto scale = pGameObj->getTransform()->mLocalScale;

        const std::string &scaleX = std::to_string(scale.getX());
        const std::string &scaleY = std::to_string(scale.getY());
        const std::string &scaleZ = std::to_string(scale.getZ());

        code += luaRef + ".transform.localScale:Set(" + scaleX + ", " + scaleY + ", " + scaleZ + ")\n";
    }

    code += "\n";

    if (!isSceneObject) {
        for (const auto &[_, pCmp]: pGameObj->getComponents()) {
            code.append(
                    std::string()
                    + luaRef + ".__components['" + pCmp->mName + "'] = " + pCmp->mName + "\n"
                    + luaRef + ".__components['" + pCmp->mName + "'].__name = '" + pCmp->mName + "'\n"
                    + luaRef + ".__components['" + pCmp->mName + "'].gameObject = nil\n"
                    + luaRef + ".__components['" + pCmp->mName + "'].transform = nil\n"
            );

            for (auto &[_, prop]: pCmp->mProperties) {
                const std::string &val = simplePropValToLuaCode(prop);
                if (val.empty())
                    continue;

                code += luaRef + ".__components['" + pCmp->mName + "']." + prop.mName + " = " + val;
                code += "\n";
            }

            code += "\n";
        }
    }

    return code;
}

std::string
AmE::Lua53Generator::simplePropValToLuaCode(const Property &prop) {
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
                   + std::to_string(vecVal.getX())
                   + ", "
                   + std::to_string(vecVal.getY())
                   + ", "
                   + std::to_string(vecVal.getZ())
                   + ")";
        }
                case PropType::PropTypeGameObject: {
            try {
                auto goID = prop.asInt();
                return std::string()
                       + LUA53_G_VAR_GO_T + "[" + std::to_string(goID) + "]";
            } catch (const std::bad_any_cast &e) {
                //Logger::Error(e.what());
            }
        }
        case PropType::PropTypeGameObjectTransform: {
            try {
                auto goID = prop.asInt();
                return std::string()
                       + LUA53_G_VAR_GO_T + "[" + std::to_string(goID) + "].transform";
            } catch (const std::bad_any_cast &e) {
                Logger::Error(e.what());
            }

        }
        case PropType::PropTypePrefabPath:
            return std::string()
                   + LUA53_G_VAR_PREFABS + "[" + prop.asString() + "]";
    }

    throw std::runtime_error("unexpected type " + PropType::asString(prop.mType));
}


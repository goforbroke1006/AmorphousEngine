//
// Created by goforbroke on 1/25/24.
//

#include "../include/SceneState.h"

#include <filesystem>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "../include/Logger.h"
#include "../include/PrefabManager.h"

AmE::SceneState::SceneState() : mAppQuit(false) {}

AmE::SceneState::~SceneState() {
    for (const auto &[_, pGameObj]: mSceneGameObjects)
        delete pGameObj;
    Logger::Trace("Clear " + std::to_string(mSceneGameObjects.size()) + " scene game objects");
    mSceneGameObjects.clear();

    for (const auto &[_, pGameObj]: mPrefabGameObjects)
        delete pGameObj;
    Logger::Trace("Clear " + std::to_string(mSceneGameObjects.size()) + " prefab game objects");
    mPrefabGameObjects.clear();
}

const std::map<GameObjectInstanceID, AmE::GameObject *> &
AmE::SceneState::getSceneGameObjects() const {
    return mSceneGameObjects;
}

AmE::GameObject *
AmE::SceneState::getSceneGameObject(GameObjectInstanceID id) const {
    if (mSceneGameObjects.find(id) == mSceneGameObjects.end())
        return nullptr;

    return mSceneGameObjects.at(id);
}

void AmE::SceneState::addSceneGameObject(AmE::GameObject *const pGameObj) {
    mSceneGameObjects[pGameObj->getID()] = pGameObj;
}

const std::map<std::string, AmE::GameObject *> &
AmE::SceneState::getPrefabGameObjects() const {
    return mPrefabGameObjects;
}

bool AmE::SceneState::isAppQuit() const {
    return mAppQuit;
}

void AmE::SceneState::setAppQuit(bool appQuit) {
    SceneState::mAppQuit = appQuit;
}

AmE::SceneState *AmE::SceneState::loadFromFile(
        std::string projectRoot,
        const std::string &jsonFilepath
) {
    if (projectRoot.empty())
        projectRoot = ".";

    std::string sceneFilepath;
    if (jsonFilepath[0] == std::filesystem::path::preferred_separator)
        sceneFilepath = jsonFilepath;
    else
        sceneFilepath = projectRoot + std::filesystem::path::preferred_separator + jsonFilepath;

    if (!std::filesystem::is_regular_file(sceneFilepath))
        throw std::runtime_error("scene file not found: " + sceneFilepath);

    std::ifstream fileStream(sceneFilepath);
    Json::Reader reader;
    Json::Value sceneJSON;

    reader.parse(fileStream, sceneJSON);

    auto pSceneState = new SceneState();

    GameObjectInstanceID nextID = 0;

    for (const auto &goVal: sceneJSON["gameObjects"]) {
        auto *pGameObject = new GameObject(nextID, goVal["name"].asString());

        auto &posVal = goVal["transform"]["position"];
        if (posVal["x"].isDouble() && posVal["y"].isDouble() && posVal["z"].isDouble()) {
            pGameObject->getTransform()->mPosition.Set(
                    posVal["x"].asDouble(),
                    posVal["y"].asDouble(),
                    posVal["z"].asDouble()
            );
        } else {
            Logger::Error("Object #" + std::to_string(nextID) + " " + pGameObject->getName() + " has invalid position");
        }

        auto &rotVal = goVal["transform"]["rotation"];
        if (rotVal["x"].isDouble() && rotVal["y"].isDouble() && rotVal["z"].isDouble()) {
            auto euler = Quaternion::Euler(rotVal["x"].asDouble(),
                                           rotVal["y"].asDouble(),
                                           rotVal["z"].asDouble());
            pGameObject->getTransform()->mRotation = euler;
        } else {
            Logger::Error("Object #" + std::to_string(nextID) + " " + pGameObject->getName() + " has invalid rotation");
        }

        auto &scaleVal = goVal["transform"]["localScale"];
        if (scaleVal["x"].isDouble() && scaleVal["y"].isDouble() && scaleVal["z"].isDouble()) {
            pGameObject->getTransform()->mLocalScale.Set(
                    scaleVal["x"].asDouble(),
                    scaleVal["y"].asDouble(),
                    scaleVal["z"].asDouble()
            );
        } else {
            Logger::Error("Object #" + std::to_string(nextID) + " " + pGameObject->getName() + " has invalid scale");
        }

        const Json::Value &componentsVals = goVal["components"];
        for (const auto &cmpVal: componentsVals) {
            auto *pCmp = new Component();
            pCmp->mName = cmpVal["name"].asString();
            pCmp->mPathname = cmpVal["pathname"].asString();

            const Json::Value &argumentsVals = cmpVal["properties"];
            for (const auto &argVal: argumentsVals) {
                const std::string &rawValue = argVal["value"].asString();

                Property prop;
                prop.mName = argVal["name"].asString();
                prop.mType = PropType::parseKind(argVal["type"].asString());
                prop.mValue = Property::parseValue(prop.mType, rawValue);
                pCmp->mProperties[prop.mName] = prop;

                // remember that need to load prefab file
                if (prop.mType == PropType::PropTypePrefabPath)
                    pSceneState->mPrefabGameObjects[rawValue] = nullptr;

            }

            pGameObject->addComponent(pCmp);
        }

        if (!goVal["mesh"].empty() && goVal["mesh"].isString()) {
            pGameObject->setMeshPathname(goVal["mesh"].asString());
        }

        pSceneState->mSceneGameObjects[nextID] = pGameObject;

        ++nextID;
    }

    for (auto &[prefabPath, _]: pSceneState->mPrefabGameObjects) {
        auto pGameObj = PrefabManager::loadFromFile(
                projectRoot + std::filesystem::path::preferred_separator + prefabPath
        );

        pSceneState->mPrefabGameObjects[prefabPath] = pGameObj;

        Logger::Trace("Load prefab " + prefabPath);
    }

    return pSceneState;
}







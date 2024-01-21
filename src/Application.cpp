//
// Created by goforbroke on 1/1/24.
//

#include <utility>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "../include/Application.h"
#include "../include/Core/Property.h"

#include "../include/Logger.h"

AmE::Application::Application(
        std::string mEngineRoot,
        std::string mProjectRoot,
        GraphicsEngine *mGraphicsEngine,
        CalculationEngine *mCalculationEngine
)
        : mEngineRoot(std::move(mEngineRoot)),
          mProjectRoot(std::move(mProjectRoot)),
          mGraphicsEngine(mGraphicsEngine),
          mCalculationEngine(mCalculationEngine) {
}

AmE::Application::~Application() {
    for (const auto &go: mGameObjects)
        delete go.second;
    Logger::Debug("Clear " + std::to_string(mGameObjects.size()) + " game objects");
}

void AmE::Application::loadScene(const std::string &filepath) {
    const std::string path = mProjectRoot + std::filesystem::path::preferred_separator + filepath;
    std::ifstream ifs(path);
    Json::Reader reader;
    Json::Value obj;

    reader.parse(ifs, obj);

    const Json::Value &gameObjectsVals = obj["gameObjects"];

    GameObjectInstanceID nextID = 0;

    for (const auto &goVal: gameObjectsVals) {
        auto *pGO = new GameObject(nextID, goVal["name"].asString());

        auto &posVal = goVal["transform"]["position"];
        pGO->getTransform()->mPosition.Set(
                posVal["x"].asDouble(),
                posVal["y"].asDouble(),
                posVal["z"].asDouble());

        auto &rotVal = goVal["transform"]["rotation"];
        auto euler = Quaternion::Euler(rotVal["x"].asDouble(),
                                       rotVal["y"].asDouble(),
                                       rotVal["z"].asDouble());
        pGO->getTransform()->mRotation = euler;

        auto &scaleVal = goVal["transform"]["localScale"];
        pGO->getTransform()->mLocalScale.Set(
                scaleVal["x"].asDouble(),
                scaleVal["y"].asDouble(),
                scaleVal["z"].asDouble()
        );

        const Json::Value &componentsVals = goVal["components"];
        for (const auto &cmpVal: componentsVals) {
            Component cmp;
            cmp.mName = cmpVal["name"].asString();
            cmp.mPathname = cmpVal["pathname"].asString();

            const Json::Value &argumentsVals = cmpVal["properties"];
            for (const auto &argVal: argumentsVals) {
                Property prop;
                prop.mName = argVal["name"].asString();
                prop.mType = PropType::parseKind(argVal["type"].asString());
                prop.mValue = Property::parseValue(prop.mType, argVal["value"].asString());
                cmp.mProperties[prop.mName] = prop;
            }

            pGO->getComponents()[cmp.mName] = cmp;
        }

        if (!goVal["mesh"].empty() && goVal["mesh"].isString()) {
            pGO->setMeshPathname(goVal["mesh"].asString());
        }

        mGameObjects[nextID] = pGO;

        ++nextID;
    }
}

void AmE::Application::runMainLoop() {
    mGraphicsEngine->initialize(mGameObjects);
    mCalculationEngine->initialize(mGameObjects);

    auto *preUpdateFrameData = new PreUpdateFrameData();
    auto *sceneState = new SceneState(mGameObjects);

    auto *inputReader = new InputReader(mGraphicsEngine->getWindowHnd(), preUpdateFrameData);

    while (!sceneState->appQuit) {
        inputReader->collectCodes();

        mCalculationEngine->update(preUpdateFrameData, sceneState);

        if (!mGraphicsEngine->update(mGameObjects))
            break;
    }

    mGraphicsEngine->stop();

    delete inputReader;
    delete sceneState;
    delete preUpdateFrameData;
}



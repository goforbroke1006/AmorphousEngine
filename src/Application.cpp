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

Application::Application(
        std::string mEngineRoot,
        std::string mProjectRoot,
        GraphicsEngine *mGraphicsEngine,
        CalculationEngine *mCalculationEngine
)
        : mEngineRoot(std::move(mEngineRoot)),
          mProjectRoot(std::move(mProjectRoot)),
          mGraphicsEngine(mGraphicsEngine),
          mCalculationEngine(mCalculationEngine),
          mInputReader(new InputReader()) {}

Application::~Application() {
    for (const auto &go: mGameObjects)
        delete go.second;
    Logger::Debug("Clear " + std::to_string(mGameObjects.size()) + " game objects");

    delete mInputReader;
    mInputReader = nullptr;
}

void Application::loadScene(const std::string &filepath) {
    const std::string path = mProjectRoot + std::filesystem::path::preferred_separator + filepath;
    std::ifstream ifs(path);
    Json::Reader reader;
    Json::Value obj;

    reader.parse(ifs, obj);

    const Json::Value &gameObjectsVals = obj["gameObjects"];

    for (const auto &goVal: gameObjectsVals) {
        auto *pGO = new GameObject(goVal["id"].asString(), goVal["name"].asString());

        pGO->mTransform = new Transform;

        auto &posVal = goVal["transform"]["position"];
        pGO->mTransform->mPosition.mX = posVal["x"].asDouble();
        pGO->mTransform->mPosition.mY = posVal["y"].asDouble();
        pGO->mTransform->mPosition.mZ = posVal["z"].asDouble();

        auto &rotVal = goVal["transform"]["rotation"];
        pGO->mTransform->mRotation = Quaternion::Euler(
                rotVal["x"].asDouble(),
                rotVal["y"].asDouble(),
                rotVal["z"].asDouble()
        );

        auto &scaleVal = goVal["transform"]["localScale"];
        pGO->mTransform->mLocalScale.mX = scaleVal["x"].asDouble();
        pGO->mTransform->mLocalScale.mY = scaleVal["y"].asDouble();
        pGO->mTransform->mLocalScale.mZ = scaleVal["z"].asDouble();

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

            pGO->mComponents[cmp.mName] = cmp;
        }

        if (!goVal["mesh"].empty() && goVal["mesh"].isString()) {
            pGO->mMeshPathname = goVal["mesh"].asString();
        }

        mGameObjects[pGO->mID] = pGO;
    }
}

void Application::runMainLoop() {
    mGraphicsEngine->initialize(mGameObjects);
    mCalculationEngine->initialize(mGameObjects);

    while (true) {
        mInputReader->collectCodes();

        mCalculationEngine->update(
                mGameObjects,
                mInputReader->pressed(),
                mInputReader->released()
        );

        if (!mGraphicsEngine->update(mGameObjects))
            break;
    }

    mGraphicsEngine->stop();
}



//
// Created by goforbroke on 1/1/24.
//

#include <utility>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "../include/Application.h"

Application::Application(
        std::string mEngineRoot,
        std::string mProjectRoot,
        GraphicsEEngine *mGraphicsEngine,
        CalculationEngine *mCalculationEngine
)
        : mEngineRoot(std::move(mEngineRoot)),
          mProjectRoot(std::move(mProjectRoot)),
          mGraphicsEngine(mGraphicsEngine),
          mCalculationEngine(mCalculationEngine) {}

void Application::loadScene(const std::string &filepath) {
    const std::string path = mProjectRoot + std::filesystem::path::preferred_separator + filepath;
    std::ifstream ifs(path);
    Json::Reader reader;
    Json::Value obj;

    reader.parse(ifs, obj);

    const Json::Value &gameObjectsVals = obj["gameObjects"];
    for (const auto &goVal: gameObjectsVals) {
        auto *pGO = new GameObject();

        pGO->mName = goVal["name"].asString();

        pGO->mTransform = new Transform;

        auto &posVal = goVal["transform"]["position"];
        pGO->mTransform->mPosition.mX = posVal["x"].asDouble();
        pGO->mTransform->mPosition.mY = posVal["y"].asDouble();
        pGO->mTransform->mPosition.mZ = posVal["z"].asDouble();

        auto &rotVal = goVal["transform"]["rotation"];
        pGO->mTransform->mRotation.mX = rotVal["x"].asDouble();
        pGO->mTransform->mRotation.mY = rotVal["y"].asDouble();
        pGO->mTransform->mRotation.mZ = rotVal["z"].asDouble();
        pGO->mTransform->mRotation.mW = rotVal["w"].asDouble();

        const Json::Value &componentsVals = goVal["components"];
        for (const auto &cmpVal: componentsVals) {
            Component cmp;
            cmp.mPathname = cmpVal["pathname"].asString();

            const Json::Value &argumentsVals = cmpVal["args"];
            for (const auto &argVal: argumentsVals) {
                ComponentArg cmpArg;

                if (argVal.isDouble()) {
                    cmpArg.doubleVal = argVal.asDouble();
                }

                if (argVal.isString()) {
                    cmpArg.refVal = argVal.asString();
                }
            }

            pGO->mComponents.push_back(cmp);
        }

        mGameObjects.push_back(pGO);
    }
}

void Application::runMainLoop() {
    mGraphicsEngine->initialize(mGameObjects);
    mCalculationEngine->initialize(mGameObjects);

    while (true) {
        mGameObjects = mCalculationEngine->update();
        mGraphicsEngine->update(mGameObjects);
    }
}

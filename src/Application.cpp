//
// Created by goforbroke on 1/1/24.
//

#include "../include/Application.h"

#include <utility>
#include <filesystem>


#include "../include/Logger.h"
#include "../include/PrefabManager.h"
#include "../include/Core/Property.h"

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
    delete mSceneState;
}

void AmE::Application::loadScene(const std::string &filepath) {
    mSceneState = SceneState::loadFromFile(mProjectRoot, filepath);
}

void AmE::Application::runMainLoop() {
    if (nullptr == mSceneState)
        throw std::runtime_error("scene is not loaded");

    mGraphicsEngine->initialize(mSceneState);
    mCalculationEngine->initialize(mSceneState);

    auto *pInputsState = new InputsState();

    auto *pInputReader = new InputReader(mGraphicsEngine->getWindowHnd(), pInputsState);

    while (!mSceneState->isAppQuit()) {
        pInputReader->collectCodes();

        mCalculationEngine->update(pInputsState, mSceneState);

        if (!mGraphicsEngine->update(mSceneState))
            break;
    }

    mGraphicsEngine->stop();

    delete pInputReader;
    delete pInputsState;
}



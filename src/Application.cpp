//
// Created by goforbroke on 1/1/24.
//

#include "../include/Application.h"

#include <utility>
#include <filesystem>
#include <chrono>

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
          mCalculationEngine(mCalculationEngine),
          mTimeDelta(new TimeDelta(100)) {}

AmE::Application::~Application() {
    delete mSceneState;
    delete mTimeDelta;
}

void AmE::Application::loadScene(const std::string &filepath) {
    mSceneState = SceneState::loadFromFile(mProjectRoot, filepath);
}

void AmE::Application::runMainLoop() {
    if (nullptr == mSceneState)
        throw std::runtime_error("scene is not loaded");

    double timeDiff = 0;
    auto timeLast = std::chrono::high_resolution_clock::now();

    mGraphicsEngine->initialize(mSceneState);
    mCalculationEngine->initialize(mSceneState);

    timeDiff = std::chrono::duration<double, std::ratio<1>>(
            std::chrono::high_resolution_clock::now() - timeLast).count();
    mTimeDelta->insert(timeDiff);
    timeLast = std::chrono::high_resolution_clock::now();

    auto *pInputsState = new InputsState();

    auto *pInputReader = new InputReader(mGraphicsEngine->getWindowHnd(), pInputsState);

    while (!mSceneState->isAppQuit()) {
        pInputReader->collectCodes();

        mCalculationEngine->update(pInputsState, mSceneState, mTimeDelta->getAvg());

        if (!mGraphicsEngine->update(mSceneState))
            break;

        timeDiff = std::chrono::duration<double, std::ratio<1>>(
                std::chrono::high_resolution_clock::now() - timeLast).count();
        mTimeDelta->insert(timeDiff);
        timeLast = std::chrono::high_resolution_clock::now();

        //std::cout << (1.0 / mTimeDelta->getAvg()) << "\t" << mTimeDelta->getAvg() << std::endl;
    }

    mGraphicsEngine->stop();

    delete pInputReader;
    delete pInputsState;
}



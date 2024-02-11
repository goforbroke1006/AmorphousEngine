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
        CalculationEngine *mCalculationEngine,
        InputReader *inputReader
)
        : mEngineRoot(std::move(mEngineRoot)),
          mProjectRoot(std::move(mProjectRoot)),
          mGraphicsEngine(mGraphicsEngine),
          mCalculationEngine(mCalculationEngine),
          mInputReader(inputReader),
          mSceneState(nullptr),
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

    mGraphicsEngine->initialize(mSceneState);
    mCalculationEngine->initialize(mSceneState);

//    timeLast = std::chrono::high_resolution_clock::now();;
//    mTimeDelta->insert(
//            std::chrono::duration<double, std::ratio<1>>(
//                    std::chrono::high_resolution_clock::now() - timeLast
//            ).count()
//    );
//    timeLast = std::chrono::high_resolution_clock::now();

    while (!mSceneState->isAppQuit()) {
        this->updateOneFrame();
        //std::cout << (1.0 / mTimeDelta->getAvg()) << "\t" << mTimeDelta->getAvg() << std::endl;
    }

    mGraphicsEngine->stop();
}

bool AmE::Application::updateOneFrame() {
    mInputReader->collectCodes();

    mCalculationEngine->update(
            mInputReader->getState(), mSceneState, mTimeDelta->getAvg()
    );

    if (!mGraphicsEngine->update(mSceneState))
        return false;

    mTimeDelta->insert(std::chrono::duration<double, std::ratio<1>>(
            std::chrono::high_resolution_clock::now() - timeLast).count());
    timeLast = std::chrono::high_resolution_clock::now();

    return true;
}

[[maybe_unused]] AmE::SceneState *AmE::Application::getSceneState() const {
    return mSceneState;
}

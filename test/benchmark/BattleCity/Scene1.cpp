//
// Created by goforbroke on 4/2/24.
//

#include <benchmark/benchmark.h>

#include "InputReaderFake.h"
#include "../../../include/Application.h"
#include "../../../include/Graphics/Null.h"
#include "../../../include/Calculation/Lua53.h"

// Battle City clone contains 122 game object. And it works really slow!
// Need to try to optimize (maybe) reading positions/rotation -
// do not retrieve transforms for objects that was not changed after frame updating.
//
// 2024 Apr 2 | Time = 731480684 ns | CPU = 731472862 ns | Iterations = 1 |
static void BM_Application_BattleCity_Scene1_NullGraphics(
        benchmark::State &state) {
    const std::string engineRoot = ".";
    const std::string projectRoot = "./testdata";

    auto *pGraphicsEngine = new AmE::Null();
    auto *pCalculationEngine = new AmE::Lua53(engineRoot, projectRoot);
    auto *pInputReader = new InputReaderFake();

    auto *app = new AmE::Application(
            engineRoot,
            projectRoot,
            pGraphicsEngine,
            pCalculationEngine,
            pInputReader);

    app->loadScene("./Scenes/level-1.json");

    // Do operation from AmE::Application::runMainLoop()
    pGraphicsEngine->initialize(app->getSceneState());
    pCalculationEngine->initialize(app->getSceneState());
    app->updateOneFrame();

    for (auto _: state) {
        std::cout << "#\n#\n#\n";
        app->updateOneFrame();
    }

    pInputReader->fakePress(AmE::KeyCode_Escape);
    pInputReader->fakeRelease(AmE::KeyCode_Escape);

    delete app;
}

BENCHMARK(BM_Application_BattleCity_Scene1_NullGraphics)->Iterations(10);


/*static void BM_Lua53_BattleCity_Scene1_NullGraphics(
        benchmark::State &state) {
    const std::string engineRoot = ".";
    const std::string projectRoot = "./testdata";

    auto *pGraphicsEngine = new AmE::Null();
    auto *pCalculationEngine = new AmE::Lua53(engineRoot, projectRoot);
    auto *pInputReader = new InputReaderFake();

    auto *app = new AmE::Application(
            engineRoot,
            projectRoot,
            pGraphicsEngine,
            pCalculationEngine,
            pInputReader);

    app->loadScene("./Scenes/level-1.json");

    // Do operation from AmE::Application::runMainLoop()
    pGraphicsEngine->initialize(app->getSceneState());
    pCalculationEngine->initialize(app->getSceneState());
    app->updateOneFrame();

    for (auto _: state)
        app->updateOneFrame();

    pInputReader->fakePress(AmE::KeyCode_Escape);
    pInputReader->fakeRelease(AmE::KeyCode_Escape);

    delete app;
}

BENCHMARK(BM_Lua53_BattleCity_Scene1_NullGraphics);*/

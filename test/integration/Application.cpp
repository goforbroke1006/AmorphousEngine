//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <string>

#include "InputReaderFake.h"
#include "../../include/Application.h"
#include "../../include/Graphics/OgreOne.h"
#include "../../include/Calculation/Lua53.h"

namespace AmE {
    TEST(TestApplication_main, _00_empty_scene) {
        const std::string engineRoot = ".";
        const std::string projectRoot = "./testdata";

        auto *pGraphicsEngine = new AmE::OgreOne("plugins.cfg", projectRoot);
        auto *pCalculationEngine = new AmE::Lua53(engineRoot, projectRoot);
        auto *pInputReader = new InputReaderFake();

        auto *app = new Application(
                engineRoot,
                projectRoot,
                pGraphicsEngine,
                pCalculationEngine,
                pInputReader);

        app->loadScene("./Scenes/level-0.json");
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 3);
        // Scene contains 3 object
        //   - Application system
        //   - Main camera
        //   - Player

        // Do operation from AmE::Application::runMainLoop()
        pGraphicsEngine->initialize(app->getSceneState());
        pCalculationEngine->initialize(app->getSceneState());

        app->updateOneFrame();
        app->updateOneFrame();
        app->updateOneFrame();
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 3);
        // Still same objects count

        // Initialize the trigger for launching projectile.
        // PlayerController creates new projectile on LMB click.
        pInputReader->getState()->pressed[AmE::KeyCode_Mouse0] = true;
        app->updateOneFrame();
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 4);

        app->updateOneFrame();
        app->updateOneFrame();
        app->updateOneFrame();
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 4);
        // Projectile still exists.

        // Projectile lives 10 frames. 4 of them already rendered.
        // Move to the end.
        for (size_t idx = 0; idx < 10; ++idx)
            app->updateOneFrame();
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 3);
        // Projectile should disappear.

        app->updateOneFrame();
        app->updateOneFrame();
        app->updateOneFrame();
        EXPECT_EQ(app->getSceneState()->getSceneGameObjects().size(), 3);
        // Initial object set still on the scene.

        EXPECT_FALSE(app->getSceneState()->isAppQuit());
        pInputReader->getState()->released[AmE::KeyCode_Escape] = true;
        app->updateOneFrame();
        // After releasing ESC application should be closed.
        EXPECT_TRUE(app->getSceneState()->isAppQuit());

        delete app;
    }
}

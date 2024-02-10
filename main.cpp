//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // std::cout std::cerr
#include <string>
#include <filesystem>

#include "include/Application.h"
#include "include/Graphics/OgreOne.h"
#include "include/Graphics/Irrlicht.h"
#include "include/Calculation/Lua53.h"

int main(int argc, char **argv) {
    // Usage:
    //     AmorphousEngine OgreNext /engine/binaries/path /project/path ./Scenes/level-0.json

    std::filesystem::path cwd = std::filesystem::current_path();

    std::string graphicsMode = "Ogre"; // OgreNext | Irrlicht
    std::string engineRoot = cwd.string() + std::filesystem::path::preferred_separator + "";
    std::string projectRoot = "./projects/01-HelloWorld/";
//    std::string projectRoot = "./projects/02-BattleCity-Clone/";
    std::string sceneFilename = "./Scenes/level-0.json";

    if (argc > 1)
        graphicsMode = argv[1];
    if (argc > 2)
        engineRoot = argv[2];
    if (argc > 3)
        projectRoot = argv[3];
    if (argc > 4)
        sceneFilename = argv[4];

    std::cout << "Engine root:    " << engineRoot << std::endl;
    std::cout << "Project root:   " << projectRoot << std::endl;
    std::cout << "Scene filename: " << sceneFilename << std::endl;

    AmE::GraphicsEngine *pGraphicsEngine = nullptr;

    if (graphicsMode == "Ogre") {
#ifdef DEBUG
        std::string pluginsCfgPathname = "plugins_d.cfg";
#else
        std::string pluginsCfgPathname = "plugins.cfg";
#endif

        pGraphicsEngine = new AmE::OgreOne(
                pluginsCfgPathname,
                projectRoot
        );
    } else if (graphicsMode == "Irrlicht") {
        pGraphicsEngine = new AmE::Irrlicht();
    } else {
        throw std::runtime_error("unexpected graphics type: " + graphicsMode);
    }

    auto *pCalculationEngine = new AmE::Lua53(engineRoot, projectRoot);
    auto *pApplication = new AmE::Application(engineRoot, projectRoot, pGraphicsEngine, pCalculationEngine);
    pApplication->loadScene(sceneFilename);
    pApplication->runMainLoop();

    delete pApplication;
    delete pCalculationEngine;
    delete pGraphicsEngine;

    return EXIT_SUCCESS;
}
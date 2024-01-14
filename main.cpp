//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // std::cout std::cerr
#include <string>
#include <filesystem>

#include "include/Application.h"
#include "include/Graphics/OgreNext/OgreNext.h"
#include "include/Graphics/Irrlicht.h"
#include "include/Calculation/Lua53.h"

int main(int argc, char **argv) {
    std::filesystem::path cwd = std::filesystem::current_path();

    std::string graphicsMode = "OgreNext"; // OgreNext | Irrlicht
    std::string engineRoot = cwd.string() + std::filesystem::path::preferred_separator + "";
    std::string projectRoot = "./projects/HelloWorld/";
    std::string sceneFilename = "./Scenes/level-0.json";

    // TODO: parse arguments
    if (argc > 1)
        graphicsMode = argv[1];

    std::cout << "Engine root:    " << engineRoot << std::endl;
    std::cout << "Project root:   " << projectRoot << std::endl;
    std::cout << "Scene filename: " << sceneFilename << std::endl;

    AmE::GraphicsEngine *pGraphicsEngine = nullptr;

    if (graphicsMode == "OgreNext") {
#ifdef DEBUG
        std::string pluginsCfgPathname = "plugins_d.cfg";
#else
        std::string pluginsCfgPathname = "plugins.cfg";
#endif

        pGraphicsEngine = new AmE::OgreNext(
                pluginsCfgPathname,
                projectRoot
        );
    }
    if (graphicsMode == "Irrlicht") {
        pGraphicsEngine = new AmE::Irrlicht();
    }

    auto *pCalculationEngine = new AmE::Lua53(projectRoot);
    auto *pApplication = new AmE::Application(engineRoot, projectRoot, pGraphicsEngine, pCalculationEngine);
    pApplication->loadScene(sceneFilename);
    pApplication->runMainLoop();

    delete pApplication;
    delete pCalculationEngine;
    delete pGraphicsEngine;

    return EXIT_SUCCESS;
}
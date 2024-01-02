//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // std::cout std::cerr
#include <string>
#include <filesystem>

#include "include/Application.h"
#include "include/Graphics/OgreNext/OgreNext.h"
#include "include/Calculation/Lua53.h"

int main(int argc, char **argv) {
    std::filesystem::path cwd = std::filesystem::current_path();

    std::string engineRoot = cwd.string() + std::filesystem::path::preferred_separator + "";
    std::string projectRoot = "./projects/HelloWorld/";
    std::string sceneFilename = "./Scenes/level-0.json";
    // TODO: parse arguments

    std::cout << "Engine root:    " << engineRoot << std::endl;
    std::cout << "Project root:   " << projectRoot << std::endl;
    std::cout << "Scene filename: " << sceneFilename << std::endl;

    auto *pGraphicsEngine = new OgreNext(
            "/usr/local/share/OGRE/plugins_d.cfg",
            "/usr/local/share/OGRE/resources2.cfg"
    );
    auto *pCalculationEngine = new Lua53(projectRoot);
    auto *pApplication = new Application(engineRoot, projectRoot, pGraphicsEngine, pCalculationEngine);
    pApplication->loadScene(sceneFilename);
    pApplication->runMainLoop();

    delete pApplication;
    delete pCalculationEngine;
    delete pGraphicsEngine;

    return EXIT_SUCCESS;
}
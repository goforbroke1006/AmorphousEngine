//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_APPLICATION_H
#define AMORPHOUS_ENGINE_APPLICATION_H

#include <string>
#include <map>

#include "GameObject.h"
#include "GraphicsEngine.h"
#include "CalculationEngine.h"

class Application {
public:
    explicit Application(
            std::string mEngineRoot,
            std::string mProjectRoot,
            GraphicsEngine *mGraphicsEngine,
            CalculationEngine *mCalculationEngine
    );

    virtual ~Application();

    void loadScene(const std::string &filepath);

    void runMainLoop();

private:
    std::string mEngineRoot;
    std::string mProjectRoot;

    GraphicsEngine *mGraphicsEngine;
    CalculationEngine *mCalculationEngine;

    std::map<std::string, GameObject *> mGameObjects;
};

#endif //AMORPHOUS_ENGINE_APPLICATION_H

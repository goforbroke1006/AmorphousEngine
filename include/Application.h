//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_APPLICATION_H
#define OGRITY_APPLICATION_H

#include <string>

#include "types.h"
#include "GameObject.h"
#include "GraphicsEEngine.h"
#include "CalculationEngine.h"

class Application {
public:
    explicit Application(
            std::string mEngineRoot,
            std::string mProjectRoot,
            GraphicsEEngine *mGraphicsEngine,
            CalculationEngine *mCalculationEngine
    );

    void loadScene(const std::string &filepath);

    void runMainLoop();

private:
    std::string mEngineRoot;
    std::string mProjectRoot;

    GraphicsEEngine *mGraphicsEngine;
    CalculationEngine *mCalculationEngine;

    std::vector<GameObject *> mGameObjects;
};

#endif //OGRITY_APPLICATION_H

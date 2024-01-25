//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_APPLICATION_H
#define AMORPHOUS_ENGINE_APPLICATION_H

#include <string>

#include "Core/KeyCode.h"
#include "Core/GameObject.h"
#include "GraphicsEngine.h"
#include "CalculationEngine.h"
#include "InputReader.h"

namespace AmE {
    class Application {
    public:
        explicit Application(
                std::string mEngineRoot,
                std::string mProjectRoot,
                GraphicsEngine *mGraphicsEngine,
                CalculationEngine *mCalculationEngine
        );

        virtual ~Application();

        /**
         * Loads scene from JSON file.
         * @param filepath - can be related to project root
         */
        void loadScene(const std::string &filepath);

        /**
         * Main execution method.
         * - Read inputs
         * - Put date to calculation engine
         * - Calculate for one frame
         * - Pull game objects data
         * - Apply game objects data to graphic engine scene
         */
        void runMainLoop();

    private:
        std::string mEngineRoot;
        std::string mProjectRoot;

        GraphicsEngine *mGraphicsEngine;
        CalculationEngine *mCalculationEngine;

        std::map<GameObjectInstanceID, GameObject *> mSceneGameObjects;
        std::map<std::string, GameObject *> mPrefabGameObjects;
    };
}

#endif //AMORPHOUS_ENGINE_APPLICATION_H

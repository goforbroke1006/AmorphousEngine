//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_CALCULATION_ENGINE_H
#define AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

#include <map>
#include <string>

#include "InputsState.h"
#include "SceneState.h"

namespace AmE {
    /**
     * CalculationEngine executes components code.
     */
    class CalculationEngine {
    public:
        /**
         * Prepare calculation engine.
         * - Create game objects
         * - Attach components to each game object
         * @param gameObjects
         */
        virtual void initialize(
                const std::map<GameObjectInstanceID, GameObject *> &sceneGameObjects,
                const std::map<std::string, GameObject *> &prefabGameObjects
        ) = 0;

        /**
         * Calculation lifecycle:
         * - Execute each component
         * - For each game object pull position, rotation and scale
         * @param preUpdateFrameData
         * @param sceneState
         */
        virtual void update(
                const InputsState *preUpdateFrameData,
                SceneState *sceneState
        ) = 0;
    };
}

#endif //AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

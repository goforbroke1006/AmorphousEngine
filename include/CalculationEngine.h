//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_CALCULATION_ENGINE_H
#define AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

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
        virtual void initialize(const SceneState *const sceneState) = 0;

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

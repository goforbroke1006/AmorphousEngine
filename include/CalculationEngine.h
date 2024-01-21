//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_CALCULATION_ENGINE_H
#define AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

#include <map>

#include "PreUpdateFrameData.h"
#include "SceneState.h"

namespace AmE {
    class CalculationEngine {
    public:
        /**
         * Prepare calculation engine.
         * @param gameObjects
         */
        virtual void initialize(const std::map<GameObjectInstanceID, GameObject *> &gameObjects) = 0;

        virtual void update(
                const PreUpdateFrameData *preUpdateFrameData,
                SceneState *sceneState
        ) = 0;
    };
}

#endif //AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_CALCULATION_ENGINE_H
#define AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

#include "GameObject.h"

class CalculationEngine {
public:
    /**
     * Prepare calculation engine.
     * @param gameObjects
     */
    virtual void initialize(const std::map<std::string, GameObject *> &gameObjects) = 0;

    virtual void update(std::map<std::string, GameObject *> &gameObjects) = 0;
};

#endif //AMORPHOUS_ENGINE_CALCULATION_ENGINE_H

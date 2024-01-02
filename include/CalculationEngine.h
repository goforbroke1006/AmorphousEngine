//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUSENGINE_CALCULATIONENGINE_H
#define AMORPHOUSENGINE_CALCULATIONENGINE_H

#include "GameObject.h"

class CalculationEngine {
public:
    /**
     * Prepare calculation engine.
     * @param gameObjects
     */
    virtual void initialize(const std::vector<GameObject *> &gameObjects) = 0;

    virtual std::vector<GameObject *> update() = 0;
};

#endif //AMORPHOUSENGINE_CALCULATIONENGINE_H

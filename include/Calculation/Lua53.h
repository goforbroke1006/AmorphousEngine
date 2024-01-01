//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_LUA53_H
#define OGRITY_LUA53_H

#include <vector>

#include "../types.h"

class Lua53 : public CalculationEngine {
public:
    void initialize(const std::vector<GameObject *> &gameObjects) override;

    std::vector<GameObject *> update() override;
};

#endif //OGRITY_LUA53_H

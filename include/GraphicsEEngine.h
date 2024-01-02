//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUSENGINE_GRAPHICSEENGINE_H
#define AMORPHOUSENGINE_GRAPHICSEENGINE_H

#include "GameObject.h"

class GraphicsEEngine {
public:
    virtual void initialize(const std::vector<GameObject *> &gameObjects) = 0;

    virtual void update(const std::vector<GameObject *> &gameObjects) = 0;
};

#endif //AMORPHOUSENGINE_GRAPHICSEENGINE_H

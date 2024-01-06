//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H
#define AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H

#include <map>

#include "GameObject.h"

class GraphicsEngine {
public:
    virtual ~GraphicsEngine() = default;

    virtual void initialize(const std::map<std::string, GameObject *> &gameObjects) = 0;

    virtual bool update(const std::map<std::string, GameObject *> &gameObjects) = 0;
};

#endif //AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H

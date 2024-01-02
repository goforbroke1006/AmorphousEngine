//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_OGRENEXT_H
#define OGRITY_OGRENEXT_H

#include "../types.h"
#include "../GameObject.h"
#include "../GraphicsEngine.h"

class OgreNext : public GraphicsEngine {
public:
    void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

    void update(const std::map<std::string,GameObject *> &gameObjects) override;
};

#endif //OGRITY_OGRENEXT_H

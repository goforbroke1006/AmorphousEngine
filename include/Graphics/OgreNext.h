//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_OGRENEXT_H
#define OGRITY_OGRENEXT_H

#include "../types.h"

class OgreNext : public GraphicsEEngine {
public:
    void initialize(const std::vector<GameObject *> &gameObjects) override;

    void update(const std::vector<GameObject *> &gameObjects) override;
};

#endif //OGRITY_OGRENEXT_H

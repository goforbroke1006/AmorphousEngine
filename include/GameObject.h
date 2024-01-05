//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_GAME_OBJECT_H
#define AMORPHOUS_ENGINE_GAME_OBJECT_H

#include <map>

#include "types.h"

struct GameObject {
public:
    explicit GameObject(std::string mID, std::string mName);

    virtual ~GameObject();

    bool isCamera();

    std::string mID;
    std::string mName;
    Transform *mTransform;
    std::map<std::string, Component> mComponents;

    std::string mMeshPathname;
};

#endif //AMORPHOUS_ENGINE_GAME_OBJECT_H

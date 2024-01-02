//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_GAME_OBJECT_H
#define AMORPHOUS_ENGINE_GAME_OBJECT_H

#include "types.h"

struct GameObject {
public:
    explicit GameObject() = default;

    virtual ~GameObject();

    std::string mID;
    std::string mName;
    Transform *mTransform;
    std::vector<Component> mComponents;

public:
    static GameObject *build(const std::string &name);

    static std::string buildID(const std::string &name);

    static size_t sNextNumID;

};

#endif //AMORPHOUS_ENGINE_GAME_OBJECT_H

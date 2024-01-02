//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUSENGINE_GAMEOBJECT_H
#define AMORPHOUSENGINE_GAMEOBJECT_H

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

#endif //AMORPHOUSENGINE_GAMEOBJECT_H

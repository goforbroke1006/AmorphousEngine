//
// Created by goforbroke on 1/2/24.
//

#include "../include/GameObject.h"

GameObject::~GameObject() {
    delete mTransform;
}

GameObject *GameObject::build(const std::string &name) {
    auto *go = new GameObject;
    go->mID = buildID(name);
    go->mName = name;
    go->mTransform = new Transform();
    return go;
}

std::string GameObject::buildID(const std::string &name) {
    const std::string &id = "go " + std::to_string(sNextNumID) + " :: " + name;
    ++sNextNumID;
    return id;
}

size_t GameObject::sNextNumID = 0;

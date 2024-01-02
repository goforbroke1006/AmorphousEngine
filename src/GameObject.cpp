//
// Created by goforbroke on 1/2/24.
//

#include <utility>
#include <algorithm>

#include "../include/GameObject.h"

GameObject::GameObject(std::string mID, std::string mName)
        : mID(std::move(mID)),
          mName(std::move(mName)),
          mTransform(new Transform()) {}

GameObject::~GameObject() {
    delete mTransform;
}

bool GameObject::isCamera() {
    return std::any_of(mComponents.begin(), mComponents.end(), [](const auto &cmp){
        return cmp.mName == "Camera";
    });
}

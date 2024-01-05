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
    return mComponents.find("Camera") != mComponents.end();
}

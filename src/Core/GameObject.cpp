//
// Created by goforbroke on 1/2/24.
//

#include <utility>
#include <algorithm>

#include "../../include/Core/GameObject.h"

GameObject::GameObject(std::string mID, std::string mName)
        : mID(std::move(mID)),
          mName(std::move(mName)),
          mTransform(new Transform()) {}

GameObject::~GameObject() {
    delete mTransform;
}

bool GameObject::isCamera() const {
    return mComponents.find("Camera") != mComponents.end();
}

bool GameObject::isLight() const {
    return mComponents.find("Light") != mComponents.end();
}

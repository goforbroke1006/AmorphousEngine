//
// Created by goforbroke on 1/2/24.
//

#include <utility>
#include <algorithm>

#include "../../include/Core/GameObject.h"

AmE::GameObject::GameObject(std::string mID, std::string mName)
        : mID(std::move(mID)),
          mName(std::move(mName)),
          mTransform(new Transform()) {}

AmE::GameObject::~GameObject() {
    delete mTransform;
}

bool AmE::GameObject::isCamera() const {
    return mComponents.find("Camera") != mComponents.end();
}

bool AmE::GameObject::isLight() const {
    return mComponents.find("Light") != mComponents.end();
}

//
// Created by goforbroke on 1/2/24.
//

#include <utility>
#include <algorithm>

#include "../../include/Core/GameObject.h"

AmE::GameObject::GameObject(GameObjectInstanceID key, std::string name)
        : mID(key),
          mName(std::move(name)),
          mTransform(new Transform()) {}

AmE::GameObject::~GameObject() {
    delete mTransform;

    for (const auto &[_, pCmp]: mComponents)
        delete pCmp;
    mComponents.clear();
}

GameObjectInstanceID AmE::GameObject::getID() const {
    return mID;
}

const std::string &AmE::GameObject::getName() const {
    return mName;
}

AmE::Transform *const AmE::GameObject::getTransform() const {
    return mTransform;
}

const std::map<std::string, AmE::Component *> &AmE::GameObject::getComponents() const {
    return mComponents;
}

AmE::Component *AmE::GameObject::getComponent(const std::string &name) const {
    return mComponents.at(name);
}

void AmE::GameObject::addComponent(AmE::Component *const component) {
    mComponents[component->mName] = component;
}

const std::string &AmE::GameObject::getMeshPathname() const {
    return mMeshPathname;
}

void AmE::GameObject::setMeshPathname(const std::string &meshPathname) {
    GameObject::mMeshPathname = meshPathname;
}

bool AmE::GameObject::isCamera() const {
    return mComponents.find("Camera") != mComponents.end();
}

bool AmE::GameObject::isLight() const {
    return mComponents.find("Light") != mComponents.end();
}



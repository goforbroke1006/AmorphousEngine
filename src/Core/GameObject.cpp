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

bool AmE::GameObject::hasComponent(const std::string &path) const {
    return mComponents.find(path) != mComponents.end();
}

AmE::Component *AmE::GameObject::getComponent(const std::string &path) const {
    if (mComponents.find(path) == mComponents.end())
//        return nullptr;
        throw std::runtime_error("GameObject '" + mName + "' has no component " + path);

    return mComponents.at(path);
}

void AmE::GameObject::addComponent(AmE::Component *const component) {
    mComponents[component->mPathname] = component;
}

bool AmE::GameObject::hasMeshRender() const {
    return mComponents.find("Component/MeshRender") != mComponents.end();
}

bool AmE::GameObject::isCamera() const {
    return mComponents.find("Component/Camera") != mComponents.end();
}

bool AmE::GameObject::isLight() const {
    return mComponents.find("Component/Light") != mComponents.end();
}



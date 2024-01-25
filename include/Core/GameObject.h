//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_GAME_OBJECT_H
#define AMORPHOUS_ENGINE_GAME_OBJECT_H

#include <string>
#include <map>

#include "Transform.h"
#include "Component.h"

#define GameObjectInstanceID long long

namespace AmE {
    struct GameObject {
    public:
        explicit GameObject(GameObjectInstanceID key, std::string name);

        virtual ~GameObject();

        [[nodiscard]] GameObjectInstanceID getID() const;

        [[nodiscard]] const std::string &getName() const;

        [[nodiscard]] Transform *const getTransform() const;

        const std::map<std::string, Component *> &getComponents() const;

        [[nodiscard]] Component *getComponent(const std::string &name) const;

        void addComponent(Component *const component);

        const std::string &getMeshPathname() const;

        void setMeshPathname(const std::string &meshPathname);

        [[nodiscard]] bool isCamera() const;

        [[nodiscard]] bool isLight() const;

    private:
        GameObjectInstanceID mID;
        std::string mName;
        Transform *mTransform;
        std::map<std::string, Component *> mComponents;

        std::string mMeshPathname;
    };
}

#endif //AMORPHOUS_ENGINE_GAME_OBJECT_H

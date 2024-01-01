//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_TYPES_H
#define OGRITY_TYPES_H

#include <string>
#include <vector>

struct Vector3 {
    double mX, mY, mZ;
};

struct Quaternion {
    double mX, mY, mZ, mW;
};

struct Transform {
    Vector3 mPosition;
    Quaternion mRotation;
};

struct ComponentArg {
    double doubleVal;
    std::string refVal;
};

struct Component {
    std::string mPathname;
    std::vector<ComponentArg> args;
};

struct GameObject {
    std::string mName;
    Transform *mTransform;
    std::vector<Component> mComponents;
};

class GraphicsEEngine {
public:
    virtual void initialize(const std::vector<GameObject *> &gameObjects) = 0;

    virtual void update(const std::vector<GameObject *> &gameObjects) = 0;
};

class CalculationEngine {
public:
    /**
     * Prepare calculation engine.
     * @param gameObjects
     */
    virtual void initialize(const std::vector<GameObject *> &gameObjects) = 0;

    virtual std::vector<GameObject *> update() = 0;
};

#endif //OGRITY_TYPES_H

//
// Created by goforbroke on 1/1/24.
//

#ifndef AMORPHOUS_ENGINE_TYPES_H
#define AMORPHOUS_ENGINE_TYPES_H

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <any>

struct Vector3 {
    double mX, mY, mZ;

    void Set(double x, double y, double z) {
        mX = x;
        mY = y;
        mZ = z;
    }
};

struct Quaternion {
    double mX, mY, mZ;

    void Set(double x, double y, double z) {
        mX = x;
        mY = y;
        mZ = z;
    }
};

struct Transform {
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mLocalScale;
};

#endif //AMORPHOUS_ENGINE_TYPES_H

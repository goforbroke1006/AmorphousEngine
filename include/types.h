//
// Created by goforbroke on 1/1/24.
//

#ifndef OGRITY_TYPES_H
#define OGRITY_TYPES_H

#include <string>
#include <utility>
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

struct Prop {
    std::string mName;
    std::string mValue;
};

struct Component {
    std::string mName;
    std::string mPathname;
    std::vector<Prop> mProperties;
};





#endif //OGRITY_TYPES_H

//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_TRANSFORM_H
#define AMORPHOUS_ENGINE_TRANSFORM_H

#include "Vector3.h"
#include "Quaternion.h"

struct Transform {
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mLocalScale;
};

#endif //AMORPHOUS_ENGINE_TRANSFORM_H

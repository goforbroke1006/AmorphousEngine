//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_TRANSFORM_H
#define AMORPHOUS_ENGINE_TRANSFORM_H

#include "Vector3.h"
#include "Quaternion.h"

namespace AmE {
    class Transform {
    public:
        Transform() {
            mPosition = Vector3();
            mRotation = Quaternion::Euler(0.0, 0.0, 0.0);
            mLocalScale = Vector3(1.0, 1.0, 1.0);
        }

        Vector3 mPosition;
        Quaternion mRotation;
        Vector3 mLocalScale;
    };
}

#endif //AMORPHOUS_ENGINE_TRANSFORM_H

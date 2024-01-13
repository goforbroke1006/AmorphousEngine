//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_VECTOR3_H
#define AMORPHOUS_ENGINE_VECTOR3_H

class Vector3 {
public:
    double mX, mY, mZ;

    Vector3() = default;

    Vector3(double mX, double mY, double mZ) : mX(mX), mY(mY), mZ(mZ) {}

    void Set(double x, double y, double z) {
        mX = x;
        mY = y;
        mZ = z;
    }
};

#endif //AMORPHOUS_ENGINE_VECTOR3_H

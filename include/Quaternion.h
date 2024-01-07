//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_QUATERNION_H
#define AMORPHOUS_ENGINE_QUATERNION_H

struct Quaternion {
    double mX, mY, mZ;

    void Set(double x, double y, double z) {
        mX = x;
        mY = y;
        mZ = z;
    }
};

#endif //AMORPHOUS_ENGINE_QUATERNION_H

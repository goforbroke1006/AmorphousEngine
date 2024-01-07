//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_QUATERNION_H
#define AMORPHOUS_ENGINE_QUATERNION_H

#include <cmath>

class Quaternion {
public:
    double mX, mY, mZ, mW;

    void Set(double x, double y, double z, double w) {
        mX = x;
        mY = y;
        mZ = z;
        mW = w;
    }

    static Quaternion Euler(double roll, double pitch, double yaw) {
        roll = roll * (M_PI / 180);
        pitch = pitch * (M_PI / 180);
        yaw = yaw * (M_PI / 180);

        double cr = std::cos(roll * 0.5);
        double sr = std::sin(roll * 0.5);
        double cp = std::cos(pitch * 0.5);
        double sp = std::sin(pitch * 0.5);
        double cy = std::cos(yaw * 0.5);
        double sy = std::sin(yaw * 0.5);

        auto q = Quaternion();

        q.mW = cr * cp * cy + sr * sp * sy;
        q.mX = sr * cp * cy - cr * sp * sy;
        q.mY = cr * sp * cy + sr * cp * sy;
        q.mZ = cr * cp * sy - sr * sp * cy;

        return q;
    }
};

#endif //AMORPHOUS_ENGINE_QUATERNION_H

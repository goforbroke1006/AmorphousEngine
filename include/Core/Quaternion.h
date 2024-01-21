//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_QUATERNION_H
#define AMORPHOUS_ENGINE_QUATERNION_H

#include <cmath>

namespace AmE {
    class Quaternion {
    public:
        Quaternion() : mX(0.0), mY(0.0), mZ(0.0), mW(1.0) {}

        [[nodiscard]] double getX() const {
            return mX;
        }

        [[nodiscard]] double getY() const {
            return mY;
        }

        [[nodiscard]] double getZ() const {
            return mZ;
        }

        [[nodiscard]] double getW() const {
            return mW;
        }

        /**
         * https://docs.unity3d.com/ScriptReference/Quaternion.Set.html
         * @param x
         * @param y
         * @param z
         * @param w
         */
        void Set(double x, double y, double z, double w) {
            if (std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w)) {
                return;
            }

            mX = x;
            mY = y;
            mZ = z;
            mW = w;
        }

    private:
        double mX, mY, mZ, mW;

    public:
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
}

#endif //AMORPHOUS_ENGINE_QUATERNION_H

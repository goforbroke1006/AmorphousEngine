//
// Created by goforbroke on 1/7/24.
//

#ifndef AMORPHOUS_ENGINE_VECTOR3_H
#define AMORPHOUS_ENGINE_VECTOR3_H

#include <cmath>

namespace AmE {
    class Vector3 {
    public:
        Vector3() : mX(0.0), mY(0.0), mZ(0.0) {}

        Vector3(double x, double y, double z) : mX(x), mY(y), mZ(z) {}

        [[nodiscard]] double getX() const {
            return mX;
        }

        [[nodiscard]] double getY() const {
            return mY;
        }

        [[nodiscard]] double getZ() const {
            return mZ;
        }

        void Set(double x, double y, double z) {
            if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
                return;
            }

            mX = x;
            mY = y;
            mZ = z;
        }

    private:
        double mX, mY, mZ;
    };
}

#endif //AMORPHOUS_ENGINE_VECTOR3_H

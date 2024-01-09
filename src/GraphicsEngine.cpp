//
// Created by goforbroke on 1/10/24.
//

#include "../include/GraphicsEngine.h"

Vector3 GraphicsEngine::convertPositionLeftToRightHand(Vector3 position) {
    position.mZ *= -1;
    return position;
}

Quaternion GraphicsEngine::convertRotationLeftToRightHand(Quaternion rotation) {
    rotation.mY *= -1;
    return rotation;
}

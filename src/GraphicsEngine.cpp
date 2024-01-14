//
// Created by goforbroke on 1/10/24.
//

#include "../include/GraphicsEngine.h"

AmE::Vector3 AmE::GraphicsEngine::convertPositionLeftToRightHand(Vector3 position) {
    position.mZ *= -1;
    return position;
}

AmE::Quaternion AmE::GraphicsEngine::convertRotationLeftToRightHand(Quaternion rotation) {
    rotation.mY *= -1;
    return rotation;
}

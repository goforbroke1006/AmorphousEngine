//
// Created by goforbroke on 1/10/24.
//

#include "../include/GraphicsEngine.h"

AmE::Vector3 AmE::GraphicsEngine::convertPositionLeftToRightHand(Vector3 position) {
    position.Set(position.getX(), position.getY(), position.getZ() * -1);
    return position;
}

AmE::Quaternion AmE::GraphicsEngine::convertRotationLeftToRightHand(Quaternion rotation) {
    rotation.Set(rotation.getX(), rotation.getY() * -1, rotation.getZ(), rotation.getW());
    return rotation;
}


AmE::Vector3 AmE::GraphicsEngine::convertPositionXYZ2XZY(Vector3 position) {
    //position.Set(position.getX(),  position.getZ() * -1, position.getY());
    position.Set(position.getX(), position.getY(), position.getZ() * -1);
    return position;
}

AmE::Quaternion AmE::GraphicsEngine::convertRotationXYZ2XZY(Quaternion rotation) {
    //rotation.Set(rotation.getX(), rotation.getZ(), rotation.getY() * -1, rotation.getW());
    rotation.Set(rotation.getX(), rotation.getY() * -1, rotation.getZ(), rotation.getW());
    return rotation;
}
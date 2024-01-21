//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H
#define AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H

#include <map>

#include "Core/GameObject.h"

namespace AmE {
    class GraphicsEngine {
    public:
        virtual ~GraphicsEngine() = default;

        [[nodiscard]] virtual size_t getWindowHnd() const = 0;

        /**
         * Create all game objects and place its right positions and with right rotations.
         * @param gameObjects
         */
        virtual void initialize(const std::map<GameObjectInstanceID, GameObject *> &gameObjects) = 0;

        /**
         * Update game objects.
         * @param gameObjects
         * @return
         */
        virtual bool update(const std::map<GameObjectInstanceID, GameObject *> &gameObjects) = 0;

        virtual void stop() = 0;

        static Vector3 convertPositionLeftToRightHand(Vector3 position);

        static Quaternion convertRotationLeftToRightHand(Quaternion rotation);
    };
}

#endif //AMORPHOUS_ENGINE_GRAPHICS_ENGINE_H

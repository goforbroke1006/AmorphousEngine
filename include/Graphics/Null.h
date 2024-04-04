//
// Created by goforbroke on 4/2/24.
//

#ifndef AMORPHOUS_ENGINE_NULL_H
#define AMORPHOUS_ENGINE_NULL_H

#include "../GraphicsEngine.h"

namespace AmE {
    class Null : public GraphicsEngine {
    public:
        [[nodiscard]] size_t getWindowHnd() const override;

        void initialize(const SceneState *sceneState) override;

        bool update(SceneState *sceneState) override;

        void stop() override;
    };
}

#endif //AMORPHOUS_ENGINE_NULL_H

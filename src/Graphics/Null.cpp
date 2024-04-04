//
// Created by goforbroke on 4/2/24.
//

#include "../../include/Graphics/Null.h"

size_t AmE::Null::getWindowHnd() const {
    return 0;
}

void AmE::Null::initialize(const AmE::SceneState *const sceneState) {
    for (const auto &[_, pGameObj]: sceneState->getSceneGameObjects()) {
        // TODO:
    }
}

bool AmE::Null::update(AmE::SceneState *const sceneState) {
    // TODO:
    return true;
}

void AmE::Null::stop() {
    // TODO:
}

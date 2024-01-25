//
// Created by goforbroke on 1/25/24.
//

#ifndef AMORPHOUS_ENGINE_PREFAB_H
#define AMORPHOUS_ENGINE_PREFAB_H

#include <string>


#include "Core/GameObject.h"

namespace AmE {
    class PrefabManager {
    public:
        static GameObject *loadFromFile(const std::string &jsonFilepath);
    };
}

#endif //AMORPHOUS_ENGINE_PREFAB_H

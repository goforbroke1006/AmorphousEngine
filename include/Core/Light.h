//
// Created by goforbroke on 2/7/24.
//

#ifndef AMORPHOUS_ENGINE_LIGHT_H
#define AMORPHOUS_ENGINE_LIGHT_H

#include <string>
#include <stdexcept>

enum LightType {
    Directional, Point, Spot,
};

/**
 * https://docs.unity3d.com/2022.1/Documentation/ScriptReference/Light.html
 * https://ogrecave.github.io/ogre/api/1.12/tut__lights_cameras_shadows.html#bt2Lights
 */
class Light {
public:
    static LightType parse(const std::string &type) {
        if ("Directional" == type) return LightType::Directional;
        if ("Point" == type) return LightType::Point;
        if ("Spot" == type) return LightType::Spot;

        throw std::runtime_error("unknown light type: " + type);
    }
};

#endif //AMORPHOUS_ENGINE_LIGHT_H

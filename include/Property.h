//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUS_ENGINE_PROPERTY_H
#define AMORPHOUS_ENGINE_PROPERTY_H

#include "types.h"

#include <sstream>
#include <exception>

class PropType {
public:
    enum Kind {
        PropTypeUnknown,
        PropTypeDouble,
        PropTypeString,
        PropTypeColor,
        PropTypeVector3,
        PropTypeGameObject,
        PropTypeGameObjectTransform,
    };

    static PropType::Kind parseKind(const std::string &str) {
        if (str == "double") return PropTypeDouble;
        if (str == "string") return PropTypeString;
        if (str == "Color") return PropTypeColor;
        if (str == "Vector3") return PropTypeVector3;
        if (str == "GameObject") return PropTypeGameObject;
        if (str == "GameObjectTransform") return PropTypeGameObjectTransform;

        throw std::runtime_error("parsing prop kind failed: " + str);
    }

    static std::string asString(const PropType::Kind & kind) {
        switch (kind) {
            case PropTypeUnknown: return "<unknown>";
            case PropTypeDouble: return "double";
            case PropTypeString: return "string";
            case PropTypeColor: return "Color";
            case PropTypeVector3: return "Vector3";
            case PropTypeGameObject: return "GameObject";
            case PropTypeGameObjectTransform: return "GameObjectTransform";
        }

        return "";
    }
};

struct Property {
    std::string mName;
    PropType::Kind mType;
    std::any mValue;

    static std::any parseValue(PropType::Kind propType, const std::string &raw) {
        switch (propType) {
            case PropType::PropTypeUnknown:
                return "";
            case PropType::PropTypeDouble:
                return std::stod(raw);
            case PropType::PropTypeString:
                return raw;
            case PropType::PropTypeColor: {
                std::string data;
                std::stringstream ss(raw);

                auto color = Color{};

                getline(ss, data, ' ');
                color.mR = std::stod(data);

                getline(ss, data, ' ');
                color.mG = std::stod(data);

                getline(ss, data, ' ');
                color.mB = std::stod(data);

                getline(ss, data, ' ');
                color.mA = std::stod(data);

                return color;
            }
            case PropType::PropTypeVector3: {
                std::string data;
                std::stringstream ss(raw);

                auto vector = Vector3{};

                getline(ss, data, ' ');
                vector.mX = std::stod(data);

                getline(ss, data, ' ');
                vector.mY = std::stod(data);

                getline(ss, data, ' ');
                vector.mZ = std::stod(data);

                return vector;
            }
            case PropType::PropTypeGameObject:
            case PropType::PropTypeGameObjectTransform:
                return raw;
        }
    }
};

#endif //AMORPHOUS_ENGINE_PROPERTY_H

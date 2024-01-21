//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUS_ENGINE_PROPERTY_H
#define AMORPHOUS_ENGINE_PROPERTY_H

#include <any>
#include <sstream>
#include <exception>

#include "Color.h"

namespace AmE {
    class PropType {
    public:
        enum Kind {
            PropTypeBoolean,
            PropTypeDouble,
            PropTypeString,
            PropTypeColor,
            PropTypeVector3,
            PropTypeGameObject,
            PropTypeGameObjectTransform,
        };

        static PropType::Kind parseKind(const std::string &str) {
            if (str == "boolean") return PropTypeBoolean;
            if (str == "double") return PropTypeDouble;
            if (str == "string") return PropTypeString;
            if (str == "Color") return PropTypeColor;
            if (str == "Vector3") return PropTypeVector3;
            if (str == "GameObject") return PropTypeGameObject;
            if (str == "GameObjectTransform") return PropTypeGameObjectTransform;

            throw std::runtime_error("parsing prop kind failed: " + str);
        }

        static std::string asString(const PropType::Kind &kind) {
            switch (kind) {
                case PropTypeBoolean:
                    return "boolean";
                case PropTypeDouble:
                    return "double";
                case PropTypeString:
                    return "string";
                case PropTypeColor:
                    return "Color";
                case PropTypeVector3:
                    return "Vector3";
                case PropTypeGameObject:
                    return "GameObject";
                case PropTypeGameObjectTransform:
                    return "GameObjectTransform";
                default:
                    throw std::runtime_error("unexpected prop kind");
            }
        }
    };

    struct Property {
        std::string mName;
        PropType::Kind mType;
        std::any mValue;

        int asInt() const {
            return std::any_cast<int>(mValue);
        }

        Color asColor() const {
            return std::any_cast<Color>(mValue);
        }

        static std::any parseValue(PropType::Kind propType, const std::string &raw) {
            switch (propType) {
                case PropType::PropTypeBoolean: {
                    return raw == "true" || raw == "1";
                }
                case PropType::PropTypeDouble:
                    return std::stod(raw);
                case PropType::PropTypeString:
                    return raw;
                case PropType::PropTypeColor: {
                    std::string data;
                    std::stringstream ss(raw);

                    auto color = Color();

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
                    double x, y, z;

                    {
                        std::string data;
                        std::stringstream ss(raw);

                        getline(ss, data, ' ');
                        x = std::stod(data);

                        getline(ss, data, ' ');
                        y = std::stod(data);

                        getline(ss, data, ' ');
                        z = std::stod(data);
                    }

                    return Vector3(x, y, z);
                }
                case PropType::PropTypeGameObject:
                case PropType::PropTypeGameObjectTransform:
                    return raw;
            }
        }
    };
}

#endif //AMORPHOUS_ENGINE_PROPERTY_H

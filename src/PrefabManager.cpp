//
// Created by goforbroke on 1/25/24.
//

#include "../include/PrefabManager.h"

#include <fstream>
#include <stdexcept>

#include <jsoncpp/json/json.h>

#include "../include/Logger.h"

AmE::GameObject *AmE::PrefabManager::loadFromFile(const std::string &jsonFilepath) {
    std::ifstream fileStream(jsonFilepath);
    Json::Reader prefabReader;
    Json::Value prefabObj;

    prefabReader.parse(fileStream, prefabObj);

    if (prefabObj["name"].empty())
        throw std::runtime_error("prefab miss 'name'");

    auto *pGameObj = new GameObject(-1, prefabObj["name"].asString());

    if (!prefabObj["transform"].empty()) {
        Json::Value &trJSON = prefabObj["transform"];

        pGameObj->getTransform()->mPosition.Set(
                trJSON["position"]["x"].asDouble(),
                trJSON["position"]["y"].asDouble(),
                trJSON["position"]["z"].asDouble()
        );
        pGameObj->getTransform()->mRotation = Quaternion::Euler(
                trJSON["rotation"]["x"].asDouble(),
                trJSON["rotation"]["y"].asDouble(),
                trJSON["rotation"]["z"].asDouble()
        );
        pGameObj->getTransform()->mLocalScale.Set(
                trJSON["localScale"]["x"].asDouble(),
                trJSON["localScale"]["y"].asDouble(),
                trJSON["localScale"]["z"].asDouble()
        );
    }

    if (prefabObj["components"].empty())
        Logger::Warn("Prefab '" + jsonFilepath + "' does not have components");

    for (const auto &cmpVal: prefabObj["components"]) {
        std::map<std::string, Property> properties;

        for (const auto &propJSON: cmpVal["properties"]) {
            Property prop;
            prop.mName = propJSON["name"].asString();
            prop.mType = PropType::parseKind(propJSON["type"].asString());
            prop.mValue = Property::parseValue(prop.mType, propJSON["value"].asString());

            properties[prop.mName] = prop;
        }

        auto *pComponent = new Component(
                cmpVal["name"].asString(),
                cmpVal["pathname"].asString(),
                properties
        );

        pGameObj->getComponents()[pComponent->mName] = pComponent;
    }

    return pGameObj;
}

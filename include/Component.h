//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUS_ENGINE_COMPONENT_H
#define AMORPHOUS_ENGINE_COMPONENT_H

#include <string>
#include <map>
#include <utility>

#include "Property.h"
#include "Logger.h"

class Component {
public:
    std::string mName;
    std::string mPathname;
    std::map<std::string, Property> mProperties;

    Component() = default;

    Component(std::string name,
              std::string pathname,
              std::map<std::string, Property> properties
    ) : mName(std::move(name)),
        mPathname(std::move(pathname)),
        mProperties(std::move(properties)) {
        if (mProperties.find("enabled") == mProperties.end())
            mProperties["enabled"] = Property{"enabled", PropType::PropTypeBoolean, true};

        // WORKAROUND: init default camera properties
        if ("Component/Camera" == mPathname) {
            if (mProperties.find("backgroundColor") == mProperties.end())
                mProperties["backgroundColor"] = Property{"backgroundColor", PropType::PropTypeColor, Color::grey};
            if (mProperties.find("nearClipPlane") == mProperties.end())
                mProperties["nearClipPlane"] = Property{"nearClipPlane", PropType::PropTypeDouble, 0.1};
            if (mProperties.find("farClipPlane") == mProperties.end())
                mProperties["farClipPlane"] = Property{"farClipPlane", PropType::PropTypeDouble, 1000.0};
        }
    }

    [[nodiscard]] bool isEnabled() const {
        return std::any_cast<bool>(mProperties.at("enabled").mValue);
    }
};

#endif //AMORPHOUS_ENGINE_COMPONENT_H

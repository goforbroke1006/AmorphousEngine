//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUSENGINE_COMPONENT_H
#define AMORPHOUSENGINE_COMPONENT_H

#include <string>
#include <map>

#include "Property.h"

struct Component {
    std::string mName;
    std::string mPathname;
    std::map<std::string, Property> mProperties;
};

#endif //AMORPHOUSENGINE_COMPONENT_H

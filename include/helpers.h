//
// Created by goforbroke on 2/13/24.
//

#ifndef AMORPHOUS_ENGINE_HELPERS_H
#define AMORPHOUS_ENGINE_HELPERS_H

#include <string>
#include <vector>
#include <sstream>

inline
std::vector<std::string> stringSplit(const std::string &s, char del) {
    std::stringstream test(s);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(test, segment, del)) {
        seglist.push_back(segment);
    }

    return seglist;
}

#endif //AMORPHOUS_ENGINE_HELPERS_H

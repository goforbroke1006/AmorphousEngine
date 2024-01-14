//
// Created by goforbroke on 1/13/24.
//

#include <utility>

#include "../../include/Core/KeyCode.h"

AmE::KeyCode::KeyCode(std::string code) : mCode(std::move(code)) {}

std::string AmE::KeyCode::toString() const {
    return mCode;
}

bool AmE::KeyCode::operator<(const KeyCode &rhs) const {
    return mCode < rhs.mCode;
}

//
// Created by goforbroke on 1/13/24.
//

#include <utility>

#include "../../include/Core/KeyCode.h"

KeyCode::KeyCode(std::string code) : mCode(std::move(code)) {}

const KeyCode KeyCode::W = KeyCode("w");
const KeyCode KeyCode::A = KeyCode("a");
const KeyCode KeyCode::S = KeyCode("s");
const KeyCode KeyCode::D = KeyCode("d");

const KeyCode KeyCode::UpArrow = KeyCode("arrow-up");
const KeyCode KeyCode::DownArrow = KeyCode("arrow-down");
const KeyCode KeyCode::RightArrow = KeyCode("arrow-right");
const KeyCode KeyCode::LeftArrow = KeyCode("arrow-left");

const KeyCode KeyCode::Escape = KeyCode("ESC");
const KeyCode KeyCode::Ctrl = KeyCode("CTRL");
const KeyCode KeyCode::Space = KeyCode("SPACE");

const KeyCode KeyCode::Mouse0 = KeyCode("MSL"); // The Left (or primary) mouse button.
const KeyCode KeyCode::Mouse1 = KeyCode("MSR"); // Right mouse button (or secondary mouse button).
const KeyCode KeyCode::Mouse2 = KeyCode("MSM"); // Middle mouse button (or third button).

bool KeyCode::operator<(const KeyCode &rhs) const {
    return mCode < rhs.mCode;
}

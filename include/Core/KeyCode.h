//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_KEYCODE_H
#define AMORPHOUS_ENGINE_KEYCODE_H

#include <string>

class KeyCode {
public:
    explicit KeyCode() = default;

    explicit KeyCode(std::string code);

    std::string mCode;

public:
    const static KeyCode W;
    const static KeyCode A;
    const static KeyCode S;
    const static KeyCode D;

    const static KeyCode UpArrow;
    const static KeyCode DownArrow;
    const static KeyCode RightArrow;
    const static KeyCode LeftArrow;

    const static KeyCode Escape;
    const static KeyCode Ctrl;
    const static KeyCode Space;

    const static KeyCode Mouse0;
    const static KeyCode Mouse1;
    const static KeyCode Mouse2;

    bool operator<(const KeyCode &rhs) const;
};

#endif //AMORPHOUS_ENGINE_KEYCODE_H

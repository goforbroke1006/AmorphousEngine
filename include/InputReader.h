//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_INPUT_READER_H
#define AMORPHOUS_ENGINE_INPUT_READER_H

#include <map>

#include <SDL2/SDL.h>

#include "Core/KeyCode.h"

class InputReader {
public:
    explicit InputReader();

    void collectCodes();

    [[nodiscard]] const std::map<KeyCode, bool> &pressed() const;

    [[nodiscard]] const std::map<KeyCode, bool> &released() const;

private:
    std::map<KeyCode, bool> mKeysPressed;
    std::map<KeyCode, bool> mKeysReleased;

private:
    static const std::map<SDL_Keycode, KeyCode> sdlToAMEKeyCode;
};

#endif //AMORPHOUS_ENGINE_INPUT_READER_H

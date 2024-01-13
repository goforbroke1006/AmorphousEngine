//
// Created by goforbroke on 1/13/24.
//

#include "../include/InputReader.h"

#include <SDL2/SDL.h>

InputReader::InputReader() {
    for (const auto &pair: InputReader::sdlToAMEKeyCode) {
        mKeysPressed[pair.second] = false;
        mKeysReleased[pair.second] = false;
    }
}

void InputReader::collectCodes() {
    // https://www.studyplan.dev/sdl-dev/sdl2-keyboard-mouse

    for (auto &pair: mKeysPressed)
        pair.second = false;
    for (auto &pair: mKeysReleased)
        pair.second = false;

    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
        if (Event.type == SDL_KEYDOWN) {
            if (sdlToAMEKeyCode.find(Event.key.keysym.sym) == sdlToAMEKeyCode.end())
                continue;

            auto keyCode = sdlToAMEKeyCode.at(Event.key.keysym.sym);
            mKeysPressed[keyCode] = true;
        }
        if (Event.type == SDL_KEYUP) {
            if (sdlToAMEKeyCode.find(Event.key.keysym.sym) == sdlToAMEKeyCode.end())
                continue;

            auto keyCode = sdlToAMEKeyCode.at(Event.key.keysym.sym);
            mKeysReleased[keyCode] = true;
        }

        if (Event.type == SDL_MOUSEBUTTONDOWN) {
            if (Event.button.button == SDL_BUTTON_LEFT) {
                mKeysPressed[KeyCode::Mouse0] = true;
            } else if (Event.button.button == SDL_BUTTON_RIGHT) {
                mKeysPressed[KeyCode::Mouse1] = true;
            }
        }
        if (Event.type == SDL_MOUSEBUTTONUP) {
            if (Event.button.button == SDL_BUTTON_LEFT) {
                mKeysReleased[KeyCode::Mouse0] = true;
            } else if (Event.button.button == SDL_BUTTON_RIGHT) {
                mKeysReleased[KeyCode::Mouse1] = true;
            }
        }
    }
}

const std::map<KeyCode, bool> &InputReader::pressed() const {
    return mKeysPressed;
}

const std::map<KeyCode, bool> &InputReader::released() const {
    return mKeysReleased;
}

const std::map<SDL_Keycode, KeyCode> InputReader::sdlToAMEKeyCode = {
        {SDLK_w,      KeyCode::W},
        {SDLK_a,      KeyCode::A},
        {SDLK_s,      KeyCode::S},
        {SDLK_d,      KeyCode::D},

        {SDLK_UP,     KeyCode::UpArrow},
        {SDLK_DOWN,   KeyCode::DownArrow},
        {SDLK_LEFT,   KeyCode::LeftArrow},
        {SDLK_RIGHT,  KeyCode::RightArrow},

        {SDLK_ESCAPE, KeyCode::Escape},
        {SDLK_LCTRL,  KeyCode::Ctrl},
        {SDLK_SPACE,  KeyCode::Space},
};

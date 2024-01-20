//
// Created by goforbroke on 1/13/24.
//

#ifndef AMORPHOUS_ENGINE_KEYCODE_H
#define AMORPHOUS_ENGINE_KEYCODE_H

#include <string>

namespace AmE {
    class KeyCode {
    public:
        explicit KeyCode() = default;

        explicit KeyCode(std::string code);

        [[nodiscard]] std::string toString() const;

        bool operator<(const KeyCode &rhs) const;

    private:
        std::string mCode;
    };

    const static KeyCode KeyCode_W = KeyCode("w");
    const static KeyCode KeyCode_S = KeyCode("s");
    const static KeyCode KeyCode_A = KeyCode("a");
    const static KeyCode KeyCode_D = KeyCode("d");

    const static KeyCode KeyCode_UpArrow = KeyCode("ARROW ^");
    const static KeyCode KeyCode_DownArrow = KeyCode("ARROW v");
    const static KeyCode KeyCode_RightArrow = KeyCode("ARROW >");
    const static KeyCode KeyCode_LeftArrow = KeyCode("ARROW <");

    const static KeyCode KeyCode_Escape = KeyCode("ESC");
    const static KeyCode KeyCode_Ctrl = KeyCode("CTRL");
    const static KeyCode KeyCode_Space = KeyCode("SPACE");

    const KeyCode KeyCode_Mouse0 = KeyCode("MSL"); // The Left (or primary) mouse button.
    const KeyCode KeyCode_Mouse1 = KeyCode("MSR"); // Right mouse button (or secondary mouse button).
    const KeyCode KeyCode_Mouse2 = KeyCode("MSM"); // Middle mouse button (or third button).
}

#endif //AMORPHOUS_ENGINE_KEYCODE_H

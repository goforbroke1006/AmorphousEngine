//
// Created by goforbroke on 1/6/24.
//

#ifndef AMORPHOUS_ENGINE_COLOR_H
#define AMORPHOUS_ENGINE_COLOR_H

namespace AmE {
    class Color {
    public:
        double mR, mG, mB, mA;

        Color() = default;

        Color(double mR, double mG, double mB, double mA);
    };

    const static Color clear = Color(0.0, 0.0, 0.0, 0.0);
    //
    const static Color black = Color(0.0, 0.0, 0.0, 1.0);
    const static Color white = Color(1.0, 1.0, 1.0, 1.0);
    const static Color grey = Color(0.5, 0.5, 0.5, 1.0);
    //
    const static Color red = Color(1.0, 0.0, 0.0, 1.0);
    const static Color green = Color(0.0, 1.0, 0.0, 1.0);
    const static Color blue = Color(0.0, 0.0, 1.0, 1.0);
    //
    const static Color magenta = Color(1.0, 0.0, 1.0, 1.0);
    const static Color cyan = Color(0.0, 1.0, 1.0, 1.0);
    //
    const static Color yellow = Color(1.0, 0.92, 0.016, 1.0);
}

#endif //AMORPHOUS_ENGINE_COLOR_H

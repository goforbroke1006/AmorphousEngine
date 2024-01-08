//
// Created by goforbroke on 1/6/24.
//

#ifndef AMORPHOUS_ENGINE_COLOR_H
#define AMORPHOUS_ENGINE_COLOR_H

class Color {
public:
    double mR, mG, mB, mA;

    Color() = default;

    Color(double mR, double mG, double mB, double mA);

    static Color clear;
    //
    static Color black;
    static Color white;
    static Color grey;
    //
    static Color red;
    static Color green;
    static Color blue;
};

#endif //AMORPHOUS_ENGINE_COLOR_H

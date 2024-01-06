//
// Created by goforbroke on 1/6/24.
//

#include "../include/Color.h"

Color::Color(double mR, double mG, double mB, double mA) : mR(mR), mG(mG), mB(mB), mA(mA) {}

Color Color::white = Color(1.0, 1.0, 1.0, 1.0);
Color Color::grey = Color(0.5, 0.5, 0.5, 1.0);
Color Color::black = Color(0.0, 0.0, 0.0, 1.0);

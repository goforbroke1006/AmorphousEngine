//
// Created by goforbroke on 1/6/24.
//

#include "../include/Color.h"

Color::Color(double mR, double mG, double mB, double mA) : mR(mR), mG(mG), mB(mB), mA(mA) {}

Color Color::clear = Color(0.0, 0.0, 0.0, 0.0);
//
Color Color::black = Color(0.0, 0.0, 0.0, 1.0);
Color Color::white = Color(1.0, 1.0, 1.0, 1.0);
Color Color::grey = Color(0.5, 0.5, 0.5, 1.0);
//
Color Color::red = Color(1.0, 0.0, 0.0, 1.0);
Color Color::green = Color(0.0, 1.0, 0.0, 1.0);
Color Color::blue = Color(0.0, 0.0, 1.0, 1.0);
//
//Color.magenta = Color:new(1.0, 0.0, 1.0, 1.0);
//Color.cyan = Color:new(0.0, 1.0, 1.0, 1.0);
//
//Color.yellow = Color:new(1.0, 0.92, 0.016, 1.0);